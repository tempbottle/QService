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

#include "Session.h"

CSession::CSession(void) : m_cStatus(Q_INVALID_STATUS), m_bLinker(false), 
    m_pHandle(NULL),  m_iSessionID(Q_INVALID_ID), 
    m_iAssociatedID(Q_INVALID_ID)    
{
    (void)m_objBuffer.setBuffer(NULL);
}

CSession::~CSession(void)
{

}

void CSession::setServerLinker(bool bLinker)
{
    m_bLinker = bLinker;
}

bool CSession::getServerLinker(void)
{
    return m_bLinker;
}

void CSession::setSessionID(const int &uiID)
{
    m_iSessionID = uiID;
}

int CSession::getSessionID(void)
{
    return m_iSessionID;
}

void CSession::setAssociatedID(const int64_t &iID)
{
    m_iAssociatedID = iID;
}

int64_t CSession::getAssociatedID(void)
{
    return m_iAssociatedID;
}

void CSession::setStatus(const char &cStatus)
{
    m_cStatus = cStatus;
}

char CSession::getStatus(void)
{
    return m_cStatus;
}

CEventBuffer *CSession::getBuffer(void)
{
    return &m_objBuffer;
}

void CSession::setHandle(void *pHandle)
{
    m_pHandle = pHandle;
}

void *CSession::getHandle(void)
{
    return m_pHandle;
}

void CSession::Clear(void)
{
    m_bLinker = false;
    m_pHandle = NULL;
    m_iAssociatedID = Q_INVALID_ID;
    m_iSessionID = Q_INVALID_ID;
    m_cStatus = Q_INVALID_STATUS;
    (void)m_objBuffer.setBuffer(NULL);
}