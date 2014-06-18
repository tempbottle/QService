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

#include "SnowflakeID.h"

#ifdef Q_OS_WIN32
#define EPOCHFILETIME 11644473600000000Ui64
#else
#define EPOCHFILETIME 11644473600000000ULL
#endif

CSnowflakeID::CSnowflakeID(void) : m_iMachineID(Q_INIT_NUMBER), m_iIndex(Q_INIT_NUMBER)
{

}

CSnowflakeID::~CSnowflakeID(void)
{

}

uint64_t CSnowflakeID::getTime(void)
{
#ifdef Q_OS_WIN32
    FILETIME stFileTime;
    uint64_t uiTime = Q_INIT_NUMBER;

    GetSystemTimeAsFileTime(&stFileTime);

    uiTime |= stFileTime.dwHighDateTime;
    uiTime <<= 32;
    uiTime |= stFileTime.dwLowDateTime;
    uiTime /= 10;
    uiTime -= EPOCHFILETIME;

    return uiTime / 1000;
#else
    struct timeval stTV;
    uint64_t uiTime = Q_INIT_NUMBER;

    gettimeofday(&stTV, NULL);

    uiTime = stTV.tv_usec;
    uiTime /= 1000;
    uiTime += (stTV.tv_sec * 1000);

    return uiTime;
#endif
}

void CSnowflakeID::setMachineID(const int iMachineID)
{
    m_iMachineID = iMachineID;
}

int64_t CSnowflakeID::generate(void)
{
    int64_t iValue = Q_INIT_NUMBER;
    uint64_t uiTime = getTime();

    // 保留后41位时间
    iValue = uiTime << 22;
    // 中间10位是机器ID
    iValue |= (m_iMachineID & 0x3FF) << 12;
    // 最后12位是m_iIndex
    iValue |= m_iIndex++ & 0xFFF;
    if (m_iIndex == 0x1000)
    {
        m_iIndex = Q_INIT_NUMBER;
    }

    return iValue;
}
