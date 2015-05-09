
#ifndef Q_MUTEX_H_
#define Q_MUTEX_H_

#include "Macros.h"

/*��������,ʧ�����쳣�׳�*/
class CQMutex
{
public:
    CQMutex(void);
    ~CQMutex(void);
    
    /*����*/
    void Lock(void);
    /*����*/
    void unLock(void);

    /*����Q_Mutex_t,��Ҫ�ź���ʱ��Ҫ��*/
    Q_Mutex_t *getMutex(void);

private:
    int Init(void);
    void Destroy(void);

private:
    Q_Mutex_t m_Mutex;
};

#endif//Q_MUTEX_H_
