
#ifndef Q_BASE64_H_
#define Q_BASE64_H_

#include "BufferMgr.h"

/*Base64 ��������࣬Decode���̰߳�ȫ*/
class CBase64
{
public:
    CBase64(void);
    ~CBase64(void);

    /*base64����*/
    std::string Encode(const unsigned char *pszData, const size_t &iLens) const;
    /*base64����*/
    const char *Decode(const char* pszData, const size_t &iLens, size_t &iOutLens);

private:
    CBuffer m_objBuffer;
};

#endif//Q_BASE64_H_
