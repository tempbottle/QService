/*****************************************************************************
* Copyright (c) 2011-2012. Qifu Luo All Rights Reserved.200309129@163.com 
* svn:http://asuraserver.googlecode.com/svn/
* github:https://github.com/QService/QService
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

#include "ServiceInit.h"

/*ҵ����ͷ�ļ�*/

#define Q_CONFIGFILE      "config.xml"
#define Q_CONFIGFOLDER    "config"
#define LOG_FOLDER        "log"

static CServerInit m_objServerInit;

/*��־����*/
class CLogSysTask : public CTask
{
public:
    CLogSysTask(void) : m_pLog(NULL)
    {};
    ~CLogSysTask(void)
    {};

    void setLogSys(CLog *pLog)
    {
        m_pLog = pLog;
    };

    void Run(void)
    {
        m_pLog->Start();
    };

private:
    CLog *m_pLog;
};

/*****************************************************************************/

class CServerTask : public CTask
{
public:
    CServerTask(void) : m_pSV(NULL)
    {};
    ~CServerTask(void)
    {};

    void setServerHandle(CServer *pSV)
    {
        m_pSV = pSV;
    };

    void Run(void)
    {
        m_pSV->Start();
    };

private:
    CServer *m_pSV;
};

/*****************************************************************************/

CServerInit::~CServerInit(void)
{
    std::vector<CEventInterface * >::iterator itInterface;
    for (itInterface = m_vcInterface.begin(); m_vcInterface.end() != itInterface; itInterface++)
    {
        Q_SafeDelete(*itInterface);
    }

    m_vcInterface.clear(); 
}

int CServerInit::Start(void)
{
    int iRtn = Q_RTN_OK;
    CLogSysTask *pLogSysTask = NULL;

    m_strConfig = Q_FormatStr("%s%s%s%s", 
        g_acModulPath, Q_CONFIGFOLDER, Q_PATH_SEPARATOR, Q_CONFIGFILE);
    Q_Printf("config file %s.", m_strConfig.c_str()); 

    /*���������ļ�*/
    Q_Printf("%s", "load config file ...");
    m_objXmlResult = m_objXmlDoc.load_file(m_strConfig.c_str());
    if (status_ok != m_objXmlResult.status)
    {
        Q_Printf("%s", "load config file error.");

        return Q_RTN_FAILE;
    }

    /*��ʼ����־*/    
    pLogSysTask = new(std::nothrow) CLogSysTask();
    if (NULL == pLogSysTask)
    {
        Q_Printf("%s", Q_EXCEPTION_ALLOCMEMORY);

        return Q_ERROR_ALLOCMEMORY;
    }

    pLogSysTask->setLogSys(&m_objLog);
    m_objThread.Execute(pLogSysTask);
    if (!m_objLog.waitForStarted())
    {
        Q_Printf("%s", "log system start timeout.");

        return Q_RTN_FAILE;
    }

    Q_Printf("%s", "init txt log system ...");
    initTxtLog();
    Q_Printf("%s", "init db log system ...");
    if (!initDBLog())
    {
        Q_Printf("%s", "init db log system error.");

        return Q_RTN_FAILE;
    }

    /*��������*/
    Q_Printf("%s", "start service ...");
    iRtn = initServer();
    if (Q_RTN_OK != iRtn)
    {
        return iRtn;
    }

    Q_Printf("%s", "start service successfully!");
    Q_LOG(LOGLV_INFO, "%s", "start service successful!");

    return Q_RTN_OK;
}

void CServerInit::Stop(void)
{
    Q_Printf("%s", "shutdown server...");
    m_objServer.Stop();
    Q_Printf("%s", "stop log system.");
    m_objLog.Stop();
}

void CServerInit::readLinkOtherConfig(std::vector<LinkOther> &vcLinkOther)
{
    pugi::xpath_node_set objNodeSet;
    pugi::xpath_node_set::const_iterator itNode;

    objNodeSet = m_objXmlDoc.select_nodes("//LinkOther");
    for (itNode = objNodeSet.begin(); objNodeSet.end() != itNode; itNode++)
    {
        LinkOther stLinkOther;
        stLinkOther.strIp = itNode->node().child_value("IP");
        stLinkOther.strName = itNode->node().child_value("Name");
        stLinkOther.usPort = atoi(itNode->node().child_value("Port"));

        vcLinkOther.push_back(stLinkOther);
    }
}

bool CServerInit::readConfig(void)
{
    std::string strIp;
    unsigned short usPort = Q_INIT_NUMBER;
    pugi::xpath_node_set objNodeSet;
    pugi::xpath_node_set::const_iterator itNode;

    objNodeSet = m_objXmlDoc.select_nodes("//ServerInfo");
    if (objNodeSet.empty())
    {
        Q_LOG(LOGLV_ERROR, "%s", "no find node //ServerInfo.");

        return false;
    }

    itNode = objNodeSet.begin();

    g_iServerID =  atoi(itNode->node().child_value("ServerID"));
    g_iServerType = atoi(itNode->node().child_value("Type"));
    Q_Printf("service id %d, type %d.", g_iServerID, g_iServerType);

    m_stServerConfig.usThreadNum = atoi(itNode->node().child_value("ThreadNum"));
    m_stServerConfig.strScript = std::string(g_acModulPath) + 
        std::string(itNode->node().child_value("Script"));
    m_stServerConfig.uiTimer = atoi(itNode->node().child_value("Timer"));

    objNodeSet = m_objXmlDoc.select_nodes("//TCP");
    if (!objNodeSet.empty())
    {
        for (itNode = objNodeSet.begin(); objNodeSet.end() != itNode; itNode++)
        {
            strIp = itNode->node().child_value("BindIP");
            usPort = atoi(itNode->node().child_value("Port"));
        }

        m_stServerConfig.mapTcp.insert(std::make_pair(usPort, strIp));
    }

    objNodeSet = m_objXmlDoc.select_nodes("//WebSock");
    if (!objNodeSet.empty())
    {
        for (itNode = objNodeSet.begin(); objNodeSet.end() != itNode; itNode++)
        {
            strIp = itNode->node().child_value("BindIP");
            usPort = atoi(itNode->node().child_value("Port"));
        }

        m_stServerConfig.mapWebSock.insert(std::make_pair(usPort, strIp));
    }

    objNodeSet = m_objXmlDoc.select_nodes("//Http");
    if (!objNodeSet.empty())
    {
        for (itNode = objNodeSet.begin(); objNodeSet.end() != itNode; itNode++)
        {
            strIp = itNode->node().child_value("BindIP");
            usPort = atoi(itNode->node().child_value("Port"));
        }

        m_stServerConfig.mapHttp.insert(std::make_pair(usPort, strIp));
    }

    return true;
}

int CServerInit::initServer(void)
{
    int iRtn = Q_RTN_OK;

    Q_Printf("%s", "read service config...");
    if (!readConfig())
    {
        Q_Printf("%s", "read service config error.");

        return Q_RTN_FAILE;
    }

    try
    {
        for (unsigned short usI = 0; usI < m_stServerConfig.usThreadNum; usI++)
        {
            CEventInterface *pInterface = 
                new(std::nothrow) CDisposeEvent(m_stServerConfig.strScript.c_str());
            if (NULL == pInterface)
            {
                Q_Printf("%s", Q_EXCEPTION_ALLOCMEMORY);

                return Q_ERROR_ALLOCMEMORY;
            }

            m_vcInterface.push_back(pInterface);
        }
    }
    catch(CException &e)
    {
        Q_Printf("get an exception. code %d, message %s", e.getErrorCode(), e.getErrorMsg());

        return e.getErrorCode();
    }     

    /*��ɳ�ʼ��*/
    iRtn = m_objServer.Init(m_stServerConfig.usThreadNum, m_stServerConfig.uiTimer,
        m_vcInterface, 
        m_stServerConfig.mapTcp, m_stServerConfig.mapWebSock, m_stServerConfig.mapHttp);
    if (Q_RTN_OK != iRtn)
    {
        return iRtn;
    }

    /*��ȡ����������������*/
    std::vector<LinkOther> vcLinkOther;
    std::vector<LinkOther>::iterator itLinkOther;

    Q_Printf("%s", "read link other config...");
    readLinkOtherConfig(vcLinkOther);

    /*���÷�����������*/
    CWorkThreadEvent *pThreadEvent = m_objServer.getServerThreadEvent();
    for (unsigned short i = 0; i < m_objServer.getThreadNum(); i++)
    {
        for (itLinkOther = vcLinkOther.begin(); vcLinkOther.end() != itLinkOther; itLinkOther++)
        {
            if (itLinkOther->strName.empty())
            {
                Q_Printf("%s", "link other name empty.");
                return Q_RTN_FAILE;
            }

            if (!(pThreadEvent[i].getLinkOther()->addHost(itLinkOther->strIp.c_str(),
                itLinkOther->usPort,
                itLinkOther->strName.c_str())))
            {
                Q_Printf("add link other %s error.", itLinkOther->strName.c_str());
                return Q_RTN_FAILE;
            }
        }
    }

    /*���뵽�߳�*/
    CServerTask *pTask = new(std::nothrow) CServerTask();
    if (NULL == pTask)
    {
        Q_LOG(LOGLV_ERROR, "%s", Q_EXCEPTION_ALLOCMEMORY);

        return Q_ERROR_ALLOCMEMORY;
    }

    pTask->setServerHandle(&m_objServer);
    m_objThread.Execute(pTask);
    if (!(m_objServer.waitForStarted()))
    {
        Q_LOG(LOGLV_ERROR, "%s", "start server error.");

        return Q_RTN_FAILE;
    }

    return Q_RTN_OK;
}

void CServerInit::initTxtLog(void)
{
    std::string strLogPath;
    std::string strLogName;
    int iMaxSize = 5*1024*1024;
    int iPriority = 600;

    g_pTxtLoger = new(std::nothrow) CTxtLoger();
    if (NULL == g_pTxtLoger)
    {
        Q_Printf("%s", Q_EXCEPTION_ALLOCMEMORY);
        return;
    }

    strLogPath = Q_FormatStr("%s%s%s", g_acModulPath, LOG_FOLDER, Q_PATH_SEPARATOR);    

    m_objXmlNode = m_objXmlDoc.child("QServer").child("TxtLog");
    if (!m_objXmlNode.empty())
    {
        strLogName = m_objXmlNode.child_value("LogName");
        Q_Printf("log file %s.", strLogName.c_str());

        iMaxSize = atoi(m_objXmlNode.child_value("MaxSize"));
        Q_Printf("log file size %d.", iMaxSize);

        iPriority = atoi(m_objXmlNode.child_value("Priority"));
        Q_Printf("log priority %d.", iPriority);
    }

    g_pTxtLoger->setLogMaxSize(iMaxSize);    
    g_pTxtLoger->setPriority((LOG_LEVEL)iPriority);
    g_pTxtLoger->setLogFile(std::string(strLogPath + strLogName).c_str());
    g_pTxtLoger->Open();

    g_TxtLogerFD = m_objLog.addLoger(g_pTxtLoger);

    return;
}

bool CServerInit::initDBLog(void)
{
    m_objXmlNode = m_objXmlDoc.child("QServer").child("DBLog");
    if (!m_objXmlNode.empty())
    {
        std::string strIp;
        std::string strUser;
        std::string strPWD;
        std::string strDB;
        unsigned short usPort = Q_INIT_NUMBER;

        g_pDBLoger = new(std::nothrow) CDBLoger();
        if (NULL == g_pDBLoger)
        {
            Q_Printf("%s", Q_EXCEPTION_ALLOCMEMORY);
            return false;
        }

        strIp = m_objXmlNode.child_value("Ip");
        strUser = m_objXmlNode.child_value("User");
        strPWD = m_objXmlNode.child_value("PWD");
        strDB = m_objXmlNode.child_value("DB");
        usPort = atoi(m_objXmlNode.child_value("Port"));
        Q_Printf("link to mysql.ip %s port %d user name %s password %s database %s...", 
            strIp.c_str(), usPort, strUser.c_str(), strPWD.c_str(), strDB.c_str());
        if (!g_pDBLoger->Init(strIp.c_str(), usPort, strUser.c_str(), strPWD.c_str(), strDB.c_str()))
        {
            Q_SafeDelete(g_pDBLoger);

            return false;
        }

        g_DBLogerFD = m_objLog.addLoger(g_pDBLoger);
        if (Q_INVALID_SOCK == g_DBLogerFD)
        {
            Q_SafeDelete(g_pDBLoger);

            return false;
        }
    }

    return true;
}

/*************************************************
* Function name:Service_InitProgram(void)
* Description  :��ʼ�����еķ���
* IN           :NONE
* OUT          :NONE
* Return       :RTN_OK --�ɹ� ���� --ʧ��
* Make By      :lqf/200309129@163.com
* Date Time    :2011/06/07
* Modification 
* ......record :first program
**************************************************/
int Service_InitProgram(void)
{
    int iRtn = Q_RTN_OK;

    try
    {
        iRtn = m_objServerInit.Start();
    }
    catch(CException &e)
    {
        Q_Printf("get an exception. code %d, message %s", e.getErrorCode(), e.getErrorMsg());

        iRtn = e.getErrorCode();
    }

    if (Q_RTN_OK != iRtn)
    {
        Service_Exit();
    }

    return iRtn;
}

/*************************************************
* Function name:Service_Exit(void)
* Description  :�˳�
* IN           :NONE
* OUT          :NONE
* Return       :RTN_OK --�ɹ� ���� --ʧ��
* Make By      :lqf/200309129@163.com
* Date Time    :2011/06/07
* Modification 
* ......record :first program
**************************************************/
int Service_Exit(void)
{
    m_objServerInit.Stop(); 

    Q_Printf("%s", "stop service successfully.");

    return Q_RTN_OK;
}

/*************************************************
* Function name:SigHandEntry(int iSigNum)
* Description  :�˳��ź�
* IN           :iSigNum --�ź�
* OUT          :NONE
* Return       :RTN_OK --�ɹ� ���� --ʧ��
* Make By      :lqf/200309129@163.com
* Date Time    :2011/06/07
* Modification 
* ......record :first program
**************************************************/
void SigHandEntry(int iSigNum)
{
#ifndef Q_OS_WIN32
    Q_LOG(LOGLV_INFO, "catch signal %d.", iSigNum);
    Q_Printf("catch signal %d.", iSigNum);

    g_objExitMutex.Lock();
    g_objExitCond.Signal();
    Q_LOG(LOGLV_INFO, "%s", "signal stop service.");
    g_objExitMutex.unLock();
#endif
}
