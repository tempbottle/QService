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

#ifndef Q_NETADDR_H_
#define Q_NETADDR_H_

#include "InitSock.h"

/*
�����ַ
*/
class CNETAddr
{
public:
    CNETAddr(void);
    explicit CNETAddr(bool ipv6);
    ~CNETAddr(void);

    /*�������*/
    void Clear(void);

    /*����sockaddr_in*/
    int setAddr(const char *pszHostName, unsigned short usPort, bool is_ipv6 = false);
    /*����sockaddr_in*/
    int	setAddr(const struct sockaddr *pAddr);
    /*����socket�����ȡԶ�˵�ַ��Ϣ*/
    int setRemoteAddr(Q_SOCK &fd);
    /*����socket�����ȡ���ص�ַ��Ϣ*/
    int setLocalAddr(Q_SOCK &fd);
    /*����sockaddr���*/
    sockaddr *getAddr(void);
    /*��ȡ��ַ�ڴ泤��*/
    size_t getAddrSize(void);
    /*��ȡIP*/
    std::string getIp(void);
    /*��ȡ�˿�*/
    unsigned short getPort(void);
    /*�Ƿ�Ϊipv4*/
    bool is_ipv4();
    /*�Ƿ�Ϊipv6*/
    bool is_ipv6();
    /*ipת����*/
    unsigned long IpToNumber(const char *pszIp);

private:
    enum
    {
        IPV4,
        IPV6,
    }m_Addr_Type;

    sockaddr_in	m_ipv4;
    sockaddr_in6 m_ipv6;
    CSockInit m_sockInit;
};

#endif//Q_NETADDR_H_
