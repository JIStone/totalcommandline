// FormViewShell.h : FormViewShell ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CFormViewShellApp:
// �� Ŭ������ ������ ���ؼ��� FormViewShell.cpp�� �����Ͻʽÿ�.
//

class CFormViewShellApp : public CWinApp
{
public:
	CFormViewShellApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	void OnFileOpen(void);
};

extern CFormViewShellApp theApp;