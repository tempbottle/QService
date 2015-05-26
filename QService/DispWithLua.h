
#ifndef Q_NETEVENT_DISPOSE_H_
#define Q_NETEVENT_DISPOSE_H_

#include "Reg2Lua.h"

class CDisposeEvent : public CEventInterface,
    public CSingleton<CDisposeEvent>
{
public:
    CDisposeEvent(void);
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
    /*debug*/
    void onDebug(const char *pszMsg, const size_t &iLens);
    /*socket�Ͽ�ʱִ��*/
    void onSockClose(class CSession *pSession);

    /*http */
    void onHttpRead(class CHttpParser *pBuffer);

    /*��������������*/
    void onLinkedOther(class CSession *pSession);

    int initLua(const std::string &strLua);

    struct lua_State *getLua(void)
    {
        return m_pLua;
    };

private:
    struct lua_State *m_pLua;//lua���
    CReg2Lua m_objReg2Lua;
};

#endif//Q_NETEVENT_DISPOSE_H_
