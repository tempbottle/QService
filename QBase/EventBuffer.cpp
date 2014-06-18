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

#include "EventBuffer.h"

/************************************************************************
* Function name:setBuffer
* Description  :���ݴ����bufferevent��ȡ��дbuffer��socket���
* IN           :pBev --bufferevent
* OUT          :NONE
* Return       :Q_RTN_OK --�ɹ� ���� --ʧ��
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
int CEventBuffer::setBuffer(struct bufferevent *pBev)
{
    if (NULL == pBev)
    {
        m_pBev = NULL;
        m_pReadBuffer = NULL;
        m_pWriteBuffer = NULL;
        m_FD = Q_INVALID_SOCK;

        return Q_ERROR_NULLPOINTER;
    }

    m_pBev = pBev;

    //��ȡ��buffer
    m_pReadBuffer = bufferevent_get_input(m_pBev);
    if (NULL == m_pReadBuffer)
    {
        Q_Printf("%s", Q_EXCEPTION_NULLPOINTER);

        return Q_ERROR_NULLPOINTER;
    }

    //��ȡдbuffer
    m_pWriteBuffer = bufferevent_get_output(m_pBev);
    if (NULL == m_pWriteBuffer)
    {
        Q_Printf("%s", Q_EXCEPTION_NULLPOINTER);

        return Q_ERROR_NULLPOINTER;
    }

    //��ȡsocket���
    m_FD = bufferevent_getfd(m_pBev);
    if (Q_INVALID_SOCK == m_FD)
    {
        Q_Printf("%s", "invalid socket handle.");

        return Q_ERROR_INVALIDSOCK;
    }

    return Q_RTN_OK;
}

/************************************************************************
* Function name:getBuffer
* Description  :��ȡbufferevent���
* IN           :NONE
* OUT          :NONE
* Return       :bufferevent *
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
struct bufferevent *CEventBuffer::getBuffer(void)
{
    return m_pBev;
}

/************************************************************************
* Function name:getFD
* Description  :��ȡsocket���
* IN           :NONE
* OUT          :NONE
* Return       :Q_SOCK
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
Q_SOCK CEventBuffer::getFD(void)
{
    return m_FD;
}

/************************************************************************
* Function name:getTotalLens
* Description  :��ȡ�������������ݵĳ���
* IN           :NONE
* OUT          :NONE
* Return       :�����������ݳ���
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
size_t CEventBuffer::getTotalLens(void)
{
    if (NULL == m_pReadBuffer)
    {
        return Q_INIT_NUMBER;
    }

    return evbuffer_get_length(m_pReadBuffer);
}

/************************************************************************
* Function name:readBuffer
* Description  :�Ӷ���������ȡ��ָ������������
* IN           :iLens --Ҫ��ȡ���ֽ���
* OUT          :NONE
* Return       :char *
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
char *CEventBuffer::readBuffer(const size_t iLens)
{
    if (NULL == m_pReadBuffer)
    {
        return NULL;
    }

    return (char*)evbuffer_pullup(m_pReadBuffer, iLens);
}

/************************************************************************
* Function name:delBuffer
* Description  :ɾ����������������
* IN           :iLens --Ҫɾ�����ֽ���
* OUT          :NONE
* Return       :Q_RTN_OK --�ɹ� ���� --ʧ��
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
int CEventBuffer::delBuffer(const size_t iLens)
{
    if (NULL == m_pReadBuffer)
    {
        return Q_RTN_OK;
    }

    return evbuffer_drain(m_pReadBuffer, iLens);
}

/************************************************************************
* Function name:writeBuffer
* Description  :������д��д������
* IN           :pszBuffer --Ҫд�������  iLens --�ֽ���
* OUT          :NONE
* Return       :Q_RTN_OK --�ɹ� ���� --ʧ��
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
int CEventBuffer::writeBuffer(const char *pszBuffer, const size_t iLens)
{
    if (NULL == pszBuffer
        || NULL == m_pWriteBuffer)
    {
        return Q_ERROR_NULLPOINTER;
    }

    return evbuffer_add(m_pWriteBuffer, pszBuffer, iLens);
}

/************************************************************************
* Function name:moveBufferData
* Description  :�����������е������ƶ���д������
* IN           :NONE
* OUT          :NONE
* Return       :Q_RTN_OK --�ɹ� ���� --ʧ��
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
int CEventBuffer::moveBufferData(void)
{
    if (NULL == m_pWriteBuffer
        || NULL == m_pReadBuffer)
    {
        return Q_ERROR_NULLPOINTER;
    }

    return evbuffer_add_buffer(m_pWriteBuffer, m_pReadBuffer);
}
