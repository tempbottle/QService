
#ifndef Q_BUFFER_H_
#define Q_BUFFER_H_

#include "Macros.h"

/*
���ݴ��
*/
class CBuffer
{
public:
    CBuffer(void);
    /*iInitSize Ĭ��buffer��С*/
    explicit CBuffer(const size_t iInitSize);
    ~CBuffer(void);

    /*�����Ϣ��buffer  pBuff ΪNULL��0���*/
    void pushBuff(const void *pBuff, const size_t &iLens);
    /*����bufferƫ�Ʊ�־*/
    void reSet(void);

    /*����buffer*/
    const char *getBuffer(void) const;
    /*��ȡbuffer����*/
    const size_t getLens(void) const;
    /*��ȡ�ܳ���*/
    size_t getTotalLens(void) const;

    //New
    char *New(const size_t iLens);
    //reNew
    char *reNew(const size_t iLens);

private:
    char *m_pBuffer;
    size_t m_iOffset;
    size_t m_iTotalSize;
};

#endif//Q_BUFFER_H_
