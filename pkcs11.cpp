#include "pkcs11.h"
#include <stdio.h>
#include <string.h>
#include<iostream>
#include "include/cryptoki_ext.h"
#include <QtCore>

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
        printf("\nCan't Acquire the Inoformation of Token.Error Code 0x%08X.\n", rv);
        return rv;
    }
    if(0 >= ulCount)
    {
        printf("\nCan't Connect to Token.Make Sure You have inserted USB Token.\n");
        return CKR_GENERAL_ERROR;
    }
    m_pSlotList = (CK_SLOT_ID_PTR)new CK_SLOT_ID[ulCount];
    if (! m_pSlotList)
    {
        printf("\nCan't allocate enough memory\n");
        return CKR_HOST_MEMORY;
    }

    rv = C_GetSlotList(TRUE, m_pSlotList, &ulCount);
    if(CKR_OK != rv )
    {
        printf("\nCan't Acquire the Inoformation of Token.Error Code 0x%08X.\n", rv);
        return rv;
    }
    if(0 >= ulCount)
    {
        printf("\nCan't Connect to Token.Make Sure You have inserted Token.\n");
        return CKR_HOST_MEMORY;
    }

    rv = C_OpenSession(
        m_pSlotList[0],  CKF_RW_SESSION | CKF_SERIAL_SESSION,
        NULL_PTR, NULL_PTR, &m_hSession);
    if(CKR_OK != rv )
    {
        printf("\nCan't Connect to Token.Error Code 0x%08X.\n", rv);
        delete[] m_pSlotList;
        m_pSlotList = NULL_PTR;
        return rv;
    }
    else
    {
        printf("Success connect to Token\n");
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
        printf("\nCan't use your pin login to Token,Error Code 0x%08X.\n", rv);
        return CKR_GENERAL_ERROR;
    }
    else
    {
        printf("\nSuccess login to Token\n");
    }

    return CKR_OK;
}

