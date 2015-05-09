
#include "EventBuffer.h"

CEventBuffer::CEventBuffer(void) : m_pBev(NULL), m_pReadBuffer(NULL),
    m_pWriteBuffer(NULL), m_FD(Q_INVALID_SOCK)
{

}

CEventBuffer::~CEventBuffer(void)
{
    m_pBev = NULL;
    m_pReadBuffer = NULL;
    m_pWriteBuffer = NULL;
}

/************************************************************************
* Function name:setBuffer
* Description  :���ݴ����bufferevent��ȡ��дbuffer��socket���
* IN           :pBev --bufferevent
* OUT          :NONE
* Return       :Q_RTN_OK --�ɹ� ���� --ʧ��
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
int CEventBuffer::setBuffer(struct bufferevent *pBev)
{
    if (NULL == pBev)
    {
        m_pBev = NULL;
        m_pReadBuffer = NULL;
        m_pWriteBuffer = NULL;
        m_FD = Q_INVALID_SOCK;

        return Q_ERROR_NULLPOINTER;
    }

    m_pBev = pBev;

    //��ȡ��buffer
    m_pReadBuffer = bufferevent_get_input(m_pBev);
    if (NULL == m_pReadBuffer)
    {
        Q_Printf("%s", Q_EXCEPTION_NULLPOINTER);

        return Q_ERROR_NULLPOINTER;
    }

    //��ȡдbuffer
    m_pWriteBuffer = bufferevent_get_output(m_pBev);
    if (NULL == m_pWriteBuffer)
    {
        Q_Printf("%s", Q_EXCEPTION_NULLPOINTER);

        return Q_ERROR_NULLPOINTER;
    }

    //��ȡsocket���
    m_FD = bufferevent_getfd(m_pBev);
    if (Q_INVALID_SOCK == m_FD)
    {
        Q_Printf("%s", "invalid socket handle.");

        return Q_ERROR_INVALIDSOCK;
    }

    return Q_RTN_OK;
}

/************************************************************************
* Function name:getBuffer
* Description  :��ȡbufferevent���
* IN           :NONE
* OUT          :NONE
* Return       :bufferevent *
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
struct bufferevent *CEventBuffer::getBuffer(void)
{
    return m_pBev;
}

/************************************************************************
* Function name:getFD
* Description  :��ȡsocket���
* IN           :NONE
* OUT          :NONE
* Return       :Q_SOCK
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
Q_SOCK CEventBuffer::getFD(void) const
{
    return m_FD;
}

/************************************************************************
* Function name:getTotalLens
* Description  :��ȡ�������������ݵĳ���
* IN           :NONE
* OUT          :NONE
* Return       :�����������ݳ���
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
size_t CEventBuffer::getTotalLens(void) const
{
    if (NULL == m_pReadBuffer)
    {
        return Q_INIT_NUMBER;
    }

    return evbuffer_get_length(m_pReadBuffer);
}

/************************************************************************
* Function name:readBuffer
* Description  :�Ӷ���������ȡ��ָ������������
* IN           :iLens --Ҫ��ȡ���ֽ���
* OUT          :NONE
* Return       :char *
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
char *CEventBuffer::readBuffer(const size_t &iLens)
{
    if (NULL == m_pReadBuffer)
    {
        return NULL;
    }

    return (char*)evbuffer_pullup(m_pReadBuffer, (ev_ssize_t)iLens);
}

/************************************************************************
* Function name:delBuffer
* Description  :ɾ����������������
* IN           :iLens --Ҫɾ�����ֽ���
* OUT          :NONE
* Return       :Q_RTN_OK --�ɹ� ���� --ʧ��
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
int CEventBuffer::delBuffer(const size_t &iLens)
{
    if (NULL == m_pReadBuffer)
    {
        return Q_RTN_OK;
    }

    return evbuffer_drain(m_pReadBuffer, iLens);
}

/************************************************************************
* Function name:writeBuffer
* Description  :������д��д������
* IN           :pszBuffer --Ҫд�������  iLens --�ֽ���
* OUT          :NONE
* Return       :Q_RTN_OK --�ɹ� ���� --ʧ��
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
int CEventBuffer::writeBuffer(const char *pszBuffer, const size_t &iLens)
{
    if (NULL == pszBuffer
        || NULL == m_pWriteBuffer)
    {
        return Q_ERROR_NULLPOINTER;
    }

    return evbuffer_add(m_pWriteBuffer, pszBuffer, iLens);
}

/************************************************************************
* Function name:moveBufferData
* Description  :�����������е������ƶ���д������
* IN           :NONE
* OUT          :NONE
* Return       :Q_RTN_OK --�ɹ� ���� --ʧ��
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
int CEventBuffer::moveBufferData(void)
{
    if (NULL == m_pWriteBuffer
        || NULL == m_pReadBuffer)
    {
        return Q_ERROR_NULLPOINTER;
    }

    return evbuffer_add_buffer(m_pWriteBuffer, m_pReadBuffer);
}

struct evbuffer_ptr CEventBuffer::Search(const char *pszWhat, size_t iLens, const struct evbuffer_ptr *pStart)
{
    return evbuffer_search(m_pReadBuffer, pszWhat, iLens, pStart);
}
