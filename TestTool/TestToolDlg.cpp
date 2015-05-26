
// TestToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestTool.h"
#include "TestToolDlg.h"
#include "afxdialogex.h"
#ifndef _WIN64
//#include "../vld/vld.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "../vld/vld.h"
#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "libevent.lib")
#pragma comment(lib, "libevent_core.lib")
#pragma comment(lib, "libevent_extras.lib")
#if _DEBUG
#pragma comment(lib, "libcurl_a_debug.lib")
#else
#pragma comment(lib, "libcurl_a.lib")
#endif
#pragma comment(lib, "QBase.lib")

#define Q_CLINETTIMER 50

class CWorkerTask : public CTask
{
public:
    void setWorker(CWorker *pWorker)
    {
        m_pWorker = pWorker;
    };

    void Run(void)
    {
        m_pWorker->Start();
    };

private:
    CWorker *m_pWorker;
};


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


// CTestToolDlg �Ի���



CTestToolDlg::CTestToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestToolDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_IPADDRESS1, m_CtrIp);
    DDX_Control(pDX, IDC_EDIT1, m_CtrPort);
    DDX_Control(pDX, IDC_EDIT4, m_CtrLinkNum);
    DDX_Control(pDX, IDC_BUTTON2, m_CtrLinkBtt);
    DDX_Control(pDX, IDC_EDIT3, m_CtrInput);
    DDX_Control(pDX, IDC_EDIT2, m_CtrOutPut);
    DDX_Control(pDX, IDC_LUAMEMO, m_CtrLuaMemory);
    DDX_Control(pDX, IDC_CHECK1, m_CtrDebug);
}

BEGIN_MESSAGE_MAP(CTestToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_MESSAGE(WM_MESSAGE_SHOWMFCMSG, ShowMsg)
    ON_MESSAGE(WM_MESSAGE_SHOWLUAMEMORY, ShowLuaMemo)
    ON_MESSAGE(WM_MESSAGE_ENBLELINKBUTT, EnableLinkButt)
    ON_BN_CLICKED(IDC_BUTTON2, &CTestToolDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON4, &CTestToolDlg::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_BUTTON3, &CTestToolDlg::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON1, &CTestToolDlg::OnBnClickedButton1)
    ON_WM_CLOSE()
//    ON_BN_CLICKED(IDC_CHECK1, &CTestToolDlg::OnBnClickedCheck1)
ON_BN_CLICKED(IDC_CHECK1, &CTestToolDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CTestToolDlg ��Ϣ�������

BOOL CTestToolDlg::OnInitDialog()
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
    srand((unsigned int)time(NULL));

    g_iServerID = rand() % 1023 + 1;
    std::string strProPath;
    Q_GetProPath(strProPath);
    memcpy(g_acModulPath, strProPath.c_str(), strProPath.size());
    g_hWnd = m_hWnd;
    m_Sock = Q_INVALID_SOCK;

    if (initLua())
    {
        m_objBinary.setLua(m_pLua);
        m_objWorker.initLua();
        m_objWorker.setTimer(Q_CLINETTIMER);

        CWorkerTask *pTask = new CWorkerTask();
        pTask->setWorker(&m_objWorker);
        CThread objThread;    
        objThread.Execute(pTask);
    }    

    m_CtrIp.SetWindowTextA("127.0.0.1");
    m_CtrPort.SetWindowTextA("15000");
    m_CtrLinkNum.SetWindowTextA("1");

    std::string strTemp = strProPath + "TestToolTemplate.lua";
    FILE *pFile = fopen(strTemp.c_str(), "r");
    if (NULL != pFile)
    {
        char acTmp[2048] = {0};
        fread(acTmp, 1, sizeof(acTmp) - 1, pFile);
        
        std::string strTmp(acTmp);
        strTmp = Q_Replace(strTmp, "\n", "\r\n");
        m_strTemplateLua = strTmp.c_str();
        /*std::list<std::string>::iterator itTemplateLua;
        std::list<std::string> lstTemplateLua;
        Q_Split(std::string(acTmp), "\n", lstTemplateLua);

        for (itTemplateLua = lstTemplateLua.begin(); lstTemplateLua.end() != itTemplateLua; itTemplateLua++)
        {
            m_strTemplateLua += itTemplateLua->c_str() + CString("\r\n");
        }*/

        fclose(pFile);
    }

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTestToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestToolDlg::OnPaint()
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
HCURSOR CTestToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CTestToolDlg::ShowMsg(WPARAM wParam, LPARAM lParam)
{
    char *pMsg = (char*)wParam;
    size_t uiLens = lParam; 
    std::string strTmp(pMsg, uiLens);
    CString cstrTmp;

    m_CtrOutPut.GetWindowTextA(cstrTmp);
    if (cstrTmp.GetLength() >= 1024 * 10)
    {
        cstrTmp = "";
    }

    if (0 != cstrTmp.GetLength())
    {
        cstrTmp = strTmp.c_str() + CString("\r\n") +cstrTmp;
    }
    else
    {
        cstrTmp = strTmp.c_str();
    }

    m_CtrOutPut.SetWindowTextA(cstrTmp);

    Q_SafeDelete(pMsg);

    return 0;
}

LRESULT CTestToolDlg::ShowLuaMemo(WPARAM wParam, LPARAM lParam)
{
    char *pMsg = (char*)wParam;
    size_t uiLens = lParam; 
    std::string strTmp(pMsg, uiLens);
    Q_SafeDelete(pMsg);

    m_CtrLuaMemory.SetWindowTextA(strTmp.c_str());

    return 0;
}

LRESULT CTestToolDlg::EnableLinkButt(WPARAM wParam, LPARAM lParam)
{
    m_CtrLinkBtt.EnableWindow(TRUE);

    return 0;
}

Q_SOCK CTestToolDlg::initSock(const char *pszIp, const unsigned short usPort)
{
    SOCKET sock = Q_INVALID_SOCK;

    CNETAddr objAddr;
    if (Q_RTN_OK != objAddr.setAddr(pszIp, usPort))
    {
        return Q_INVALID_SOCK;
    }

    //����socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (Q_INVALID_SOCK == sock)
    {
        return Q_INVALID_SOCK;
    }

    if (0 != connect(sock, objAddr.getAddr(), objAddr.getAddrSize()))
    {
        closesocket(sock);
        sock = Q_INVALID_SOCK;

        return sock;
    }

    int iKeepAlive = 1;
    (void)setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char *)&iKeepAlive, sizeof(iKeepAlive));

    return sock;
}

bool CTestToolDlg::initLua(void)
{
    m_pLua = luaL_newstate();
    if (NULL == m_pLua)
    {
        return false;
    }

    luaL_openlibs(m_pLua);
    m_objRegLua.setLState(m_pLua);
    m_objRegLua.Register();

    luabridge::getGlobalNamespace(m_pLua)
        .addFunction("showMfcMsg", showMfcMsg)
        .addFunction("showLuaMemory", showLuaMemory);

    luabridge::getGlobalNamespace(m_pLua)
        .beginClass<CTestToolDlg>("CTestToolDlg")
            .addFunction("sendMainMsg", &CTestToolDlg::sendMainMsg)
            .addFunction("sendBMainMsg", &CTestToolDlg::sendBMainMsg)
        .endClass();

    std::string strLuaPath = Q_FormatStr("%sToolLua%sTool.lua", 
        g_acModulPath, Q_PATH_SEPARATOR);
    int iRtn = luaL_dofile(m_pLua, strLuaPath.c_str());
    if (Q_RTN_OK != iRtn)
    {
        const char *pError = lua_tostring(m_pLua, -1);
        std::string strLuaError = ((NULL == pError) ? "" : pError);
        if (NULL != m_pLua)
        {
            lua_close(m_pLua);
            m_pLua = NULL;
        }

        showMfcMsg(strLuaError.c_str(), strLuaError.size());

        return false;
    }

    try
    {
        luabridge::getGlobal(m_pLua, "Lua_setMainParam")(this, &m_objBinary);
    }
    catch(luabridge::LuaException &e)
    {
        showMfcMsg(e.what(), strlen(e.what()));

        return false;
    }

    return true;
}

void CTestToolDlg::sendMainMsg(const char *pszMsg, const size_t iLens)
{
    size_t iHeadLens = Q_INIT_NUMBER;
    const char *pHead = m_objTcpParser.createHead(iLens, iHeadLens);
    Q_SockWrite(m_Sock, pHead, iHeadLens);
    if (NULL != pszMsg
        && 0 != iLens)
    {
        Q_SockWrite(m_Sock, pszMsg, iLens);
    }
}

void CTestToolDlg::sendBMainMsg(void)
{ 
    size_t iHeadLens = Q_INIT_NUMBER;
    CBuffer *pBuffer = m_objBinary.getBufferMgr();
    const char *pHead = m_objTcpParser.createHead(pBuffer->getLens(), iHeadLens);
    Q_SockWrite(m_Sock, pHead, iHeadLens);
    if (0 != pBuffer->getLens())
    {
        Q_SockWrite(m_Sock, pBuffer->getBuffer(), pBuffer->getLens());
    }
}

//link
void CTestToolDlg::OnBnClickedButton2()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString cstrVal;
    m_CtrIp.GetWindowTextA(cstrVal);
    std::string strIp = cstrVal.GetBuffer();
    m_CtrPort.GetWindowTextA(cstrVal);
    unsigned short usPort = atoi(cstrVal.GetBuffer());

    m_Sock = initSock(strIp.c_str(), usPort);
    if (Q_INVALID_SOCK == m_Sock)
    {
        AfxMessageBox("��������ʧ��.");
        return;
    }

    size_t iHeadLens = Q_INIT_NUMBER;
    const char *pszHead = m_objTcpParser.createHead(sizeof(m_Sock), iHeadLens);
    m_objWorker.sendMainMsg(pszHead, iHeadLens);
    m_objWorker.sendMainMsg((const char*)&m_Sock, sizeof(m_Sock));

    m_CtrLinkBtt.EnableWindow(FALSE);
}

//create
void CTestToolDlg::OnBnClickedButton4()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString cstrVal;
    m_CtrLinkNum.GetWindowTextA(cstrVal);
    if (0 == cstrVal.GetLength())
    {
        AfxMessageBox("������������.");
        return;
    }

    unsigned int uiCount = atoi(cstrVal.GetBuffer());
    if (uiCount <= 0)
    {
        AfxMessageBox("�������������0.");
        return;
    }

    m_CtrIp.GetWindowTextA(cstrVal);
    std::string strIp = cstrVal.GetBuffer();
    m_CtrPort.GetWindowTextA(cstrVal);
    unsigned short usPort = atoi(cstrVal.GetBuffer());

    for (unsigned int i = 0; i < uiCount; i++)
    {
        evutil_socket_t sock = initSock(strIp.c_str(), usPort);
        if (INVALID_SOCKET == sock)
        {
            AfxMessageBox("��������ʧ��.");
            return;
        }

        size_t iHeadLens = Q_INIT_NUMBER;
        const char *pszHead = m_objTcpParser.createHead(sizeof(sock), iHeadLens);
        m_objWorker.sendAssistMsg(pszHead, iHeadLens);
        m_objWorker.sendAssistMsg((const char*)&sock, sizeof(sock));
    }
}

//send
void CTestToolDlg::OnBnClickedButton3()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (INVALID_SOCKET == m_Sock)
    {
        AfxMessageBox("δ����.");
        return;
    }
    if (NULL == m_pLua)
    {
        AfxMessageBox("lua init error.");
        return;
    }

    CString cstrVal;
    m_CtrInput.GetWindowTextA(cstrVal);
    std::string strMsg = cstrVal.GetBuffer();
    bool bDebug = true;

    int iCheck = m_CtrDebug.GetCheck();
    if (1 == iCheck)
    {
        bDebug = false;
    }

    //ִ��lua
    try
    {
        luabridge::getGlobal(m_pLua, "Lua_createMsg")(strMsg, bDebug);
    }
    catch(luabridge::LuaException &e)
    {
        showMfcMsg(e.what(), strlen(e.what()));

        return;
    }
}

//clear
void CTestToolDlg::OnBnClickedButton1()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_CtrOutPut.SetWindowTextA("");
}


void CTestToolDlg::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    m_objWorker.Stop();
    if (NULL != m_pLua)
    {
        lua_close(m_pLua);
        m_pLua = NULL;
    }

    CDialogEx::OnClose();
}


void CTestToolDlg::OnBnClickedCheck1()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    g_iChecked = m_CtrDebug.GetCheck();
    if (1 == g_iChecked)
    {
        m_CtrInput.SetWindowTextA(m_strTemplateLua);
    }
    else
    {
        m_CtrInput.SetWindowTextA("");
    }
}
