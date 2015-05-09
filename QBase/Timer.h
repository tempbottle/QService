
#ifndef Q_TIMER_H_
#define Q_TIMER_H_

#include "Macros.h"

/*
��ʱ��
*/
class CTimer
{
public:
    CTimer(void);
    ~CTimer(void){};

    /*���ü�ʱ��*/
    void reStart(void);
    /*�������ŵ�ʱ��(ms)*/
    double Elapsed(void);

private:
#ifdef Q_OS_WIN32
    bool m_bUseFrequency;    
    LARGE_INTEGER m_StartCount;//��¼��ʼʱ��     
    LARGE_INTEGER m_EndCount;//��¼����ʱ��     
    LARGE_INTEGER m_Freq;//����CPUʱ��Ƶ��
    struct Q_TIMEB m_BgTime;
    struct Q_TIMEB m_EndTime;
#else
    struct timeval m_stStart;  
    struct timeval m_stEnd;
#endif    
};

#endif//Q_TIMER_H_
