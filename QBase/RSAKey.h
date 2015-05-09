
#ifndef Q_RSAKEY_H_
#define Q_RSAKEY_H_

#include "AES.h"
#include "Base64.h"
#include <RSAEuro/rsa.h>

/*RSA�㷨������*/
class CRSAKey
{
public:
    CRSAKey(void);
    ~CRSAKey(void);

    /*����key(����512 - 1024)*/
    int creatKey(const unsigned short usKeyLens);

    /*����AES��������*/
    int setAESKey(const char *pszKey);

    /*���浽�ļ��� ��aes����Ȼ��base64���뱣��*/
    /*����R_RANDOM_STRUCT ��Կ������Ҫʹ��*/
    int saveRandom(const char *pszFile);
    /*���湫Կ*/
    int savePublicKey(const char *pszFile);
    /*����˽Կ*/
    int savePrivateKey(const char *pszFile);

    /*��ȡKey*/
    int loadPublicKey(const char *pszFile);
    int loadPrivateKey(const char *pszFile);
    int loadRandom(const char *pszFile);

    /*�������ɵ�Key���ߴ��ļ���ȡ��Key*/
    R_RSA_PUBLIC_KEY *getPublicKey(void);
    R_RSA_PRIVATE_KEY *getPrivateKey(void);
    R_RANDOM_STRUCT *getRandom(void);

private:
    const char *Decode(const char *pszMsg, const size_t iLens, size_t &iOutLens);
    int Encode(const char *pszMsg, const size_t iLens, std::string &strBase64);
    int fileWrite(const char *pszMsg, const size_t iLens, const char *pszFile) const;
    std::string fileRead(const char *pszFile) const;
    int loadInfo(char *pInfo, const size_t iLens, const char *pszFile);

private:
    enum RSAKeyLen
    {
        RSAKeyMinLen = 512,
        RSAKeyMaxLen = 1024
    };

    R_RANDOM_STRUCT m_stRandom;
    R_RSA_PUBLIC_KEY m_stPublicKey;    /* RSA public key */   
    R_RSA_PRIVATE_KEY m_stPrivateKey;  /* RSA private key */
    CAESEncode m_objAESEncode;
    CAESDecode m_objAESDecode;
    CBase64 m_objBase64;
};

#endif//Q_RSAKEY_H_
