// FormViewShellDoc.cpp : CFormViewShellDoc 클래스의 구현
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


// CFormViewShellDoc 생성/소멸

CFormViewShellDoc::CFormViewShellDoc()
: m_IniFilePath(_T(""))
, m_TiltleString(_T(""))
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CFormViewShellDoc::~CFormViewShellDoc()
{
}

BOOL CFormViewShellDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

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
		// TODO: 여기에 저장 코드를 추가합니다.
		
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
		pView->GetDlgItemText(IDC_EDIT8, execArg);
		ar << execArg;
		
		int lbExFolderCnt = pView->m_ExFolderListBox.GetCount();
		ar << lbExFolderCnt;
		for(int lbIndex = 0; lbIndex < lbExFolderCnt; lbIndex++)
		{
			pView->m_ExFolderListBox.GetText(lbIndex, execArg);
			ar << execArg;
		}

	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.

		BOOL isEmpty = ar.IsBufferEmpty();

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

		ar >> execArg;
		pView->SetDlgItemText(IDC_EDIT8, execArg);

		// 새로운 설정요소가 추가되어 일어들일때 과거 버전인지를 검사해준다
		isEmpty = ar.IsBufferEmpty();
		
		if(isEmpty)
		{
			// 제외 폴더리스트 로드
			pView->m_ExFolderListBox.ResetContent();
			// 이전버전은 기본적으로 svn폴더를 제외해준 것 대응
			pView->m_ExFolderListBox.AddString(".svn");
		}
		else
		{
			// 제외 폴더리스트 로드
			int lbExFolderCnt = 0;
			pView->m_ExFolderListBox.ResetContent();
			ar >> lbExFolderCnt;
			for(int lbIndex = 0; lbIndex < lbExFolderCnt; lbIndex++)
			{
				ar >> execArg;
				pView->m_ExFolderListBox.AddString(execArg);
			}
		}

	}

	//설정파일 저장
	CFile myFile;
	CFileException e;
	if(!myFile.Open(m_IniFilePath, CFile::modeCreate | CFile::modeWrite, &e))
	{
		TRACE(_T("File could not be opened %s : %d\n"), m_IniFilePath, e.m_cause);
	}
	myFile.Write(ar.m_strFileName, ar.m_strFileName.GetLength());
	myFile.Close();
}


// CFormViewShellDoc 진단

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


// CFormViewShellDoc 명령
