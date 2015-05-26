
#ifndef Q_RSAKEY_H_
#define Q_RSAKEY_H_

#include "Macros.h"
#include <RSAEuro/rsa.h>

/*RSA�㷨������*/
class CRSAKey
{
public:
    CRSAKey(void);
    ~CRSAKey(void);

    /*����key(����512 - 1024)*/
    int creatKey(const unsigned short usKeyLens);

    /*���浽�ļ���base64���뱣��*/
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
    std::string Decode(std::string &strVal);
    std::string Encode(const char *pszMsg, const size_t &iLens);
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
};

#endif//Q_RSAKEY_H_
