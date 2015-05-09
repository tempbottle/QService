
#ifndef Q_SESSION_MANAGER_H_
#define Q_SESSION_MANAGER_H_

#include "Session.h"
#include "EventInterface.h"
#include "WebSockParser.h"
#include "TcpParser.h"
#include "LinkOther.h"
#include "Binary.h"

/*session����*/
class CSessionManager
{
public:
    CSessionManager(void);
    ~CSessionManager(void);

    /*����ID�ر�����*/
    void closeLinkByID(const int iID);
    /*�رյ�ǰ����*/
    void closeCurLink(void);
    /*��ȡ��ǰ����session*/
    CSession *getCurSession(void);
    /*����ID��ȡsession*/
    CSession *getSessionByID(const int iID);
    /*ȷ��ֹͣ����*/
    void confirmStop(void);
    /*�������ӵ�������������TCP����*/
    bool addLinkOther(const char *pszIp, const unsigned short usPort, const char *pszNam);
    /*�������ƻ�ȡ����*/
    int getLinkOtherID(const char *pszName);
    /*��ȡsession����*/
    size_t getSessionSize(void) const;

    /*����*/
    bool sendToCur(const char *pszData, const size_t uiLens);
    bool sendToByID(const int iID, const char *pszData, const size_t uiLens);
    /*����m_objBinary�е�����  Lua�ж�Ӧ��g_objBinary*/
    bool sendBToCur(void);
    bool sendBToByID(const int iID);

    /*���õ�ǰ����������session*/ 
    void setCurSession(CSession *pSession);
    /*ɾ��session*/
    void dellSession(struct bufferevent *bev);
    /*��ʼ��һsession*/
    int addSession(struct bufferevent *bev);
    /*��ȡsession*/
    CSession *getSession(struct bufferevent *bev);
    /*�����¼��������*/
    void setInterface(class CEventInterface *pInterface);
    class CEventInterface *getInterface(void);
    /*��ʱ������ʱ��*/
    void setTimer(const unsigned int &uiMS);
    unsigned int getTimer(void) const;
    /*��ʱ����������*/
    void addCount(void);
    unsigned int getCount(void) const;
    /*����lua�����*/
    void setLua(struct lua_State *pLua);
    /*���ù����߳�ָ��*/
    void setWorkThread(class CWorkThreadEvent *pThread);

    CLinkOther *getLinkOther(void)
    {
        return &m_objLinkOther;
    };

    CLuaBinary *getBinary(void)
    {
        return &m_objBinary;
    };
private:
    /*�ͷ�����session*/
    void freeAllSession(void);
    bool sendTcp(CSession *pCurrent, const char *pszData, const size_t &uiLens);
    bool sendWebSock(CSession *pCurrent, const char *pszData, const size_t &uiLens);

private:
    unsigned int m_uiTimer;
    unsigned int m_uiCount;
    struct lua_State *m_pLua;
    CSession *m_pCurrent; //��ǰSession
    class CEventInterface *m_pInterface;
    class CWorkThreadEvent *m_pWorkThread;
    std::tr1::unordered_map<int, CSession *> m_umapSession;//����Session
    std::queue<CSession *> m_quFreeSession;//���е�session
    std::tr1::unordered_map<std::string, int> m_umapLinkOther;
    CWebSockParser m_objWebSockParser;
    CTcpParser m_objTcpParser;
    CLinkOther m_objLinkOther;
    CLuaBinary m_objBinary;    
};

#endif//Q_SESSION_MANAGER_H_
