/*****************************************************************************
 * Copyright (c) 2011-2012. Qifu Luo All Rights Reserved.200309129@163.com 
 * svn:http://asuraserver.googlecode.com/svn/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include "QTime.h"

/*************************************************
* Function name:Q_Now
* Description  :��ȡ��ǰϵͳʱ��
* IN           :None
* OUT          :None
* Return       :string ��2011-06-01 21:41:30��
* Make By      :lqf/200309129@163.com
* Date Time    :2011/06/01
* Modification 
* ......record :fist program
**************************************************/
std::string Q_Now(void)
{
    char acTimeStr[Q_TIME_LENS] = {0};
    time_t t = time(NULL);	

    strftime(acTimeStr, sizeof(acTimeStr) - 1, "%Y-%m-%d %H:%M:%S", localtime(&t));

    return std::string(acTimeStr);
}

/*************************************************
* Function name:Q_Date
* Description  :��ȡ����
* IN           :None
* OUT          :None
* Return       :string ��2011-06-01��
* Make By      :lqf/200309129@163.com
* Date Time    :2011/06/01
* Modification 
* ......record :fist program
**************************************************/
std::string Q_Date(void)
{
    char acTimeStr[Q_TIME_LENS] = {0};
    time_t t = time(NULL);	

    strftime(acTimeStr, sizeof(acTimeStr) - 1, "%Y-%m-%d", localtime(&t));

    return std::string(acTimeStr);
}

/*************************************************
* Function name:Q_LocalTime
* Description  :��ȡtm�ṹ��
* IN           :None
* OUT          :None
* Return       :tm *
* Make By      :lqf/200309129@163.com
* Date Time    :2011/06/01
* Modification 
* ......record :fist program
**************************************************/
tm *Q_LocalTime(time_t *pTm)
{
    return localtime(pTm);
}

/*************************************************
* Function name:Q_CurWeek
* Description  :��ȡ�ڼ���
* IN           :None
* OUT          :None
* Return       :int
* Make By      :lqf/200309129@163.com
* Date Time    :2011/06/01
* Modification 
* ......record :fist program
**************************************************/
int Q_CurWeek(void)
{
    time_t now = time(NULL);
    char buf[3] = {0};

    strftime(buf, sizeof(buf), "%W", localtime(&now));

    return atoi(buf);
}

/*************************************************
* Function name:Q_WDay
* Description  :��ȡ���ڼ�
* IN           :None
* OUT          :None
* Return       :int
* Make By      :lqf/200309129@163.com
* Date Time    :2011/06/01
* Modification 
* ......record :fist program
**************************************************/
int Q_WDay(void)
{
    time_t now = time(NULL);

    return localtime(&now)->tm_wday;
}

/*************************************************
* Function name:Q_TimeToStr
* Description  :ʱ��ת�����ַ���
* IN           :time --ʱ��
* OUT          :None
* Return       :��ʽ�����ʱ��
* Make By      :lqf/200309129@163.com
* Date Time    :2011/06/01
* Modification 
* ......record :fist program
**************************************************/
std::string Q_TimeToStr(const time_t &time)
{
    char acTimeStr[Q_TIME_LENS] = {0};

    strftime(acTimeStr, sizeof(acTimeStr) - 1, "%Y-%m-%d %H:%M:%S", localtime(&time));

    return std::string(acTimeStr);
}

/*************************************************
* Function name:Q_StrToTime
* Description  :�ַ���ת����ʱ��
* IN           :strTime --ʱ���ַ���
* OUT          :None
* Return       :time_t
* Make By      :lqf/200309129@163.com
* Date Time    :2011/06/01
* Modification 
* ......record :fist program
**************************************************/
time_t Q_StrToTime(const std::string &strTime)
{
    time_t t;
    t = (time_t)NULL;
    char *pBeginPos = (char*) strTime.c_str();
    char *pPos = strstr(pBeginPos, "-");
    if(pPos == NULL)
    {
        return t;
    }

    int iYear = atoi(pBeginPos);
    int iMonth = atoi(pPos + 1);
    pPos = strstr(pPos + 1, "-");
    if(pPos == NULL)
    {
        return t;
    }

    int iDay = atoi(pPos + 1);
    int iHour=0;
    int iMin=0;
    int iSec=0;
    pPos = strstr(pPos + 1," ");

    if( pPos != NULL )
    {
        iHour=atoi(pPos + 1);
        pPos = strstr(pPos + 1,":");
        if(pPos != NULL)
        {
            iMin=atoi(pPos + 1);
            pPos = strstr(pPos + 1,":");
            if(pPos != NULL)
            {
                iSec=atoi(pPos + 1);
            }
        }
    }

    struct tm sourcedate;

    Q_Zero((void*)&sourcedate, sizeof(sourcedate));

    sourcedate.tm_sec = iSec;
    sourcedate.tm_min = iMin; 
    sourcedate.tm_hour = iHour;
    sourcedate.tm_mday = iDay;
    sourcedate.tm_mon = iMonth - 1; 
    sourcedate.tm_year = iYear - 1900;

    return mktime(&sourcedate);
}

/*************************************************
* Function name:Q_DiffTime
* Description  :����ʱ���(����ֵ��)
* IN           :strSourceTime --��ʼʱ�� strTargetTime --����ʱ��
* OUT          :None
* Return       :Q_DOUBLE ʱ���
* Make By      :lqf/200309129@163.com
* Date Time    :2011/06/01
* Modification 
* ......record :fist program
**************************************************/
double Q_DiffTime(const std::string &strSourceTime, 
    const std::string &strTargetTime)
{
    time_t time2;
    time_t time1;

    time1 = Q_StrToTime(strSourceTime);
    time2 = Q_StrToTime(strTargetTime);

    return fabs(difftime(time1, time2));
}