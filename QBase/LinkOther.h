
#ifndef Q_LINKOTHER_H_
#define Q_LINKOTHER_H_

#include "InitSock.h"
#include "Mutex.h"

//���ӵ�����
class CLinkOther
{
public:
    CLinkOther(void);
    ~CLinkOther(void);

    //����CWorkThreadEvent����
    void setThreadEvent(class CWorkThreadEvent *pThreadEvent);
    class CWorkThreadEvent *getThreadEvent(void);

    //��������״̬
    void setStatus(const RunStatus emStatus);
    RunStatus getStatus(void) const;

    //����Ҫ���ӵ�������Ϣ
    bool addHost(const char *pszIp, unsigned short usPort, const char *pszName);

    //��ʼ��ֹͣ
    bool Start(void);
    void Stop(void);

    //����
    void Link(void);

    //������������״̬
    void setSockStatus(Q_SOCK sock, SessionStatus emStatus);
    
    //�������ƻ�ȡ����ID
    int getSockByName(const char *pszName);

private:
    struct LinkInfo
    {
        SessionStatus emStatus;
        unsigned short usPort;
        Q_SOCK sock;
        std::string strIp;
        std::string strName;
        void Clear(void)
        {
            emStatus = SessionStatus_Closed;
            sock = Q_INVALID_SOCK;
        };
    };

private:
    Q_SOCK initSock(const char *pszIp, const unsigned short &usPort) const;
    bool checkHaveHost(const char *pszName);

private:
    RunStatus m_emStatus;
    class CWorkThreadEvent *m_pThreadEvent;
    std::vector<LinkInfo> m_vcLinkInfo;
    CQMutex m_objMutex;
};

#endif//Q_LINKOTHER_H_
