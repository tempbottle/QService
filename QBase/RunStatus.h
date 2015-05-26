
#ifndef Q_RUNSTATUS_H_
#define Q_RUNSTATUS_H_

#include "Thread.h"

//����״̬
class CRunStatus
{
public:
    CRunStatus(void) : m_emStatus(RUNSTATUS_UNKNOWN)
    {

    };
    virtual ~CRunStatus(void)
    {
    };

    //�ȴ����
    bool waitForStarted(void) const
    {
        while(true)
        {
            if (getIsRun())
            {
                return true;
            }

            if (getError())
            {
                return false;
            }

            Q_Sleep(10);
        }
    };
    //״̬���á���ȡ
    void setRunStatus(RunStatus emStatus)
    {
        m_emStatus = emStatus;
    };
    RunStatus getRunStatus(void) const
    {
        return m_emStatus;
    };

    //�Ƿ�������
    bool getError(void) const
    {
        return ((RUNSTATUS_ERROR == getRunStatus()) ? true : false);
    };
    //�Ƿ�������
    bool getIsRun(void) const
    {
        return ((RUNSTATUS_RUNING == getRunStatus()) ? true : false);
    };

private:
    RunStatus m_emStatus;
};

#endif//Q_RUNSTATUS_H_
