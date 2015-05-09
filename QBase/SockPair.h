
#ifndef Q_SOCKPAIR_H_
#define Q_SOCKPAIR_H_

#include "NETAddr.h"

/*
�໥���ӵ�2��socket
*/
class CSockPair
{
public:
    CSockPair(void);
    ~CSockPair(void);
    
    /*��ȡ��sock���*/
    Q_SOCK getReadFD(void) const
    {
        return m_ReadFD;
    };
    /*��ȡдsock���*/
    Q_SOCK getWriteFD(void) const
    {
        return m_WriteFD;
    };
    
    /*�Ӷ������ȡ����*/
    int Read(char *pBuf, const size_t &iLens) const;
    /*��д���д������*/
    int Write(const char *pBuf, const size_t &iLens) const;

private:
    int Init(void);
    void Destroy(void);
    int creatSockPair(Q_SOCK aeSock[2]) const;
    int creatListener(Q_SOCK &Listener) const;

private:
    Q_SOCK m_ReadFD;
    Q_SOCK m_WriteFD;
    CSockInit m_sockInit;
};

#endif//Q_SOCKPAIR_H_
