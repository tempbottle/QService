
// StreeTesting.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStreeTestingApp:
// �йش����ʵ�֣������ StreeTesting.cpp
//

class CStreeTestingApp : public CWinApp
{
public:
	CStreeTestingApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CStreeTestingApp theApp;