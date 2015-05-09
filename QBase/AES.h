
#ifndef Q_AES_H_
#define Q_AES_H_

#include "BufferMgr.h"

/*aes���� ���̰߳�ȫ*/
class CAESEncode
{
public:
    CAESEncode(void);
    ~CAESEncode(void);

    /*�������� pszKey ���룬 emKeyType ��������*/
    int setKey(const char *pszKey, AESKeyType emKeyType);
    /*���� pszPlaint��Ҫ���ܵ����ݣ�iLens ��Ҫ�������ݳ��ȣ� iOutLes���ܺ󳤶�*/
    const char *Encode(const char *pszPlaint, const size_t &iLens, size_t &iOutLes);

private:
    enum BlockSize
    {
        AES_BlockSize = 16
    };

private:
    unsigned long *m_pRK;
    unsigned char *m_pKey;    
    int m_iRounds;
    unsigned char m_uacPlain[AES_BlockSize];
    unsigned char m_uacCipher[AES_BlockSize];
    CBuffer m_objBuffer;
};

/*aes���� ���̰߳�ȫ*/
class CAESDecode
{
public:
    CAESDecode(void);
    ~CAESDecode(void);

    /*�������� pszKey ���룬 emKeyType ��������*/
    int setKey(const char *pszKey, AESKeyType emKeyType);
    /*����, pszCipher��Ҫ���ܵ����ݣ�iLens ��Ҫ�������ݳ��ȣ� iOutLes���ܺ󳤶�*/
    const char *Decode(const char *pszCipher, const size_t &iLens, size_t &iOutLes);

private:
    enum BlockSize
    {
        AES_BlockSize = 16
    };

private:
    unsigned long *m_pRK;
    unsigned char *m_pKey;
    int m_iRounds;
    unsigned char m_uacPlain[AES_BlockSize];
    unsigned char m_uacCipher[AES_BlockSize];
    CBuffer m_objBuffer;
};

#endif//Q_AES_H_
