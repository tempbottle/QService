
#ifndef Q_THREADPOOL_H_
#define Q_THREADPOOL_H_

#include "Thread.h"

/*�̳߳���*/
class CThreadPool
{
public:
    /*uiThreadNum �̳߳ذ������߳���*/
    explicit CThreadPool(const unsigned int uiThreadNum);
    ~CThreadPool(void);

    /*���̳߳�����������,����ָ����ִ����ɺ��Զ�ɾ��*/
    void Append(CTask *pTask, TaskLevel taskLV = Q_ThreadLV_Normal);
    /*��ȡ�ȴ���������������*/
    unsigned int getTaskNumber(void);
    /*��ȡ�̳߳ش�С*/
    unsigned int getPoolSize(void) const;

private:
    CThreadPool(void);
    int Init(const unsigned int &uiThreadNum);
    void Destroy(void);

private:
    struct ThreadPool *m_pstPool;
};

#endif//Q_THREADPOOL_H_
