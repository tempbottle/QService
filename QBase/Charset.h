
#ifndef Q_CHARSET_H_
#define Q_CHARSET_H_

#include "Macros.h"

/*
* �����ʽ
*/
class CCharset
{
public:
    CCharset(void);
    ~CCharset(void);

    /*��ȡ�ַ������ʽ,����"" ʧ�ܻ�Ϊ��ascii*/
    std::string getStrCharset(const char *pszData, const size_t iSize);
    /*��ȡ�ļ������ʽ, ����"" ʧ�ܻ�Ϊ��ascii*/
    std::string getCharset(const char *pszFile);
    std::string getCharset(FILE *pFile);

private:
    void *m_pCharset;
};

#endif//Q_CHARSET_H_
