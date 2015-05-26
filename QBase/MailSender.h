
#ifndef Q_MAILSENDER_H_
#define Q_MAILSENDER_H_

#include "CTask.h"
#include "SockPairEvent.h"
#include "Singleton.h"
#include "TcpParser.h"
#include "jwsmtp/mailer.h"

class CMailSender : public CTask,
    public CSockPairEvent,
    public CSingleton<CMailSender>
{
public:
    CMailSender(void);
    ~CMailSender(void);

    //����ӿ�
    void Run(void)
    {
        Q_Printf("%s", "mail system running...");
        Start();
    };

    //SockPairEvent���ӿ�
    void onMainRead(CEventBuffer *pEventBuffer);

    //����
    void setServer(const std::string &strServer);
    void setAuthType(const enum jwsmtp::mailer::authtype &emType);
    void setSender(const std::string &strSender);
    void setUserName(const std::string &strUserName);
    void setPassWord(const std::string &strPassWord);

    //����json��ʽ
    void sendMail(const std::string &strMail);

private:
    int parseMail(std::string &strMail);

private:
    jwsmtp::mailer m_objMailer;
    CTcpParser m_objTcpParser;   
};

#endif//Q_MAILSENDER_H_
