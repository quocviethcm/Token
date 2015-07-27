#include "pkcs11.h"
#include <stdio.h>
#include <string.h>
#include<iostream>
#include "include/cryptoki_ext.h"
#include <QtCore>
#include <dlfcn.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <QDebug>

#include <sstream>
#include <iomanip>
#include <memory>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>

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
CK_RV pkcs11::GetTokenInfo()
{
    //CK_RV rv;

    rv = C_GetTokenInfo(m_pSlotList[0], &tokenInfo);
    if (CKR_OK != rv)
    {
        return 22;
    }
    return 0;
}

void pkcs11::toHex(
    void *const data,                   //!< Data to convert
    const size_t dataLength,            //!< Length of the data to convert
    std::string &dest                   //!< Destination string
    )
{
    unsigned char       *byteData = reinterpret_cast<unsigned char*>(data);
    std::stringstream   hexStringStream;

    hexStringStream << std::hex << std::setfill('0');
    for(size_t index = 0; index < dataLength; ++index)
        hexStringStream << std::setw(2) << static_cast<int>(byteData[index]);
    dest = hexStringStream.str();
}

CK_BYTE pkcs11::GetCertificate(DataCertificate Cer[], QSslCertificate *QsslObj[])
{
    int rv=0;
    //QString Label;
    CK_OBJECT_CLASS dataClass = CKO_CERTIFICATE;
    bool IsToken=true;
    CK_ATTRIBUTE pTempl[] =
    {
        {CKA_CLASS, &dataClass, sizeof(CKO_CERTIFICATE)},
        {CKA_TOKEN, &IsToken, sizeof(true)}
    };

    C_FindObjectsInit(m_hSession, pTempl, 2);

    CK_OBJECT_HANDLE hCKObj;
    CK_ULONG ulRetCount = 0;
    CK_RV ckrv = 0;
    int numObj=0;
    do
    {
        ckrv = C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
        if(CKR_OK != ckrv)
        {
            rv = 1;
            break;
        }
        if(1 != ulRetCount)
        {
            rv = 2;
            break;
        }
        //qDebug()<<"ulRetCount: "<<ulRetCount;
        CK_ATTRIBUTE pAttrTemp[] =
        {
            {CKA_CLASS, NULL, 0},
            {CKA_CERTIFICATE_TYPE,NULL,0},
            {CKA_LABEL, NULL, 0},
            {CKA_SUBJECT,NULL,0},
            {CKA_ID,NULL,0},
            {CKA_VALUE,NULL,0},
            {CKA_CONTAINER_NAME,NULL,0}
        };

        ckrv = C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 7);
        //qDebug()<<"C_GetAttr: "<<ckrv;
        if(ckrv != CKR_OK)
        {
            rv = 3;
            break;
        }

        pAttrTemp[0].pValue = new char[pAttrTemp[0].ulValueLen];
        pAttrTemp[1].pValue = new char[pAttrTemp[1].ulValueLen];
        pAttrTemp[2].pValue = new char[pAttrTemp[2].ulValueLen+1];
        pAttrTemp[3].pValue = new char[pAttrTemp[3].ulValueLen+1];
        pAttrTemp[4].pValue = new char[pAttrTemp[4].ulValueLen+1];
        pAttrTemp[5].pValue = new char[pAttrTemp[5].ulValueLen];
        pAttrTemp[6].pValue = new char[pAttrTemp[6].ulValueLen+1];

        memset(pAttrTemp[0].pValue, 0, pAttrTemp[0].ulValueLen);
        memset(pAttrTemp[1].pValue, 0, pAttrTemp[1].ulValueLen);
        memset(pAttrTemp[2].pValue, 0, pAttrTemp[2].ulValueLen+1);
        memset(pAttrTemp[3].pValue, 0, pAttrTemp[3].ulValueLen+1);
        memset(pAttrTemp[4].pValue, 0, pAttrTemp[4].ulValueLen+1);
        memset(pAttrTemp[5].pValue, 0, pAttrTemp[5].ulValueLen);
        memset(pAttrTemp[6].pValue, 0, pAttrTemp[6].ulValueLen+1);



        ckrv = C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 7);
        if(ckrv != CKR_OK)
        {
            delete[](char*) pAttrTemp[0].pValue;
            delete[](char*) pAttrTemp[1].pValue;
            delete[](char*) pAttrTemp[2].pValue;
            delete[](char*) pAttrTemp[3].pValue;
            delete[](char*) pAttrTemp[4].pValue;
            delete[](char*) pAttrTemp[5].pValue;
            delete[](char*) pAttrTemp[6].pValue;
            rv = 4;
            break;
        }

        unsigned char *intckaclass = (unsigned char*)pAttrTemp[1].pValue;
        char *strvale = (char*)pAttrTemp[2].pValue;
        unsigned char *strsubject = (unsigned char*)pAttrTemp[3].pValue;
        unsigned char *strckaid=(unsigned char*)pAttrTemp[4].pValue;
        unsigned char *strckavalue=(unsigned char*)pAttrTemp[5].pValue;

        const char *strckavalue1=(const char*)pAttrTemp[5].pValue;
        char *container = (char*)pAttrTemp[6].pValue;

        //do copy
        std::string str_temp;

        QByteArray mybytearray = QByteArray::fromRawData(strckavalue1, pAttrTemp[5].ulValueLen);

        QSslCertificate *QSSLObject = new QSslCertificate (mybytearray,QSsl::Der);

        QsslObj[numObj] = QSSLObject;

        Cer[numObj].CER_COMMOND_NAME = QsslObj[numObj]->subjectInfo(QSslCertificate::CommonName);

        //pAttrTemp[0]
        Cer[numObj].CER_CKA_CLASS = "CKO_CERTIFICATE";

        //pAttrTemp[1]
        toHex(intckaclass, pAttrTemp[1].ulValueLen, str_temp);
        Cer[numObj].CER_CKA_CERTIFICATE_TYPE = hexFormat(QString::fromStdString(str_temp), 0);

        //pAttrTemp[2]
        Cer[numObj].CER_CKA_LABEL = QString::fromUtf8(strvale, -1);

        //pAttrTemp[3]
        toHex(strsubject, pAttrTemp[3].ulValueLen, str_temp);
        Cer[numObj].CER_CKA_SUBJECT = hexFormat(QString::fromStdString(str_temp), 0);

        //pAttrTemp[4]
        toHex(strckaid, pAttrTemp[4].ulValueLen, str_temp);
        Cer[numObj].CER_CKA_ID = hexFormat(QString::fromStdString(str_temp), 0);

        //pAttrTemp[5]
        toHex(strckavalue, pAttrTemp[5].ulValueLen, str_temp);
        Cer[numObj].CER_CKA_VALUE = hexFormat(QString::fromStdString(str_temp), 0);

        //pAttrTemp[6]
        Cer[numObj].CER_CKA_CONTAINNER = QString::fromUtf8(container, -1);

        //qDebug()<<"cert: "<<Cer[numObj].CER_CKA_CONTAINNER;

        numObj++;

        delete[](char *) pAttrTemp[0].pValue;
        delete[](char *) pAttrTemp[1].pValue;
        delete[](char *) pAttrTemp[2].pValue;
        delete[](char *) pAttrTemp[3].pValue;
        delete[](char *) pAttrTemp[4].pValue;
        delete[](char *) pAttrTemp[5].pValue;
        delete[](char *) pAttrTemp[6].pValue;

    }while(true);
    C_FindObjectsFinal(m_hSession);
    return rv;

}

CK_BYTE pkcs11::GetPublicKey(DataPublicKey Pub[])
{
    int rv = 0;
    CK_OBJECT_CLASS dataClass = CKO_PUBLIC_KEY;
    bool IsToken=true;
    CK_ATTRIBUTE pTempl[] =
    {
        {CKA_CLASS, &dataClass, sizeof(CKO_PUBLIC_KEY)},
        {CKA_TOKEN, &IsToken, sizeof(true)}
    };

    C_FindObjectsInit(m_hSession, pTempl, 2);

    CK_OBJECT_HANDLE hCKObj;
    CK_ULONG ulRetCount = 0;
    CK_RV ckrv = 0;
    int numObj=0;
    do
    {
        ckrv = C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
        if(CKR_OK != ckrv)
        {
            break;
        }
        if(1 != ulRetCount)
            break;
        //qDebug()<<"ulRetCount: "<<ulRetCount;
        CK_ATTRIBUTE pAttrTemp[] =
        {
            {CKA_CLASS, NULL, 0},
            {CKA_KEY_TYPE,NULL,0},
            {CKA_ENCRYPT,NULL,0},
            {CKA_MODULUS,NULL,0},
            {CKA_PUBLIC_EXPONENT,NULL, 0},
            {CKA_ID, NULL, 0},
            {CKA_CONTAINER_NAME, NULL, 0},
            {CKA_MODULUS_BITS, NULL, 0}
        };

        ckrv = C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 8);
        //qDebug()<<"C_GetAttr: "<<ckrv;
        if(ckrv != CKR_OK)
        {
            break;
        }

        pAttrTemp[0].pValue = new char[pAttrTemp[0].ulValueLen];
        pAttrTemp[1].pValue = new char[pAttrTemp[1].ulValueLen];
        pAttrTemp[2].pValue = new char[pAttrTemp[2].ulValueLen];
        pAttrTemp[3].pValue = new char[pAttrTemp[3].ulValueLen];
        pAttrTemp[4].pValue = new char[pAttrTemp[4].ulValueLen];
        pAttrTemp[5].pValue = new char[pAttrTemp[5].ulValueLen+1];
        pAttrTemp[6].pValue = new char[pAttrTemp[6].ulValueLen+1];
        pAttrTemp[7].pValue = new char[pAttrTemp[7].ulValueLen+1];

        memset(pAttrTemp[0].pValue, 0, pAttrTemp[0].ulValueLen);
        memset(pAttrTemp[1].pValue, 0, pAttrTemp[1].ulValueLen);
        memset(pAttrTemp[2].pValue, 0, pAttrTemp[2].ulValueLen);
        memset(pAttrTemp[3].pValue, 0, pAttrTemp[3].ulValueLen);
        memset(pAttrTemp[4].pValue, 0, pAttrTemp[4].ulValueLen);
        memset(pAttrTemp[5].pValue, 0, pAttrTemp[5].ulValueLen+1);
        memset(pAttrTemp[6].pValue, 0, pAttrTemp[6].ulValueLen+1);
        memset(pAttrTemp[7].pValue, 0, pAttrTemp[7].ulValueLen+1);

        ckrv = C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 8);
        if(ckrv != CKR_OK)
        {
            delete[](char*) pAttrTemp[0].pValue;
            delete[](char*) pAttrTemp[1].pValue;
            delete[](char*) pAttrTemp[2].pValue;
            delete[](char*) pAttrTemp[3].pValue;
            delete[](char*) pAttrTemp[4].pValue;
            delete[](char*) pAttrTemp[5].pValue;
            delete[](char*) pAttrTemp[6].pValue;
            delete[](char*) pAttrTemp[7].pValue;
            break;
        }

        unsigned char *intckakeytype = (unsigned char*)pAttrTemp[1].pValue;
        unsigned char *strckaencrypt = (unsigned char*)pAttrTemp[2].pValue;
        unsigned char *strCkaModulus =(unsigned char*)pAttrTemp[3].pValue;
        char *strCkaPublic=(char *)pAttrTemp[4].pValue;
        unsigned char *strckaid = (unsigned char*)pAttrTemp[5].pValue;
        char *container = (char *)pAttrTemp[6].pValue;
        char *modolus_bit = (char *)pAttrTemp[7].pValue;
        //do copy
        std::string str_temp;

        Pub[numObj].KEYPU_CKA_CLASS = "CKO_PUBLIC_KEY";
        //pAttrTemp[1].pValue
        toHex(intckakeytype, pAttrTemp[1].ulValueLen, str_temp);
        Pub[numObj].KEYPU_CKA_KEY_TYPE = hexFormat(QString::fromStdString(str_temp), 0);

        //pAttrTemp[2].pValue
        toHex(strckaencrypt, pAttrTemp[2].ulValueLen, str_temp);
        Pub[numObj].KEYPU_CKA_ENCRYPT = hexFormat(QString::fromStdString(str_temp), 0);

        //pAttrTemp[3].pValue
        toHex(strCkaModulus, pAttrTemp[3].ulValueLen, str_temp);
        Pub[numObj].KEYPU_CKA_MODULUS = hexFormat(QString::fromStdString(str_temp), 0);

        //pAttrTemp[4].pValue
        toHex(strCkaPublic, pAttrTemp[4].ulValueLen, str_temp);
        Pub[numObj].KEYPU_CKA_PUBLIC_EXPONENT = hexFormat(QString::fromStdString(str_temp), 0);

        //pAttrTemp[5].pValue
        toHex(strckaid, pAttrTemp[5].ulValueLen, str_temp);
        Pub[numObj].KEYPU_CKA_ID = hexFormat(QString::fromStdString(str_temp), 0);

        //pAttrTemp[6].pValue
        Pub[numObj].KEYPU_CKA_CONTAINNER = QString::fromLocal8Bit(container, -1);
        //qDebug()<<"public: "<<Pub[numObj].KEYPU_CKA_CONTAINNER;

        //toHex(modolus_bit, pAttrTemp[7].ulValueLen, str_temp);
        //QString modolus_bit1 = hexFormat(QString::fromStdString(str_temp), 0);
        //qDebug()<<"modolus bit: "<<modolus_bit1;
        numObj++;


        delete[](char *) pAttrTemp[0].pValue;
        delete[](char *) pAttrTemp[1].pValue;
        delete[](char *) pAttrTemp[2].pValue;
        delete[](char *) pAttrTemp[3].pValue;
        delete[](char *) pAttrTemp[4].pValue;
        delete[](char *) pAttrTemp[5].pValue;
        delete[](char *) pAttrTemp[6].pValue;
        delete[](char *) pAttrTemp[7].pValue;

    }while(true);
    C_FindObjectsFinal(m_hSession);
    return rv;
}

CK_BYTE pkcs11::GetPrivateKey(DataPrivateKey Pri[])
{
    int rv = 0;
    CK_OBJECT_CLASS dataClass = CKO_PRIVATE_KEY;
    bool IsToken=true;
    CK_ATTRIBUTE pTempl[] =
    {
        {CKA_CLASS, &dataClass, sizeof(CKO_PRIVATE_KEY)},
        {CKA_TOKEN, &IsToken, sizeof(true)}
    };


    C_FindObjectsInit(m_hSession, pTempl, 2);

    CK_OBJECT_HANDLE hCKObj;
    CK_ULONG ulRetCount = 0;
    CK_RV ckrv = 0;
    int numObj=0;
    do
    {
        ckrv = C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
        if(CKR_OK != ckrv)
        {
            break;
        }
        if(1 != ulRetCount)
            break;
        //qDebug()<<"ulRetCount: "<<ulRetCount;
        CK_ATTRIBUTE pAttrTemp[] =
        {
            {CKA_CLASS, NULL, 0},
            {CKA_KEY_TYPE,NULL,0},
//            {CKA_LABEL, NULL, 0},
//            {CKA_SUBJECT,NULL,0},
            {CKA_ID,NULL,0},
            {CKA_SENSITIVE,NULL,0},
            {CKA_DECRYPT,NULL,0},
            {CKA_SIGN,NULL,0},
            {CKA_MODULUS,NULL,0},
            {CKA_PUBLIC_EXPONENT,NULL,0},
            {CKA_CONTAINER_NAME, NULL, 0}
        };

        ckrv = C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 9);
        //qDebug()<<"C_GetAttr: "<<ckrv;
        if(ckrv != CKR_OK)
        {
            break;
        }

        pAttrTemp[0].pValue = new char[pAttrTemp[0].ulValueLen];
        pAttrTemp[1].pValue = new char[pAttrTemp[1].ulValueLen];
        pAttrTemp[2].pValue = new char[pAttrTemp[2].ulValueLen+1];
        pAttrTemp[3].pValue = new char[pAttrTemp[3].ulValueLen];
        pAttrTemp[4].pValue = new char[pAttrTemp[4].ulValueLen];
        pAttrTemp[5].pValue = new char[pAttrTemp[5].ulValueLen];
        pAttrTemp[6].pValue = new char[pAttrTemp[6].ulValueLen];
        pAttrTemp[7].pValue = new char[pAttrTemp[7].ulValueLen];
        pAttrTemp[8].pValue = new char[pAttrTemp[8].ulValueLen+1];


        memset(pAttrTemp[0].pValue, 0, pAttrTemp[0].ulValueLen);
        memset(pAttrTemp[1].pValue, 0, pAttrTemp[1].ulValueLen);
        memset(pAttrTemp[2].pValue, 0, pAttrTemp[2].ulValueLen+1);
        memset(pAttrTemp[3].pValue, 0, pAttrTemp[3].ulValueLen);
        memset(pAttrTemp[4].pValue, 0, pAttrTemp[4].ulValueLen);
        memset(pAttrTemp[5].pValue, 0, pAttrTemp[5].ulValueLen);
        memset(pAttrTemp[6].pValue, 0, pAttrTemp[6].ulValueLen);
        memset(pAttrTemp[7].pValue, 0, pAttrTemp[7].ulValueLen);
        memset(pAttrTemp[8].pValue, 0, pAttrTemp[8].ulValueLen+1);

        ckrv = C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 9);
        if(ckrv != CKR_OK)
        {
            delete[](char*) pAttrTemp[0].pValue;
            delete[](char*) pAttrTemp[1].pValue;
            delete[](char*) pAttrTemp[2].pValue;
            delete[](char*) pAttrTemp[3].pValue;
            delete[](char*) pAttrTemp[4].pValue;
            delete[](char*) pAttrTemp[5].pValue;
            delete[](char*) pAttrTemp[6].pValue;
            delete[](char*) pAttrTemp[7].pValue;
            delete[](char*) pAttrTemp[8].pValue;
            break;
        }


        unsigned char *strckakeytype = (unsigned char*)pAttrTemp[1].pValue; //*(int*)pAttrTemp[1].pValue

        unsigned char *strCkaId=(unsigned char*)pAttrTemp[2].pValue;
        unsigned char *strsensitive = (unsigned char*)pAttrTemp[3].pValue;//(int)*(char*)pAttrTemp[5].pValue
        unsigned char *strdecrypt = (unsigned char*)pAttrTemp[4].pValue;//(int)*(char*)pAttrTemp[6].pValue
        unsigned char *strsign = (unsigned char*)pAttrTemp[5].pValue;//(int)*(char*)pAttrTemp[7].pValue
        unsigned char *strmodulus = (unsigned char*)pAttrTemp[6].pValue;
        unsigned char *strexpo = (unsigned char*)pAttrTemp[7].pValue;
        char *container = (char*)pAttrTemp[8].pValue;


        // do copy
        //pAttrTemp[0].pValue
        std::string str_temp;
        Pri[numObj].KEYPR_CKA_CLASS = "CKO_PRIVATE_KEY";

        //pAttrTemp[1].pValue
        toHex(strckakeytype, pAttrTemp[1].ulValueLen, str_temp);
        Pri[numObj].KEYPR_CKA_KEY_TYPE = hexFormat(QString::fromStdString(str_temp), 0);

        //pAttrTemp[2].pValue
        toHex(strCkaId, pAttrTemp[2].ulValueLen, str_temp);
        Pri[numObj].KEYPR_CKA_ID = hexFormat(QString::fromStdString(str_temp), 0);

        //pAttrTemp[3].pValue
        toHex(strsensitive, pAttrTemp[3].ulValueLen, str_temp);
        Pri[numObj].KEYPR_CKA_SENSITIVE = hexFormat(QString::fromStdString(str_temp), 0);

        //pAttrTemp[4].pValue
        toHex(strdecrypt, pAttrTemp[4].ulValueLen, str_temp);
        Pri[numObj].KEYPR_CKA_DECRYPT = hexFormat(QString::fromStdString(str_temp), 0);

        //pAttrTemp[5].pValue
        toHex(strsign, pAttrTemp[5].ulValueLen, str_temp);
        Pri[numObj].KEYPR_CKA_SIGN = hexFormat(QString::fromStdString(str_temp), 0);

        //pAttrTemp[6].pValue
        toHex(strmodulus, pAttrTemp[6].ulValueLen, str_temp);
        Pri[numObj].KEYPR_CKA_MODULUS = hexFormat(QString::fromStdString(str_temp), 0);

        //pAttrTemp[7].pValue
        toHex(strexpo, pAttrTemp[7].ulValueLen, str_temp);
        Pri[numObj].KEYPR_CKA_PUBLIC_EXPONENT = hexFormat(QString::fromStdString(str_temp), 0);

        //pAttrTemp[7].pValue
        Pri[numObj].KEYPR_CKA_CONTAINNER = QString::fromLocal8Bit(container, -1);
        //qDebug()<<"private: "<<Pri[numObj].KEYPR_CKA_CONTAINNER;

        numObj++;

        delete[](char*) pAttrTemp[0].pValue;
        delete[](char*) pAttrTemp[1].pValue;
        delete[](char*) pAttrTemp[2].pValue;
        delete[](char*) pAttrTemp[3].pValue;
        delete[](char*) pAttrTemp[4].pValue;
        delete[](char*) pAttrTemp[5].pValue;
        delete[](char*) pAttrTemp[6].pValue;
        delete[](char*) pAttrTemp[7].pValue;
        delete[](char*) pAttrTemp[8].pValue;

    }while(true);
    C_FindObjectsFinal(m_hSession);
    return rv;
}
QString pkcs11::hexFormat(QString str, int mode)
{
    if(mode == 0)
    {
        int position = 2;
        while(position < str.length())
        {
            str.insert(position, QString(" "));
            position +=3;
        }

    } else
    {
        str = str.replace(QString(":"), QString(" "));
    }
    str = str.toUpper();
    return str;
}
