
// StreeTestingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StreeTesting.h"
#include "StreeTestingDlg.h"
#include "afxdialogex.h"
#include <string>
#include <vector>
#include <map>
#include <sys/timeb.h>
#include <sstream>

#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CStreeTestingDlg �Ի���




CStreeTestingDlg::CStreeTestingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStreeTestingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStreeTestingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_Edit_ThreadNum);
    DDX_Control(pDX, IDC_EDIT2, m_Edit_LinkNum);
    DDX_Control(pDX, IDC_EDIT5, m_Edit_SendNum);
    DDX_Control(pDX, IDC_EDIT4, m_Edit_SendTime);
    DDX_Control(pDX, IDC_EDIT6, m_Edit_OpCode1);
    DDX_Control(pDX, IDC_EDIT7, m_Edit_EnCode1);
    DDX_Control(pDX, IDC_EDIT8, m_Edit_Msg1);
    DDX_Control(pDX, IDC_Closed, m_Closed);
    DDX_Control(pDX, IDC_EDIT16, m_Edit_Port);
    DDX_Control(pDX, IDC_EDIT15, m_Edit_Ip);
    DDX_Control(pDX, IDC_BUTTON1, m_But_Start);
    DDX_Control(pDX, IDC_BUTTON2, m_But_Stop);
    DDX_Control(pDX, IDC_Show, m_Show);
    DDX_Control(pDX, IDC_RUNNING, m_Statues);
}

BEGIN_MESSAGE_MAP(CStreeTestingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CStreeTestingDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CStreeTestingDlg::OnBnClickedButton2)
    ON_WM_TIMER()
    ON_WM_CLOSE()
    ON_EN_CHANGE(IDC_EDIT2, &CStreeTestingDlg::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CStreeTestingDlg ��Ϣ�������

BOOL CStreeTestingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������ 
    m_Edit_ThreadNum.SetWindowTextW(L"4");
    m_Edit_LinkNum.SetWindowTextW(L"200");
    m_Edit_SendNum.SetWindowTextW(L"200");
    m_Edit_Ip.SetWindowTextW(L"127.0.0.1");
    m_Edit_Port.SetWindowTextW(L"15000");
    m_Edit_SendTime.SetWindowTextW(L"10");

    m_Edit_OpCode1.SetWindowTextW(L"2");
    m_Edit_EnCode1.SetWindowTextW(L"0");
    m_Edit_EnCode1.EnableWindow(FALSE);
    m_Edit_Msg1.SetWindowTextW(L"{\"MSG\": \"this is from client..\"}");    

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CStreeTestingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CStreeTestingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CStreeTestingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

struct Info
{
    int iLinkNum;
    int iSendNum;
    int iSendTime;
    std::string strMsg;
    std::vector<SOCKET> vcSock;
};

struct ThreadInfo
{
    int iIndex;
    Info *pInfo;
};

enum RunOrStop
{
    Statues_Run = 0,
    Statues_Stop,
};

ThreadInfo *g_pThreadInfo = NULL;
Info g_stInfo;
long g_RunIngThread = 0;
long g_RunOrStop = Statues_Run;
long g_ClosedSock = 0;

#ifdef WIN32
#define  Q_TIMEB  _timeb
#define  ftime    _ftime
#else
#define  Q_TIMEB timeb
#endif

template<class T>
std::string Q_ToString(T number)
{
    std::stringstream strStream;

    strStream << number;

    return strStream.str();
};

std::string Q_Now(void)
{
    char acTimeStr[30] = {0};
    time_t t = time(NULL);	

    strftime(acTimeStr, sizeof(acTimeStr) - 1, "%Y-%m-%d %H:%M:%S", localtime(&t));

    return std::string(acTimeStr);
}

unsigned int __stdcall Run(void * args)
{
    InterlockedExchangeAdd(&g_RunIngThread, 1);

    ThreadInfo *pThreadInfo = (ThreadInfo*)args;

    int iRtn = 0;
    int iCount = 0;
    char acRtnMsg[1024];
    struct Q_TIMEB stBgTime;
    struct Q_TIMEB stEndTime;
    long long t_sec = 0;
    long long t_ms = 0;
    unsigned int uiTime = 0;
    unsigned int uiCount = 0;
    int iBegainPos = (pThreadInfo->iIndex) * (pThreadInfo->pInfo->iLinkNum);
    int iEndPos = (pThreadInfo->iIndex + 1) * (pThreadInfo->pInfo->iLinkNum);
    FILE *pFile = NULL;
    std::string strFileName;
    strFileName = "StreeTesting" + Q_ToString(pThreadInfo->iIndex) + ".txt";

    ULONGLONG size = 0;  
    CString strFilePath;  
    CFileStatus fileStatus;

    strFilePath.Format(L"%s", strFileName.c_str());

    if (CFile::GetStatus(strFilePath, fileStatus))  
    {  
        size = fileStatus.m_size;  
    } 

    pFile = fopen(strFileName.c_str(), "a");
    if (NULL == pFile)
    {
        InterlockedExchangeAdd(&g_RunIngThread, -1);
        return 1;
    }

    if (0 == size)
    {
        std::string strTmp("ʱ��	������ʱ	��������\n");
        fwrite(strTmp.c_str(), 1, strTmp.size(), pFile);
    }

    while(Statues_Run == InterlockedExchangeAdd(&g_RunOrStop, 0))
    {
        iCount = 0;
        ftime(&stBgTime);

        for (int i = iBegainPos; i < iEndPos; i++)
        {
            SOCKET sock = pThreadInfo->pInfo->vcSock[i];
            if (-1 == sock)
            {
                continue;
            }
            
            iRtn = send(sock, pThreadInfo->pInfo->strMsg.c_str(), 
                pThreadInfo->pInfo->strMsg.size(), 0);
            if (0 >= iRtn)
            {
                closesocket(sock);
                pThreadInfo->pInfo->vcSock[i] = -1;
                InterlockedExchangeAdd(&g_ClosedSock, 1);
                continue;
            }

            iRtn = recv(sock, acRtnMsg, sizeof(acRtnMsg), 0);
            if (0 >= iRtn)
            {
                closesocket(sock);
                pThreadInfo->pInfo->vcSock[i] = -1;
                InterlockedExchangeAdd(&g_ClosedSock, 1);
            }

            iCount++;
            if (iCount >= pThreadInfo->pInfo->iSendNum)
            {
                break;
            }
        }

        ftime(&stEndTime);

        t_sec = stEndTime.time - stBgTime.time;//��������
        t_ms = stEndTime.millitm - stBgTime.millitm;//���������

        uiCount += iCount;
        uiTime += (unsigned int)(t_sec*1000 + t_ms);

        if (uiTime >= 60 * 1000)
        {
            std::string strWriteMsg(Q_Now() + "	" + Q_ToString(uiTime) + 
                "	" + Q_ToString(uiCount) + "\n");
            fwrite(strWriteMsg.c_str(), 1, strWriteMsg.size(), pFile);
            fflush(pFile);

            uiTime = 0;
            uiCount = 0;
        }

        if (pThreadInfo->pInfo->iSendTime > 0)
        {
            Sleep(pThreadInfo->pInfo->iSendTime);
        }
    }

    fclose(pFile);
    pFile = NULL;

    InterlockedExchangeAdd(&g_RunIngThread, -1);
    return 0;
}

std::string CStrToStr(CString &cStr)
{
    char acTmp[1024] = {0};  
    wcstombs(acTmp, cStr, 1024);
    return std::string(acTmp);
}

void StartRun(int &iThreadNum, Info &stInfo)
{   
    g_pThreadInfo = new ThreadInfo[iThreadNum];
    for (int i = 0; i < iThreadNum; i++)
    {
        unsigned int uiThread = 0;

        g_pThreadInfo[i].iIndex = i;
        g_pThreadInfo[i].pInfo = &g_stInfo;

        _beginthreadex(NULL, 0, Run, &g_pThreadInfo[i], 0, &uiThread);
    }
}

void CStreeTestingDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (Statues_Run == InterlockedExchangeAdd(&g_RunOrStop, 0))
    {
        CString cStrTmp;
        cStrTmp.Format(L"%d", InterlockedExchangeAdd(&g_ClosedSock, 0));
        m_Closed.SetWindowTextW(cStrTmp);

        KillTimer(1);
        SetTimer(1, 2000, NULL);

        m_Statues.GetWindowTextW(cStrTmp);
        if (0 == cStrTmp.GetLength())
        {
            m_Statues.SetWindowTextW(L"��������...");
        }
        else
        {
            m_Statues.SetWindowTextW(L"");
        }
    }
    else
    {
        KillTimer(1);
        m_Statues.SetWindowTextW(L"ֹͣ...");
    }
}

bool CStreeTestingDlg::ReadVal(unsigned char &cEnType, unsigned short &usOpCode,
    int &iThreadNum, unsigned short &usPort, 
    CString &cStrIp, CString &cCtrMsg)
{
    CString cstrTmp;
    std::string strTmp;

    //��ֵ
    m_Edit_ThreadNum.GetWindowText(cstrTmp);
    strTmp = CStrToStr(cstrTmp);
    iThreadNum = atoi(strTmp.c_str());
    if (iThreadNum <= 0)
    {
        AfxMessageBox(L"�߳������벻�Ϸ�");
        return false;
    }

    m_Edit_LinkNum.GetWindowText(cstrTmp);
    strTmp = CStrToStr(cstrTmp);
    g_stInfo.iLinkNum = atoi(strTmp.c_str());
    if (g_stInfo.iLinkNum <= 0)
    {
        AfxMessageBox(L"���������벻�Ϸ�");
        return false;
    }

    m_Edit_SendNum.GetWindowText(cstrTmp);
    strTmp = CStrToStr(cstrTmp);
    g_stInfo.iSendNum = atoi(strTmp.c_str());
    if (g_stInfo.iSendNum <= 0)
    {
        AfxMessageBox(L"���������벻�Ϸ�");
        return false;
    }

    m_Edit_Ip.GetWindowText(cStrIp);
    if (0 == cStrIp.GetLength())
    {
        AfxMessageBox(L"IP���벻�Ϸ�");
        return false;
    }

    m_Edit_Port.GetWindowText(cstrTmp);
    strTmp = CStrToStr(cstrTmp);
    usPort = atoi(strTmp.c_str());
    if (usPort <= 0)
    {
        AfxMessageBox(L"�˿����벻�Ϸ�");
        return false;
    }

    m_Edit_SendTime.GetWindowText(cstrTmp);
    strTmp = CStrToStr(cstrTmp);
    g_stInfo.iSendTime = atoi(strTmp.c_str());
    if (g_stInfo.iSendTime < 0)
    {
        AfxMessageBox(L"���ͼ�����벻�Ϸ�");
        return false;
    }

    m_Edit_OpCode1.GetWindowText(cstrTmp);
    strTmp = CStrToStr(cstrTmp);
    usOpCode = atoi(strTmp.c_str());
    if (usOpCode < 0)
    {
        AfxMessageBox(L"���������벻�Ϸ�");
        return false;
    }

    m_Edit_EnCode1.GetWindowText(cstrTmp);
    strTmp = CStrToStr(cstrTmp);
    cEnType = atoi(strTmp.c_str());
    if (cEnType != 0)
    {
        AfxMessageBox(L"�����������벻�Ϸ�");
        return false;
    }

    m_Edit_Msg1.GetWindowText(cCtrMsg);
    if (0 == cCtrMsg.GetLength())
    {
        AfxMessageBox(L"��Ϣ���벻�Ϸ�");
        return false;
    }

    return true;
}

class CEnableBut 
{
public:
    CEnableBut(CButton *pButton)
    {
        m_But_Start = pButton;
        m_But_Start->EnableWindow(FALSE);
    };

    ~CEnableBut()
    {
        m_But_Start->EnableWindow(TRUE);
    };
private:
    CButton *m_But_Start;
};

void CStreeTestingDlg::OnBnClickedButton1()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CEnableBut objEnableBut(&m_But_Start);
    if (0 != InterlockedExchangeAdd(&g_RunIngThread, 0))
    {
        AfxMessageBox(L"���ڹ���...");
        return;
    }
    
    g_RunIngThread = 0;
    g_ClosedSock = 0;
    g_RunOrStop = Statues_Run;
    if (NULL != g_pThreadInfo)
    {
        delete []g_pThreadInfo;
        g_pThreadInfo = NULL;
    }    

    unsigned char cEnType = 0;
    unsigned short usOpCode = 0;
    int iThreadNum = 1;
    CString cStrIP;
    CString cStrMsg;
    std::string strIp;
    std::string strMsg;
    unsigned short usPort;
    if (!ReadVal(cEnType, usOpCode, iThreadNum, usPort, cStrIP, cStrMsg))
    {
        return;
    }

    strIp = CStrToStr(cStrIP);
    strMsg = CStrToStr(cStrMsg);
    
    //�����ַ
    sockaddr_in stAddr;
    struct addrinfo stAddrInfo = {0};
    struct addrinfo *pAddrInfo = NULL;

    stAddrInfo.ai_flags  = AI_PASSIVE;
    stAddrInfo.ai_family = AF_INET;
    if (0 != getaddrinfo(strIp.c_str(), NULL, &stAddrInfo, &pAddrInfo))
    {
        if (NULL != pAddrInfo)
        {
            freeaddrinfo(pAddrInfo);
        }

        AfxMessageBox(L"����IP��ַ����.");

        return;
    }

    memcpy(&stAddr, pAddrInfo->ai_addr, pAddrInfo->ai_addrlen);
    stAddr.sin_port = htons(usPort);
    freeaddrinfo(pAddrInfo);

    const unsigned int uiMS = 5000;

    //����socket
    m_Show.SetWindowTextW(L"����socket:");
    for (int i = 0; i < iThreadNum * g_stInfo.iLinkNum; i++)
    {
        SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
        if (-1 == sock)
        {
            AfxMessageBox(L"����socket�������.");
            return;
        }

        if (0 != connect(sock, (sockaddr*)(&stAddr), sizeof(stAddr)))
        {
            AfxMessageBox(L"socket���Ӵ���.");
            closesocket(sock);
            return;
        }

        (void)setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)(&uiMS), sizeof(uiMS));  
        (void)setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)(&uiMS), sizeof(uiMS));

        g_stInfo.vcSock.push_back(sock);

        CString cstrtmp;
        cstrtmp.Format(L"%d", i + 1);
        m_Closed.SetWindowTextW(cstrtmp);

        Sleep(10);
    }

    char acMsg[1024 * 2] = {0};
    unsigned short usTmp;
    /*unsigned short usHeadLens = sizeof(unsigned short) + sizeof(unsigned short) + sizeof(char);*/

    usTmp = ntohs(strMsg.size());
    memcpy(acMsg, &usTmp, sizeof(usTmp));
    /*usTmp = ntohs(usOpCode);
    memcpy(acMsg + sizeof(usTmp), &usTmp, sizeof(usTmp));
    memcpy(acMsg + sizeof(usTmp) * 2, &cEnType, sizeof(cEnType));*/
    if (!strMsg.empty())
    {
        memcpy(acMsg + sizeof(usTmp), strMsg.c_str(), strMsg.size());
    }

    std::string strTmp(acMsg, sizeof(usTmp) + strMsg.size());
    g_stInfo.strMsg = strTmp;

    m_Show.SetWindowTextW(L"�Ͽ�������:");
    m_Closed.SetWindowTextW(L"0");

    StartRun(iThreadNum, g_stInfo);

    SetTimer(1, 2000, NULL);
}

void CStreeTestingDlg::OnBnClickedButton2()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CEnableBut objEnableBut(&m_But_Stop);
    InterlockedExchange(&g_RunOrStop, Statues_Stop);
    while(0 != InterlockedExchangeAdd(&g_RunIngThread, 0))
    {
        Sleep(100);
    }

    std::vector<SOCKET>::iterator itSock;
    //�ͷ�
    for (itSock = g_stInfo.vcSock.begin(); g_stInfo.vcSock.end() != itSock; itSock++)
    {
        if (-1 != *itSock)
        {
            closesocket(*itSock);
        }
    }

    g_stInfo.vcSock.clear();

    m_Statues.SetWindowTextW(L"ֹͣ...");
}


void CStreeTestingDlg::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    std::vector<SOCKET>::iterator itSock;
    for (itSock = g_stInfo.vcSock.begin(); g_stInfo.vcSock.end() != itSock; itSock++)
    {
        if (-1 != *itSock)
        {
            closesocket(*itSock);
        }
    }

    g_stInfo.vcSock.clear();

    if (NULL != g_pThreadInfo)
    {
        delete []g_pThreadInfo;
        g_pThreadInfo = NULL;
    }

    CDialogEx::OnClose();
}


void CStreeTestingDlg::OnEnChangeEdit2()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CString cStr;
    m_Edit_LinkNum.GetWindowTextW(cStr);
    m_Edit_SendNum.SetWindowTextW(cStr);
}
