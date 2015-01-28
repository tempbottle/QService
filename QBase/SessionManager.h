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
#include "Buffer.h"
#include "EventInterface.h"
#include "LuaBridge/LuaBridge.h"

/*session����*/
class CSessionManager
{
public:
    CSessionManager(void);
    ~CSessionManager(void);

    /*��ȡsession����*/
    size_t getSessionSize(void);
    /*��ȡ��������������*/
    int getGetSVLinkerNum(void);
    /*����ID�ر�����*/
    void closeLinkByID(const int iID);
    /*�رյ�ǰ����*/
    void closeCurLink(void);
    /*��ȡ��ǰ����session*/
    CSession *getCurSession(void);
    /*����ID��ȡsession*/
    CSession *getSessionByID(const int iID);
    /*��ȡ�����������ӵ�Session*/
    CSession *getServerLinkerSession(const char *pszName);
    /*ping ���(uiTime ��ʱʱ��(ms))*/
    void checkPing(const unsigned int uiTime);

    /*ֱ�ӷ��� ��ʽ unsigned short(��Ϣ�ĳ��� + ��Ϣ�볤��) + ������ + ��Ϣ*/
    bool sendToCur(const unsigned short usOpCode, const char *pszData, const size_t uiLens);
    bool sendToByID(const int iID, const unsigned short usOpCode, const char *pszData, const size_t uiLens);

    /*���õ�ǰ����������session*/ 
    void setCurSession(CSession *pSession);
    /*ɾ��session*/
    void dellSession(struct bufferevent *bev);
    /*��ʼ��һsession*/
    int addSession(struct bufferevent *bev);
    /*��ȡsession*/
    CSession *getSession(struct bufferevent *bev);
    /*��ӷ�����������*/
    void addServerLinker(const char *pszName, struct bufferevent *pBufEvent);
    /*ɾ��������������*/
    void delServerLinker(const char *pszName);
    /*�����¼��������*/
    void setInterface(class CEventInterface *pInterface);
    class CEventInterface *getInterface(void);
    /*�̱߳��*/
    void setThreadIndex(const short &sIndex);
    short getThreadIndex(void);
    /*��ʱ������ʱ��*/
    void setTimer(unsigned int &uiMS);
    unsigned int getTimer(void);
    /*��ʱ����������*/
    void addCount(void);
    unsigned int getCount(void);
    /*����lua�����*/
    void setLua(struct lua_State *pLua);

    /*����ָ�����͵ķ���������*/
    luabridge::LuaRef getSVLinkerNameByType(const int iType);
    /*�ж��Ƿ�Ϊָ�����͵ķ���������*/
    bool checkType(const int iType, const int iClientID);

private:
    /*�ͷ�����session*/
    void freeAllSession(void);
    bool sendWithHead(CSession *pCurrent, 
        const unsigned short &usOpCode, const char *pszData, const size_t &uiLens);

private:
    short m_sThreadIndex;
    unsigned int m_uiTimer;
    unsigned int m_uiCount;
    struct lua_State *m_pLua;
    CSession *m_pCurrent; //��ǰSession
    class CEventInterface *m_pInterface;
    std::tr1::unordered_map<int, CSession *> m_unmapSession;//����Session
    std::queue<CSession *> m_quFreeSession;//���е�session
    std::tr1::unordered_map<std::string, bufferevent* > m_mapServerLinker;
};

#endif//Q_SESSION_MANAGER_H_
