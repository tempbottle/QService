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

#ifndef Q_SOCKPAIREVENT_H_
#define Q_SOCKPAIREVENT_H_

#include "SockPair.h"
#include "EventBuffer.h"
#include "Cond.h"

enum OrderType
{
    OrderType_AddServerLinker = 0,
};

struct OrderMsg
{
    OrderType emType;
    void *pHandle;
    OrderMsg(void)
    {
        emType = OrderType_AddServerLinker;
        pHandle = NULL;
    }
};

/*
SockPair�¼�ѭ��
*/
class CSockPairEvent
{
public:
    CSockPairEvent(void);
    virtual ~CSockPairEvent(void);

    /*�����¼�ѭ��*/
    int Start(void);
    /*ֹͣ�¼�ѭ��*/
    void Stop(void);
    /*��ȡ�Ƿ�����¼�ѭ��*/
    bool getIsRun(void);
    /*�ȴ������¼�ѭ��*/
    bool waitForStarted(void);
    /*�Ƿ�������*/
    void setError(bool bError);
    bool getError(void);

    /*����дsock ����*/
    void setTcpParam(void *pArg = NULL);
    /*�˳�ѭ��sock ����*/
    void setExitParam(void *pArg = NULL);
    /*����sock����*/
    void setOrderParam(void *pArg = NULL);
    /*����sock����*/
    void setWebSockParam(void *pArg = NULL);
    /*������дsockд������*/
    int sendTcpMsg(const char *pszBuff, const size_t &iSize);
    /*������sockд������*/
    int sendOrderMsg(const char *pszBuff, const size_t &iSize);
    /*������websockд������*/
    int sendWebSockMsg(const char *pszBuff, const size_t &iSize);
    /*��ȡevent_base*/
    struct event_base *getBase(void)
    {
        return m_pEventBase;
    };

public:
    virtual void onTcpRead(struct SockPairEventParam *pParam){};
    virtual void onStop(struct SockPairEventParam *pParam){};
    virtual void onOrderRead(struct SockPairEventParam *pParam){};
    virtual void onWebSockRead(struct SockPairEventParam *pParam){};
    virtual void onStartUp(void){};
public:
    static void sockPairEventReadCB(struct bufferevent *bev, void *arg);
    static void sockPairEventCB(struct bufferevent *bev, short event, void *arg);

private:
    int Init(void);
    void freeAll(void);
    int initEvent(void);
    int initEvent(struct bufferevent **pBev, struct SockPairEventParam *pParam, CSockPair &objPair);

private:
    bool m_bExitNormal;
    bool m_bRun;
    bool m_bError;
    struct event_base *m_pEventBase;
    struct bufferevent **m_pBevs;
    CSockPair *m_pSockPairs;
    CEventBuffer *m_pBuffers;
    struct SockPairEventParam *m_pParam;
    CMutex m_Mutex;
    CCond m_Cond;
};

/*��buffer�ж�ȡ.����ָ����Ҫ�ֶ�ɾ��*/
template<class T>
T *Q_GetEventValue(CEventBuffer *pBuffer)
{
    size_t iSize = sizeof(T);

    size_t iTotalSize = pBuffer->getTotalLens();
    if (iSize > iTotalSize)
    {
        return NULL;
    }

    T *pDate = new(std::nothrow) T();
    if (NULL == pDate)
    {
        Q_Printf("%s", Q_EXCEPTION_ALLOCMEMORY);

        return NULL;
    }

    char *pTmp = pBuffer->readBuffer(iSize);
    *pDate = *((T *)pTmp);
    pBuffer->delBuffer(iSize);

    return pDate;
}

/*��buffer�ж�ȡ*/
template<class T>
bool Q_GetEventValue(CEventBuffer *pBuffer, T &Date)
{
    size_t iSize = sizeof(T);

    size_t iTotalSize = pBuffer->getTotalLens();
    if (iSize > iTotalSize)
    {
        return false;
    }

    char *pTmp = pBuffer->readBuffer(iSize);
    Date = *((T*)pTmp);
    pBuffer->delBuffer(iSize);

    return true;
}

#endif//Q_SOCKPAIREVENT_H_
