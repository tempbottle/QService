
#ifndef Q_QRSA_H_
#define Q_QRSA_H_

#include "RSAKey.h"
#include "BufferMgr.h"

/*
RSA���ܽ����࣬���̰߳�ȫ
*/
class CRSA
{
public:
    CRSA(void);
    ~CRSA(void);

    /*����RSA Key*/
    void setKey(CRSAKey *pRSAKey);

    /*��Կ����,��ҪR_RANDOM_STRUCT�ṹ��*/
    const char * publicKeyEncrypt(const char* pszData, const size_t &iDataLens, size_t &iOutLens);
    /*˽Կ����*/
    const char * privateKeyDecrypt(const char* pszData, const size_t &iDataLens, size_t &iOutLens);

    /*˽Կ����*/
    const char * privateKeyEncrypt(const char* pszData, const size_t &iDataLens, size_t &iOutLens);
    /*��Կ����*/
    const char * publicKeyDecrypt(const char* pszData, const size_t &iDataLens, size_t &iOutLens);

private:
    /*���ܷ�ʽ*/
    enum RSAEncryptType
    {
        RSAEncryptType_Public = 0,//��Կ����
        RSAEncryptType_Private   //˽Կ����
    };

    /*���ܷ�ʽ*/
    enum RSADecryptType
    {
        RSADecryptType_Public = 0,//��Կ����
        RSADecryptType_Private   //˽Կ����
    };

private:
    const char *RSAEncrypt(RSAEncryptType emEnType, const char* pszData, 
        const size_t &iDataLens, size_t &iOutLens);
    const char *RSADecrypt(RSADecryptType emEnType, const char* pszData, 
        const size_t &iDataLens, size_t &iOutLens);

private:
    CRSAKey *m_pRSAKey;//key
    unsigned char m_acOutBuf[MAX_RSA_MODULUS_LEN];
    CBuffer m_objBuffer;
};

#endif//Q_QRSA_H_
