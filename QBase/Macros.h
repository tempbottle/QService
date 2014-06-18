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

#ifndef Q_MACROS_H_
#define Q_MACROS_H_

#include "Include.h"
#include "ErrorCode.h"
#include "Version.h"
#include "Enum.h"

#define Q_FILEPATH_LENS              260        //·����󳤶�
#define Q_TIME_LENS                  30         //ʱ�䳤��
#define Q_ONEK                       1024       //1K
#define Q_SERVICE_NAME_LEN           260        //windows ����������󳤶�
#define Q_UUIDLENS                   64         //UUID����
#define Q_MaxSendSize                1450       //ÿ�η�������ֽ���

#define QService "QService"

//��Ϣ��ͷ���ͣ�ת��
typedef unsigned short Q_PackHeadType;
#define Q_NTOH  ntohs

#ifndef Q_OS_WIN32
    #define Q_SIGNAL_EXIT    SIGRTMIN + 10
#endif

#ifdef Q_OS_WIN32
    #define Q_SOCK intptr_t
#else
    #define Q_SOCK int
#endif

#ifdef Q_OS_WIN32
    #define  Q_TIMEB  _timeb
    #define  ftime    _ftime
#else
    #define  Q_TIMEB timeb
#endif

#define Q_INVALID_SOCK -1
#define Q_INVALID_ID   -1
#define Q_INVALID_STATUS   -1

#ifndef Q_OS_WIN32
    #ifdef Q_OS_SOLARIS
        #define Q_AWK "nawk"
    #else
        #define Q_AWK "awk"
    #endif
#endif

#ifdef Q_OS_WIN32
    #define Q_PATH_SEPARATOR "\\"
#else
    #define Q_PATH_SEPARATOR "/"
#endif

#ifdef Q_OS_WIN32
    #define Q_PATH_SEPARATOR_CHAR '\\'
#else
    #define Q_PATH_SEPARATOR_CHAR '/'
#endif

#ifdef Q_OS_WIN32
    #define Q_DLL_EXTENSIONNAME "dll"
#else
    #define Q_DLL_EXTENSIONNAME "so"
#endif

#ifdef Q_OS_WIN32
    #define Q_SCRIPT_EXTENSIONNAME "bat"
#else
    #define Q_SCRIPT_EXTENSIONNAME "sh"
#endif


#define Q_RTN_OK   0
#define Q_RTN_FAILE -1
#define Q_RTN_ERROR 1
#define Q_INIT_NUMBER 0

#ifdef Q_OS_WIN32
    #define Q_SD_BOTH SD_BOTH
#else
    #define Q_SD_BOTH SHUT_RDWR
#endif

/*ϵͳ��־,ֱ��д�ļ�*/
extern class CSysLoger g_SysLoger;
#define Q_SYSLOG(emLogLV, acFormat, ...)  \
    g_SysLoger.sysLog(emLogLV, __FILE__, __FUNCTION__, __LINE__, \
    acFormat, ##__VA_ARGS__)

/*����x,y������*/
#define Q_Concatenate(x, y) x##y
/*�����С*/#define Q_Array_Size(a) (sizeof(a)/sizeof(a[0]))
#define Q_Max(a, b) ((a > b) ? (a) : (b))
#define Q_Min(a, b) ((a < b) ? (a) : (b))
/*���*/
#define Q_Zero(name, len) memset(name, 0, len)

#define Q_Printf(pszFormat, ...) \
            printf((std::string("[%s %d][Print] ") + std::string(pszFormat) + std::string("\n")).c_str(),\
                 __FUNCTION__, __LINE__, ##__VA_ARGS__)

#ifdef Q_OS_WIN32
    typedef HANDLE  Q_Mutex_t;
    typedef HANDLE  Q_Cond_t;
    typedef unsigned int Q_Thread_t;
    typedef DWORD   Q_Thread_Attr_t;
    typedef unsigned int Q_Thread_Result_t;

    #define Q_MUTEX_INITVAL  NULL
    #define Q_THREAD_CREATE_DETACHED 1
    #define Q_THREAD_CALL __stdcall
    typedef Q_Thread_Result_t (Q_THREAD_CALL * Q_Thread_Func_t)(void *args);    
#else
    typedef pthread_mutex_t Q_Mutex_t;
    typedef pthread_cond_t  Q_Cond_t;
    typedef pthread_t       Q_Thread_t;
    typedef pthread_attr_t  Q_Thread_Attr_t;
    typedef void * Q_Thread_Result_t;

    #define Q_MUTEX_INITVAL  PTHREAD_MUTEX_INITIALIZER

    /*PTHREAD_CREATE_DETACHED �����̣߳�
    �������߳����зǳ��죬���ܿ�����pthread_create��������֮ǰ����ֹ�ˣ�
    ����ֹ�Ժ�Ϳ��ܽ��̺߳ź�ϵͳ��Դ�ƽ����������߳�ʹ�ã�
    ��������pthread_create���߳̾͵õ��˴�����̺߳š�Ҫ��������������Բ�ȡһ����ͬ����ʩ��
    ��򵥵ķ���֮һ�ǿ����ڱ��������߳������pthread_cond_timewait������������̵߳ȴ�һ�����
    �����㹻��ʱ���ú���pthread_create����*/
    #define Q_THREAD_CREATE_DETACHED     PTHREAD_CREATE_DETACHED
    #define Q_THREAD_CALL
    typedef Q_Thread_Result_t (Q_THREAD_CALL * Q_Thread_Func_t)(void *args);
#endif//Q_OS_WIN32

#ifdef Q_OS_WIN32
    #define Q_StrTok strtok_s
    #define Q_Snprintf _snprintf
    #define Q_Swprintf swprintf
    #define Q_Strncpy strncpy_s
    #define Q_ITOA _itoa_s
    #define Q_WCSICMP(pszFst, pwsiScd) _wcsicmp(pszFst, pwsiScd)
    #define Q_SockError() WSAGetLastError()
    #define Q_SockError2Str(errcode) strerror(errcode)
    #define Q_Error() GetLastError()
    #define Q_Error2Str(errcode) strerror(errcode)
    #define Q_STAT _stat
    #define Q_ACCESS _access
    #define Q_AtomicAdd InterlockedExchangeAdd    
    #define Q_AtomicSet InterlockedExchange
    #define Q_AtomicCompareSwap(ppSrc, pOldval, pNewVal) \
                       InterlockedCompareExchangePointer(ppSrc, pNewVal, pOldval)
#else
    #define Q_StrTok strtok_r
    #define Q_Snprintf snprintf
    #define Q_Swprintf swprintf
    #define Q_Strncpy strncpy
    #define Q_ITOA itoa
    #define Q_WCSICMP(pszFst, pwsiScd) \
        wmemcpy(pszFst, pwsiScd,\
        wcslen(pszFst) >= wcslen(pwsiScd) ? wcslen(pszFst) : wcslen(pwsiScd))
    #define Q_SockError() (errno)
    #define Q_SockError2Str(errcode) strerror(errcode)
    #define Q_Error() (errno)
    #define Q_Error2Str(errcode) strerror(errcode)
    #define Q_STAT stat
    #define Q_ACCESS access
    #define Q_AtomicAdd __sync_fetch_and_add
    #define Q_AtomicSet __sync_lock_test_and_set
    #define Q_AtomicCompareSwap(ppSrc, pOldval, pNewVal) \
                       __sync_val_compare_and_swap(ppSrc, pOldval, pNewVal)
#endif

#define Q_AtomicGet(ppSrc) Q_AtomicAdd(ppSrc, 0)

#define Q_SafeDelete(v_para)\
do\
{\
    if (NULL != v_para)\
    {\
        delete v_para;\
        v_para = NULL;\
    }\
}while(0)

#define Q_SafeDelete_Array(v_para)\
do\
{\
    if (NULL != v_para)\
    {\
        delete[] v_para;\
        v_para = NULL;\
    }\
}while(0)

#define Q_SafeFree(v_para)\
do\
{\
    if (NULL != v_para)\
    {\
        free(v_para);\
        v_para = NULL;\
    }\
}while(0)

#endif//Q_MACROS_H_