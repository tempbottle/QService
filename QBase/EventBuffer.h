
#ifndef Q_EVENTBUFFER_H_
#define Q_EVENTBUFFER_H_

#include "Macros.h"

/*
bufferevent������
*/
class CEventBuffer
{
public:
    CEventBuffer(void);
    ~CEventBuffer(void);

    /*���ݴ����bufferevent��ȡ��дbuffer��socket���*/
    int setBuffer(struct bufferevent *pBev);
    /*��ȡbufferevent���*/
    struct bufferevent *getBuffer(void);    

    /*��ȡ�������������ݵĳ���*/
    size_t getTotalLens(void) const;
    /*�Ӷ���������ȡ��iLens���ֽڵ�����*/
    char *readBuffer(const size_t &iLens);
    /*ɾ����������������*/
    int delBuffer(const size_t &iLens);
    /*�����ݼ���д������*/
    int writeBuffer(const char *pszBuffer, const size_t &iLens);
    /*�����������е������ƶ���д������*/
    int moveBufferData(void);
    /*����*/
    struct evbuffer_ptr Search(const char *pszWhat, size_t iLens, const struct evbuffer_ptr *pStart = NULL);
    /*��ȡsocket���*/
    Q_SOCK getFD(void) const;

private:
    struct bufferevent *m_pBev; 
    struct evbuffer *m_pReadBuffer;
    struct evbuffer *m_pWriteBuffer;
    Q_SOCK m_FD;
};

#endif//Q_EVENTBUFFER_H_
