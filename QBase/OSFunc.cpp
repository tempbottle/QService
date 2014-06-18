/*****************************************************************************
 * Copyright (c) 2011-2012. Qifu Luo All Rights Reserved.200309129@163.com 
 * svn:http://asuraserver.googlecode.com/svn/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this std::list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this std::list of conditions and the following disclaimer in the
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

#include "OSFunc.h"

/************************************************************************
* Function name:Q_HostName
* Description  :��ȡ����������
* IN           :NONE
* OUT          :NONE
* Return       :std::string
* Make By      :lqf/200309129@163.com
* Date Time    :2014/04/30
* Modification 
* ......record :first program
************************************************************************/
std::string Q_HostName(void)
{
    int iRtn = Q_RTN_OK;

    char acHostName[512] = {0};    
    CSockInit objSockInit;

    iRtn = gethostname(acHostName, sizeof(acHostName));
    if (Q_RTN_FAILE != iRtn)
    {
        iRtn = Q_SockError();
        Q_Printf("gethostname error. error code %d, message %s", iRtn, Q_SockError2Str(iRtn));

        return "";
    }

    acHostName[sizeof(acHostName) - 1] = '\0';

    return std::string(acHostName);
}

#ifndef Q_OS_WIN32
int ExecCmdLinux(const char *acCmd, std::list<std::string> *lstRst)
{
    int iRtn = Q_RTN_OK;
    FILE * pStream = NULL;   

    if (NULL == lstRst)
    {
        iRtn = system(acCmd);
        if (Q_RTN_OK != iRtn)
        {
            return iRtn;
        }

        return Q_RTN_OK;
    }
    else
    {
        char acTempBuf[Q_ONEK] = {0};
        pStream = popen(acCmd, "r");
        if (NULL == pStream)
        {
            Q_Printf("%s", "popen error");

            return Q_RTN_FAILE;
        }

        while(!feof(pStream))
        {
            Q_Zero(acTempBuf, sizeof(acTempBuf));
            (void)fgets(acTempBuf, sizeof(acTempBuf), pStream);
            if((0 == acTempBuf[0]) || ('\n' == acTempBuf[0]))
            {
                continue;
            }

            if (NULL != lstRst)
            {
                lstRst->push_back(std::string(acTempBuf));
            }
        }

        pclose(pStream);

        return Q_RTN_OK;
    }
}
#else
int ExecCmdWindows(const char *acCmd, const int iTimeOut, 
    std::list<std::string> *lstRst, unsigned long *ulRST)
{
    int iRtn = Q_RTN_OK;
    SECURITY_ATTRIBUTES sa;
    HANDLE hRead,hWrite;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
    sa.lpSecurityDescriptor = NULL;          //ʹ��ϵͳĬ�ϵİ�ȫ������ 
    sa.bInheritHandle = TRUE;                //�����Ľ��̼̳о��

    if (!CreatePipe(&hRead,&hWrite,&sa,0))   //���������ܵ�
    {
        Q_Printf("%s", "CreatePipe error");

        return Q_RTN_FAILE;
    }

    STARTUPINFO startupInfo;
    PROCESS_INFORMATION processInfo;
    DWORD dwCode = 0;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.hStdError = hWrite; 
    startupInfo.hStdOutput = hWrite;         //�´������̵ı�׼�������д�ܵ�һ��
    startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    startupInfo.wShowWindow = SW_HIDE;

    ZeroMemory(&processInfo,sizeof(processInfo));
    if(!CreateProcess(NULL,
        TEXT((char*)acCmd),
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &startupInfo,
        &processInfo))
    {
        Q_Printf("%s", "CreateProcess error");

        return Q_RTN_FAILE;
    }

    CloseHandle(hWrite);   //�رչܵ����

    if (WAIT_TIMEOUT == WaitForSingleObject(processInfo.hProcess, 1000 * iTimeOut))
    {
        BOOL bRNT;
        HANDLE handle;
        PROCESSENTRY32 proEntry32;
        proEntry32.dwSize = sizeof(PROCESSENTRY32);
        handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (INVALID_HANDLE_VALUE == handle)
        {
            iRtn = Q_RTN_FAILE;
            CloseHandle(processInfo.hProcess);
            CloseHandle(processInfo.hThread);
            CloseHandle(hRead);

            return iRtn;
        }

        bRNT = Process32First(handle, &proEntry32);
        while(bRNT)
        {
            if(proEntry32.th32ParentProcessID == processInfo.dwProcessId)
            {
                HANDLE pvChild;
                pvChild = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proEntry32.th32ProcessID);
                TerminateProcess(pvChild, 0);
                CloseHandle(pvChild);
            }

            bRNT = Process32Next(handle, &proEntry32);
        }

        TerminateProcess(processInfo.hProcess, 0);
        CloseHandle(handle);
        CloseHandle(hRead);

        *ulRST = 1;
    }
    else
    {
        GetExitCodeProcess(processInfo.hProcess, &dwCode);

        *ulRST = dwCode;

        if (NULL != lstRst)
        {
            char acBuffer[Q_ONEK] = {0};
            DWORD bytesRead;
            while (true) 
            {
                Q_Zero(acBuffer, sizeof(acBuffer));
                if (FALSE == ReadFile(hRead, acBuffer, sizeof(acBuffer), 
                    &bytesRead, NULL))   //��ȡ�ܵ�
                {
                    break;
                }

                lstRst->push_back(std::string(acBuffer));
                Sleep(10);
            }
        }
        CloseHandle(hRead);
    }

    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    return Q_RTN_OK;
}
#endif

/*************************************************
* Function name:Q_ExecCmd
* Description  :ִ������ű�
* IN           :pszCmd --�ű�������  iSec --��ʱ(s)
* OUT          :lstRst --ִ�н��
* Return       :Q_RNT_OK �ɹ�������ʧ��
* Make By      :lqf/200309129@163.com
* Date Time    :2011/06/01
* Modification 
* ......record :first program
**************************************************/
int Q_ExecCmd(const char *pszCmd, const unsigned int uimSec, 
    std::list<std::string> *lstRst)
{
    if (NULL == pszCmd)
    {
        return Q_ERROR_NULLPOINTER;
    }

    if (NULL != lstRst)
    {
        lstRst->clear();
    }

#ifdef Q_OS_WIN32
    unsigned long ulReCode = Q_INIT_NUMBER;
    int iRtn = Q_RTN_OK;

    iRtn = ExecCmdWindows(pszCmd, uimSec, lstRst, &ulReCode);
    if ((Q_RTN_OK == iRtn)
        &&(Q_RTN_OK == ulReCode))
    {
        return Q_RTN_OK;
    }

    return Q_RTN_FAILE;
#else
    return ExecCmdLinux(pszCmd, lstRst);
#endif
}