/*****************************************************************************
 * Copyright (c) 2011-2012. Qifu Luo All Rights Reserved.200309129@163.com 
 * svn:http://asuraserver.googlecode.com/svn/
 *
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

CSessionManager::CSessionManager(void) : m_sThreadIndex(-1), m_uiTimer(Q_INIT_NUMBER),
    m_uiCount(Q_INIT_NUMBER), m_pCurrent(NULL), m_pInterface(NULL)
{

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
    std::map<int, CSession *>::iterator itSession;    
    evutil_socket_t fd = bufferevent_getfd(bev);
    CSession *pSession = NULL;

    itSession = m_mapSession.find(fd);
    if (m_mapSession.end() == itSession)
    {
        return;
    }

    pSession = itSession->second;
    if (NULL == pSession)
    {
        m_mapSession.erase(itSession);

        return;
    }

    pSession->Clear();
    m_vcFreeSession.push_back(pSession);//�ӽ������б�
    m_mapSession.erase(itSession);

    return;
}

int CSessionManager::addSession(struct bufferevent *bev)
{
    int iRtn = Q_RTN_OK;
    CSession *pSession = NULL;

    if (m_vcFreeSession.empty())
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
        pSession = m_vcFreeSession.back();
        m_vcFreeSession.pop_back();
    }
    
    iRtn = pSession->getBuffer()->setBuffer(bev);
    if (Q_RTN_OK != iRtn)
    {
        pSession->Clear();
        m_vcFreeSession.push_back(pSession);

        return iRtn;
    }

    Q_SOCK fd = pSession->getBuffer()->getFD();
    pSession->setSessionID(fd);

    m_mapSession[fd] = pSession;

    return Q_RTN_OK;
}

CSession *CSessionManager::getSession(struct bufferevent *bev)
{
    evutil_socket_t fd = bufferevent_getfd(bev);
    if (Q_INVALID_SOCK == fd)
    {
        return NULL;
    }

    std::map<int, CSession *>::iterator itSession = m_mapSession.find(fd);
    if (m_mapSession.end() == itSession)
    {
        return NULL;
    }

    return itSession->second;
}

void CSessionManager::addServerLinker(const char *pszName, struct bufferevent *pBufEvent)
{
    std::map<std::string, bufferevent* >::iterator itServerLinker;
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
    std::map<std::string, bufferevent* >::iterator itServerLinker;

    itServerLinker = m_mapServerLinker.find(std::string(pszName));
    if (m_mapServerLinker.end() != itServerLinker)
    {
        m_mapServerLinker.erase(itServerLinker);
    }
}

CSession *CSessionManager::getServerLinkerSession(const char *pszName)
{
    std::map<std::string, bufferevent* >::iterator itServerLinker;

    itServerLinker = m_mapServerLinker.find(std::string(pszName));
    if (m_mapServerLinker.end() == itServerLinker)
    {
        return NULL;
    }

    return getSession(itServerLinker->second);
}

size_t CSessionManager::getSessionSize(void)
{
    return m_mapSession.size();
}

void CSessionManager::freeAllSession(void)
{
    std::map<int, CSession *>::iterator itSession;
    std::vector<CSession *>::iterator itFreeSession;

    for (itSession = m_mapSession.begin(); m_mapSession.end() != itSession; itSession++)
    {
        bufferevent_free(itSession->second->getBuffer()->getBuffer());
        Q_SafeDelete(itSession->second);
    }

    for (itFreeSession = m_vcFreeSession.begin(); m_vcFreeSession.end() != itFreeSession; 
        itFreeSession++)
    {
        Q_SafeDelete(*itFreeSession);
    }

    m_mapSession.clear();
    m_vcFreeSession.clear();

    return;
}

void CSessionManager::closeClintByID(const int iID)
{
    std::map<int, CSession *>::iterator itSession;

    itSession = m_mapSession.find(iID);
    if (m_mapSession.end() == itSession)
    {
        return;
    }

    bufferevent_free(itSession->second->getBuffer()->getBuffer());
    itSession->second->Clear();

    m_vcFreeSession.push_back(itSession->second);
    m_mapSession.erase(itSession);

    return;
}

void CSessionManager::closeCurClint(void)
{
    if (NULL == m_pCurrent)
    {
        return;
    }

    closeClintByID(m_pCurrent->getSessionID());
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
    if (NULL == m_pCurrent)
    {
        return NULL;
    }

    return m_pCurrent;
}

CSession *CSessionManager::getSessionByID(const int iID)
{
    std::map<int, CSession *>::iterator itSession;

    itSession = m_mapSession.find(iID);
    if (m_mapSession.end() == itSession)
    {
        return NULL;
    }

    return itSession->second;
}

bool CSessionManager::checkParam(CSession *pCurrent, 
    const char *pszData, const size_t &uiLens)
{
    if (NULL == pCurrent)
    {
        Q_Printf("%s", "input session pointer is null.");
        return false;
    }
    if (NULL == pszData)
    {
        Q_Printf("%s", "input data pointer is null.");
        return false;
    }
    if (0 >= uiLens)
    {
        Q_Printf("%s", "message lens must big than zero.");
        return false;
    }

    return true;
}

bool CSessionManager::sendWithHead(CSession *pCurrent, 
    const char *pszData, const size_t &uiLens)
{
    Q_PackHeadType msgLens = Q_NTOH(uiLens);

    if (Q_RTN_OK != pCurrent->getBuffer()->writeBuffer((const char*)(&msgLens), sizeof(msgLens)))
    {
        Q_Printf("send message lens to session: id %d error", pCurrent->getSessionID());

        return false;
    }

    if (Q_RTN_OK != pCurrent->getBuffer()->writeBuffer(pszData, uiLens))
    {
        Q_Printf("send message to session: id %d error", pCurrent->getSessionID());

        return false;
    }

    return true;
}

bool CSessionManager::sendToCur(const char *pszData, const size_t uiLens)
{
    if (!checkParam(m_pCurrent, pszData, uiLens))
    {
        return false;
    }

    return sendWithHead(m_pCurrent, pszData, uiLens);
}

bool CSessionManager::sendToByID(const int iID, const char *pszData, const size_t uiLens)
{
    std::map<int, CSession *>::iterator itSession;

    itSession = m_mapSession.find(iID);
    if (m_mapSession.end() == itSession)
    {
        Q_Printf("no find id %u", iID);
        return false;
    }

    if (!checkParam(itSession->second, pszData, uiLens))
    {
        return false;
    }

    return sendWithHead(itSession->second, pszData, uiLens);
}

bool CSessionManager::sendToAll(const char *pszData, const size_t uiLens)
{
    if (NULL == pszData)
    {
        Q_Printf("%s", "input data pointer is null.");
        return false;
    }
    if (0 >= uiLens)
    {
        Q_Printf("%s", "message lens must big than zero.");
        return false;
    }

    std::map<int, CSession *>::iterator itSession;

    for (itSession = m_mapSession.begin(); m_mapSession.end() != itSession; itSession++)
    {
        (void)sendWithHead(itSession->second, pszData, uiLens);
    }

    return true;
}

bool CSessionManager::pushMsg(const char *pszData, const size_t usLens)
{
    try
    {
        m_objBuffer.pushBuff(pszData, usLens);
    }
    catch (CException &e)
    {
        Q_Printf("%s", e.getErrorMsg());

        return false;
    }

    return true;
}

bool CSessionManager::sendPushMsgToCur(void)
{
    return sendToCur(m_objBuffer.getBuffer(), m_objBuffer.getLens());
}

bool CSessionManager::sendPushMsgToByID(const int iID)
{
    return sendToByID(iID, m_objBuffer.getBuffer(), m_objBuffer.getLens());
}

bool CSessionManager::sendPushMsgToAll(void)
{
    return sendToAll(m_objBuffer.getBuffer(), m_objBuffer.getLens());
}

void CSessionManager::Clear(void)
{
    m_objBuffer.reSet();
}

std::vector<std::string> CSessionManager::getServerLinkerByType(const int iType)
{
    CServerLinker *pLinker = NULL;
    CSession *pSession = NULL;
    std::vector<std::string> vcTmp;
    std::map<std::string, bufferevent* >::iterator itServerLinker;

    for (itServerLinker = m_mapServerLinker.begin(); m_mapServerLinker.end() != itServerLinker; itServerLinker++)
    {
        pSession = getSession(itServerLinker->second);
        if (pSession->getServerLinker())
        {
            pLinker = (CServerLinker *)(pSession->getHandle());
            if (NULL != pLinker)
            {
                if (iType == pLinker->getType())
                {
                    vcTmp.push_back(std::string(pLinker->getLinkerName()));
                }
            }
        }
    }

    return vcTmp;
}

bool CSessionManager::checkType(const int iType, const int iClientID)
{
    CServerLinker *pLinker = NULL;
    CSession *pSesson = getSessionByID(iClientID);
    if(NULL == pSesson)
    {
        return false;
    }

    if (!(pSesson->getServerLinker()))
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