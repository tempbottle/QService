
#ifndef Q_THREAD_H_
#define Q_THREAD_H_

#include "Macros.h"
#include "CTask.h"

/*�߳���*/
class CThread
{
public:
    CThread(void);
    ~CThread(void);

    /*����һ�߳�ִ������*/
    void Execute(CTask *pTask, bool bDel = true);

    /*����һ�߳�ִ������*/
    void Execute(CTask *pTask, Q_Thread_t *pThread, bool bDel = true);    

private:
    int attrInit(Q_Thread_Attr_t *pAttr) const;
    int attrDestroy(Q_Thread_Attr_t *pAttr) const;
    int attrSetDetaChstate(Q_Thread_Attr_t *pAttr, int iDetachstate) const;
    int Create(Q_Thread_t *pThread, Q_Thread_Attr_t *pAttr,
        Q_Thread_Func_t myfunc, void *args) const;
    int Init(void);
    void Destroy(void);

private:
    Q_Thread_Attr_t m_Attr;
};

/*��ȡ�߳�ID*/
Q_Thread_t Q_ThreadID(void);
/*����(ms)*/
void Q_Sleep(const unsigned int iMsec);

#endif//Q_THREAD_H_
