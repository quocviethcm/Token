#include "pkcs11.h"
#include <stdio.h>
#include <string.h>
#include<iostream>
#include "include/cryptoki_ext.h"
#include <QtCore>
#include <dlfcn.h>
pkcs11::pkcs11() : m_pSlotList(NULL_PTR),m_hSession(NULL_PTR)
{
}

pkcs11::~pkcs11()
{
    if(m_hSession)
    {
        C_CloseSession(m_hSession);
        m_hSession = NULL_PTR;
    }
    if(m_pSlotList)
    {
        delete[] m_pSlotList;
        m_pSlotList = NULL_PTR;
    }
}
CK_BYTE pkcs11::Logout()
{
    if(m_hSession)
    {
        C_Logout(m_hSession);
        C_CloseSession(m_hSession);
        m_hSession = NULL_PTR;
    }
    if(m_pSlotList)
    {
        delete[] m_pSlotList;
        m_pSlotList = NULL_PTR;
    }
    return 0;
}

CK_RV pkcs11::Close(){
    return C_CloseSession(m_hSession);
}

CK_RV pkcs11::Connect()
{
    CK_RV rv;
    CK_ULONG ulCount = 0;
    rv = C_GetSlotList(TRUE, NULL_PTR, &ulCount);
    if(CKR_OK != rv )
    {
        //printf("\nCan't Acquire the Inoformation of Token.Error Code 0x%08X.\n", rv);
        return rv;
    }
    if(0 >= ulCount)
    {
        //printf("\nCan't Connect to Token.Make Sure You have inserted USB Token.\n");
        return CKR_GENERAL_ERROR;
    }
    m_pSlotList = (CK_SLOT_ID_PTR)new CK_SLOT_ID[ulCount];
    if (! m_pSlotList)
    {
        //printf("\nCan't allocate enough memory\n");
        return CKR_HOST_MEMORY;
    }

    rv = C_GetSlotList(TRUE, m_pSlotList, &ulCount);
    if(CKR_OK != rv )
    {
        //printf("\nCan't Acquire the Inoformation of Token.Error Code 0x%08X.\n", rv);
        return rv;
    }
    if(0 >= ulCount)
    {
        //printf("\nCan't Connect to Token.Make Sure You have inserted Token.\n");
        return CKR_HOST_MEMORY;
    }

    rv = C_OpenSession(
        m_pSlotList[0],  CKF_RW_SESSION | CKF_SERIAL_SESSION,
        NULL_PTR, NULL_PTR, &m_hSession);
    if(CKR_OK != rv )
    {
        //printf("\nCan't Connect to Token.Error Code 0x%08X.\n", rv);
        delete[] m_pSlotList;
        m_pSlotList = NULL_PTR;
        return rv;
    }
    else
    {
        //printf("Success connect to Token\n");
        return CKR_OK;
    }
}
CK_RV pkcs11::Login(QString pin)
{
    CK_RV rv;
    std::string sTemp = pin.toStdString();

    rv = C_Login(m_hSession, CKU_USER, (CK_BYTE_PTR)&sTemp[0], sTemp.length());
    if(CKR_OK != rv)
    {
        //printf("\nCan't use your pin login to Token,Error Code 0x%08X.\n", rv);
        return CKR_GENERAL_ERROR;
    }
    else
    {
        //printf("\nSuccess login to Token\n");
    }

    return CKR_OK;
}

CK_RV pkcs11::ChangePin(QString oldPin, QString newPin)
{
    int oldLength = oldPin.length();
    int newLength = newPin.length();

//    QByteArray QBA_oldPin = oldPin.toLocal8Bit();
//    char *c_str_oldpin = QBA_oldPin.data();

//    QByteArray QBA_newPin = newPin.toLocal8Bit();
//    char *c_str_newpin = QBA_newPin.data();
    //std::string tempOP = oldPin.toStdString();
    //std::string tempNP = newPin.toStdString();
    //CK_UTF8CHAR oldPin_temp[] = {oldPin};
    //CK_UTF8CHAR newPin_temp[] = {newPin};

    CK_RV rv1 = C_SetPIN(m_hSession, (unsigned char*)qstrdup(oldPin.toAscii().constData()), oldLength, (unsigned char*)qstrdup(newPin.toAscii().constData()), newLength);

    if(rv1 == CKR_OK)
        return 0;
    return rv1;
}

CK_RV pkcs11::SetTokenName(QString label)
{
    CK_RV rv;
    m_hPkiLib = dlopen(LIBPATH, RTLD_NOW);
    if(NULL_PTR == m_hPkiLib)
    {
        //cout<<"Can't load lib \"libcastle.so.1.0.0\""<<endl;
        return CKR_GENERAL_ERROR;
    }
    EP_GetAuxFunctionList pE_GetAuxFunctionList = (EP_GetAuxFunctionList)dlsym(m_hPkiLib,"E_GetAuxFunctionList");
    if(NULL_PTR == pE_GetAuxFunctionList)
    {
        dlclose(m_hPkiLib);
        //cout<<"Can't get function list"<<endl;
        return CKR_GENERAL_ERROR;
    }
    rv = pE_GetAuxFunctionList(&m_pAuxFunc);
    if(CKR_OK != rv)
    {
        dlclose(m_hPkiLib);
        //cout<<"Can't get function list"<<endl;
        return CKR_GENERAL_ERROR;
    }
    rv = ((EP_SetTokenLabel)(m_pAuxFunc->pFunc[EP_SET_TOKEN_LABEL]))(m_pSlotList[0],CKU_USER,NULL_PTR,0,(unsigned char*)qstrdup(label.toAscii().constData()));
    if(CKR_OK != rv)
    {
        dlclose(m_hPkiLib);
        //cout<<"Set token name fault"<<endl;
        return rv;
    } else {
        //cout<<"change name successfully"<<endl;
        dlclose(m_hPkiLib);
        return rv;
    }

}
