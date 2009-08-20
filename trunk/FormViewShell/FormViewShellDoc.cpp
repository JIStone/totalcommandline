// FormViewShellDoc.cpp : CFormViewShellDoc 클래스의 구현
//

#include "stdafx.h"
#include "FormViewShell.h"

#include "FormViewShellDoc.h"
#include "FormViewShellView.h"
#include "MainFrm.h"
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
, m_bIsFirstLoad(TRUE)
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
	if(pWnd == NULL)
		return;

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

		// 멀티커맨드라인 강제 딜레이
		pView->GetDlgItemText(IDC_EDIT_DELAY_TIME, execArg);
		ar << execArg;

		int lbTclFilesCnt = pView->m_TclFilesListBox.GetCount();
		ar << lbTclFilesCnt;
		for(int lbIndex = 0; lbIndex < lbTclFilesCnt; lbIndex++)
		{
			pView->m_TclFilesListBox.GetText(lbIndex, execArg);
			ar << execArg;
			pView->m_SettingFilePath = ar.m_strFileName;
			// 다중 쉘API 호출시 창 확대
			((CMainFrame*)(pWnd->GetActiveFrame()))->SetWindowPos(NULL,0,0,1300,700,SWP_NOMOVE);
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
		
		pView->SetDlgItemText(IDC_EDIT6, "");
		if(!ar.IsBufferEmpty())
		{	
			ar >> execArg;
			pView->SetDlgItemText(IDC_EDIT6, execArg);
		}
		
		pView->SetDlgItemText(IDC_EDIT7, "");
		if(!ar.IsBufferEmpty())
		{	
			ar >> execArg;
			pView->SetDlgItemText(IDC_EDIT7, execArg);
		}

		pView->SetDlgItemText(IDC_EDIT8, "");
		if(!ar.IsBufferEmpty())
		{
			ar >> execArg;
			pView->SetDlgItemText(IDC_EDIT8, execArg);
		}
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

		// 멀티커맨드라인 강제 딜레이
		if(!ar.IsBufferEmpty())
		{	
			ar >> execArg;
			pView->SetDlgItemText(IDC_EDIT_DELAY_TIME, execArg);
		}
	
		isEmpty = ar.IsBufferEmpty();

		// 읽어올 설정이 있고, 멀티실행중이어서 로딩하는 것이 아니면
		if((!isEmpty && !pView->m_bMultiMode) || !(ar.m_strFileName.Right(5)).Compare(".mtcl"))
		{
			int lbTclFilesCnt = 0;
			pView->m_TclFilesListBox.ResetContent();
			ar >> lbTclFilesCnt;
			CREATESTRUCT cs;
			cs = ((CMainFrame*)(pWnd->GetActiveFrame()))->m_cs;
			
			if(lbTclFilesCnt)
			{
				// 다중 쉘API 호출시 창 확대
				//(pWnd)->SetWindowPos(NULL,0,0,1300,700,SWP_NOMOVE);
				if(m_bIsFirstLoad)
					(pWnd)->SetWindowPos(NULL,0,0,1269,689,SWP_NOMOVE);
				else
					(pWnd)->SetWindowPos(NULL,0,0,1280,700,SWP_NOMOVE);
				
				pView->m_BtnMultiTCLExcute.EnableWindow(TRUE);
				pView->m_BtnExcute.EnableWindow(FALSE);
			}
			else
			{
				if(m_bIsFirstLoad)
					(pWnd)->SetWindowPos(NULL,0,0,540,707,SWP_NOMOVE);
				else
					(pWnd)->SetWindowPos(NULL,0,0,540,700,SWP_NOMOVE);
				
				pView->m_BtnMultiTCLExcute.EnableWindow(FALSE);
				pView->m_BtnExcute.EnableWindow(TRUE);
			}

			for(int lbIndex = 0; lbIndex < lbTclFilesCnt; lbIndex++)
			{
				ar >> execArg;
				pView->m_TclFilesListBox.AddString(execArg);
			}
			pView->m_SettingFilePath = ar.m_strFileName;

		}
		else if(!pView->m_bMultiMode)
		{
			pView->m_TclFilesListBox.ResetContent();
			if(m_bIsFirstLoad)
				(pWnd)->SetWindowPos(NULL,0,0,540,707,SWP_NOMOVE);
			else
				(pWnd)->SetWindowPos(NULL,0,0,540,700,SWP_NOMOVE);
			
			pView->m_BtnMultiTCLExcute.EnableWindow(FALSE);
			pView->m_BtnExcute.EnableWindow(TRUE);
		}
	}
	
	m_bIsFirstLoad = FALSE;
	
		//설정파일 저장
	CFile myFile;
	CFileException e;
	//AfxMessageBox(m_IniFilePath);
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
