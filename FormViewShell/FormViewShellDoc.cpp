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
, m_tclFilePath(_T(""))
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
		int lbCnt = pView->m_ListBox_Out.GetCount();
		ar << lbCnt;
		for(int lbIndex = 0; lbIndex < lbCnt; lbIndex++)
		{
			pView->m_ListBox_Out.GetText(lbIndex, execArg);
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


		// 멀티커맨드라인 경로 저장
		int lbTclFilesCnt = pView->m_TclFilesListBox.GetCount();
		ar << lbTclFilesCnt;
		for(int lbIndex = 0; lbIndex < lbTclFilesCnt; lbIndex++)
		{
			//pView->m_SettingFilePath = ar.m_strFileName;
			CString tempFilePath = ar.m_strFileName;

			int i = tempFilePath.ReverseFind('\\');//실행 파일 이름을 지우기 위해서 왼쪽에 있는 '/'를 찾는다.
			CString currPath = tempFilePath.Right(tempFilePath.GetLength() - i);
			tempFilePath = m_tclFilePath + currPath;
			
			pView->m_TclFilesListBox.GetText(lbIndex, execArg);
			i = execArg.ReverseFind('\\');//실행 파일 이름을 지우기 위해서 왼쪽에 있는 '/'를 찾는다.
			execArg = execArg.Right(execArg.GetLength() - i);// 파일이름과 확장자만 얻는다.
			execArg = m_tclFilePath + execArg;

			ar << execArg;

			// 다중 쉘API 호출시 창 확대
			((CMainFrame*)(pWnd->GetActiveFrame()))->SetWindowPos(NULL,0,0,1300,700,SWP_NOMOVE);
		}
		// 멀티커맨드라인 체그박스저장
		for(int lbIndex = 0; lbIndex < lbTclFilesCnt; lbIndex++)
		{
			ar << pView->m_TclFilesListBox.GetCheck(lbIndex);;
		}
		// 멀티커맨드라인라면 생략
		if(lbTclFilesCnt){
			// mtcl 창닫기
			ar << pView->m_MTCL_ExitCheckBox.GetCheck();
		}else{
			// 하위폴더 검색 유무
			ar << pView->m_ChkSubFolder.GetCheck();
			// 결과리스트 보기 유무
			ar << pView->m_ViewList.GetCheck();
			// 빈폴더 제거 유무
			ar << pView->m_EmptyFolderCheck.GetCheck();
			// stcl 창닫기
			ar << pView->m_ExitCheckBox.GetCheck();
		}
		//===================== 저장할 데이터를 이하에 구현=====================
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.

		BOOL isEmpty = ar.IsBufferEmpty();
		BOOL isMultiCommand = !(ar.m_strFileName.Right(5)).Compare(".mtcl");

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
		pView->m_ListBox_Out.ResetContent();
		for(int lbIndex = 0; lbIndex < lbCnt; lbIndex++)
		{
			ar >> execArg;
			pView->m_ListBox_Out.AddString(execArg);
			
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
		int lbTclFilesCnt = 0;
		if(!ar.IsBufferEmpty())
			ar >> lbTclFilesCnt;
		// 읽어올 설정이 있고, 멀티실행중이어서 로딩하는 것이 아니면
		if((!isEmpty && !pView->m_bMultiMode) || isMultiCommand)
		{
			
			pView->m_TclFilesListBox.ResetContent();
			CREATESTRUCT cs;
			cs = ((CMainFrame*)(pWnd->GetActiveFrame()))->m_cs;
			
			if(lbTclFilesCnt)
			{
				// 다중 쉘API 호출시 창 확대
				//(pWnd)->SetWindowPos(NULL,0,0,1300,700,SWP_NOMOVE);

				((CMainFrame*)(pWnd->GetActiveFrame()))->m_cs.cx = 1280;
				((CMainFrame*)(pWnd->GetActiveFrame()))->m_cs.cy = 700;

				if(m_bIsFirstLoad)
				{
					(pWnd)->SetWindowPos(NULL,0,0,1269,689,SWP_NOMOVE);
				}
				else
				{
					(pWnd)->SetWindowPos(NULL,0,0,1280,700,SWP_NOMOVE);
				}

				pView->m_BtnMultiTCLExcute.EnableWindow(TRUE);
				pView->m_BtnExcute.EnableWindow(FALSE);
				
				// 단일 설정 초기화 표시
				CString clrString = "";
				pView->SetDlgItemText(IDC_EDIT3, clrString);
				((CEdit*)pView->GetDlgItem(IDC_EDIT3))->SetReadOnly(TRUE);

				pView->SetDlgItemText(IDC_EXEC_FILE, clrString);
				((CEdit*)pView->GetDlgItem(IDC_EXEC_FILE))->SetReadOnly(TRUE);

				pView->SetDlgItemText(IDC_SRC_FILE, clrString);
				((CEdit*)pView->GetDlgItem(IDC_SRC_FILE))->SetReadOnly(TRUE);

				pView->SetDlgItemText(IDC_PATH, clrString);
				((CEdit*)pView->GetDlgItem(IDC_PATH))->SetReadOnly(TRUE);

				pView->SetDlgItemText(IDC_EDIT4, clrString);
				((CEdit*)pView->GetDlgItem(IDC_EDIT4))->SetReadOnly(TRUE);

				pView->SetDlgItemText(IDC_EDIT6, clrString);
				((CEdit*)pView->GetDlgItem(IDC_EDIT6))->SetReadOnly(TRUE);

				pView->SetDlgItemText(IDC_EDIT7, clrString);
				((CEdit*)pView->GetDlgItem(IDC_EDIT7))->SetReadOnly(TRUE);

				pView->SetDlgItemText(IDC_EDIT8, clrString);
				((CEdit*)pView->GetDlgItem(IDC_EDIT8))->SetReadOnly(TRUE);

				pView->SetDlgItemText(IDC_EDIT_DELAY_TIME, clrString);
				((CEdit*)pView->GetDlgItem(IDC_EDIT_DELAY_TIME))->SetReadOnly(TRUE);
				
				pView->SetDlgItemText(IDC_EDIT5, clrString);
				((CEdit*)pView->GetDlgItem(IDC_EDIT5))->SetReadOnly(TRUE);

				pView->SetDlgItemText(IDC_EDIT5, clrString);
				((CEdit*)pView->GetDlgItem(IDC_EDIT5))->SetReadOnly(TRUE);

				pView->SetDlgItemText(IDC_LIST_OUT, clrString);
				(pView->GetDlgItem(IDC_BTN_OUT_ADD))->EnableWindow(FALSE);
				(pView->GetDlgItem(IDC_BTN_OUT_DEL))->EnableWindow(FALSE);
				(pView->GetDlgItem(IDC_SELFILE))->EnableWindow(FALSE);
				(pView->GetDlgItem(IDC_SELFILE2))->EnableWindow(FALSE);
				(pView->GetDlgItem(IDC_SELPATH))->EnableWindow(FALSE);
				(pView->GetDlgItem(IDC_BUTTON_ADD_FOLDER))->EnableWindow(FALSE);
				(pView->GetDlgItem(IDC_BUTTON_DEL_FOLDER))->EnableWindow(FALSE);

				pView->m_ListBox_Out.ResetContent();
				pView->m_ExFolderListBox.ResetContent();
				pView->m_Check_EnableErrPop.SetCheck(0);
				pView->m_Check_EnableErrPop.EnableWindow(FALSE);
				pView->m_ChkSubFolder.SetCheck(0);
				pView->m_ChkSubFolder.EnableWindow(FALSE);
				pView->m_ViewList.SetCheck(0);
				pView->m_ViewList.EnableWindow(FALSE);
				pView->m_EmptyFolderCheck.SetCheck(0);
				pView->m_EmptyFolderCheck.EnableWindow(FALSE);
				pView->m_ExitCheckBox.EnableWindow(FALSE);
			}
			else
			{
				((CMainFrame*)(pWnd->GetActiveFrame()))->m_cs.cx = 540;
				((CMainFrame*)(pWnd->GetActiveFrame()))->m_cs.cy = 700;

				if(m_bIsFirstLoad)
					(pWnd)->SetWindowPos(NULL,0,0,540,707,SWP_NOMOVE);
				else
					(pWnd)->SetWindowPos(NULL,0,0,540,700,SWP_NOMOVE);
				
				pView->m_BtnMultiTCLExcute.EnableWindow(FALSE);
				pView->m_BtnExcute.EnableWindow(TRUE);


				// 단일 설정 초기화 표시
				BOOL _enable = FALSE;
				((CEdit*)pView->GetDlgItem(IDC_EDIT3))->SetReadOnly(_enable);
				((CEdit*)pView->GetDlgItem(IDC_EXEC_FILE))->SetReadOnly(_enable);
				((CEdit*)pView->GetDlgItem(IDC_SRC_FILE))->SetReadOnly(_enable);
				((CEdit*)pView->GetDlgItem(IDC_PATH))->SetReadOnly(_enable);
				((CEdit*)pView->GetDlgItem(IDC_EDIT4))->SetReadOnly(_enable);
				((CEdit*)pView->GetDlgItem(IDC_EDIT6))->SetReadOnly(_enable);
				((CEdit*)pView->GetDlgItem(IDC_EDIT7))->SetReadOnly(_enable);
				((CEdit*)pView->GetDlgItem(IDC_EDIT8))->SetReadOnly(_enable);
				((CEdit*)pView->GetDlgItem(IDC_EDIT_DELAY_TIME))->SetReadOnly(_enable);
				((CEdit*)pView->GetDlgItem(IDC_EDIT5))->SetReadOnly(_enable);
				((CEdit*)pView->GetDlgItem(IDC_EDIT5))->SetReadOnly(_enable);
				(pView->GetDlgItem(IDC_BTN_OUT_ADD))->EnableWindow(!_enable);
				(pView->GetDlgItem(IDC_BTN_OUT_DEL))->EnableWindow(!_enable);
				(pView->GetDlgItem(IDC_SELFILE))->EnableWindow(!_enable);
				(pView->GetDlgItem(IDC_SELFILE2))->EnableWindow(!_enable);
				(pView->GetDlgItem(IDC_SELPATH))->EnableWindow(!_enable);
				(pView->GetDlgItem(IDC_BUTTON_ADD_FOLDER))->EnableWindow(!_enable);
				(pView->GetDlgItem(IDC_BUTTON_DEL_FOLDER))->EnableWindow(!_enable);
				pView->m_Check_EnableErrPop.EnableWindow(!_enable);
				pView->m_ChkSubFolder.EnableWindow(!_enable);
				pView->m_ViewList.EnableWindow(!_enable);
				pView->m_EmptyFolderCheck.EnableWindow(!_enable);
				pView->m_ExitCheckBox.EnableWindow(!_enable);
			}

			for(int lbIndex = 0; lbIndex < lbTclFilesCnt; lbIndex++)
			{
				ar >> execArg;
				pView->m_TclFilesListBox.AddString(execArg);
			}

			for(int lbIndex = 0; lbIndex < lbTclFilesCnt && !ar.IsBufferEmpty(); lbIndex++)
			{
				int nCheck = 0;
				ar >> nCheck;
				pView->m_TclFilesListBox.SetCheck(lbIndex, nCheck);
			}
			// 다중실행 창닫기
			if(lbTclFilesCnt && !ar.IsBufferEmpty())
			{	
				ar >> nCheck;
				pView->m_MTCL_ExitCheckBox.SetCheck(nCheck);	
			}
			else if(!pView->m_MTCL_ExitCheckBox.GetCheck())
			{
				pView->m_MTCL_ExitCheckBox.SetCheck(FALSE);	
			}
		}
		else if(!pView->m_bMultiMode)
		{
			pView->m_TclFilesListBox.ResetContent();

			((CMainFrame*)(pWnd->GetActiveFrame()))->m_cs.cx = 540;
			((CMainFrame*)(pWnd->GetActiveFrame()))->m_cs.cy = 700;
			
			if(m_bIsFirstLoad)
				(pWnd)->SetWindowPos(NULL,0,0,540,707,SWP_NOMOVE);
			else
				(pWnd)->SetWindowPos(NULL,0,0,540,700,SWP_NOMOVE);
			
			pView->m_BtnMultiTCLExcute.EnableWindow(FALSE);
			pView->m_BtnExcute.EnableWindow(TRUE);
		}
		pView->m_SettingFilePath = ar.m_strFileName;
		//CString currPath = Path;
		int i = pView->m_SettingFilePath.ReverseFind('\\');//실행 파일 이름을 지우기 위해서 왼쪽에 있는 '/'를 찾는다.
		CString currPath = pView->m_SettingFilePath.Right(pView->m_SettingFilePath.GetLength() - i);
		pView->m_SettingFilePath = m_tclFilePath + currPath;

		// 하위폴더 찾기유무
		if(!ar.IsBufferEmpty())
		{	
			ar >> nCheck;
			pView->m_ChkSubFolder.SetCheck(nCheck);	
		}
		else
		{
			pView->m_ChkSubFolder.SetCheck(TRUE);	
		}

		// 결과리스트 보기 유무
		if(!ar.IsBufferEmpty())
		{	
			ar >> nCheck;
			pView->m_ViewList.SetCheck(nCheck);	
		}
		else
		{
			pView->m_ViewList.SetCheck(FALSE);	
		}

		// 빈폴더 제거 유무
		if(!ar.IsBufferEmpty())
		{	
			ar >> nCheck;
			pView->m_EmptyFolderCheck.SetCheck(nCheck);	
		}
		else
		{
			pView->m_EmptyFolderCheck.SetCheck(FALSE);	
		}

		// 창닫기
		if(!ar.IsBufferEmpty())
		{	
			ar >> nCheck;
			pView->m_ExitCheckBox.SetCheck(nCheck);	
		}
		else
		{
			pView->m_ExitCheckBox.SetCheck(FALSE);	
		}

		//===================== 불러들일데이터를 이하에 구현=====================
	}
	
	m_bIsFirstLoad = FALSE;
	
	//========================!!!!설정파일 저장!!!===============================
	//	연속실행시에도 로드하므로 로드했던 파일을 로드하기위해 연속실행중이라면
	//	다음에 실행하면 자동으로 로드될 파일을 저장하지않는다
	//===========================================================================

	if(!pView->m_bMultiMode)
	{
		BOOL isMultiMode = !(ar.m_strFileName.Right(5)).Compare(".mtcl");
		BOOL isSingleLoaded = !(pView->m_SettingFilePath.Right(4)).Compare(".tcl");
		CString nextLoad = pView->m_SettingFilePath;
		// 연속모드인데 단일모드 파일경로를 저장하게 된다면
		if(isMultiMode && isSingleLoaded)
			nextLoad = ar.m_strFileName;
		CFile myFile;
		CFileException e;
		//AfxMessageBox(m_IniFilePath);
		if(!myFile.Open(m_IniFilePath, CFile::modeCreate | CFile::modeWrite, &e))
		{
			TRACE(_T("File could not be opened %s : %d\n"), m_IniFilePath, e.m_cause);
		}
		myFile.Write(nextLoad, nextLoad.GetLength());
		// 다음에 실행하면 최후에 로드했던 파일을 로드
		
		myFile.Close();
	}
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
