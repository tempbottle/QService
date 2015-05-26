
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

    /*���̳߳�����������*/
    void Append(CTask *pTask, TaskLevel taskLV = TASKLV_NORMAL, const bool bDel = true);
    /*��ȡ�ȴ���������������*/
    size_t getTaskNumber(void);
    /*��ȡ�̳߳ش�С*/
    size_t getPoolSize(void) const;

private:
    CThreadPool(void);
    int Init(const unsigned int &uiThreadNum);
    void Destroy(void);

private:
    struct ThreadPool *m_pstPool;
};

#endif//Q_THREADPOOL_H_
