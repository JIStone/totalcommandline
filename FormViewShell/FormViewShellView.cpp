// FormViewShellView.cpp : CFormViewShellView 클래스의 구현
//

#include "stdafx.h"
#include "FormViewShell.h"

#include "FormViewShellDoc.h"
#include "FormViewShellView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define NUM_OF_CVTR_FILE_MAX (500)
#define NUM_OF_CVTR_FOLDER_MAX (2000)

// 확장자의 최대길이는 6으로 가정				
#define FILE_EXT_LENGTH_MAX (6)

// CFormViewShellView

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

IMPLEMENT_DYNCREATE(CFormViewShellView, CFormView)

BEGIN_MESSAGE_MAP(CFormViewShellView, CFormView)
	ON_BN_CLICKED(IDC_SELFILE2, &CFormViewShellView::OnBnClickedSelfile2)
//	ON_BN_CLICKED(IDC_SELFILE, &CFormViewShellView::OnBnClickedSelfile)
	ON_BN_CLICKED(IDC_SELPATH, &CFormViewShellView::OnBnClickedSelpath)
//	ON_EN_CHANGE(IDC_EDIT4, &CFormViewShellView::OnEnChangeEdit4)
ON_BN_CLICKED(IDC_CHECK1, &CFormViewShellView::OnBnClickedCheck1)
//ON_BN_CLICKED(IDC_CHECK2, &CFormViewShellView::OnBnClickedCheck2)
ON_BN_CLICKED(IDC_CHECK3, &CFormViewShellView::OnBnClickedCheck3)
ON_BN_CLICKED(IDC_EXECAPPL, &CFormViewShellView::OnBnClickedExecappl)
//ON_BN_CLICKED(IDOK, &CFormViewShellView::OnBnClickedOk)
ON_EN_CHANGE(IDC_PATH, &CFormViewShellView::OnEnChangePath)
ON_EN_CHANGE(IDC_SRC_FILE, &CFormViewShellView::OnEnChangeSrcFile)
ON_EN_CHANGE(IDC_EXEC_FILE, &CFormViewShellView::OnEnChangeExecFile)
ON_EN_CHANGE(IDC_EDIT3, &CFormViewShellView::OnEnChangeEdit3)
ON_EN_CHANGE(IDC_EDIT4, &CFormViewShellView::OnEnChangeEdit4)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CFormViewShellView::OnNMCustomdrawProgress1)

ON_EN_CHANGE(IDC_EDIT5, &CFormViewShellView::OnEnChangeEdit5)

ON_BN_CLICKED(IDC_BTN_OUT_ADD, &CFormViewShellView::OnBnClickedBtnOutAdd)
ON_BN_CLICKED(IDC_BTN_OUT_DEL, &CFormViewShellView::OnBnClickedBtnOutDel)

ON_BN_CLICKED(IDC_SELFILE, &CFormViewShellView::OnBnClickedSelfile)

ON_BN_CLICKED(IDC_RADIO1_MOVE, &CFormViewShellView::OnBnClickedRadio1Move)
ON_BN_CLICKED(IDC_RADIO2_COPY, &CFormViewShellView::OnBnClickedRadio2Copy)
ON_BN_CLICKED(IDC_RADIO3_DELETE, &CFormViewShellView::OnBnClickedRadio3Delete)
END_MESSAGE_MAP()

// CFormViewShellView 생성/소멸

CFormViewShellView::CFormViewShellView()
	: CFormView(CFormViewShellView::IDD)
	, m_ExecFilePath(_T(""))
	, m_FullFileName(_T(""))
	, m_DestPath(_T(""))
	, m_SubDirCnt(0)
	, m_SearchedFileCnt(0)
	,m_PreCmdOptStr(_T(""))
	, m_AllCmdLnText(_T(""))
	, m_FirstLoaded(FALSE)

	, m_ListData(_T(""))

	, m_IsToolTipInit(false)
	, m_AddExtTypeRadio(0)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CFormViewShellView::~CFormViewShellView()
{
}

void CFormViewShellView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_Check_EnableErrPop);
	//DDX_Control(pDX, IDC_CHECK2, m_Check_Bmp);
	//DDX_Control(pDX, IDC_CHECK3, m_ExtName);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtrl);
	DDX_Control(pDX, IDC_EDIT2, m_EditProgCnt);
	//	DDX_Control(pDX, IDC_EDIT5, m_Eidt_PreCmdOpt);
	//DDX_Control(pDX, IDC_EDIT_OUT, m_ListData);
	DDX_Control(pDX, IDC_LIST_OUT, m_ListBox);
	DDX_Text(pDX, IDC_EDIT_OUT, m_ListData);
	//	DDX_Control(pDX, IDC_RADIO1_MOVE, m_AddExtTypeRadio);
	DDX_Radio(pDX, IDC_RADIO1_MOVE, m_AddExtTypeRadio);
	DDX_Control(pDX, IDC_EDIT_SUCCESS_FAIL, m_Edit_Success_Fail);
}

BOOL CFormViewShellView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	


	return CFormView::PreCreateWindow(cs);
}

void CFormViewShellView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	
	if(!m_IsToolTipInit)
	{
		// 툴팁 설정
		m_toolTip.Create(this);
		m_toolTip.Activate(TRUE);
		m_toolTip.AddTool(GetDlgItem(IDC_EDIT4), "ex) *.txt, mainBG.nsc");
		m_toolTip.AddTool(GetDlgItem(IDC_EDIT_OUT), "ex) .NCGR,.NCER ... "); 
		m_toolTip.SetMaxTipWidth(500);
		m_IsToolTipInit = true;
	}

}


// CFormViewShellView 진단

#ifdef _DEBUG
void CFormViewShellView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormViewShellView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFormViewShellDoc* CFormViewShellView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFormViewShellDoc)));
	return (CFormViewShellDoc*)m_pDocument;
}
#endif //_DEBUG


// CFormViewShellView 메시지 처리기

void CFormViewShellView::OnBnClickedSelfile2()
{
	// TODO: Add your control notification handler code here

    CFileDialog dlg(TRUE);

	if(dlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_EXEC_FILE, dlg.GetPathName());
		m_ExecFileName = dlg.GetFileName();
		m_ExecFilePath = dlg.GetPathName();
		
	}
	DisplayCommand(TRUE);
}

//void CFormViewShellView::OnBnClickedSelfile()
//{
//	// TODO: Add your control notification handler code here
//
//	LPITEMIDLIST pidlBrowse;
//
//	BROWSEINFO BRinfo;
//	BRinfo.hwndOwner = GetSafeHwnd();
//	BRinfo.pidlRoot = NULL;
//	BRinfo.pszDisplayName = m_FullFileName.GetBuffer(MAX_PATH);
//	BRinfo.lpszTitle = "폴더를 선택하세요.";
//	BRinfo.ulFlags = BIF_RETURNONLYFSDIRS;
//	BRinfo.lpfn = NULL;
//	BRinfo.lParam = 0;
//
//	pidlBrowse = SHBrowseForFolder(&BRinfo);
//	if(pidlBrowse != NULL) 
//	{
//		SHGetPathFromIDList(pidlBrowse, m_FullFileName.GetBuffer(MAX_PATH));
//		SetDlgItemText(IDC_SRC_FILE, m_FullFileName);
//		char *temp = m_FullFileName.GetBuffer(MAX_PATH);
//		m_FullFileName = temp;
//	
//	}
//	DisplayCommand(TRUE);
//}

void CFormViewShellView::OnBnClickedSelpath()
{
	// TODO: Add your control notification handler code here

	LPITEMIDLIST pidlBrowse;

	BROWSEINFO BRinfo;
	BRinfo.hwndOwner = GetSafeHwnd();
	BRinfo.pidlRoot = NULL;
	BRinfo.pszDisplayName = m_DestPath.GetBuffer(MAX_PATH);
	BRinfo.lpszTitle = "폴더를 선택하세요.";
	BRinfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_STATUSTEXT | BIF_UAHINT;
	// m_DestPath에 설정한 경로를 찾아감
	BRinfo.lpfn = BrowseCallbackProc;
	BRinfo.lParam = (LPARAM)BRinfo.pszDisplayName;

	pidlBrowse = SHBrowseForFolder(&BRinfo);
	//memcpy(&m_OutputPathiIdList, pidlBrowse, sizeof(ITEMIDLIST));
	if(pidlBrowse != NULL) 
	{
		SHGetPathFromIDList(pidlBrowse, m_DestPath.GetBuffer(MAX_PATH));
		SetDlgItemText(IDC_PATH, m_DestPath);
		char *temp = m_DestPath.GetBuffer(MAX_PATH);
		m_DestPath = temp;
	}
	DisplayCommand(TRUE);
}

//void CFormViewShellView::OnEnChangeEdit4()
//{
//	// TODO:  If this is a RICHEDIT control, the control will not
//	// send this notification unless you override the CFormView::OnInitDialog()
//	// function and call CRichEditCtrl().SetEventMask()
//	// with the ENM_CHANGE flag ORed into the mask.
//
//	// TODO:  Add your control notification handler code here
//}

void CFormViewShellView::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	CString opStr;
	GetDlgItemText(IDC_EDIT3, opStr);

/*	switch(m_Check_EnableErrPop.GetCheck())
	{
	case 0:
		opStr.Replace(" -bg", "");
		SetDlgItemText(IDC_EDIT3, opStr);
		break;
	case 1:
		opStr +=  " -bg";
		SetDlgItemText(IDC_EDIT3, opStr);
		break;
	}
*/
}

void CFormViewShellView::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler code here
	/*
	switch(m_ExtName.GetCheck())
	{
	case 0:
		SetDlgItemText(IDC_EDIT4, "");
		break;
	case 1:
		SetDlgItemText(IDC_EDIT4, "*.ncg");
		break;
	}
	*/
}

#include <shlwapi.h>

void CFormViewShellView::OnBnClickedExecappl()
{
	// TODO: Add your control notification handler code here
	//GetDlgItemText(IDC_SRC_FILE, m_FullFileName);
	GetDlgItemText(IDC_SRC_FILE, m_FullFileName);
	GetDlgItemText(IDC_EXEC_FILE, m_ExecFilePath);
	GetDlgItemText(IDC_PATH, m_DestPath);

	if(m_FullFileName.IsEmpty()) 
	{ 
		AfxMessageBox("변환할 파일을 선택하세요.");

		OnBnClickedSelfile();
		if(m_FullFileName.IsEmpty())
		{
			return;
		}
	}
	else if(m_ExecFilePath.IsEmpty()) 
	{ 
		AfxMessageBox("실행파일을 선택하세요.");

		OnBnClickedSelfile();
		if(m_ExecFilePath.IsEmpty())
		{
			return;
		}
	}
	
	CString execArg;
	GetDlgItemText(IDC_EDIT3, execArg);

	CString midPath;
	GetDlgItemText(IDC_EDIT4, midPath);

	GetDlgItemText(IDC_PATH, m_DestPath);

	CString searchRootDirStr = m_FullFileName;
	CString searchSubDirList[NUM_OF_CVTR_FOLDER_MAX];
	int iSuccessCnt = 0;
	// 하위 폴더 갯수 초기화
	m_SubDirCnt = 0;
	// 검색된 갯수 초기화
	m_SearchedFileCnt = 0;
	SearchDir(m_FullFileName, searchSubDirList);

	CString searchFileStr = midPath + "\0";

	CString searchedFileList[NUM_OF_CVTR_FILE_MAX];
	
	m_ProgressCtrl.SetRange(0, 100);
	m_ProgressCtrl.SetPos(0);

	int progressCnt = 0;
	for(int dirCnt = 0;dirCnt < m_SubDirCnt; dirCnt++) 
	{
		progressCnt += SearchFile(searchRootDirStr + searchSubDirList[dirCnt], searchFileStr, NULL);

	}
	m_EditProgCnt.ShowWindow(SW_SHOW);
	
	for(int dirCnt = 0;dirCnt < m_SubDirCnt; dirCnt++) 
	{
		int starIndex = m_SearchedFileCnt;
		//대상파일 검색
		SearchFile(searchRootDirStr + searchSubDirList[dirCnt], searchFileStr, searchedFileList);
	
		for(int listCnt = starIndex; listCnt < m_SearchedFileCnt; listCnt++)
		{
			m_ProgressCtrl.SetPos(((listCnt + 1) * 100) / progressCnt);

			CString prgCnt;
			prgCnt.Format( "[ %d / %d ]", listCnt+1, progressCnt );
			
			if(listCnt+1 == progressCnt)
				prgCnt += " Complet!!!";

			SetDlgItemText(IDC_EDIT2, prgCnt);
			GetDlgItem(IDC_EDIT2)->UpdateWindow();
			//UpdateWindow();
			

			CString testAllPath = searchedFileList[listCnt] + m_PreCmdOptStr + m_DestPath + searchSubDirList[dirCnt] + execArg;
			// 출력폴더 옵션이 없으면
			if(m_DestPath.IsEmpty() || m_PreCmdOptStr.IsEmpty())
			{
				testAllPath = searchedFileList[listCnt] + execArg;
			}

			//========g2dcvtr은 한글폴더를 출력폴더를 정할수 없나?==========
			HINSTANCE ret = ShellExecute(NULL, "open", m_ExecFilePath, testAllPath, NULL, SW_HIDE);
			
			
			for(int lbIndex = 0; lbIndex < m_ListBox.GetCount(); lbIndex++)
			{
				int sIndex = 0;
				CString oExtStr;
				//실행결과로 생성된 파일패스
				CString outPutResultPath = "";
				CString iExtStr = searchFileStr;
			
				// 파일처리 타입 설정
				int iExtType = 0;
				iExtStr.Replace("*", "");
				
				m_ListBox.GetText(lbIndex, oExtStr);
				CString tExtTypeStr = oExtStr.Left(3);
				
				if(!(oExtStr.Left(3)).Compare("[M]"))
				{
					iExtType = FO_MOVE;
				}
				else if(!(oExtStr.Left(3)).Compare("[C]"))
				{
					iExtType = FO_COPY;
				}
				else if(!(oExtStr.Left(3)).Compare("[D]"))
				{
					iExtType = FO_DELETE;
				}
				
				//확장자에서 파일처리를 위한 스트링만 제거
				sIndex = oExtStr.Find('.', 0);
				oExtStr = oExtStr.Right(oExtStr.GetLength() - sIndex);


				//실행결과로 생성된 파일패스 만들기
				//sIndex = testAllPath.Find('.',testAllPath.GetLength() - FILE_EXT_LENGTH_MAX);
				sIndex = testAllPath.Find('.',0);
				outPutResultPath = testAllPath.Left(sIndex) + oExtStr;

				CString tempOutPutStr = searchedFileList[listCnt];
				// 파일 이름만 분리하기
				//sIndex = tempOutPutStr.Find('.',tempOutPutStr.GetLength() - FILE_EXT_LENGTH_MAX);
				sIndex = tempOutPutStr.Find('.',0);
				CString sExte = tempOutPutStr.Right(tempOutPutStr.GetLength() - sIndex);
				// 출력파일이름에 확장자 붙이기
				tempOutPutStr.Replace(sExte, oExtStr);

				//tempOutPutStr.Replace(iExtStr, oExtStr);
				tempOutPutStr.Replace(searchRootDirStr, "");
				//CString movSrc = searchRootDirStr + tempOutPutStr;
				tempOutPutStr = m_DestPath + tempOutPutStr;
				// 이름 강제 바꾸고 이동하기////////
				CString movSrc = searchRootDirStr + searchSubDirList[dirCnt] + "\\*" + oExtStr;//SE_ERR_ACCESSDENIED
				CString movDest = tempOutPutStr;
				//생성된 파일 찾기위한 파일 패스
				CString movingStr = "";
				int findCnt = 0;
				
				BOOL bAtiveFileProc = TRUE;
				// 컨버팅된 파일이 원본 파일이 있는 폴더에 아직 없을 경우 계속 찾음
				while(movingStr.IsEmpty() )
				{	
					if(iExtType == FO_COPY)
					{
						SearchOneFile(NULL, outPutResultPath, &movingStr);
					}
					else if(iExtType == FO_DELETE)
					{
						// 삭제할 파일의 경로
						movingStr = tempOutPutStr;
						bAtiveFileProc = PathFileExists(tempOutPutStr);
						break;
					}
					else
					{
						SearchOneFile(searchRootDirStr + searchSubDirList[dirCnt], oExtStr, &movingStr);
					}
					Sleep(10);

					findCnt++;
					// 릴리즈모드 대응(디버그라면 더늘려야함)
					if(findCnt > 10)
						break;
				}	
				// CMD 명령
/*				CString testSTRING ="/c move " + movSrc + " " + movDest;
				HINSTANCE movRet = ShellExecute(NULL, "open", "cmd", testSTRING, NULL, SW_HIDE);
*/

//========================================= 결과 파일처리====================================================
				if(bAtiveFileProc)
				{

					SHFILEOPSTRUCT shos;
					ZeroMemory(&shos, sizeof(SHFILEOPSTRUCT));
					// 파일처리 타입 결정
					shos.wFunc = iExtType;
					movingStr.Append("\0", 1);
					movDest.Append("\0", 1);
					shos.pFrom = NULL;
					shos.pTo = NULL;
					// 이런식으로 넣어줘야함!!!
					TCHAR        pszFrom[1024] = {0}; 
					strcpy_s(pszFrom , movingStr);
					shos.pFrom = pszFrom;//"D:\\WORK\\Macheon_Trial\\MC\\Program\\NC\\MainUi\\2d\\MainUI_bottom.NCER";

					TCHAR        pszTo[1024] = {0}; 
					strcpy_s(pszTo , movDest);
				
					if(iExtType != FO_DELETE)
						shos.pTo = pszTo;

					// 작업과정 정보를 표시안함
					shos.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI;

					int shRet = 0;
					int tryCount = 0;
					// 생성결과 파일이 큰 이유등으로 SHFileOperation()가 제대로 마치지 못했으면 될때까지한다.
					do
					{
						shRet = SHFileOperation(&shos);
						Sleep(10);
						tryCount++;
					}
					while(tryCount < 10 && shRet != 0);
					

					//처리해야할 파일이 없고 에러메세지를 표시한 상태이면
					if(shRet && m_Check_EnableErrPop.GetCheck())
					{
						int popRet = MessageBox(movDest + " \n작업을 마치지 못했습니다. 계속 하시겠습니까?","파일이동 에러", MB_OKCANCEL);
						
						if(popRet == 1)
						{
							continue;
						}
						else if(popRet == 2)
						{
							m_ProgressCtrl.SetPos(0);
							return;
						}
					}

				}
//==============================================================================================================================

				BOOL isDestFileExist = FALSE;
				if(iExtType == FO_DELETE)
				{
					isDestFileExist = PathFileExists(movingStr);// 삭제할 파일이 경로에 있는지 검사
				}

				int findMovedFileCnt = 0;
				while((iExtType == FO_DELETE && isDestFileExist) || (iExtType != FO_DELETE && !isDestFileExist))
				{
					isDestFileExist = PathFileExists(tempOutPutStr);

					Sleep(10);
					findMovedFileCnt++;
					if(findMovedFileCnt > 10)
						break;
				}
				// 에러메세지 표시여부
				if(m_Check_EnableErrPop.GetCheck() && ((iExtType == FO_DELETE && isDestFileExist) || (iExtType != FO_DELETE && !isDestFileExist)))
				{
					//int popRet = MessageBox(searchedFileList[listCnt] + " \n에대한 작업을 마치지 못했습니다. \n계속 하시겠습니까?","일지 정지", MB_OKCANCEL);
					int popRet = MessageBox(tempOutPutStr + " \n에대한 작업을 마치지 못했습니다. \n계속 하시겠습니까?","일지 정지", MB_OKCANCEL);
					if(popRet == 1)
					{
						continue;
					}
					else if(popRet == 2)
					{
						m_ProgressCtrl.SetPos(0);
						return;
					}
				}
				else if(!((iExtType == FO_DELETE && isDestFileExist) || (iExtType != FO_DELETE && !isDestFileExist)))
				{
					iSuccessCnt +=  (lbIndex + 1) / m_ListBox.GetCount();//  ++;
				}

			}

			switch((int)ret)
			{
				case SE_ERR_NOASSOC:
				{
					MessageBox("지정된 파일 확장자로 실행할 수 있는 프로그램이 없습니다.",
					"파일 확장자 오류", MB_OKCANCEL);
				}
				break;
				case SE_ERR_FNF:
				{
					MessageBox("지정된 파일을 찾을수 없습니다.",
					"파일 오류", MB_OKCANCEL);
				}
				break;
			}
			
		}

	}
	CString resultCnt;
	//resultCnt.Format( "성공: %d", iSuccessCnt / m_ListBox.GetCount());
	resultCnt.Format( "성공: %d", iSuccessCnt);
	m_Edit_Success_Fail.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_EDIT_SUCCESS_FAIL, resultCnt);
	GetDlgItem(IDC_EDIT_SUCCESS_FAIL)->UpdateWindow();
	//m_ProgressCtrl.SetPos(100);
}
int CFormViewShellView::SearchDir(CString sDirName, CString *sDirNameList)
{
	CFileFind dirFinder;
	BOOL bWorking = dirFinder.FindFile(sDirName + "\\*.*");
	int dCnt = 0;
	// 최상위 폴더 추가
	if(!m_SubDirCnt)
	{
		//CString tempfolderPath = sDirName;
		//tempfolderPath.Replace(m_FullFileName, "");
		sDirNameList[m_SubDirCnt++];// = tempfolderPath;
		CreateDirectory(sDirName, NULL);
	}

	while (bWorking)
	{
		bWorking = dirFinder.FindNextFile();
	
		if (dirFinder.IsDots())
			continue;

		if (dirFinder.IsDirectory())
		{
			TRACE(_T("%s\n"), (LPCTSTR)dirFinder.GetFileName());
			TRACE(_T("%s\n"), (LPCTSTR)dirFinder.GetFilePath());
			// 서브폴더 절대경로
			CString tempfolderPath = dirFinder.GetFilePath();
			
			if(tempfolderPath.Find('.svn',0))
				continue;

			tempfolderPath.Replace(m_FullFileName, "");
			sDirNameList[m_SubDirCnt++] = tempfolderPath;
			// 서브폴더명만 저장
			
			
			// 타겟 파일이 있는것만 폴더를 생성해줌
			// 타겟 파일이 없는데 그 하위폴더에 있는 경우도 폴더를 생성하지 않으므로 주석처리
/*			CString exetStr;
			GetDlgItemText(IDC_EDIT4, exetStr);
			if(SearchFile(sDirName + sDirNameList[m_SubDirCnt-1], exetStr, NULL))
*/			{

				CString path = m_DestPath + sDirNameList[m_SubDirCnt-1];
				
				CreateDirectory(path, NULL);
			}
		

			SearchDir(dirFinder.GetFilePath(), sDirNameList);
		}
	}
	return dCnt;
}

int CFormViewShellView::SearchFile(CString sDirName, CString sFileName, CString *sFileNameList)
{

	CFileFind finder;
	CString findFileStr = sDirName + "\\" + sFileName;
	BOOL bWorking = finder.FindFile(findFileStr);
	int fCnt = 0;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
	
		if (finder.IsDots())
			continue;

		TRACE(_T("%s\n"), (LPCTSTR)finder.GetFileName());
		TRACE(_T("%s\n"), (LPCTSTR)finder.GetFilePath());
		if(sFileNameList)
		{
			sFileNameList[m_SearchedFileCnt++] = finder.GetFilePath();
		}
		else
		{
			// 변환할 파일릐 갯수만 증가시킴
			fCnt++;
		}
	} 
	return fCnt;
}

int CFormViewShellView::SearchOneFile(CString sDirName, CString sFileName, CString *sFileNameList)
{

	CFileFind finder;

	CString findFileStr;
	if(!sDirName.IsEmpty())
		findFileStr = sDirName + "\\*" + sFileName;
	else
		findFileStr = sFileName;
	BOOL bWorking = finder.FindFile(findFileStr);
	int fCnt = 0;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
	
		if (finder.IsDots())
			continue;

		TRACE(_T("%s\n"), (LPCTSTR)finder.GetFileName());
		TRACE(_T("%s\n"), (LPCTSTR)finder.GetFilePath());
		if(sFileNameList)
		{
			*sFileNameList = finder.GetFilePath();
		}
		else
		{
			// 변환할 파일릐 갯수만 증가시킴
			fCnt++;
		}
	} 
	return fCnt;
}

// // 커맨드 명령 표시
void CFormViewShellView::DisplayCommand(BOOL modifyed)
{
	CString execArg;
	GetDlgItemText(IDC_EDIT3, execArg);

	GetDlgItemText(IDC_SRC_FILE, m_FullFileName);

	GetDlgItemText(IDC_EXEC_FILE, m_ExecFilePath);
	
	CString midPath;
	GetDlgItemText(IDC_EDIT4, midPath);


	GetDlgItemText(IDC_PATH, m_DestPath);
	
	GetDlgItemText(IDC_EDIT5, m_PreCmdOptStr);

	CString testAllPath = m_ExecFileName + " " + m_FullFileName + " " + "[" + midPath +"]"+ m_PreCmdOptStr + " " + "[" + m_DestPath + "]" + execArg;
	
	testAllPath.Replace(" ", "□");

	//if(m_DestPath.IsEmpty())
	//{
	//	testAllPath = m_FullFileName + "\\" + midPath + " " +execArg;
	//}
	
	
	SetDlgItemText(IDC_EDIT1, testAllPath);
	m_ProgressCtrl.SetPos(0);
	m_EditProgCnt.ShowWindow(SW_HIDE);
	if(m_FirstLoaded && m_AllCmdLnText.Compare(testAllPath))
		GetDocument()->SetModifiedFlag(TRUE);
	else if(!m_FirstLoaded)
	{
		m_AllCmdLnText = testAllPath;
	
	}
	else
	{
		GetDocument()->SetModifiedFlag(FALSE);
	}
	
}
//void CFormViewShellView::OnBnClickedOk()
//{
//	PostQuitMessage(0);
//}

void CFormViewShellView::OnEnChangePath()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	DisplayCommand(TRUE);
}

void CFormViewShellView::OnEnChangeSrcFile()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	DisplayCommand(TRUE);
}

void CFormViewShellView::OnEnChangeExecFile()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	DisplayCommand(TRUE);
}

void CFormViewShellView::OnEnChangeEdit3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	DisplayCommand(TRUE);
}

void CFormViewShellView::OnEnChangeEdit4()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	DisplayCommand(TRUE);
}

void CFormViewShellView::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}





void CFormViewShellView::OnEnChangeEdit5()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	DisplayCommand(TRUE);
}

//void CFormViewShellView::OnBnClickedOk()
//{
//	// TODO: Add your control notification handler code here
//}

void CFormViewShellView::OnBnClickedBtnOutAdd()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(!m_ListData.IsEmpty())
	{
		switch(m_AddExtTypeRadio)
		{
		//선택된 확장자 이동
		case 0:
			m_ListData = "[M]" + m_ListData;
			break;
		//선택된 확장자 복사
		case 1:
			m_ListData = "[C]" + m_ListData;
			break;
		//선택된 확장자 제거
		case 2:
			m_ListData = "[D]" + m_ListData;
			break;
		}
		m_ListBox.AddString(m_ListData);
		GetDocument()->SetModifiedFlag(TRUE);
	}
}

void CFormViewShellView::OnBnClickedBtnOutDel()
{
	// TODO: Add your control notification handler code here
	int loc;
	loc = m_ListBox.GetCurSel();
	m_ListBox.DeleteString(loc);
	if(loc == m_ListBox.GetCount())
	{
		loc = loc - 1;
		GetDocument()->SetModifiedFlag(TRUE);
	}
	m_ListBox.SetCurSel(loc);
}

//void CFormViewShellView::OnLbnSelchangeListOut()
//{
//	// TODO: Add your control notification handler code here
//	DisplayCommand(TRUE);
//}

void CFormViewShellView::OnBnClickedSelfile()
{
	// TODO: Add your control notification handler code here
	//LPITEMIDLIST idRoot;
    //SHGetSpecialFolderLocation(0, CSIDL_COMMON_PROGRAMS, &idRoot);

	LPITEMIDLIST pidlBrowse;

	BROWSEINFO BRinfo;
	BRinfo.hwndOwner = GetSafeHwnd();
	BRinfo.pidlRoot = 0;
	BRinfo.pszDisplayName = m_FullFileName.GetBuffer(MAX_PATH);
	BRinfo.lpszTitle = "폴더를 선택하세요.";
	BRinfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_STATUSTEXT | BIF_UAHINT;
	// m_FullFileName에 설정한 경로를 찾아감
	BRinfo.lpfn = BrowseCallbackProc;
	BRinfo.lParam = (LPARAM)BRinfo.pszDisplayName;

	pidlBrowse = SHBrowseForFolder(&BRinfo);

	if(pidlBrowse != NULL) 
	{
		SHGetPathFromIDList(pidlBrowse, m_FullFileName.GetBuffer(MAX_PATH));
		SetDlgItemText(IDC_SRC_FILE, m_FullFileName);
		char *temp = m_FullFileName.GetBuffer(MAX_PATH);
		m_FullFileName = temp;
	
	}

/*
    CFileDialog dlg(TRUE);

	if(dlg.DoModal() == IDOK)
	{
		CString fileName = dlg.GetFileName();
		m_FullFileName = dlg.GetPathName();
		m_FullFileName.Replace("\\" + fileName, "");

		SetDlgItemText(IDC_SRC_FILE, m_FullFileName);
	}
*/
	DisplayCommand(TRUE);
}
BOOL CFormViewShellView::PreTranslateMessage(MSG* pMsg)
{
   switch(pMsg->message)
   {
      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
      case WM_MOUSEMOVE:
             m_toolTip.RelayEvent(pMsg);
             break;
      default:
             break;
   }

   return CFormView::PreTranslateMessage(pMsg);

}





void CFormViewShellView::OnBnClickedRadio1Move()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CFormViewShellView::OnBnClickedRadio2Copy()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CFormViewShellView::OnBnClickedRadio3Delete()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}


int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	CHAR lpszDir[MAX_PATH];
	switch(uMsg)
	{
	case BFFM_INITIALIZED:
		//
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		break;
	case BFFM_SELCHANGED:
		if(SHGetPathFromIDList((LPITEMIDLIST)lParam, lpszDir))
		{ //
			SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)lpszDir);
		}
		break;
	}
	return 0;
}