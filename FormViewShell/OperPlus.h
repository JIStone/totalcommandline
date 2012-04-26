#pragma once

#include "FormViewShellView.h"

// COperPlus

class COperPlus : public CWinThread
{
	DECLARE_DYNCREATE(COperPlus)

protected:
	COperPlus();           // protected constructor used by dynamic creation
	virtual ~COperPlus();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CFormViewShellView* m_pDlg;
	void SetDialogPointer(CFormViewShellView* ptr);
	virtual int Run();
};


