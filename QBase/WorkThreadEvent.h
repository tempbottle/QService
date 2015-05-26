
#ifndef Q_SERVERTHREAD_EVENT_H_
#define Q_SERVERTHREAD_EVENT_H_

#include "SockPairEvent.h"
#include "WebSockParser.h"
#include "TcpParser.h"
#include "HttpParser.h"
#include "Singleton.h"
#include "CTask.h"

struct TriggerSock
{
    SessionType emType;
    Q_SOCK iSock;
    TriggerSock(void) : emType(STYPE_TCP), iSock(Q_INVALID_SOCK)
    {

    };
};

/*
* �����߳��¼�
* ͨ�Ÿ�ʽ unsigned short(��Ϣ����) + ��Ϣ
*/
class CWorkThreadEvent : public CSockPairEvent,
    public CTask, 
    public CSingleton<CWorkThreadEvent>
{
public:
    CWorkThreadEvent(void);
    ~CWorkThreadEvent(void);

    //http
    int setHttpSock(std::vector<Q_SOCK> &vcHttpSock);

    CWebSockParser *getWebSockParser(void);
    CTcpParser *getTcpParser(void);
    CHttpParser *getHttpParser(void);

    void Run(void)
    {
        Q_Printf("%s", "worker service running...");
        Start();
    };

public:
    //�ӿ�ʵ��
    void onMainRead(CEventBuffer *);
    void onAssistRead(CEventBuffer *);
    void onTimerRead(CEventBuffer *);
    void onStop(void);
    bool onStartUp(void);

public:
    static void workThreadReadCB(struct bufferevent *bev, void *arg);
    static void workThreadEventCB(struct bufferevent *bev, short, void *arg);
    static void workThreadHttpCB(struct evhttp_request *req, void *arg);

    //��ͬ����Э���ȡ
    static void dispTcp(class CSession *pSession);
    static void dispWebSock(class CSession *pSession);

public:
    //websocket ��Ƭ֡����
    void addContinuation(const Q_SOCK &sock, const char *pszData, const size_t &iLens);
    std::string *getContinuation(const Q_SOCK &sock);
    void delContinuation(const Q_SOCK &sock);

private:
    TriggerSock m_stWorkSock;
    CWebSockParser m_objWebSockParser;
    CTcpParser m_objTcpParser;
    CHttpParser m_objHttpParser;
    std::vector<struct evhttp *> m_vcEvHttp;
    std::tr1::unordered_map<Q_SOCK, std::string> m_mapWebSockPack;
};

#endif//Q_SERVERTHREAD_EVENT_H_
