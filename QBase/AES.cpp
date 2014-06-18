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

#include "AES.h"
#include "rijndael/rijndael.h"

/*
���һ���ʣ���ֽ���ȫ�����϶�����ֽ������������һ���СΪ16�ֽڣ�5�ֽ���Ч��
����������ֽ����11(16-5),��ʾ��11���ֽ���Ч���������ܺ��ȥ�ⲿ�־ͺ��ˡ�
���ԭʼ���ݳ��ȸպ���16����������������һ��block,����Ϊ16,
��ʾ��blockȫ��Ϊ��Ч���ݡ�
*/

CAESEncode::CAESEncode(void) : m_pRK(NULL), m_pKey(NULL), 
    m_iRounds(Q_INIT_NUMBER)
{
}

CAESEncode::~CAESEncode(void)
{
    Q_SafeDelete(m_pRK);
    Q_SafeDelete(m_pKey);
}

/************************************************************************
* Function name:setKey
* Description  :��������
* IN           :pszKey --���� emKeyType --��������
* OUT          :NONE
* Return       :Q_RTN_OK --�ɹ� ���� --ʧ��
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
int CAESEncode::setKey(const char *pszKey, AESKeyType emKeyType)
{
    Q_SafeDelete(m_pRK);
    Q_SafeDelete(m_pKey);

    if (KeyNone == emKeyType)
    {
        return Q_RTN_FAILE;
    }

    int iRKLens = RKLENGTH(emKeyType);
    int iKeyLens = KEYLENGTH(emKeyType);

    m_pRK = new(std::nothrow) unsigned long[iRKLens + 1];
    if (NULL == m_pRK)
    {
        Q_Printf("%s", Q_EXCEPTION_ALLOCMEMORY);

        return Q_ERROR_ALLOCMEMORY;
    }

    Q_Zero(m_pRK, iRKLens + 1);

    m_pKey = new(std::nothrow) unsigned char[iKeyLens + 1];
    if (NULL == m_pKey)
    {
        Q_SafeDelete(m_pRK);
        Q_Printf("%s", Q_EXCEPTION_ALLOCMEMORY);

        return Q_ERROR_ALLOCMEMORY;
    }

    Q_Zero(m_pKey, iKeyLens + 1);

    memcpy(m_pKey, pszKey, strlen(pszKey) > (size_t)iKeyLens ? iKeyLens : strlen(pszKey));
    m_iRounds = rijndaelSetupEncrypt(m_pRK, m_pKey, emKeyType);

    return Q_RTN_OK;
}

/************************************************************************
* Function name:Encode
* Description  :����
* IN           :pszPlaint --Ҫ���ܵ����� iLens --���� 
* OUT          :iOutLes --���ܺ󳤶�
* Return       :const char *
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
const char *CAESEncode::Encode(const char *pszPlaint, const size_t iLens, size_t &iOutLes)
{    
    size_t iCopyLen = Q_INIT_NUMBER;
    bool bFill = false;

    iOutLes = Q_INIT_NUMBER;
    m_objBuffer.reSet();

    if (NULL == m_pRK)
    {
        Q_Printf("%s", Q_EXCEPTION_NULLPOINTER);

        return NULL;
    }

    if (0 == iLens % AES_BlockSize)
    {
        bFill = true;
    }

    for (size_t i = 0; i < iLens;)
    {
        iCopyLen = ((iLens - i) >= AES_BlockSize) ? (AES_BlockSize) : (iLens - i); 
        memcpy(m_uacPlain, pszPlaint + i, iCopyLen);
        if (iCopyLen < AES_BlockSize)//����һ����Բ���ֽ������
        {
            memset(m_uacPlain + iCopyLen, AES_BlockSize - iCopyLen, AES_BlockSize - iCopyLen);
        }

        rijndaelEncrypt(m_pRK, m_iRounds, m_uacPlain, m_uacCipher);
        m_objBuffer.pushBuff(m_uacCipher, AES_BlockSize);

        i += AES_BlockSize;
    }

    if (bFill)//���ȸպ�Ϊһ������������ټ�һ�����
    {
        memset(m_uacPlain, AES_BlockSize, AES_BlockSize);

        rijndaelEncrypt(m_pRK, m_iRounds, m_uacPlain, m_uacCipher);
        m_objBuffer.pushBuff(m_uacCipher, AES_BlockSize);
    }

    iOutLes = m_objBuffer.getLens();

    return m_objBuffer.getBuffer();
}

/*************************************************/
CAESDecode::CAESDecode(void) : m_pRK(NULL), m_pKey(NULL), 
    m_iRounds(Q_INIT_NUMBER)
{
}

CAESDecode::~CAESDecode(void)
{
    Q_SafeDelete(m_pRK);
    Q_SafeDelete(m_pKey);
}

/************************************************************************
* Function name:setKey
* Description  :��������
* IN           :pszKey --���� emKeyType --��������
* OUT          :NONE
* Return       :Q_RTN_OK --�ɹ� ���� --ʧ��
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
int CAESDecode::setKey(const char *pszKey, AESKeyType emKeyType)
{
    Q_SafeDelete(m_pRK);
    Q_SafeDelete(m_pKey);

    if (KeyNone == emKeyType)
    {
        return Q_RTN_FAILE;
    }

    int iRKLens = RKLENGTH(emKeyType);
    int iKeyLens = KEYLENGTH(emKeyType);

    m_pRK = new(std::nothrow) unsigned long[iRKLens + 1];
    if (NULL == m_pRK)
    {
        Q_Printf("%s", Q_EXCEPTION_ALLOCMEMORY);

        return Q_ERROR_ALLOCMEMORY;
    }

    Q_Zero(m_pRK, iRKLens + 1);

    m_pKey = new(std::nothrow) unsigned char[iKeyLens + 1];
    if (NULL == m_pKey)
    {
        Q_SafeDelete(m_pRK);
        Q_Printf("%s", Q_EXCEPTION_ALLOCMEMORY);

        return Q_ERROR_ALLOCMEMORY;
    }

    Q_Zero(m_pKey, iKeyLens + 1);

    memcpy(m_pKey, pszKey, strlen(pszKey) > (size_t)iKeyLens ? iKeyLens : strlen(pszKey));
    m_iRounds = rijndaelSetupDecrypt(m_pRK, m_pKey, emKeyType);

    return Q_RTN_OK;
}

/************************************************************************
* Function name:Decode
* Description  :����
* IN           :pszPlaint --Ҫ���ܵ����� iLens --���� 
* OUT          :iOutLes --���ܺ󳤶�
* Return       :const char *
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
const char *CAESDecode::Decode(const char *pszCipher, const size_t iLens, size_t &iOutLes)
{
    iOutLes = Q_INIT_NUMBER;
    m_objBuffer.reSet();

    if (NULL == m_pRK)
    {
        Q_Printf("%s", Q_EXCEPTION_NULLPOINTER);

        return NULL;
    }

    if (0 != (iLens % AES_BlockSize))
    {
        return NULL;
    }

    for (size_t i = 0; i < iLens; )
    {
        memcpy(m_uacCipher, pszCipher + i, AES_BlockSize);

        rijndaelDecrypt(m_pRK, m_iRounds, m_uacCipher, m_uacPlain);
        m_objBuffer.pushBuff(m_uacPlain, AES_BlockSize);

        i += AES_BlockSize;
    }

    iOutLes = m_objBuffer.getLens() - (size_t)m_uacPlain[AES_BlockSize - 1];

    return m_objBuffer.getBuffer();
}
