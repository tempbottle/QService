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

#ifndef Q_AES_H_
#define Q_AES_H_

#include "Buffer.h"

/*aes���� ���̰߳�ȫ*/
class CAESEncode
{
public:
    CAESEncode(void);
    ~CAESEncode(void);

    /*�������� pszKey ���룬 emKeyType ��������*/
    int setKey(const char *pszKey, AESKeyType emKeyType);
    /*���� pszPlaint��Ҫ���ܵ����ݣ�iLens ��Ҫ�������ݳ��ȣ� iOutLes���ܺ󳤶�*/
    const char *Encode(const char *pszPlaint, const size_t iLens, size_t &iOutLes);

private:
    enum BlockSize
    {
        AES_BlockSize = 16,
    };

private:
    unsigned long *m_pRK;
    unsigned char *m_pKey;
    unsigned char m_uacPlain[AES_BlockSize];
    unsigned char m_uacCipher[AES_BlockSize];
    int m_iRounds;
    CBuffer m_objBuffer;
};

/*aes���� ���̰߳�ȫ*/
class CAESDecode
{
public:
    CAESDecode(void);
    ~CAESDecode(void);

    /*�������� pszKey ���룬 emKeyType ��������*/
    int setKey(const char *pszKey, AESKeyType emKeyType);
    /*����, pszCipher��Ҫ���ܵ����ݣ�iLens ��Ҫ�������ݳ��ȣ� iOutLes���ܺ󳤶�*/
    const char *Decode(const char *pszCipher, const size_t iLens, size_t &iOutLes);

private:
    enum BlockSize
    {
        AES_BlockSize = 16,
    };

private:
    unsigned long *m_pRK;
    unsigned char *m_pKey;
    unsigned char m_uacPlain[AES_BlockSize];
    unsigned char m_uacCipher[AES_BlockSize];
    int m_iRounds;
    CBuffer m_objBuffer;
};

#endif//Q_AES_H_
