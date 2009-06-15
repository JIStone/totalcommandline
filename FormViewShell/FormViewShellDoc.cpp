// FormViewShellDoc.cpp : CFormViewShellDoc Ŭ������ ����
//

#include "stdafx.h"
#include "FormViewShell.h"

#include "FormViewShellDoc.h"
#include "FormViewShellView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFormViewShellDoc

IMPLEMENT_DYNCREATE(CFormViewShellDoc, CDocument)

BEGIN_MESSAGE_MAP(CFormViewShellDoc, CDocument)
END_MESSAGE_MAP()


// CFormViewShellDoc ����/�Ҹ�

CFormViewShellDoc::CFormViewShellDoc()
: m_IniFilePath(_T(""))
, m_TiltleString(_T(""))
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CFormViewShellDoc::~CFormViewShellDoc()
{
}

BOOL CFormViewShellDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CFormViewShellDoc serialization

void CFormViewShellDoc::Serialize(CArchive& ar)
{
	CString execArg;

	CFrameWnd * pWnd = (CFrameWnd*)AfxGetMainWnd();

	CFormViewShellView* pView = (CFormViewShellView*)pWnd->GetActiveView();

	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
		
		pView->GetDlgItemText(IDC_EDIT3, execArg);
		ar << execArg;
		pView->GetDlgItemText(IDC_EXEC_FILE, execArg);
		ar << execArg;
		pView->GetDlgItemText(IDC_SRC_FILE, execArg);
		ar << execArg;
		pView->GetDlgItemText(IDC_PATH, execArg);
		ar << execArg;
		pView->GetDlgItemText(IDC_EDIT4, execArg);
		ar << execArg;


		ar << pView->m_ExecFileName;
		ar << pView->m_PreCmdOptStr;
		int lbCnt = pView->m_ListBox.GetCount();
		ar << lbCnt;
		for(int lbIndex = 0; lbIndex < lbCnt; lbIndex++)
		{
			pView->m_ListBox.GetText(lbIndex, execArg);
			ar << execArg;
		}
		
		ar << pView->m_Check_EnableErrPop.GetCheck();
//ITEMIDLIST t = pView->m_InputPathiIdList;
//		ar << pView->m_InputPathiIdList;
//		ar << pView->m_OutputPathiIdList;

		pView->GetDlgItemText(IDC_EDIT6, execArg);
		ar << execArg;
		pView->GetDlgItemText(IDC_EDIT7, execArg);
		ar << execArg;
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
		pView->m_FirstLoaded = FALSE;
		ar >> execArg;
		pView->SetDlgItemText(IDC_EDIT3, execArg);
		ar >> execArg;
		pView->SetDlgItemText(IDC_EXEC_FILE, execArg);
		ar >> execArg;
		pView->SetDlgItemText(IDC_SRC_FILE, execArg);
		ar >> execArg;
		pView->SetDlgItemText(IDC_PATH, execArg);
		ar >> execArg;
		pView->SetDlgItemText(IDC_EDIT4, execArg);


		/*if(!execArg.IsEmpty())
			pView->m_ExtName.SetCheck(TRUE);
		else
			pView->m_ExtName.SetCheck(FALSE);
		*/
		ar >> pView->m_ExecFileName;
		ar >> pView->m_PreCmdOptStr;
		
		int lbCnt = 0;
		
		ar >> lbCnt;
		pView->m_ListBox.ResetContent();
		for(int lbIndex = 0; lbIndex < lbCnt; lbIndex++)
		{
			ar >> execArg;
			pView->m_ListBox.AddString(execArg);
			
		}
		
		pView->SetDlgItemText(IDC_EDIT5, pView->m_PreCmdOptStr);

		pView->DisplayCommand(FALSE);

		pView->m_FirstLoaded = TRUE;
		int nCheck = 0;
		ar >> nCheck;
		pView->m_Check_EnableErrPop.SetCheck(nCheck);
		ar >> execArg;
		pView->SetDlgItemText(IDC_EDIT6, execArg);
		ar >> execArg;
		pView->SetDlgItemText(IDC_EDIT7, execArg);
	}

	//�������� ����
	CFile myFile;
	CFileException e;
	if(!myFile.Open(m_IniFilePath, CFile::modeCreate | CFile::modeWrite, &e))
	{
		TRACE(_T("File could not be opened %s : %d\n"), m_IniFilePath, e.m_cause);
	}
	myFile.Write(ar.m_strFileName, ar.m_strFileName.GetLength());
	myFile.Close();
}


// CFormViewShellDoc ����

#ifdef _DEBUG
void CFormViewShellDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFormViewShellDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFormViewShellDoc ���
