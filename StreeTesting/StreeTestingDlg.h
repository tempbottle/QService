
// StreeTestingDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CStreeTestingDlg �Ի���
class CStreeTestingDlg : public CDialogEx
{
// ����
public:
	CStreeTestingDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_STREETESTING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
    void OnTimer(UINT_PTR nIDEvent);

private:
    bool ReadVal(unsigned char &cEnType, unsigned short &usOpCode,
        int &iThreadNum, unsigned short &usPort, 
        CString &cStrIp, CString &cCtrMsg);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CEdit m_Edit_ThreadNum;
    CEdit m_Edit_LinkNum;
    CEdit m_Edit_SendNum;
    CEdit m_Edit_SendTime;
    CEdit m_Edit_OpCode1;
    CEdit m_Edit_EnCode1;
    CEdit m_Edit_Msg1;
    CStatic m_Closed;
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    CEdit m_Edit_Port;
    CEdit m_Edit_Ip;
    afx_msg void OnClose();
    CButton m_But_Start;
    CButton m_But_Stop;
    CStatic m_Show;
    afx_msg void OnEnChangeEdit2();
    CStatic m_Statues;
};
