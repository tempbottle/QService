
#ifndef Q_NETEVENT_DISPOSE_H_
#define Q_NETEVENT_DISPOSE_H_

#include "Reg2Lua.h"

class CDisposeEvent : public CEventInterface
{
public:
    CDisposeEvent(const char *pszLuaFile);
    ~CDisposeEvent(void);

    /*�����߳�����ʱִ��*/
    bool onSerciveStartUp(void);
    /*�����̹߳ر�ʱִ��*/
    void onSerciveShutDown(void);

    /*��ʱ������ʱִ��*/
    void onTimerEvent(void);    
    
    /*���ӳɹ�*/
    void onConnected(class CSession *pSession);
    /*socket��ȡ��������ʱִ��*/
    void onSockRead(const char *pszMsg, const size_t &iLens);
    /*socket�Ͽ�ʱִ��*/
    void onSockClose(class CSession *pSession);

    /*http */
    void onHttpRead(class CHttpParser *pBuffer);

    /*��������������*/
    void onLinkedOther(class CSession *pSession);

private:
    CDisposeEvent(void);

private:
    struct lua_State *m_pLua;//lua���
    CReg2Lua m_objReg2Lua;
};

#endif//Q_NETEVENT_DISPOSE_H_
