// OperPlus.cpp : implementation file
//

#include "stdafx.h"
#include "FormViewShell.h"
#include "OperPlus.h"
#include ".\operplus.h"


// COperPlus

IMPLEMENT_DYNCREATE(COperPlus, CWinThread)

COperPlus::COperPlus()
: m_pDlg(NULL)
{
}

COperPlus::~COperPlus()
{
}

BOOL COperPlus::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int COperPlus::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(COperPlus, CWinThread)
END_MESSAGE_MAP()


// COperPlus message handlers

void COperPlus::SetDialogPointer(CFormViewShellView* ptr)
{
	m_pDlg = ptr;
}

int ggg = 0;
int COperPlus::Run()
{
	// TODO: Add your specialized code here and/or call the base class

	CSingleLock sLock(&(m_pDlg->m_CS));

	//for(;;)
	{
		//sLock.Lock();
		//if(!ggg){
		m_pDlg->m_ProgressCtrl.UpdateWindow();
			ggg = 1;
		//}

		//sLock.Unlock();
	}

	return CWinThread::Run();
}
