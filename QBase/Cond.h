
#ifndef Q_COND_H_
#define Q_COND_H_

#include "Mutex.h"

/*�ź�����,ʧ�����쳣�׳�*/
class CCond
{
public:
    CCond(void);
    ~CCond(void);

    /*�ȴ��ź���*/
    void Wait(CQMutex *pMutex);
    /*�ȴ��ź�������ָ����ʱʱ�䣬��λ����*/
    void Wait(CQMutex *pMutex, const unsigned int uimSec);

    /*�����ź���*/
    void Signal(void);

private:
    int Init(void);
    void Destroy(void);

private:
    Q_Cond_t m_Cond;
};

#endif//Q_COND_H_
