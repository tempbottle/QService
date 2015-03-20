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

#include "SessionManager.h"
#include "Exception.h"
#include "ServerLinker.h"

#define Q_INITSESSIONSIZE 2000

CSessionManager::CSessionManager(void) : m_sThreadIndex(-1), m_uiTimer(Q_INIT_NUMBER),
    m_uiCount(Q_INIT_NUMBER), m_pLua(NULL), m_pCurrent(NULL), m_pInterface(NULL)
{
    for (int i = 0; i < Q_INITSESSIONSIZE; i++)
    {
        CSession *pSession = new(std::nothrow) CSession();
        if (NULL == pSession)
        {
            Q_Printf("%s", Q_EXCEPTION_ALLOCMEMORY);

            break;
        }

        m_quFreeSession.push(pSession);
    }

    m_objWebSock.setSessionMgr(this);
}

CSessionManager::~CSessionManager(void)
{
    freeAllSession();
}

void CSessionManager::setTimer(unsigned int &uiMS)
{
    m_uiTimer = uiMS;
}

unsigned int CSessionManager::getTimer(void)
{
    return m_uiTimer;
}

void CSessionManager::addCount(void)
{
    m_uiCount++;
}

unsigned int CSessionManager::getCount(void)
{
    return m_uiCount;
}

void CSessionManager::setLua(struct lua_State *pLua)
{
    m_pLua = pLua;
}

void CSessionManager::setThreadIndex(const short &sIndex)
{
    m_sThreadIndex = sIndex; 
}

short CSessionManager::getThreadIndex(void)
{
    return m_sThreadIndex;
}

void CSessionManager::setCurSession(CSession *pSession)
{
    m_pCurrent = pSession;
}

void CSessionManager::dellSession(struct bufferevent *bev)
{
    std::tr1::unordered_map<int, CSession *>::iterator itSession;    
    evutil_socket_t fd = bufferevent_getfd(bev);

    itSession = m_unmapSession.find(fd);
    if (m_unmapSession.end() == itSession)
    {
        return;
    }

    CSession *pSession = itSession->second;
    if (NULL == pSession)
    {
        m_unmapSession.erase(itSession);

        return;
    }

    pSession->Clear();
    m_quFreeSession.push(pSession);//�ӽ������б�
    m_unmapSession.erase(itSession);

    return;
}

int CSessionManager::addSession(struct bufferevent *bev)
{
    int iRtn = Q_RTN_OK;
    CSession *pSession = NULL;

    if (m_quFreeSession.empty())
    {
        pSession = new(std::nothrow) CSession();
        if (NULL == pSession)
        {
            Q_Printf("%s", Q_EXCEPTION_ALLOCMEMORY);

            return Q_ERROR_ALLOCMEMORY;
        }
    }
    else
    {
        pSession = m_quFreeSession.front();
        m_quFreeSession.pop();
    }
    
    iRtn = pSession->getBuffer()->setBuffer(bev);
    if (Q_RTN_OK != iRtn)
    {
        pSession->Clear();
        m_quFreeSession.push(pSession);

        return iRtn;
    }

    Q_SOCK fd = pSession->getBuffer()->getFD();
    pSession->setSessionID(fd);
    pSession->setPing(getCount());

    m_unmapSession[fd] = pSession;

    return Q_RTN_OK;
}

CSession *CSessionManager::getSession(struct bufferevent *bev)
{
    evutil_socket_t fd = bufferevent_getfd(bev);
    if (Q_INVALID_SOCK == fd)
    {
        return NULL;
    }

    std::tr1::unordered_map<int, CSession *>::iterator itSession;
    itSession = m_unmapSession.find(fd);
    if (m_unmapSession.end() == itSession)
    {
        return NULL;
    }

    return itSession->second;
}

void CSessionManager::addServerLinker(const char *pszName, struct bufferevent *pBufEvent)
{
    std::tr1::unordered_map<std::string, bufferevent* >::iterator itServerLinker;
    std::string strName(pszName);

    itServerLinker = m_mapServerLinker.find(strName);
    if (m_mapServerLinker.end() == itServerLinker)
    {
        m_mapServerLinker.insert(std::make_pair(strName, pBufEvent));
    }
    else
    {
        itServerLinker->second = pBufEvent;
    }
}

void CSessionManager::delServerLinker(const char *pszName)
{
    std::tr1::unordered_map<std::string, bufferevent* >::iterator itServerLinker;

    itServerLinker = m_mapServerLinker.find(std::string(pszName));
    if (m_mapServerLinker.end() != itServerLinker)
    {
        m_mapServerLinker.erase(itServerLinker);
    }
}

CSession *CSessionManager::getServerLinkerSession(const char *pszName)
{
    std::tr1::unordered_map<std::string, bufferevent* >::iterator itServerLinker;

    itServerLinker = m_mapServerLinker.find(std::string(pszName));
    if (m_mapServerLinker.end() == itServerLinker)
    {
        return NULL;
    }

    return getSession(itServerLinker->second);
}

void CSessionManager::checkPing(const unsigned int uiTime)
{
    std::tr1::unordered_map<int, CSession *>::iterator itMap;
    std::vector<int> vcTimeOut;
    std::vector<int>::iterator itTimeOut;

    for (itMap = m_unmapSession.begin(); m_unmapSession.end() != itMap; itMap++)
    {
        if (SType_TcpClient != itMap->second->getType())
        {
            continue;
        }

        if ((getCount() - itMap->second->getPing())*getTimer() >= uiTime)
        {
            vcTimeOut.push_back(itMap->first);
        }
    }

    for (itTimeOut = vcTimeOut.begin(); vcTimeOut.end() != itTimeOut; itTimeOut++)
    {
        closeLinkByID(*itTimeOut);
    }
}

size_t CSessionManager::getSessionSize(void)
{
    return m_unmapSession.size();
}

void CSessionManager::freeAllSession(void)
{
    std::tr1::unordered_map<int, CSession *>::iterator itSession;
    CSession *pSession = NULL;

    for (itSession = m_unmapSession.begin(); m_unmapSession.end() != itSession; itSession++)
    {
        bufferevent_free(itSession->second->getBuffer()->getBuffer());
        Q_SafeDelete(itSession->second);
    }

    while(!m_quFreeSession.empty())
    {
        pSession = m_quFreeSession.front();
        m_quFreeSession.pop();
        Q_SafeDelete(pSession);
    }

    m_unmapSession.clear();

    return;
}

void CSessionManager::closeLinkByID(const int iID)
{
    std::tr1::unordered_map<int, CSession *>::iterator itSession;

    itSession = m_unmapSession.find(iID);
    if (m_unmapSession.end() == itSession)
    {
        return;
    }

    setCurSession(itSession->second);
    m_pInterface->onSocketClose();
    setCurSession(NULL);

    CServerLinker *pLink = NULL;
    if (SType_SVLinker == itSession->second->getType())
    {
        pLink = (CServerLinker *)(itSession->second->getHandle());
    }
    if (SType_WebSock == itSession->second->getType())
    {
        m_objWebSock.delContinuation(iID);
    }

    bufferevent_free(itSession->second->getBuffer()->getBuffer());
    itSession->second->Clear();

    m_quFreeSession.push(itSession->second);
    m_unmapSession.erase(itSession);

    if (NULL != pLink)
    {
        pLink->setLinked(false);
    }

    return;
}

void CSessionManager::closeCurLink(void)
{
    if (NULL == m_pCurrent)
    {
        return;
    }

    closeLinkByID(m_pCurrent->getSessionID());
}

void CSessionManager::setInterface(class CEventInterface *pInterface)
{
    pInterface->setSessionManager(this);
    m_pInterface = pInterface;
}

class CEventInterface *CSessionManager::getInterface(void)
{
    return m_pInterface;
}

CSession *CSessionManager::getCurSession(void)
{
    return m_pCurrent;
}

CSession *CSessionManager::getSessionByID(const int iID)
{
    std::tr1::unordered_map<int, CSession *>::iterator itSession;

    itSession = m_unmapSession.find(iID);
    if (m_unmapSession.end() == itSession)
    {
        return NULL;
    }

    return itSession->second;
}

bool CSessionManager::sendWebSock(CSession *pCurrent, 
    const unsigned short &usOpCode, const char *pszData, const size_t &uiLens)
{
    unsigned short usOp = ntohs(usOpCode);
    Q_PackHeadType msgLens = uiLens + sizeof(usOp);
    size_t iOutLens = Q_INIT_NUMBER;

    const char *pszHead = m_objWebSock.createWebSockHead(true, BINARY_FRAME, msgLens, iOutLens);
    if (NULL == pszHead)
    {
        return false;
    }

    if (Q_RTN_OK != pCurrent->getBuffer()->writeBuffer(pszHead, iOutLens))
    {
        Q_Printf("send message to session: id %d error", pCurrent->getSessionID());

        return false;
    }

    if (Q_RTN_OK != pCurrent->getBuffer()->writeBuffer((const char*)(&usOp), sizeof(usOp)))
    {
        Q_Printf("send message to session: id %d error", pCurrent->getSessionID());

        return false;
    }

    if ((NULL != pszData) 
        && (0 != uiLens))
    {
        if (Q_RTN_OK != pCurrent->getBuffer()->writeBuffer(pszData, uiLens))
        {
            Q_Printf("send message to session: id %d error", pCurrent->getSessionID());

            return false;
        }
    }

    return true;
}

bool CSessionManager::sendWithHead(CSession *pCurrent, 
    const unsigned short &usOpCode, const char *pszData, const size_t &uiLens)
{
    unsigned short usOp = ntohs(usOpCode);
    Q_PackHeadType msgLens = Q_NTOH(uiLens + sizeof(usOp));

    if (Q_RTN_OK != pCurrent->getBuffer()->writeBuffer((const char*)(&msgLens), sizeof(msgLens)))
    {
        Q_Printf("send message lens to session: id %d error", pCurrent->getSessionID());

        return false;
    }

    if (Q_RTN_OK != pCurrent->getBuffer()->writeBuffer((const char*)(&usOp), sizeof(usOp)))
    {
        Q_Printf("send message to session: id %d error", pCurrent->getSessionID());

        return false;
    }

    if ((NULL != pszData) 
        && (0 != uiLens))
    {
        if (Q_RTN_OK != pCurrent->getBuffer()->writeBuffer(pszData, uiLens))
        {
            Q_Printf("send message to session: id %d error", pCurrent->getSessionID());

            return false;
        }
    }

    return true;
}

bool CSessionManager::sendToCur(const unsigned short usOpCode, const char *pszData, const size_t uiLens)
{
    if (NULL == m_pCurrent)
    {
        Q_Printf("%s", "current session pointer is null.");

        return false;
    }

    bool bOK = false;
    switch(m_pCurrent->getType())
    {
    case SType_SVLinker:
    case SType_TcpClient:
        {
            bOK = sendWithHead(m_pCurrent, usOpCode, pszData, uiLens);
        }
        break;

    case SType_WebSock:
        {
            bOK = sendWebSock(m_pCurrent, usOpCode, pszData, uiLens);
        }
        break;

    default:
        break;
    }

    return bOK;
}

bool CSessionManager::sendToByID(const int iID, 
    const unsigned short usOpCode, const char *pszData, const size_t uiLens)
{
    std::tr1::unordered_map<int, CSession *>::iterator itSession;

    itSession = m_unmapSession.find(iID);
    if (m_unmapSession.end() == itSession)
    {
        Q_Printf("no find id %u", iID);
        return false;
    }

    if (NULL == itSession->second)
    {
        Q_Printf("%s", "input session pointer is null.");

        return false;
    }

    bool bOK = false;
    switch(itSession->second->getType())
    {
    case SType_SVLinker:
    case SType_TcpClient:
        {
            bOK = sendWithHead(itSession->second, usOpCode, pszData, uiLens);
        }
        break;

    case SType_WebSock:
        {
            bOK = sendWebSock(itSession->second, usOpCode, pszData, uiLens);
        }
        break;

    default:
        break;
    }

    return bOK;
}

luabridge::LuaRef CSessionManager::getSVLinkerNameByType(const int iType)
{
    assert(NULL != m_pLua);

    CServerLinker *pLinker = NULL;
    CSession *pSession = NULL;
    luabridge::LuaRef luaTable = luabridge::newTable(m_pLua);
    std::tr1::unordered_map<std::string, bufferevent* >::iterator itServerLinker;

    for (itServerLinker = m_mapServerLinker.begin(); m_mapServerLinker.end() != itServerLinker; 
        itServerLinker++)
    {
        pSession = getSession(itServerLinker->second);
        if (SType_SVLinker == pSession->getType())
        {
            pLinker = (CServerLinker *)(pSession->getHandle());
            if (NULL != pLinker)
            {
                if (iType == pLinker->getType())
                {
                    luaTable.append(pLinker->getLinkerName());
                }
            }
        }
    }

    return luaTable;
}

bool CSessionManager::checkType(const int iType, const int iClientID)
{
    CServerLinker *pLinker = NULL;
    CSession *pSesson = getSessionByID(iClientID);
    if(NULL == pSesson)
    {
        return false;
    }

    if (SType_SVLinker != pSesson->getType())
    {
        return false;
    }

    pLinker = (CServerLinker *)(pSesson->getHandle());
    if (NULL == pLinker)
    {
        return false;
    }

    return (pLinker->getType() == iType) ? true : false;
}

int CSessionManager::getGetSVLinkerNum(void)
{
    return m_mapServerLinker.size();
}

CWebSock *CSessionManager::getWebSock(void)
{
    return &m_objWebSock;
}
