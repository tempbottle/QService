
#include "RWLock.h"

#ifdef Q_OS_WIN32

CRWLock::CRWLock(void)
{
    m_lReader_Count = 0; 
    m_bEnter_Writer = false;

    //��ʼ���ٽ���
    InitializeCriticalSection(&m_Write_Lock);
    InitializeCriticalSection(&m_Read_Lock);
}

CRWLock::~CRWLock(void)
{
    //ɾ���ٽ���
    DeleteCriticalSection(&m_Write_Lock);
    DeleteCriticalSection(&m_Read_Lock);
}

void CRWLock::readLock(void)
{
    //�����ٽ���
    EnterCriticalSection(&m_Write_Lock);
    if(ReadLock_Lock == ::InterlockedIncrement(&m_lReader_Count))//ԭ���Լ� ,�������δ�������������ٽ���
    {
        EnterCriticalSection(&m_Read_Lock);
    }

    //�뿪�ٽ���
    LeaveCriticalSection(&m_Write_Lock);
}

void CRWLock::writeLock(void)
{
    EnterCriticalSection(&m_Write_Lock);
    EnterCriticalSection(&m_Read_Lock);

    m_bEnter_Writer = true;
}

void CRWLock::unLock(void)
{
    if(m_bEnter_Writer)
    {
        m_bEnter_Writer = false;

        LeaveCriticalSection(&m_Read_Lock);
        LeaveCriticalSection(&m_Write_Lock);
    }
    else
    {
        if(ReadLock_UnLock == ::InterlockedDecrement(&m_lReader_Count))//ԭ���Լ�,���ж����뿪������ٽ���
        {
            LeaveCriticalSection(&m_Read_Lock);
        }
    }
}

#else

#include "Exception.h"

CRWLock::CRWLock(void)
{
    int iRtn = Q_RTN_OK;

    iRtn = pthread_rwlock_init(&m_RWLock, NULL);
    if (Q_RTN_OK != iRtn)
    {
        Q_EXCEPTION(iRtn, "%s", Q_Error2Str(iRtn));
    }
}

CRWLock::~CRWLock(void)
{
    (void)pthread_rwlock_destroy(&m_RWLock);
}

void CRWLock::readLock(void)
{
    int iRtn = Q_RTN_OK;

    iRtn = pthread_rwlock_rdlock(&m_RWLock);
    if (Q_RTN_OK != iRtn)
    {
        Q_EXCEPTION(iRtn, "%s", Q_Error2Str(iRtn));
    }
}

void CRWLock::writeLock(void)
{
    int iRtn = Q_RTN_OK;

    iRtn = pthread_rwlock_wrlock(&m_RWLock);
    if (Q_RTN_OK != iRtn)
    {
        Q_EXCEPTION(iRtn, "%s", Q_Error2Str(iRtn));
    }
}

void CRWLock::unLock(void)
{
    int iRtn = Q_RTN_OK;

    iRtn = pthread_rwlock_unlock(&m_RWLock);
    if (Q_RTN_OK != iRtn)
    {
        Q_EXCEPTION(iRtn, "%s", Q_Error2Str(iRtn));
    }
}

#endif
