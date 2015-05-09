
#ifndef Q_TXTLOGERER_H_
#define Q_TXTLOGERER_H_

#include "Loger.h"
#include "Mutex.h"
#include "TcpParser.h"

//�ı���־
class CTxtLoger : public CLoger
{
public:
    CTxtLoger(void);
    ~CTxtLoger(void);

    /*���ô�ӡ���𣬵��ڸü���Ĳ����, ����LOGLV_NOLOG�������־*/
    void setPriority(const LOG_LEVEL emLV);
    /*������־�ļ�*/
    void setLogFile(const char *pLogFile);
    /*������־�ļ���С*/
    void setLogMaxSize(const size_t iSize);

    void writeLog(const LOG_LEVEL emInLogLv,
        const char *pFile, const char *pFunction, const int iLine, const Q_SOCK &fd,
        const char *pFormat, ...);

    void Open(void);

public:
    void Write(const char *pszMsg, const size_t iLens);

private:
    std::string getLV(LOG_LEVEL emInLogLv) const;

private:
    FILE *m_pFile;
    LOG_LEVEL m_emPriorityLV;
    size_t m_uiLogSize;
    time_t m_LastCheckTime;
    std::string m_strFilePath;
    CQMutex m_objMutex;
    CTcpParser m_objTcpParser;
};

#endif//Q_TXTLOGERER_H_
