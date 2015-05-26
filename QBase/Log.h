
#ifndef Q_LOG_H_
#define Q_LOG_H_

#include "SockPairEvent.h"
#include "TcpParser.h"
#include "Loger.h"
#include "Singleton.h"
#include "CTask.h"

struct LogerInfo;
//��־
class CLog : public CSockPairEvent, 
    public CTask,
    public CSingleton<CLog>
{
public:
    CLog(void);
    ~CLog(void);

    //���һ��־��¼��,����һ������д�ľ��
    void addLoger(CLoger *pLoger);

public:
    //����ӿ�ʵ��
    void Run(void)
    {
        Q_Printf("%s", "log service running...");
        Start();
    };

public:
    /*�ӿ�ʵ��*/
    void onMainRead(CEventBuffer *pBuffer);
    void onStop(void);

public:
    static void LogerReadCB(struct bufferevent *, void *arg);
    static void timerCB(evutil_socket_t, short, void *arg);

private:
    int setTimer(unsigned int uiMS);

private:
    struct event *m_pTimeEvent;
    CQMutex m_objMutex;
    std::list<LogerInfo *> m_lstLoger;
    CTcpParser m_objTcpParser;
};

#endif//Q_LOG_H_
