
#ifndef Q_SERVERTHREAD_EVENT_H_
#define Q_SERVERTHREAD_EVENT_H_

#include "SockPairEvent.h"
#include "SessionManager.h"
#include "WebSockParser.h"
#include "TcpParser.h"
#include "HttpParser.h"

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
class CWorkThreadEvent : public CSockPairEvent
{
public:
    CWorkThreadEvent(void);
    ~CWorkThreadEvent(void);

    /*���ýӿڶ���*/
    void setInterface(CEventInterface *pInterface);
    /*��ʱ��ʱ��(ms)*/
    int setTimer(unsigned int uiMS);
    int setHttpSock(std::vector<Q_SOCK> &vcHttpSock);

    /*��ȡ���¼�ѭ��ά����CSessionManager*/
    CSessionManager *getSessionManager(void);
    /*��ȡWebSockParser*/
    CWebSockParser *getWebSockParser(void);
    /*��ȡCTcpParser*/
    CTcpParser *getTcpParser(void);
    /*��ȡ���ӵ���������������*/
    CLinkOther *getLinkOther(void);
    CHttpParser *getHttpParser(void);

public:
    //�ӿ�ʵ��
    void onMainRead(CEventBuffer *);
    void onAssistRead(CEventBuffer *);
    void onStop(void);
    bool onStartUp(void);

public:
    static void workThreadReadCB(struct bufferevent *bev, void *arg);
    static void workThreadEventCB(struct bufferevent *bev, short, void *arg);
    static void workThreadTimerCB(evutil_socket_t, short, void *arg);
    static void workThreadHttpCB(struct evhttp_request *req, void *arg);

    //��ͬ����Э���ȡ
    static void dispTcp(CWorkThreadEvent *pWorkThreadEvent,
        CSessionManager *pSessionManager, CSession *pSession);
    static void dispWebSock(CWorkThreadEvent *pWorkThreadEvent,
        CSessionManager *pSessionManager, CSession *pSession);

public:
    //websocket ��Ƭ֡����
    void addContinuation(const Q_SOCK &sock, const char *pszData, const size_t &iLens);
    std::string *getContinuation(const Q_SOCK &sock);
    void delContinuation(const Q_SOCK &sock);

private:
    struct event *m_pEvent;
    CSessionManager m_objSessionManager;
    TriggerSock m_stWorkSock;
    CWebSockParser m_objWebSockParser;
    CTcpParser m_objTcpParser;
    CHttpParser m_objHttpParser;
    std::vector<struct evhttp *> m_vcEvHttp;
    std::tr1::unordered_map<Q_SOCK, std::string> m_mapWebSockPack;
};

#endif//Q_SERVERTHREAD_EVENT_H_
