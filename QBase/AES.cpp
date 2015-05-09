
#include "AES.h"
#include <rijndael/rijndael.h>

/*
���һ���ʣ���ֽ���ȫ�����϶�����ֽ������������һ���СΪ16�ֽڣ�5�ֽ���Ч��
����������ֽ����11(16-5),��ʾ��11���ֽ���Ч���������ܺ��ȥ�ⲿ�־ͺ��ˡ�
���ԭʼ���ݳ��ȸպ���16����������������һ��block,����Ϊ16,
��ʾ��blockȫ��Ϊ��Ч���ݡ�
*/

CAESEncode::CAESEncode(void) : m_pRK(NULL), m_pKey(NULL), 
    m_iRounds(Q_INIT_NUMBER), m_objBuffer()
{
    Q_Zero(m_uacPlain, sizeof(m_uacPlain));
    Q_Zero(m_uacCipher, sizeof(m_uacCipher));
}

CAESEncode::~CAESEncode(void)
{
    Q_SafeDelete_Array(m_pRK);
    Q_SafeDelete_Array(m_pKey);
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

    size_t iRKLens = RKLENGTH(emKeyType);
    size_t iKeyLens = KEYLENGTH(emKeyType);

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
        Q_SafeDelete_Array(m_pRK);
        Q_Printf("%s", Q_EXCEPTION_ALLOCMEMORY);

        return Q_ERROR_ALLOCMEMORY;
    }

    Q_Zero(m_pKey, iKeyLens + 1);

    memcpy(m_pKey, pszKey, strlen(pszKey) > iKeyLens ? iKeyLens : strlen(pszKey));
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
const char *CAESEncode::Encode(const char *pszPlaint, const size_t &iLens, size_t &iOutLes)
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
            memset(m_uacPlain + iCopyLen, (int)(AES_BlockSize - iCopyLen), AES_BlockSize - iCopyLen);
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
    m_iRounds(Q_INIT_NUMBER), m_objBuffer()
{
    Q_Zero(m_uacPlain, sizeof(m_uacPlain));
    Q_Zero(m_uacCipher, sizeof(m_uacCipher));
}

CAESDecode::~CAESDecode(void)
{
    Q_SafeDelete_Array(m_pRK);
    Q_SafeDelete_Array(m_pKey);
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

    size_t iRKLens = RKLENGTH(emKeyType);
    size_t iKeyLens = KEYLENGTH(emKeyType);

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
        Q_SafeDelete_Array(m_pRK);
        Q_Printf("%s", Q_EXCEPTION_ALLOCMEMORY);

        return Q_ERROR_ALLOCMEMORY;
    }

    Q_Zero(m_pKey, iKeyLens + 1);

    memcpy(m_pKey, pszKey, strlen(pszKey) > iKeyLens ? iKeyLens : strlen(pszKey));
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
const char *CAESDecode::Decode(const char *pszCipher, const size_t &iLens, size_t &iOutLes)
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
