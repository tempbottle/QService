
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
    static std::string Encode(const unsigned char *pszData, const size_t &iLens);
    /*base64����*/
    static std::string Decode(std::string &strEncoded);
};

#endif//Q_BASE64_H_
