
#ifndef Q_LIBRARY_H_
#define Q_LIBRARY_H_

#include "Macros.h"

/*��̬�������*/
class CLibrary
{
public:
    explicit CLibrary(const char *pszLib);
    ~CLibrary(void);

    /*��ȡdllָ��������ַ*/
    void *getFuncAddr(const char *pSym);
    /*���ش�����Ϣ*/
    const char *getError(void) const;

private:
    CLibrary(void);
    void Init(const char *pszLib);
    void Destroy(void);

private:
#ifdef Q_OS_HPUX
    shl_t m_pHandle;
#else
    void *m_pHandle;
#endif
};

#endif//Q_LIBRARY_H_
