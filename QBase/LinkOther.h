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

#ifndef Q_LINKOTHER_H_
#define Q_LINKOTHER_H_

#include "InitSock.h"
#include "Mutex.h"

//���ӵ�����
class CLinkOther
{
public:
    CLinkOther(void);
    ~CLinkOther(void);

    //����CWorkThreadEvent����
    void setThreadEvent(class CWorkThreadEvent *pThreadEvent);
    class CWorkThreadEvent *getThreadEvent(void);

    //��������״̬
    void setStatus(const RunStatus emStatus);
    RunStatus getStatus(void);

    //����Ҫ���ӵ�������Ϣ
    bool addHost(const char *pszIp, unsigned short usPort, const char *pszName);

    //��ʼ��ֹͣ
    bool Start(void);
    void Stop(void);

    //����
    void Link(void);

    //������������״̬
    void setSockStatus(Q_SOCK sock, SessionStatus emStatus);
    
    //�������ƻ�ȡ����ID
    int getSockByName(const char *pszName);

private:
    struct LinkInfo
    {
        SessionStatus emStatus;
        unsigned short usPort;
        Q_SOCK sock;
        std::string strIp;
        std::string strName;
        void Clear(void)
        {
            emStatus = SessionStatus_Closed;
            sock = Q_INVALID_SOCK;
        };
    };

private:
    Q_SOCK initSock(const char *pszIp, unsigned short usPort);

private:
    RunStatus m_emStatus;
    class CWorkThreadEvent *m_pThreadEvent;
    std::vector<LinkInfo> m_vcLinkInfo;
};

#endif//Q_LINKOTHER_H_