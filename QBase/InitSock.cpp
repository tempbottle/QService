
#include "InitSock.h"

CSockInit::CSockInit(void)
{
    (void)Init();
}

CSockInit::~CSockInit(void)
{
    try
    {
        Destroy();
    }
    catch(...)
    {

    }
}

int CSockInit::Init(void) const
{
#ifdef Q_OS_WIN
    WORD wVersionReq;
    WSAData wsData;

    wVersionReq = MAKEWORD(2, 2);

    int iRtn = WSAStartup(wVersionReq, &wsData);
    if (Q_RTN_OK != iRtn)
    {
        iRtn = Q_SockError();
        Q_Printf("WSAStartup error. error code %d, message %s", iRtn, Q_SockError2Str(iRtn));

        return iRtn;
    }
#endif

    return Q_RTN_OK;
}

void CSockInit::Destroy(void) const
{
#ifdef Q_OS_WIN    
    (void)WSACleanup();
#endif
}

/************************************************************************
* Function name:Q_SockRead
* Description  :��socket�ж�ȡ����
* IN           :fd --socket��� iLens --��ȡ����
* OUT          :pBuf --��ȡ���ݴ�ŵ�
* Return       :Q_RTN_OK --�ɹ� ���� --ʧ��
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
int Q_SockRead(const Q_SOCK &fd, char *pBuf, const size_t &iLens)
{
    int iRecvSize = Q_INIT_NUMBER;
    size_t iRecvTotalSize = Q_INIT_NUMBER;

    do 
    {
        iRecvSize = recv(fd, pBuf + iRecvTotalSize, (int)(iLens - iRecvTotalSize), 0);
        if (iRecvSize <= 0)
        {
            int iRtn = Q_SockError();
            Q_Printf("recv error. error code %d, message %s ", iRtn, Q_SockError2Str(iRtn));

            return Q_RTN_FAILE;
        }

        iRecvTotalSize += (size_t)iRecvSize;

    } while (iRecvTotalSize < iLens);

    return Q_RTN_OK;
}

/************************************************************************
* Function name:Q_SockWrite
* Description  :��socket��д�뷢�͵�����
* IN           :fd --socket��� pBuf --Ҫ���͵����� iLens --���ݳ���
* OUT          :NONE
* Return       :Q_RTN_OK --�ɹ� ���� --ʧ��
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
int Q_SockWrite(const Q_SOCK &fd, const char *pBuf, const size_t &iLens)
{
    int iSendSize = Q_INIT_NUMBER;
    size_t iSendTotalSize = Q_INIT_NUMBER;

    do 
    {
        iSendSize = send(fd, pBuf + iSendTotalSize, (int)(iLens - iSendTotalSize), 0);
        if (iSendSize <= 0)
        {
            int iRtn = Q_SockError();
            Q_Printf("send error. error code %d, message %s ", iRtn, Q_SockError2Str(iRtn));

            return Q_RTN_FAILE;
        }

        iSendTotalSize += (size_t)iSendSize;

    } while (iLens > iSendTotalSize);

    return Q_RTN_OK;
}
