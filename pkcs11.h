#ifndef PKCS11_H
#define PKCS11_H
#include <QtCore>
#include "include/cryptoki_ext.h"
#include "include/auxiliary.h"

#define LIBPATH "/usr/lib/viettel-ca_v4.so"

typedef struct PositionItem
{
    int indexOfRoot;
    int indexOfCer;
    int indexOfPub;
    int indexOfPri;
    QString CerID;
    QString PubID;
    QString PriID;

}PositionItem;

typedef struct DataCertificate
{
    QString CER_CKA_CLASS;
    QString CER_CKA_CERTIFICATE_TYPE;
    QString CER_CKA_LABEL;
    QString CER_CKA_SUBJECT;
    QString CER_CKA_ID;
    QString CER_CKA_VALUE;
    QString CER_CKA_CONTAINNER;
    QString CER_COMMOND_NAME;
}DataCertificate;

typedef struct DataPublicKey
{
    QString KEYPU_CKA_CLASS;
    QString KEYPU_CKA_KEY_TYPE;
    QString KEYPU_CKA_LABEL;
    QString KEYPU_CKA_WRAP;
    QString KEYPU_CKA_ENCRYPT;
    QString KEYPU_CKA_MODULUS;
    QString KEYPU_CKA_PUBLIC_EXPONENT;
    QString KEYPU_CKA_ID;
    QString KEYPU_CKA_CONTAINNER;
}DataPublicKey;

typedef struct DataPrivateKey
{
    QString KEYPR_CKA_CLASS;
    QString KEYPR_CKA_KEY_TYPE;
    QString KEYPR_CKA_LABEL;
    QString KEYPR_CKA_SUBJECT;
    QString KEYPR_CKA_ID;
    QString KEYPR_CKA_SENSITIVE;
    QString KEYPR_CKA_DECRYPT;
    QString KEYPR_CKA_SIGN;
    QString KEYPR_CKA_MODULUS;
    QString KEYPR_CKA_PUBLIC_EXPONENT;
    QString KEYPR_CKA_CONTAINNER;

}DataPrivateKey;
typedef struct ItemSelect
{
    QString ParentName;
    QString ItemName;
    QString CurrentItemName;
    int IndexofParent;
    int ParentID;
}ItemSelect;

enum P12_ERROR
{
    OK = 0, ERR_OPEN_FILE, ERR_READ_P12_FILE, ERR_PARSE_P12,
    ERR_PARSE_CERT
};

struct P12_Certificate
{
    CK_BYTE_PTR pValue;
    CK_ULONG ulValue;

    CK_BYTE_PTR pSubject;
    CK_ULONG ulSubject;

    CK_BYTE_PTR pIssuer;
    CK_ULONG ulIssuer;

    CK_BYTE_PTR pSerial;
    CK_ULONG ulSerial;

    //X509_NAME *SubName;
};

struct P12_PublicKey
{
    CK_BYTE_PTR pModulus;
    CK_ULONG ulModulus;

    CK_BYTE_PTR pPublicExponent;
    CK_ULONG ulPublicExponent;

    CK_ULONG modulus_bits;
};

struct P12_PrivateKey
{
    CK_BYTE_PTR pModulus;
    CK_ULONG ulModulus;

    CK_BYTE_PTR pPublicExponent;
    CK_ULONG ulPublicExponent;

    CK_BYTE_PTR pPrivateExponent;
    CK_ULONG ulPrivateExponent;

    CK_BYTE_PTR pPrimeP;
    CK_ULONG ulPrimeP;

    CK_BYTE_PTR pPrimeQ;
    CK_ULONG ulPrimeQ;

    CK_BYTE_PTR pExp1;
    CK_ULONG ulExp1;
    CK_BYTE_PTR pExp2;
    CK_ULONG ulExp2;

    CK_BYTE_PTR pCoefficient;
    CK_ULONG ulCoefficient;
};


class pkcs11
{

public:
    pkcs11();
    ~pkcs11();
     CK_RV rv;
    CK_SLOT_ID_PTR m_pSlotList;
    CK_VOID_PTR m_pApplication;
    CK_SESSION_HANDLE m_hSession;
    void *m_hPkiLib;
    AUX_FUNC_LIST_PTR m_pAuxFunc;


    void ShowErr(char *strInfo, CK_RV rv);
    void StartOP();
    CK_RV Connect();
    CK_RV Login(QString pin);
    CK_BYTE Logout();

    CK_RV ChangePin(QString oldPin, QString newPin);
    CK_BYTE ChangePin(std::string oldPin, std::string newPin);
    CK_RV SetTokenName(QString tokenName);
    void Enum();
    void Data();
    void Public();
    void Private();
    void Secret();
    CK_RV Close();
};

#endif // PKCS11_H
