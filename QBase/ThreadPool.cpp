
#include "ThreadPool.h"
#include "Cond.h"
#include "Exception.h"

#define Q_THREADPOOL_CONDTIMEOUT   1000

enum ThRunStatus
{
    RUN = 0,
    STOP
};

struct ThreadPool
{
    unsigned int uiThreadNum;//�߳���
    CQMutex objQueueMutex;//��Ϣ������
    CQMutex objMutex_Thread;//�߳̽���
    CCond objCond_Thread;
    CQMutex objMutex_Main;//���߳̽���
    CCond objCond_Main;
    long lStop;//ֹͣ��־
    long lIndex;//���е��߳���
    std::queue<CTask *> quHigh;
    std::queue<CTask *> quNormal;
    std::queue<CTask *> quLow;
};

class CThreadPoolTask : public CTask
{
public:
    CThreadPoolTask(ThreadPool *pPool) : m_pPool(NULL)
    {
        m_pPool = pPool;
    };

    ~CThreadPoolTask(void) 
    {
        m_pPool = NULL;
    };

    void Run(void)
    {
        CTask *pTask = NULL;
        Q_AtomicAdd(&(m_pPool->lIndex), 1);

        m_pPool->objMutex_Main.Lock();
        m_pPool->objCond_Main.Signal();
        m_pPool->objMutex_Main.unLock();

        while(RUN == Q_AtomicGet(&(m_pPool->lStop)))
        {
            m_pPool->objMutex_Thread.Lock();
            m_pPool->objCond_Thread.Wait(&(m_pPool->objMutex_Thread), Q_THREADPOOL_CONDTIMEOUT);
            m_pPool->objMutex_Thread.unLock();

            while(NULL != (pTask = getTask()))
            {
                pTask->Run();

                if (pTask->getDel())
                {
                    Q_SafeDelete(pTask);
                }                
            }
        }

        Q_AtomicAdd(&(m_pPool->lIndex), -1);

        m_pPool->objMutex_Main.Lock();
        m_pPool->objCond_Main.Signal();
        m_pPool->objMutex_Main.unLock();
    };

private:
    CThreadPoolTask(void);
    CTask *getTaskFromQueue(std::queue<CTask *> &taskQueue)
    {
        CTask *pTask = NULL;

        m_pPool->objQueueMutex.Lock();
        if (!(taskQueue.empty()))
        {
            pTask = taskQueue.front();
            taskQueue.pop();
        }
        m_pPool->objQueueMutex.unLock();

        return pTask;
    };

    CTask *getTask(void)
    {
        CTask *pTask = NULL;

        pTask = getTaskFromQueue(m_pPool->quHigh);
        if (NULL != pTask)
        {
            return pTask;
        }

        pTask = getTaskFromQueue(m_pPool->quNormal);
        if (NULL != pTask)
        {
            return pTask;
        }

        pTask = getTaskFromQueue(m_pPool->quLow);
        if (NULL != pTask)
        {
            return pTask;
        }

        return NULL;
    }

private:
    ThreadPool *m_pPool;
};

CThreadPool::CThreadPool(const unsigned int uiThreadNum) : m_pstPool(NULL)
{
    int iRtn = Init(uiThreadNum);
    if (Q_RTN_OK != iRtn)
    {
        Destroy();

        Q_EXCEPTION(iRtn, "%s", "init thread pool error.");
    }
}

CThreadPool::~CThreadPool(void)
{
    try
    {
        Destroy();
    }
    catch(...)
    {

    }
}

int CThreadPool::Init(const unsigned int &uiThreadNum)
{
    //��ʼ��m_pstPool
    m_pstPool = new(std::nothrow) ThreadPool();
    if (NULL == m_pstPool)
    {
        Q_Printf("%s", Q_EXCEPTION_ALLOCMEMORY);
        
        return Q_ERROR_ALLOCMEMORY;
    }

    m_pstPool->uiThreadNum = uiThreadNum;
    if (0 == uiThreadNum)
    {
        m_pstPool->uiThreadNum = 1;
    }

    m_pstPool->lIndex = Q_INIT_NUMBER;
    m_pstPool->lStop = RUN;    

    //����ָ���������߳�
    try
    {
        CThread objThread;

        for (unsigned int i = 0; i < m_pstPool->uiThreadNum; i++)
        {
            CThreadPoolTask *pTask = new(std::nothrow) CThreadPoolTask(m_pstPool);
            if (NULL == pTask)
            {
                Q_Printf("%s", Q_EXCEPTION_ALLOCMEMORY);

                return Q_ERROR_ALLOCMEMORY;
            }

            m_pstPool->objMutex_Main.Lock();
            objThread.Execute(pTask);
            m_pstPool->objCond_Main.Wait(&(m_pstPool->objMutex_Main), Q_THREADPOOL_CONDTIMEOUT);
            m_pstPool->objMutex_Main.unLock();
        }
    }
    catch(CQException &e)
    {
        Q_Printf("run thread exception, code %d, message %s", 
            e.getErrorCode(), e.getErrorMsg());

        return e.getErrorCode();
    }

    return Q_RTN_OK;
}

void CThreadPool::Destroy(void)
{
    if (NULL == m_pstPool)
    {
        return;
    }

    unsigned int uiTimeCount = Q_INIT_NUMBER;
    size_t uiTaskNum = Q_INIT_NUMBER;

    /*�ȴ������е����������*/
    while(0 != (uiTaskNum = getTaskNumber()))
    {
        Q_Sleep(500);
        uiTimeCount += 500;

        if (uiTimeCount >= 5000)
        {
            Q_Printf("wait %d task run over.", uiTaskNum);
            uiTimeCount = 0;
        }
    }

    Q_AtomicSet(&(m_pstPool->lStop), STOP);

    m_pstPool->objMutex_Thread.Lock();
    m_pstPool->objCond_Thread.Signal();
    m_pstPool->objMutex_Thread.unLock();

    /*�ȴ������߳��˳�*/
    while(Q_AtomicGet(&(m_pstPool->lIndex)) > 0)
    {
        m_pstPool->objMutex_Thread.Lock();
        m_pstPool->objCond_Thread.Signal();
        m_pstPool->objMutex_Thread.unLock();

        m_pstPool->objMutex_Main.Lock();
        m_pstPool->objCond_Main.Wait(&(m_pstPool->objMutex_Main), Q_THREADPOOL_CONDTIMEOUT);
        m_pstPool->objMutex_Main.unLock();
    }

    Q_SafeDelete(m_pstPool);

    return;
}

size_t CThreadPool::getTaskNumber(void)
{
    size_t uiSize = Q_INIT_NUMBER;

    m_pstPool->objQueueMutex.Lock();
    uiSize += m_pstPool->quHigh.size();
    uiSize += m_pstPool->quNormal.size();
    uiSize += m_pstPool->quLow.size();
    m_pstPool->objQueueMutex.unLock();

    return uiSize;
}

size_t CThreadPool::getPoolSize(void) const
{
    return m_pstPool->uiThreadNum;
}

/************************************************************************
* Function name:Append
* Description  :���̳߳�����������,����ָ����ִ����ɺ��Զ�ɾ��
* IN           :pTask --���� taskLV --����
* OUT          :NONE
* Return       :NONE
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
void CThreadPool::Append(CTask *pTask, TaskLevel taskLV, const bool bDel)
{
    if(NULL == pTask)
    {
        Q_Printf("%s", Q_EXCEPTION_NULLPOINTER);

        return;
    }

    pTask->setDel(bDel);

    m_pstPool->objQueueMutex.Lock();
    switch(taskLV)
    {
    case TASKLV_HIGHT:
        m_pstPool->quHigh.push(pTask);            
        break;

    case TASKLV_NORMAL:
        m_pstPool->quNormal.push(pTask);        
        break;

    case TASKLV_LOW:
        m_pstPool->quLow.push(pTask);
        break;

    default:
        break;
    }
    m_pstPool->objQueueMutex.unLock();

    m_pstPool->objMutex_Thread.Lock();
    m_pstPool->objCond_Thread.Signal();
    m_pstPool->objMutex_Thread.unLock();

    return;
}
