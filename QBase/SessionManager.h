/*****************************************************************************
 * Copyright (c) 2011-2012. Qifu Luo All Rights Reserved.200309129@163.com 
 * svn:http://asuraserver.googlecode.com/svn/
 * github:https://github.com/QService/QService
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#ifndef Q_SESSION_MANAGER_H_
#define Q_SESSION_MANAGER_H_

#include "Session.h"
#include "EventInterface.h"
#include "WebSockParser.h"
#include "TcpParser.h"
#include "LinkOther.h"
#include "Binary.h"
#include "LuaBridge/LuaBridge.h"

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
    /*ping ���(uiTime ��ʱʱ��(ms))*/
    void checkPing(const unsigned int uiTime);
    /*ȷ��ֹͣ����*/
    void confirmStop(void);
    /*�������ƻ�ȡ����*/
    int getLinkOtherID(const char *pszName);
    /*��ȡsession����*/
    size_t getSessionSize(void);

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
    void setTimer(unsigned int &uiMS);
    unsigned int getTimer(void);
    /*��ʱ����������*/
    void addCount(void);
    unsigned int getCount(void);
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
    CWebSockParser m_objWebSockParser;
    CTcpParser m_objTcpParser;
    CLinkOther m_objLinkOther;
    CLuaBinary m_objBinary;
    std::tr1::unordered_map<std::string, int> m_umapLinkOther;
};

#endif//Q_SESSION_MANAGER_H_
