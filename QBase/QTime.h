
#ifndef Q_TIME_H_
#define Q_TIME_H_

#include "Macros.h"

/*��ȡ��ǰϵͳʱ��*/
std::string Q_Now(void);
/*��ȡ����*/
std::string Q_Date(void);
/*��ȡtm�ṹ��*/
const tm *Q_LocalTime(const time_t *pTm);
/*��ȡ�ڼ���*/
int Q_CurWeek(void);
/*��ȡ���ڼ�*/
int Q_WDay(void);
/*ʱ��ת�����ַ���*/
std::string Q_TimeToStr(const time_t &time);
/*�ַ���ת����ʱ��*/
time_t Q_StrToTime(const std::string &strTime);
/*����ʱ���(����ֵ)����λ��*/
double Q_DiffTime(const std::string &strSourceTime, 
                  const std::string &strTargetTime);

#endif//Q_TIME_H_
