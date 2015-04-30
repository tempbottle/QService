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

#ifndef Q_WEBSOCK_PARSER_H_
#define Q_WEBSOCK_PARSER_H_

#include "Base64.h"
#include "SHA1.h"

#define FRAME_HEAD_BASE_LEN 6
#define FRAME_HEAD_EXT16_LEN 8
#define FRAME_HEAD_EXT64_LEN 14

//websock �������ͷ
struct WebSockFram
{
    char cFin;
    char cRsv1;
    char cRsv2;
    char cRsv3;
    WebSockOpCode emOpCode;
    char cMask;
    unsigned char ucPayloadLen;
    size_t uiDataLens;
    char acMaskKey[4];
    WebSockFram(void) : cFin(Q_INIT_NUMBER), cRsv1(Q_INIT_NUMBER),
        cRsv2(Q_INIT_NUMBER), cRsv3(Q_INIT_NUMBER), emOpCode(CONTINUATION),
        cMask(Q_INIT_NUMBER), ucPayloadLen(Q_INIT_NUMBER), uiDataLens(Q_INIT_NUMBER)
    {
        Q_Zero(acMaskKey, sizeof(acMaskKey));
    };
};

class CWebSockParser
{
public:
    CWebSockParser(void);
    ~CWebSockParser(void);

    //��������
    const std::string *shakeHands(class CEventBuffer *pBuffer);

    //���
    bool parsePack(class CEventBuffer *pBuffer);
    bool getClose(void) const
    {
        return m_bClose;
    };
    size_t getParsedLens(void) const
    {
        return m_iParsedLens;
    };
    const char *getMsg(void) const
    {
        if (NULL == m_pMsg)
        {
            return "";
        }

        return m_pMsg;
    };
    const WebSockFram *getHead(void) const
    {
        return &m_stFram;
    };

    //������ͷ
    const char *createHead(const bool &bFin, const WebSockOpCode emCode, 
        const size_t &iDataLens, size_t &iOutLens);

private:
    std::string parseShakeHands(std::list<std::string> &lstShakeHands) const;
    std::string createChallengeKey(std::string &strKey);
    std::string createHandshakeResponse(const std::string &strKey) const;
    bool parseHead(class CEventBuffer *pBuffer);

private:
    bool m_bClose;
    size_t m_iParsedLens;
    size_t m_iHeadLens;
    size_t m_iTotalLens;
    size_t m_iNeedReadLens;
    size_t m_iShakeHandsEndFlagLens;
    char *m_pMsg;
    unsigned char m_acShaKey[20];
    char m_acWebSockHead[FRAME_HEAD_EXT64_LEN];
    std::string m_strVersion;
    std::string m_strVal;
    CBase64 m_objbase64;
    CSHA1 m_objSHA1;
    WebSockFram m_stFram;
    struct evbuffer_ptr m_stPos;
};

#endif//Q_WEBSOCK_PARSER_H_
