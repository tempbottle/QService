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

#ifndef Q_SERVERLINKER_H_
#define Q_SERVERLINKER_H_

#include "Cond.h"
#include "InitSock.h"
#include "SockPairEvent.h"

/*���ӵ�����������*/
class CServerLinker
{
public:
    CServerLinker(void);
    ~CServerLinker(void);

    /*����״��*/
    void setLinked(bool bLinked);
    bool getLinked(void);
    /*�˿�*/
    void setPort(unsigned short usPort);
    unsigned short getPort(void);
    /*IP*/
    void setIp(const char *pszIp);
    const char *getIp(void);
    /*��������*/
    void setType(const int iType);
    int getType(void);
    /*�������¼�ѭ����ŵ�*/
    void setSockPairEvent(CSockPairEvent *pEvent);
    /*���������ӵļ��*/
    int Monitor(void);
    /*����*/
    int Link(void);
    /*�������ӷ����¼�ѭ��*/
    int addInLoop(void);
    /*�ر�*/
    void closeSock(void);
    /*���÷�����*/
    void setNonblock(void);
    /*��ȡ�����ӵ�sock���*/
    Q_SOCK getSock(void);
    /*�ȴ��˳�*/
    CMutex *getCloseMutex(void);
    CCond *getCloseCond(void);
    /*�����̷߳����ź�*/
    CMutex *getMutex(void);
    CCond *getCond(void);
    /*�˳���־*/
    bool getStop(void);
    /*����߳�����״��*/
    bool getMonitorRun(void);
    void setMonitorRun(bool bMonitorRun);
    /*��������*/
    void setLinkerName(const char *pszName);
    const char *getLinkerName(void);

private:
    void setTimeOut(const unsigned int uiMS);

private:
    bool m_bMonitorRun;
    bool m_bStop;
    bool m_bLinked;            //�Ƿ�����
    unsigned short m_usDesPort;//����Ŀ�������˿�
    int m_iType;
    Q_SOCK m_Sock;
    CSockPairEvent *m_pEvent;
    std::string m_strDesIp;  //����Ŀ������IP
    CMutex m_objMutex_Close;
    CCond m_objCond_Close;
    CMutex m_objMutex;
    CCond m_objCond;
    CSockInit m_objSockInit;
    std::string m_strLinkerName;
};

#endif//Q_SERVERLINKER_H_
