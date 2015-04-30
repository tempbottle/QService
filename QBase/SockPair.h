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

#ifndef Q_SOCKPAIR_H_
#define Q_SOCKPAIR_H_

#include "NETAddr.h"

/*
�໥���ӵ�2��socket
*/
class CSockPair
{
public:
    CSockPair(void);
    ~CSockPair(void);
    
    /*��ȡ��sock���*/
    Q_SOCK getReadFD(void) const
    {
        return m_ReadFD;
    };
    /*��ȡдsock���*/
    Q_SOCK getWriteFD(void) const
    {
        return m_WriteFD;
    };
    
    /*�Ӷ������ȡ����*/
    int Read(char *pBuf, const size_t &iLens) const;
    /*��д���д������*/
    int Write(const char *pBuf, const size_t &iLens) const;

private:
    int Init(void);
    void Destroy(void);
    int creatSockPair(Q_SOCK aeSock[2]) const;
    int creatListener(Q_SOCK &Listener) const;

private:
    Q_SOCK m_ReadFD;
    Q_SOCK m_WriteFD;
    CSockInit m_sockInit;
};

#endif//Q_SOCKPAIR_H_
