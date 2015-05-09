
#ifndef Q_SESSION_H_
#define Q_SESSION_H_

#include "EventBuffer.h"

/*session ��Ϣ*/
class CSession
{
public:
    CSession(void);
    ~CSession(void);

    /*session id��*/
    void setSessionID(const int &uiID);
    int getSessionID(void) const;

    /*������ID*/
    void setExterID(const char *pszID);
    const char * getExterID(void) const;

    /*У��ID*/
    void setCheckID(const char *pszID);
    const char * getCheckID(void) const;

    /*��ʶ��sessionΨһID*/
    void setIDCard(const char *pszID);
    const char * getIDCard(void) const;

    /*ping*/
    void setPing(const unsigned int uiPing);
    unsigned int getPing(void) const;

    /*�˺�*/
    void setAccount(const char *pszAccount);
    const char *getAccount(void) const;

    /*״̬*/
    void setStatus(const int iStatus);
    int getStatus(void) const;

    /*session ����  enum SessionType*/
    void setType(const SessionType emType);
    int getType(void) const;

    void Clear(void);

    CEventBuffer *getBuffer(void);

private:
    SessionType m_emSessionType;   
    SessionStatus m_emStatus;
    int m_iSessionID;//session id      
    unsigned int m_uiPing;
    std::string m_strID;//������ID
    std::string m_strCheckID;//У��ID
    std::string m_strAccount;
    std::string m_strIDCard;
    CEventBuffer m_objBuffer;
};

#endif//Q_SESSION_H_
