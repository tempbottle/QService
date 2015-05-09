
#ifndef Q_EVENT_INTERFACE_H_ 
#define Q_EVENT_INTERFACE_H_

#include "Macros.h"

/*
�¼��������
*/
class CEventInterface
{
public:
    CEventInterface(void) : m_pSessionManager(NULL)
    {
    };

    virtual ~CEventInterface(void)
    {
        m_pSessionManager = NULL;
    };

    void setSessionManager(class CSessionManager *pSessionManager)
    {
        m_pSessionManager = pSessionManager;
    };

    class CSessionManager *getSessionManager(void)
    {
        return m_pSessionManager;
    };
    /*�����߳�����ʱִ��*/
    virtual bool onSerciveStartUp(void){return true;};
    /*�����̹߳ر�ʱִ��*/
    virtual void onSerciveShutDown(void){};

    /*��ʱ������ʱִ��*/
    virtual void onTimerEvent(void){};

    /*���ӳɹ�*/
    virtual void onConnected(class CSession *){};
    /*tcp��ȡ��������ʱִ��*/
    virtual void onSockRead(const char *, const size_t &){};
    /*socket�Ͽ�ʱִ��*/
    virtual void onSockClose(class CSession *){};  

    /*http */
    virtual void onHttpRead(class CHttpParser *){};

    /*��������������*/
    virtual void onLinkedOther(class CSession *){};

private:
    class CSessionManager *m_pSessionManager;
};

#endif//Q_EVENT_INTERFACE_H_
