
#ifndef Q_SOCKPAIREVENT_H_
#define Q_SOCKPAIREVENT_H_

#include "SockPair.h"
#include "EventBuffer.h"
#include "Cond.h"
#include "InitEventBase.h"
#include "RunStatus.h"

/*
SockPair�¼�ѭ��
*/
class CSockPairEvent : public CRunStatus
{
public:
    CSockPairEvent(void);
    virtual ~CSockPairEvent(void);

    /*�����¼�ѭ��*/
    int Start(void);
    /*ֹͣ�¼�ѭ��*/
    void Stop(void);
    /*onStop�Ƿ�ִ��*/
    void setRunOnStop(bool bRun);
    bool getRunOnStop(void) const;

    /*д������*/
    int sendMainMsg(const char *pszBuff, const size_t &iSize) const;
    int sendAssistMsg(const char *pszBuff, const size_t &iSize) const;
    int sendTimerMsg(const char *pszBuff, const size_t &iSize) const;

    /*��ȡevent_base*/
    struct event_base *getBase(void)
    {
        return m_pBase;
    };

    CEventBuffer *getMainBuffer(void)
    {
        return &m_objMainBuffer;
    };
    CEventBuffer *getAssistBuffer(void)
    {
        return &m_objAssistBuffer;
    };
    CEventBuffer *getTimerBuffer(void)
    {
        return &m_objTimerBuffer;
    };

public:
    virtual void onMainRead(CEventBuffer *){};
    virtual void onAssistRead(CEventBuffer *){};
    virtual void onTimerRead(CEventBuffer *){};
    virtual void onStop(void)
    {
        setRunStatus(RUNSTATUS_STOPPED);
    };
    virtual bool onStartUp(void)
    {
        return true;
    };
public:
    static void mainReadCB(struct bufferevent *, void *arg);
    static void assistReadCB(struct bufferevent *, void *arg);
    static void timerReadCB(struct bufferevent *, void *arg);
    static void eventCB(struct bufferevent *, short event, void *arg);
    static void exitMonitorCB(evutil_socket_t, short, void *arg);

private:
    void freeAll(void);
    int initMainEvent(void);
    int initAssistEvent(void);
    int initTimerEvent(void);
    int initExitMonitor(unsigned int uiMS);

private:
    bool m_bRunOnStop;
    RunStatus m_emStatus;
    struct event_base *m_pBase;
    struct bufferevent *m_pMainBev;
    struct bufferevent *m_pAssistBev;
    struct bufferevent *m_pTimerBev;
    struct event *m_pExitEvent;
    CSockPair m_objMainSockPair;
    CSockPair m_objAssistSockPair;
    CSockPair m_objTimerSockPair;
    CEventBuffer m_objMainBuffer; 
    CEventBuffer m_objAssistBuffer; 
    CEventBuffer m_objTimerBuffer; 
    CQMutex m_Mutex;
    CCond m_Cond;
    CInitBase m_objInitBase;
};

/*��buffer�ж�ȡ.����ָ����Ҫ�ֶ�ɾ��*/
template<class T>
T *Q_GetEventValue(CEventBuffer *pBuffer)
{
    size_t iSize = sizeof(T);

    size_t iTotalSize = pBuffer->getTotalLens();
    if (iSize > iTotalSize)
    {
        return NULL;
    }

    T *pDate = new(std::nothrow) T();
    if (NULL == pDate)
    {
        Q_Printf("%s", Q_EXCEPTION_ALLOCMEMORY);

        return NULL;
    }

    char *pTmp = pBuffer->readBuffer(iSize);
    *pDate = *((T *)pTmp);
    (void)pBuffer->delBuffer(iSize);

    return pDate;
}

/*��buffer�ж�ȡ*/
template<class T>
bool Q_GetEventValue(CEventBuffer *pBuffer, T &Date)
{
    size_t iSize = sizeof(T);

    size_t iTotalSize = pBuffer->getTotalLens();
    if (iSize > iTotalSize)
    {
        return false;
    }

    char *pTmp = pBuffer->readBuffer(iSize);
    Date = *((T*)pTmp);
    (void)pBuffer->delBuffer(iSize);

    return true;
}

#endif//Q_SOCKPAIREVENT_H_
