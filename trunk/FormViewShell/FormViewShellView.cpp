// FormViewShellView.cpp : CFormViewShellView 클래스의 구현
//

#include "stdafx.h"
#include "FormViewShell.h"

#include "FormViewShellDoc.h"
#include "FormViewShellView.h"
#include "PreviewDlg.h"
#include <shlwapi.h>
#include "MainFrm.h"
#include <string>

#include <direct.h>
#include "OperPlus.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 확장자의 최대길이는 6으로 가정				
#define FILE_EXT_LENGTH_MAX (6)

// CFormViewShellView

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

IMPLEMENT_DYNCREATE(CFormViewShellView, CFormView)

BEGIN_MESSAGE_MAP(CFormViewShellView, CFormView)
	ON_BN_CLICKED(IDC_SELFILE2, &CFormViewShellView::OnBnClickedSelExcfile)
	ON_BN_CLICKED(IDC_SELPATH, &CFormViewShellView::OnBnClickedSelpath)
	ON_BN_CLICKED(IDC_CHECK1, &CFormViewShellView::OnBnClickedOptionCheck)

	ON_BN_CLICKED(IDC_EXECAPPL, &CFormViewShellView::OnBnClickedExecappl)
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
	ON_EN_CHANGE(IDC_EDIT6, &CFormViewShellView::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT7, &CFormViewShellView::OnEnChangeEdit7)
	ON_EN_CHANGE(IDC_EDIT8, &CFormViewShellView::OnEnChangeEdit8)
	ON_LBN_SELCHANGE(IDC_LIST_OUT, &CFormViewShellView::OnLbnSelchangeListOut)
	ON_EN_CHANGE(IDC_EDIT_OUT, &CFormViewShellView::OnEnChangeEditOut)
	ON_BN_CLICKED(IDC_BUTTON_ADD_FOLDER, &CFormViewShellView::OnBnClickedButtonAddFolder)
	ON_BN_CLICKED(IDC_BUTTON_DEL_FOLDER, &CFormViewShellView::OnBnClickedButtonDelFolder)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON2, &CFormViewShellView::OnBnSeqExcuteButton)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CFormViewShellView::OnDeltaposSpin)
	ON_LBN_SELCHANGE(IDC_LIST_TCL_FILES, &CFormViewShellView::OnLbnSelchangeListTclFiles)
	ON_BN_CLICKED(IDC_DEL_MTCL_LIST, &CFormViewShellView::OnBnClickedDelMtclList)
	ON_BN_CLICKED(IDC_BUTTON_PRE, &CFormViewShellView::OnBnClickedButtonPre)
	ON_BN_CLICKED(IDC_BUTTON_PRELIST, &CFormViewShellView::OnBnClickedButtonPrelist)
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, &CFormViewShellView::OnBnClickedButtonPreview)
END_MESSAGE_MAP()

// CFormViewShellView 생성/소멸

extern "C" __declspec(dllimport) char* GetDateTime(bool& isTimeOver);

CFormViewShellView::CFormViewShellView()
	: CFormView(CFormViewShellView::IDD)
	, m_ExecFilePath(_T(""))
	, m_FullFileName(_T(""))
	, m_DestPath(_T(""))
	, m_SubDirCnt(0)
	, m_SearchedFileCnt(0)
	, m_PreCmdOptStr(_T(""))
	, m_AllCmdLnText(_T(""))
	, m_FirstLoaded(FALSE)
	, m_ListData(_T(""))
	, m_IsToolTipInit(false)
	, m_AddExtTypeRadio(0)
	, m_ExFolderName(_T(""))
	, m_bMultiMode(FALSE)
	, m_bIsPreview(FALSE)
	, m_OperPlus(0)
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
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtrl);
	DDX_Control(pDX, IDC_EDIT2, m_EditProgCnt);
	DDX_Control(pDX, IDC_LIST_OUT, m_ListBox_Out);
	DDX_Text(pDX, IDC_EDIT_OUT, m_ListData);
	DDX_Radio(pDX, IDC_RADIO1_MOVE, m_AddExtTypeRadio);
	DDX_Control(pDX, IDC_EDIT_SUCCESS_FAIL, m_Edit_Success_Fail);

	DDX_Control(pDX, IDC_LIST3, m_ExFolderListBox);
	DDX_Text(pDX, IDC_EDIT_EX_FOLDER, m_ExFolderName);
	DDX_Control(pDX, IDC_LIST_TCL_FILES, m_TclFilesListBox);
	DDX_Control(pDX, IDC_BUTTON2, m_BtnMultiTCLExcute);
	DDX_Control(pDX, IDC_EXECAPPL, m_BtnExcute);
	DDX_Control(pDX, IDC_VIEWLIST, m_ViewList);
	DDX_Control(pDX, IDC_SUB_FOLDER, m_ChkSubFolder);
	DDX_Control(pDX, IDC_EmptyFolderCheck, m_EmptyFolderCheck);
	DDX_Control(pDX, IDC_CHECK4, m_ExitCheckBox);
	DDX_Control(pDX, IDC_CHECK5, m_MTCL_ExitCheckBox);
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
		m_toolTip.AddTool(GetDlgItem(IDC_EDIT7), "파일처리후 최종확장자 ex) .nlz");
		m_toolTip.AddTool(GetDlgItem(IDC_EDIT1), m_AllCmdLnText);
		m_toolTip.AddTool(GetDlgItem(IDC_EDIT_DELAY_TIME), "x10ms");
		m_toolTip.SetMaxTipWidth(1000);
		m_IsToolTipInit = true;
	}
	
	// 폼뷰에 스크롤바 제거
	ResizeParentToFit(FALSE);

	CRect rect;

	GetClientRect(&rect);

	SetScaleToFitSize(rect.Size());  


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
// m_ExecFilePath에 설정한 경로를 찾아감
void CFormViewShellView::OnBnClickedSelExcfile()
{
	// TODO: Add your control notification handler code here
/*
    CFileDialog dlg(TRUE);

	if(dlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_EXEC_FILE, dlg.GetPathName());
		m_ExecFileName = dlg.GetFileName();
		m_ExecFilePath = dlg.GetPathName();
		
	}
*/
	LPITEMIDLIST pidlBrowse;

	BROWSEINFO BRinfo;
	BRinfo.hwndOwner = GetSafeHwnd();
	BRinfo.pidlRoot = 0;
	BRinfo.pszDisplayName = m_ExecFilePath.GetBuffer(MAX_PATH);
	BRinfo.lpszTitle = "파일을 선텍하세요.";
	BRinfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_STATUSTEXT | BIF_UAHINT | BIF_RETURNFSANCESTORS | BIF_BROWSEINCLUDEFILES;
	// m_ExecFilePath에 설정한 경로를 찾아감
	BRinfo.lpfn = BrowseCallbackProc;
	BRinfo.lParam = (LPARAM)BRinfo.pszDisplayName;

	pidlBrowse = SHBrowseForFolder(&BRinfo);

	if(pidlBrowse != NULL) 
	{
		SHGetPathFromIDList(pidlBrowse, m_ExecFilePath.GetBuffer(MAX_PATH));
		SetDlgItemText(IDC_EXEC_FILE, m_ExecFilePath);
		char *temp = m_ExecFilePath.GetBuffer(MAX_PATH);
		m_ExecFilePath = temp;
	}
	
	// DisplayCommand(TRUE);
}

/*void CFormViewShellView::OnBnClickedSelfile()
{
	TODO: Add your control notification handler code here

	LPITEMIDLIST pidlBrowse;

	BROWSEINFO BRinfo;
	BRinfo.hwndOwner = GetSafeHwnd();
	BRinfo.pidlRoot = NULL;
	BRinfo.pszDisplayName = m_FullFileName.GetBuffer(MAX_PATH);
	BRinfo.lpszTitle = "폴더를 선택하세요.";
	BRinfo.ulFlags = BIF_RETURNONLYFSDIRS;
	BRinfo.lpfn = NULL;
	BRinfo.lParam = 0;

	pidlBrowse = SHBrowseForFolder(&BRinfo);
	if(pidlBrowse != NULL) 
	{
		SHGetPathFromIDList(pidlBrowse, m_FullFileName.GetBuffer(MAX_PATH));
		SetDlgItemText(IDC_SRC_FILE, m_FullFileName);
		char *temp = m_FullFileName.GetBuffer(MAX_PATH);
		m_FullFileName = temp;
	
	}
	DisplayCommand(TRUE);
}
*/

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
	//DisplayCommand(TRUE);
}



void CFormViewShellView::OnBnClickedOptionCheck()
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


/*---------------------------------------------------------------------------*
  Name:         SearchDir

  Description:  	실행버튼 누른면 호출 명령을 수행한다.
 *---------------------------------------------------------------------------*/
void CFormViewShellView::OnBnClickedExecappl()
{
	// TODO: Add your control notification handler code here
	
	const int FILE_EXIST_CHECK_CNT = 2;
	// 사용기간 초과 검사
	bool isTimeOver = false;
	// dll파일은 실행파일과 같은 폴더에 위치시켜야함
	char* timeStr = GetDateTime(isTimeOver);
	if(isTimeOver)
	{
		AfxMessageBox("사용기간 초과");
		return;
	}

	// 갱신파일 저장(코드워리어 make용)
	// 실행시점, 사용된 파일경로 저장
	CFile myFile;
	CFileException e;
	if(!myFile.Open(GetDocument()->m_IniFilePath + ".txt", CFile::modeCreate | CFile::modeWrite, &e))
	{
		TRACE(_T("File could not be opened %s : %d\n"), GetDocument()->m_IniFilePath + ".txt", e.m_cause);
	}
	CString refreshStr = m_SettingFilePath + "\r\n" + timeStr;
	myFile.Write(refreshStr, refreshStr.GetLength());
	myFile.Close();

	BOOL bUseShellExCute = TRUE;
	BOOL isInternalCmd = FALSE;

	GetDlgItemText(IDC_SRC_FILE, m_FullFileName);
	GetDlgItemText(IDC_EXEC_FILE, m_ExecFilePath);
	GetDlgItemText(IDC_PATH, m_DestPath);
	
	// 외부명령어인지 검사함
	if(!(m_ExecFilePath).Compare("command.com") || 
		!(m_ExecFilePath).Compare("cmd.exe") || 
		!(m_ExecFilePath).Compare("explorer.exe"))
	{
		isInternalCmd = TRUE;
	}
	// 외부명령어 모드가 안닌데 작업할 파일이 지정되지 않았으면
	if(m_FullFileName.IsEmpty() && !isInternalCmd) 
	{ 
		AfxMessageBox("변환할 파일을 선택하세요.");

		OnBnClickedSelfile();
		if(m_FullFileName.IsEmpty())
		{
			return;
		}
	}
	// 외부명령어 모드가 안닌데 실행파일이 없으면
	else if(m_ExecFilePath.IsEmpty())// && (fPath.Right(5)).Compare(".mtcl")) 
	{ 
		AfxMessageBox("실행파일을 선택하세요.");

		OnBnClickedSelfile();
		if(m_ExecFilePath.IsEmpty())
		{
			return;
		}
	}

	int iSuccessCnt = 0;

	// 하위경로를 저장할 벡터
	std::vector<CString> vecSearchSubDirList;
	// 하위경로에 있는 파일명을 저장할 벡터
	std::vector<CString> vecFileList;
	// 실행 파일 다음 옵션 파라미터
	CString execFirstArg;
	GetDlgItemText(IDC_EDIT6, execFirstArg);

	CString execArg;
	GetDlgItemText(IDC_EDIT3, execArg);

	GetDlgItemText(IDC_PATH, m_DestPath);

	int fileCheckCnt = 0;
	fileCheckCnt = GetDlgItemInt(IDC_EDIT8);
	// 목적경로를 상대경로로 표시했을경우 대응
	/*// 추후대응
	CFileFind dirFinder;
	BOOL bWorking = dirFinder.FindFile(m_DestPath + "\\*.*");
	if(!m_DestPath.IsEmpty() && !m_DestPath.Compare(".."))
		m_DestPath = m_outRootPath = dirFinder.GetRoot();
	bWorking = dirFinder.FindFile(m_FullFileName + "\\*.*");
	if(!m_DestPath.IsEmpty() && !m_DestPath.Compare(".."))
		m_FullFileName = dirFinder.GetRoot();
`*/
	CString searchRootDirStr = m_FullFileName;
	CString searchFileStr = m_midPath + "\0";

	// 하위 폴더 갯수 초기화
	m_SubDirCnt = 0;
	// 검색된 갯수 초기화
	m_SearchedFileCnt = 0;
	
	// 하위경로에 있는 SubFolder를 검사함
	if(!isInternalCmd)
		SearchDir(m_FullFileName, vecSearchSubDirList);
	// 프로그래스표시 초기화
	m_ProgressCtrl.SetRange(0, 100);
	m_ProgressCtrl.SetPos(0);
	// 단일 작업일때는 여기서 작업결과 텍스트 초기화
	if(m_BtnExcute.IsWindowEnabled())
		m_ExcuteFilePath = "";
	
	int progressCnt = 0;
	// 변환할 파일의 개수를 결정. 
	for(int dirCnt = 0;dirCnt < m_SubDirCnt; dirCnt++) 
	{	
		int noUse = -1;
		progressCnt += SearchFile(searchRootDirStr + vecSearchSubDirList[dirCnt], searchFileStr, vecFileList, noUse);
	}
	
	m_EditProgCnt.ShowWindow(SW_SHOW);
	
	
	// 내부 명령어 대응
	if(isInternalCmd)
	{
		//CString CMDLine = execFirstArg + "\"" + searchRootDirStr + "" + searchFileStr +  "\"" + ""+ "\"" + m_DestPath + execArg + "\"";
		CString CMDLine = execFirstArg;

		ShellCommon(m_ExecFilePath, CMDLine, SW_SHOW);

		//fMask에 SEE_MASK_NOCLOSEPROCESS를 포함하였다면
		//hProcesss 멤버를 통하여 새로생긴 프로세스의 핸들을 반환 받을 수 있다.
		progressCnt= 1;// 분모가 0이되는걸 막음
		isInternalCmd = TRUE && m_SearchedFileCnt;

		return;
	}

	for(int dirCnt = 0; dirCnt < m_SubDirCnt; dirCnt++)
	{
		int starIndex = m_SearchedFileCnt;
		//대상파일 검색
		SearchFile(searchRootDirStr + vecSearchSubDirList[dirCnt], searchFileStr, vecFileList, m_SearchedFileCnt);
		
		if(isInternalCmd)
			m_SearchedFileCnt = 1;

		for(int listCnt = starIndex; listCnt < m_SearchedFileCnt || isInternalCmd; listCnt++)
		{
			CString prgCnt;
			// 셀함수 반환값
			HINSTANCE ret = 0;
			
			m_ProgressCtrl.SetPos(((listCnt + 1) * 100) / progressCnt);
			m_ProgressCtrl.UpdateWindow();
			prgCnt.Format( "[ %d / %d ]", listCnt+1, progressCnt);
			
			if(listCnt+1 == progressCnt)
				prgCnt += " FIN!!!";

			SetDlgItemText(IDC_EDIT2, prgCnt);
			
			GetDlgItem(IDC_EDIT2)->UpdateWindow();
			//UpdateWindow();
			
			CString testAllPath;// = execFirstArg + "\"" + vecFileList[listCnt] + m_PreCmdOptStr + m_DestPath + vecSearchSubDirList[dirCnt] +"\"" + execArg;
			INT nShowCmd = SW_HIDE;	

		//================================ 특수 상황 커맨드=======================================================================================================================
			if(m_DestPath.IsEmpty() || m_PreCmdOptStr.IsEmpty())
			{
				testAllPath = execFirstArg + "\"" + vecFileList[listCnt] + "\"" +  execArg;
			  // PVRTexTool 커맨드라인대응(cocos2d-x)
				if(!(m_PreCmdOptStr.Left(3)).Compare("pvr")){
					
					CString tempStr = m_PreCmdOptStr;
					CString tempFileStr =  vecFileList[listCnt];
					int sIndex = tempFileStr.Find('.', 0);
					CString tempFileExetStr = tempFileStr.Right(tempFileStr.GetLength() - sIndex);
					tempFileStr.Replace(tempFileExetStr, ".pvr"); 
					tempStr.Replace("pvr", "");
					testAllPath += tempStr + "\"" + tempFileStr;
				
				}
				// A2d 컨버트(Aztool)
				else if((!(m_PreCmdOptStr.Left(3)).Compare("A2d")) || (!(m_PreCmdOptStr.Left(3)).Compare("a2d")))
				{
					testAllPath = execFirstArg + "\"" + vecFileList[listCnt] + "\"";
					CString tempStr = m_PreCmdOptStr;
					CString tempFileStr =  vecFileList[listCnt];
					int sIndex = tempFileStr.Find('.', 0);
					CString tempFileExetStr = tempFileStr.Right(tempFileStr.GetLength() - sIndex);
					tempFileStr.Replace(tempFileExetStr, ".vrp"); 
					tempStr.Replace("vrp", "");
					testAllPath +=  " \"" + tempFileStr  + "\"" +  execArg;
				}
			}
			else
			{	// Avitomoviclipds.exe 대응 경로부분의 공백이 있으면 문제발생 " "로 묶음
				testAllPath = execFirstArg + "\"" + vecFileList[listCnt] + "\"" + m_PreCmdOptStr + "\"" + m_DestPath + vecSearchSubDirList[dirCnt] +"\"" + execArg;
			}

			CString oExtStrTop;

			if(m_ListBox_Out.GetCount())
				m_ListBox_Out.GetText(0, oExtStrTop);
			
			if(0 <= oExtStrTop.Find('*',0))
			{
				// 파일 처리작업만 한다.
				bUseShellExCute = FALSE;
				
				if(m_DestPath.IsEmpty())
					m_DestPath = m_FullFileName;
			}
			else
			{
				ShellCommon(m_ExecFilePath, testAllPath, SW_HIDE);
			}
			
			for(int lbIndex = 0; lbIndex < m_ListBox_Out.GetCount(); lbIndex++)
			{
				// 파일처리 타입 설정
				int iExtType = 0;
				// 확장자 추출용
				int sIndex = 0;
				// 확장자
				CString oExtStr;
				//실행결과로 생성된 파일패스
				CString outPutResultPath = "";
				// 입력 파일의 확장자
				CString iExtStr = "";
				// 확장자만 추출
				sIndex = testAllPath.Find('.', 0);
				iExtStr = testAllPath.Right(testAllPath.GetLength() - sIndex);
				iExtStr.Replace("\"", "");
				
				m_ListBox_Out.GetText(lbIndex, oExtStr);

				CString tExtTypeStr = oExtStr.Left(3);

				char *fileOp[3]= {"[M]", "[C]", "[D]"};
				
				if(!(oExtStr.Left(3)).Compare(fileOp[FO_MOVE - 1]))
				{
					iExtType = FO_MOVE;
				}
				else if(!(oExtStr.Left(3)).Compare(fileOp[FO_COPY - 1]))
				{
					iExtType = FO_COPY;
				}
				else if(!(oExtStr.Left(3)).Compare(fileOp[FO_DELETE - 1]))
				{
					iExtType = FO_DELETE;
				}
				
				//확장자에서 파일처리를 위한 스트링만 제거
				sIndex = oExtStr.Find('.', 0);
				oExtStr = oExtStr.Right(oExtStr.GetLength() - sIndex);

				//실행결과로 생성된 파일패스 만들기
				//sIndex = testAllPath.Find('.',testAllPath.GetLength() - FILE_EXT_LENGTH_MAX);
				sIndex = testAllPath.Find('.',0);
				
				if(!bUseShellExCute)
				{
					sIndex = testAllPath.Find('.',0);
					CString sExte = testAllPath.Right(testAllPath.GetLength() - sIndex);
					sExte.Replace("\"","");
					oExtStr = sExte;
					outPutResultPath = testAllPath;
				}
				else
				{
					outPutResultPath = testAllPath.Left(sIndex) + oExtStr;
				}

				outPutResultPath.Replace("\"","");

				CString tempOutPutStr = vecFileList[listCnt];
				// 파일 이름만 분리하기
				//sIndex = tempOutPutStr.Find('.',tempOutPutStr.GetLength() - FILE_EXT_LENGTH_MAX);
				sIndex = tempOutPutStr.Find('.',0);
				CString sExte = tempOutPutStr.Right(tempOutPutStr.GetLength() - sIndex);
				// 출력파일이름에 확장자 붙이기
				tempOutPutStr.Replace(sExte, oExtStr);

				tempOutPutStr.Replace(searchRootDirStr, "");
				//CString movSrc = searchRootDirStr + tempOutPutStr;
				tempOutPutStr = m_DestPath + tempOutPutStr;
				// 이름 강제 바꾸고 이동하기
				//CString movSrc = searchRootDirStr + vecSearchSubDirList[dirCnt] + "\\*" + oExtStr;//SE_ERR_ACCESSDENIED
				CString movDest = tempOutPutStr;
				//생성된 파일 찾기위한 파일 패스
				CString movingStr = "";
				int findCnt = 0;
				//
				BOOL bAtiveFileProc = TRUE;
				// 컨버팅된 파일이 원본 파일이 있는 폴더에 아직 없을 경우 계속 찾음
				while(movingStr.IsEmpty())
				{	
					if(iExtType == FO_COPY)
					{
						SearchOneFile(NULL, outPutResultPath, &movingStr);
					}
					else if(iExtType == FO_DELETE)
					{
						// 삭제할 파일의 경로
						//movingStr = tempOutPutStr;
						//bAtiveFileProc = PathFileExists(tempOutPutStr);
						
						movingStr = outPutResultPath;
						bAtiveFileProc = PathFileExists(movingStr);
						break;
					}
					else
					{
						CString extModify;
						GetDlgItemText(IDC_EDIT7, extModify);
						// 파일이동처리시 특정확장자로 변경 
						if(!extModify.IsEmpty()){
							//movDest.Replace(oExtStr, iExtStr + extModify);
							movDest.Replace(oExtStr, extModify);
						}
							//movDest += extModify;
						// 파일이동 올바르게 하려면 하나의 파일만 선택되도록 해야함
						if(bUseShellExCute)
							SearchOneFile(searchRootDirStr + vecSearchSubDirList[dirCnt], oExtStr, &movingStr);
						else
							movingStr = outPutResultPath;
					}
					//Sleep(10);
					m_ExcuteFilePath +=  ".\r\n";
					findCnt++;
					// 릴리즈모드 대응(디버그라면 더늘려야함)
					// 의미가 없게 되었음
					fileCheckCnt = 0;
					if(findCnt > fileCheckCnt)
						break;
				}	
				// CMD 명령
/*				CString testSTRING ="/c move " + movSrc + " " + movDest;
				HINSTANCE movRet = ShellExecute(NULL, "open", "cmd", testSTRING, NULL, SW_HIDE);
*/

				//========================================= 결과 파일처리 표시====================================================
				BOOL isDestFileExist = FALSE;
				if(bAtiveFileProc)
				{

					if(m_ViewList.GetCheck() && !movingStr.IsEmpty())
					{
						//파일처리 리스트 출력하기
						CString fileOp[3]= {" 이동", " 복사", " 삭제"};
						if(iExtType == FO_DELETE)
						{
							m_ExcuteFilePath +=	movingStr + " 를" + fileOp[iExtType - 1] +"\r\n";
						}
						else
						{
							m_ExcuteFilePath +=	movingStr + " 를"+ "  \r\n"  + movDest + "로" + fileOp[iExtType - 1] + "\r\n";
						}

						//SetDlgItemText(IDC_EDIT9, m_ExcuteFilePath);
						//GetDlgItem(IDC_EDIT9)->UpdateWindow();
					}

					SHFILEOPSTRUCT shos;
					ZeroMemory(&shos, sizeof(SHFILEOPSTRUCT));
					// 파일처리 타입 결정
					shos.wFunc = iExtType;

					//movingStr = "\"" + movingStr + "\"";
					movingStr.Append("\0", 1);
					movDest.Append("\0", 1);
					shos.pFrom = NULL;
					shos.pTo = NULL;
					// 이런식으로 넣어줘야함!!!
					TCHAR        pszFrom[1024] = {0}; 
					strcpy_s(pszFrom , movingStr);
					shos.pFrom = pszFrom;//"D:\\WORK\\Macheon_Trial\\MC\\Program\\NC\\MainUi\\2d\\MainUI_bottom.NCER";

					TCHAR        pszTo[1024] = {0};
					//movDest = "\"" + movDest + "\"";
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
					//===================================쉘파일처러===========================================================
						// 미리보기
						if(!m_bIsPreview)
						{
							/*!!! 디버그모드시 에러발생가능 릴리즈 모드에서도 테스트 하도록!!!*/
							// shRet이 0 이외의 값이면 실패
							shRet = SHFileOperation(&shos);
						}
						//Sleep(10);
						if(tryCount > 0)
							m_ExcuteFilePath +=  "파일처리 중";
						
						tryCount++;
					}
					while(tryCount < 2 && shRet != 0);
					isDestFileExist = PathFileExists(movingStr);// 처리할 파일(결과파일)이 경로에 있는지 검사
					if(!isDestFileExist && m_Check_EnableErrPop.GetCheck())
					{
						int popRet = MessageBox(movDest + " \n파일이 없습니다. 계속 하시겠습니까?","파일처리 에러", MB_OKCANCEL);
						
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
					//처리해야할 파일이 없고 에러메세지를 표시한 상태이면
					if(shRet && m_Check_EnableErrPop.GetCheck())
					{
						int popRet = MessageBox(movDest + " \n작업을 마치지 못했습니다. 계속 하시겠습니까?","파일처리 에러", MB_OKCANCEL);
						
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


				if(iExtType == FO_DELETE)
				{
					isDestFileExist = PathFileExists(movingStr);// 삭제할 파일이 경로에 있는지 검사
				}

				int findMovedFileCnt = 0;
				while((iExtType == FO_DELETE && isDestFileExist) || (iExtType != FO_DELETE && !isDestFileExist))
				{
					isDestFileExist = PathFileExists(movDest);
					//Sleep(10);
					findMovedFileCnt++;
					
					if(findMovedFileCnt > FILE_EXIST_CHECK_CNT)
						break;
				}
				// 에러메세지 표시여부
				if(m_Check_EnableErrPop.GetCheck() && ((iExtType == FO_DELETE && isDestFileExist) || (iExtType != FO_DELETE && !isDestFileExist)))
				{
					int popRet = MessageBox(outPutResultPath + " \n에대한 작업을 마치지 못했습니다. \n계속 하시겠습니까?","일지 정지", MB_OKCANCEL);
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
					iSuccessCnt +=  (lbIndex + 1) / m_ListBox_Out.GetCount();//  ++;
				}
			}
			
			if(bUseShellExCute)
			{
				switch((int)ret)
				{
					case SE_ERR_NOASSOC:
					{
						MessageBox("지정된 파일 확장자로 실행할 수 있는 프로그램이 없습니다.","파일 확장자 오류", MB_OKCANCEL);
					}
					break;
					case SE_ERR_FNF:
					{
						MessageBox("지정된 파일을 찾을수 없습니다.","파일 오류", MB_OKCANCEL);
					}
					break;
				}
			}
		}
	}
	
	// 소스쪽의 빈폴더 지우기
	for(int dirCnt = m_SubDirCnt - 1; m_EmptyFolderCheck.GetCheck() && dirCnt >= 1; dirCnt--) // dirCnt >= 1 : 최상위 폴더는 삭제하지않음
	{
		//***하위에 파일이나 폴더가 있으면 폴더가 지워지지 않음***
		//WCHAR	wstringTemp[1024];
		CString		delFolderPath = m_FullFileName + vecSearchSubDirList[dirCnt];

		//ZeroMemory(wstringTemp, sizeof(wstringTemp));
		//int nLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, (LPCSTR)delFolderPath , delFolderPath.GetLength(), wstring, 1024 );

		// convert a cstring to a wstring(http://social.msdn.microsoft.com/Forums/en-US/vcgeneral/thread/def5f69a-57da-456a-bba9-dad0743f5929)
		std::wstring	wStrigPath =	CStringW(delFolderPath);
		//_wrmdir(wstringTemp);
		_wrmdir(wStrigPath.c_str());
		int tempErr = errno;//GetLastError();
		errno;
	}
	
	// 결과 표시
	CString resultCnt;
	resultCnt.Format( "%d / %d 성공", iSuccessCnt, progressCnt);
	m_Edit_Success_Fail.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_EDIT_SUCCESS_FAIL, resultCnt);
	GetDlgItem(IDC_EDIT_SUCCESS_FAIL)->UpdateWindow();
	//m_ProgressCtrl.SetPos(100);
	if(m_ViewList.GetCheck())
	{
		OnBnClickedButtonPre();
	}
	if(m_ExitCheckBox.GetCheck())
		exit(0);
}

/*---------------------------------------------------------------------------*
  Name:         SearchDir

  Description:  하위폴더를 찾아 없으면 생성하고 하위 경로를 sDirNameList에 넣는다.

  Arguments:    sDirName:	검사할 상위폴더
                            
                sDirNameList:  저장될 하위폴더의 경로

  Returns:      0
 *---------------------------------------------------------------------------*/
int CFormViewShellView::SearchDir(CString sDirName, std::vector<CString> &sDirNameList)
{
	CFileFind dirFinder;
	BOOL bWorking = dirFinder.FindFile(sDirName + "\\*.*");	

	int dCnt = 0;

	// 목적경로에 하위 폴더가 되면 연쇄적으로 계속 폴더가 생성되는 것을 방지 
	if(!(m_DestPath.Compare(sDirName)))
		return dCnt;
	// 최상위 폴더 추가
	if(!m_SubDirCnt)
	{
		//CString tempfolderPath = sDirName;
		//tempfolderPath.Replace(m_FullFileName, "");
		//sDirNameList[m_SubDirCnt++];// = tempfolderPath;
		m_SubDirCnt++;
		sDirNameList.push_back("");
		CreateDirectory(sDirName, NULL);

	}

	while (bWorking && m_ChkSubFolder.GetCheck())
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
			// 목적경로에 하위 폴더가 되면 연쇄적으로 계속 폴더가 생성되는 것을 방지 
			if(!(m_DestPath.Compare(tempfolderPath)))
				continue;

			CString exFolderName;

			// 작업에서 제외할 폴더명 검사
			BOOL isExFolder = FALSE;
			for(int lbIndex = 0; lbIndex < m_ExFolderListBox.GetCount(); lbIndex++)
			{
				m_ExFolderListBox.GetText(lbIndex, exFolderName);
			
				if(-1 != tempfolderPath.Find(exFolderName,0))
				{
					isExFolder = TRUE;			
					continue;
				}
			}

			if(isExFolder)
				continue;

			tempfolderPath.Replace(m_FullFileName, "");
			//sDirNameList[m_SubDirCnt++] = tempfolderPath;
			m_SubDirCnt++;
			sDirNameList.push_back(tempfolderPath);
			// 서브폴더명만 저장
			
			
			// 타겟 파일이 있는것만 폴더를 생성해줌
			// 타겟 파일이 없는데 그 하위폴더에 있는 경우도 폴더를 생성하지 않으므로 주석처리
			//if(SearchFile(sDirName + sDirNameList[m_SubDirCnt-1], m_midPath, NULL))
			{

				CString path = m_DestPath + sDirNameList[m_SubDirCnt-1];
				// 목적경로에 하위 폴더가 되면 연쇄적으로 계속 폴더가 생성되는 것을 방지 
				if((!m_DestPath.IsEmpty() && m_DestPath.Compare(path)))
					CreateDirectory(path, NULL);
			}
		
			//if(m_ChkSubFolder.GetCheck())
			SearchDir(dirFinder.GetFilePath(), sDirNameList);
		}
	}
	return dCnt;
}

/*---------------------------------------------------------------------------*
  Name:         SearchFile

  Description:  하위폴더를 찾아 없으면 생성함

  Arguments:    sDirName:		검사할 경로            
                sFileName:		찾을 파일명
				vecFileList:	파일의 전체경로
				searchedFileCnt:실제로 작업할 파일의 개수

  Returns:      찾은 파일의 수
 *---------------------------------------------------------------------------*/
int CFormViewShellView::SearchFile(CString sDirName, CString sFileName, std::vector<CString> &vecFileList, int &searchedFileCnt)
{
	CFileFind finder;
	CString findFileStr = sDirName + "\\" + sFileName;
	BOOL bWorking = finder.FindFile(findFileStr);
	int fCnt = 0;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
	
		if (finder.IsDots() || finder.IsDirectory())
			continue;

		TRACE(_T("%s\n"), (LPCTSTR)finder.GetFileName());
		TRACE(_T("%s\n"), (LPCTSTR)finder.GetFilePath());
		if(searchedFileCnt != -1)
		{
			vecFileList.push_back(finder.GetFilePath());
			searchedFileCnt++;
		}
		else
		{
			// 변환할 파일의 갯수만 증가시킴
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
	// 실행 파일 바로 다음 옵션	
	CString execFirstArg;
	GetDlgItemText(IDC_EDIT6, execFirstArg);

	CString execSecondArg;
	GetDlgItemText(IDC_EDIT3, execSecondArg);

	GetDlgItemText(IDC_SRC_FILE, m_FullFileName);

	GetDlgItemText(IDC_EXEC_FILE, m_ExecFilePath);
	
	
	GetDlgItemText(IDC_EDIT4, m_midPath);


	GetDlgItemText(IDC_PATH, m_DestPath);
	
	GetDlgItemText(IDC_EDIT5, m_PreCmdOptStr);

	//CString testAllPath = /*m_ExecFileName */ m_ExecFilePath + " " + execFirstArg + m_FullFileName + " " + "[" + midPath +"]"+ m_PreCmdOptStr + " " + "[" + m_DestPath + "]" + execSecondArg;
	CString testAllPath = /*m_ExecFileName */ m_ExecFilePath + " " + execFirstArg + m_FullFileName + "\\" + m_midPath + m_PreCmdOptStr + " " + m_DestPath + execSecondArg;
	
	//testAllPath.Replace(" ", "∨");

	//if(m_DestPath.IsEmpty())
	//{
	//	testAllPath = m_FullFileName + "\\" + midPath + " " +execArg;
	//}
	
	
	SetDlgItemText(IDC_EDIT1, testAllPath);
	m_ProgressCtrl.SetPos(0);
	m_EditProgCnt.ShowWindow(SW_HIDE);
	if(m_FirstLoaded && m_AllCmdLnText.Compare(testAllPath))
	{
		GetDocument()->SetModifiedFlag(TRUE);
		m_AllCmdLnText = testAllPath;
	}
	else if(!m_FirstLoaded)
	{
		m_AllCmdLnText = testAllPath;
	
	}
	else
	{
		GetDocument()->SetModifiedFlag(FALSE);
	}
	m_toolTip.AddTool(GetDlgItem(IDC_EDIT1), m_AllCmdLnText);
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
		m_ListBox_Out.AddString(m_ListData);
		GetDocument()->SetModifiedFlag(TRUE);
	}
}

void CFormViewShellView::OnBnClickedBtnOutDel()
{
	// TODO: Add your control notification handler code here
	int loc;
	loc = m_ListBox_Out.GetCurSel();
	m_ListBox_Out.DeleteString(loc);
	if(loc == m_ListBox_Out.GetCount())
	{
		loc = loc - 1;
		GetDocument()->SetModifiedFlag(TRUE);
	}
	m_ListBox_Out.SetCurSel(loc);
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
	// OnEnChangeSrcFile 에서 호출하고 있음 주석처리
//	DisplayCommand(TRUE);
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
void CFormViewShellView::OnEnChangeEdit6()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	DisplayCommand(TRUE);
}

void CFormViewShellView::OnEnChangeEdit7()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	DisplayCommand(TRUE);
}

void CFormViewShellView::OnEnChangeEdit8()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	DisplayCommand(TRUE);
}

void CFormViewShellView::OnLbnSelchangeListOut()
{
	// TODO: Add your control notification handler code here
}

void CFormViewShellView::OnEnChangeEditOut()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CFormViewShellView::OnBnClickedButtonAddFolder()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(!m_ExFolderName.IsEmpty())
	{
		m_ExFolderListBox.AddString(m_ExFolderName);
		GetDocument()->SetModifiedFlag(TRUE);
	}
}

void CFormViewShellView::OnBnClickedButtonDelFolder()
{
	// TODO: Add your control notification handler code here
	int loc;
	loc = m_ExFolderListBox.GetCurSel();
	m_ExFolderListBox.DeleteString(loc);
	if(loc == m_ExFolderListBox.GetCount())
	{
		loc = loc - 1;
		GetDocument()->SetModifiedFlag(TRUE);
	}
	m_ExFolderListBox.SetCurSel(loc);
}

void CFormViewShellView::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default

	//CFormView::OnDropFiles(hDropInfo);

	char szFullPath[512] = {0,};
	//--xx 드래그앤 드랍한 파일의 개수 획득
	int iCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	int curCnt = m_TclFilesListBox.GetCount();
	
	//m_TclFilesListBox.ResetContent();

	for(int i=0; i<iCount; i++)
	{
		//--xx 드래그앤 드랍한 파일의 경로정보를 하나씩 획득
		DragQueryFile(hDropInfo, i, szFullPath, sizeof(szFullPath));
		//AfxMessageBox(szFullPath);
		
		CString fPath = szFullPath;
		if(!fPath.IsEmpty() && i + curCnt == 0)
		{
			GetDocument()->OnOpenDocument(fPath);
			GetDocument()->SetPathName(fPath,0);
		}
		
		UpdateData(TRUE);
		if(!fPath.IsEmpty() && (fPath.Right(5)).Compare(".mtcl"))
		{
			m_TclFilesListBox.AddString(fPath);
			m_TclFilesListBox.SetCheck(curCnt, 1);
			GetDocument()->SetModifiedFlag(TRUE);
		}
	}

	curCnt = m_TclFilesListBox.GetCount();
	if(curCnt > 1)
	{
		CFrameWnd * pWnd = (CFrameWnd*)AfxGetMainWnd();
		((CMainFrame*)(pWnd->GetActiveFrame()))->m_cs.cx = 1280;
		((CMainFrame*)(pWnd->GetActiveFrame()))->m_cs.cy = 700;	
		pWnd->SetWindowPos(NULL,0,0,1280,700,SWP_NOMOVE);

		m_BtnMultiTCLExcute.EnableWindow(TRUE);
		m_BtnExcute.EnableWindow(FALSE);
	}

/*
	CString fPath = szFullPath;
	if(!fPath.IsEmpty())
	{
		GetDocument()->OnOpenDocument(fPath);
		GetDocument()->SetPathName(fPath,0);
	}
	CFormView::OnDropFiles(hDropInfo);
*/
}
// 다중실행
void CFormViewShellView::OnBnSeqExcuteButton()
{
	// TODO: Add your control notification handler code here
	
	int lbTclFilesCnt = m_TclFilesListBox.GetCount();
	int forceDelay = 0;
	// 결과 텍스트 초기화
	m_ExcuteFilePath = "";
	
	for(int lbIndex = 0; lbIndex < lbTclFilesCnt; lbIndex++)
	{
		CString fPath;	
		m_TclFilesListBox.GetText(lbIndex, fPath);
		// 고정된 폴더에서만 tcl 로드
		int i = fPath.ReverseFind('\\');//실행 파일 이름을 지우기 위해서 왼쪽에 있는 '/'를 찾는다.
		fPath = fPath.Right(fPath.GetLength() - i);
		fPath = GetDocument()->m_tclFilePath + fPath;		
		if(!fPath.IsEmpty() && m_TclFilesListBox.GetCheck(lbIndex))
		{
			m_bMultiMode = TRUE;
			m_TclFilesListBox.SetCurSel(lbIndex);
			GetDocument()->OnOpenDocument(fPath);
			//GetDocument()->SetPathName(fPath,0);
			m_ExcuteFilePath += "\r\n================================================================\r\n";
			m_ExcuteFilePath += " " + fPath + "\r\n"
								+ "==================================================================\r\n";
			UpdateWindow();
			OnBnClickedExecappl();
			forceDelay = GetDlgItemInt(IDC_EDIT_DELAY_TIME);
			if(lbIndex < lbTclFilesCnt - 1)
				Sleep(forceDelay * 10);
		}
	}
	// 다수의 설정파일을 로드 했기때문에 처음 설정파일을 재로드		
	m_bMultiMode = FALSE;
	//GetDocument()->OnOpenDocument(m_SettingFilePath);
	// 여러개 실행후 타이틀이름이 바뀌어 저장할때 마지막이름으로 저장되어 버림
	//GetDocument()->SetPathName(m_SettingFilePath,0);
	//GetDocument()->SetPathName(m_SettingFilePath + "여기",0);
	if(m_MTCL_ExitCheckBox.GetCheck()){
		MessageBox("==== 완료! ====", "TotalCommandLine");
		exit(0);
	}
}
/*
void CFormViewShellView::OnBnClickedButtonUp()
{
	// TODO: Add your control notification handler code here
	
	int loc;
	CString tempUp;
	CString tempDn;
	loc = m_TclFilesListBox.GetCurSel();
	UpdateData(TRUE);
	if(loc > 0)
	{
		m_TclFilesListBox.GetText(loc - 1, tempUp);
		m_TclFilesListBox.GetText(loc, tempDn);
		m_TclFilesListBox.DeleteString(loc - 1);
		m_TclFilesListBox.InsertString(loc, tempUp);
		GetDocument()->SetModifiedFlag(TRUE);
		m_TclFilesListBox.SetCurSel(loc - 1);
	}
}

void CFormViewShellView::OnBnClickedButtonDn()
{
	// TODO: Add your control notification handler code here
	int loc;
	CString tempUp;
	CString tempDn;
	loc = m_TclFilesListBox.GetCurSel();
	UpdateData(TRUE);
	if(loc < m_TclFilesListBox.GetCount() - 1)
	{
		m_TclFilesListBox.GetText(loc, tempUp);
		m_TclFilesListBox.GetText(loc + 1, tempDn);
		m_TclFilesListBox.DeleteString(loc + 1);
		m_TclFilesListBox.InsertString(loc, tempDn);
		GetDocument()->SetModifiedFlag(TRUE);
		m_TclFilesListBox.SetCurSel(loc + 1);
	}

}
*/

void CFormViewShellView::OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	int loc;
	CString tempUp;
	CString tempDn;
	int checkUp;
	int checkDn;

	if(pNMUpDown->iDelta < 0)
	{


		loc = m_TclFilesListBox.GetCurSel();
		UpdateData(TRUE);
		if(loc > 0)
		{
			checkUp = m_TclFilesListBox.GetCheck(loc - 1);
			checkDn = m_TclFilesListBox.GetCheck(loc);
			m_TclFilesListBox.GetText(loc - 1, tempUp);
			m_TclFilesListBox.GetText(loc, tempDn);

			m_TclFilesListBox.DeleteString(loc - 1);
			m_TclFilesListBox.InsertString(loc, tempUp);
			m_TclFilesListBox.SetCheck(loc - 1, checkDn);
			m_TclFilesListBox.SetCheck(loc, checkUp);

			GetDocument()->SetModifiedFlag(TRUE);
			m_TclFilesListBox.SetCurSel(loc - 1);
		}
	}
	else if(pNMUpDown->iDelta > 0)
	{
		loc = m_TclFilesListBox.GetCurSel();
		UpdateData(TRUE);
		if(loc >= 0 && loc < m_TclFilesListBox.GetCount() - 1)
		{
			checkUp = m_TclFilesListBox.GetCheck(loc);
			checkDn = m_TclFilesListBox.GetCheck(loc + 1);
			m_TclFilesListBox.GetText(loc, tempUp);
			m_TclFilesListBox.GetText(loc + 1, tempDn);
	
			m_TclFilesListBox.DeleteString(loc + 1);
			m_TclFilesListBox.InsertString(loc, tempDn);
			m_TclFilesListBox.SetCheck(loc, checkDn);
			m_TclFilesListBox.SetCheck(loc + 1, checkUp);

			GetDocument()->SetModifiedFlag(TRUE);
			m_TclFilesListBox.SetCurSel(loc + 1);
		}
	}
	*pResult = 0;
}
void CFormViewShellView::ShellCommon(CString excutteFile, CString CMDLine, int isShow)
{
	DWORD dwExitCode;
	SHELLEXECUTEINFO si;

	ZeroMemory(&si, sizeof(SHELLEXECUTEINFO));
	//PathIP.Format("interface ip set address name=\"%s\" source=dhcp", AdapterName);

	si.cbSize = (sizeof(SHELLEXECUTEINFO));
	si.lpVerb = __TEXT("open");                 //mode : print, write
	si.lpFile = __TEXT(excutteFile);           //실행파일
	si.lpParameters = CMDLine;                    //실행파일 인자값.
	// shellIP.lpDirectory =                        //실핼파일 디렉토리.
	si.nShow = isShow;                    //SW_HIDE
	si.fMask = SEE_MASK_NOCLOSEPROCESS|SEE_MASK_FLAG_DDEWAIT;

	BOOL isSuccess = ShellExecuteEx(&si);             //성공시 true

	if(!isSuccess)
	{
		DWORD err = GetLastError() ;
		switch(err)
		{
			case ERROR_FILE_NOT_FOUND:
			{

				MessageBox("지정된 파일을 찾을수 없다.");
			}
			break;
			case ERROR_DDE_FAIL:
			{

				MessageBox("동적 데이터 교환(DDE) 트랜잭션(transaction)에 실패했습니다.");
			}
			break;
			case ERROR_NO_ASSOCIATION:
			{

				MessageBox("주어진 파일의 확장명과 연결되는 응용 프로그램이 없다.");
			}
			break;
			case ERROR_ACCESS_DENIED:
			{

				MessageBox("지정된 파일에 대한 액세스가 거부 되었다.");
			}
			break;
			case ERROR_DLL_NOT_FOUND:
			{

				MessageBox("응용 프로그램을 실행하는 데 필요한 하나의 라이브러리(library)  파일을 찾을 수없습니다.");
			}
			break;
			case ERROR_CANCELLED:
			{

				MessageBox("함수가 사용자를 위한 추가정보를 알려주었지만 사용자가 요청을 취소 했다.");
			}
			break;
			case ERROR_NOT_ENOUGH_MEMORY:
			{

				MessageBox("그곳에 지정된 작업을 수행하기 위한 충분한 메모리가 없다.");
			}
			break;
			case ERROR_SHARING_VIOLATION:
			{

				MessageBox("공유 위반이 발생했다.");
			}
			break;
		}
	}

	// 프로세스가 완료될때까지 기다림
	while(1)
	{
		::GetExitCodeProcess( si.hProcess, &dwExitCode );

		if( dwExitCode != STILL_ACTIVE )
		{
			//MessageBox("종료");
			break;
		}
	}

}
void CFormViewShellView::OnLbnSelchangeListTclFiles()
{
	// TODO: Add your control notification handler code here
	
	int lbTclFilesCnt = m_TclFilesListBox.GetCount();
	int lbIdx = m_TclFilesListBox.GetCurSel();

	CString fPath;
	if(lbIdx >= 0  && lbIdx <lbTclFilesCnt)
	{
		m_TclFilesListBox.GetText(lbIdx, fPath);
		// 고정된 폴더에서만 tcl 로드
		int i = fPath.ReverseFind('\\');//실행 파일 이름을 지우기 위해서 왼쪽에 있는 '/'를 찾는다.
		fPath = fPath.Right(fPath.GetLength() - i);
		fPath = GetDocument()->m_tclFilePath + fPath;
	}
	else
	{
		return;
	}
	if(!fPath.IsEmpty())
	{
		m_bMultiMode = TRUE;
		GetDocument()->OnOpenDocument(fPath);
	//	GetDocument()->SetPathName(fPath,0);
		m_bMultiMode = FALSE;
	}
	//m_toolTip.AddTool(GetDlgItem(IDC_EDIT1), m_AllCmdLnText);
}

//void CFormViewShellView::OnBnClickedButton3()
//{
//	// TODO: Add your control notification handler code here
//	int loc;
//	loc = m_TclFilesListBox.GetCurSel();
//	m_TclFilesListBox.DeleteString(loc);
//	if(loc == m_TclFilesListBox.GetCount())
//	{
//		loc = loc - 1;
//		GetDocument()->SetModifiedFlag(TRUE);
//	}
//	m_TclFilesListBox.SetCurSel(loc);
//}

void CFormViewShellView::OnBnClickedDelMtclList()
{
	int loc;
	loc = m_TclFilesListBox.GetCurSel();
	m_TclFilesListBox.DeleteString(loc);
	if(loc == m_TclFilesListBox.GetCount())
	{
		loc = loc - 1;
		GetDocument()->SetModifiedFlag(TRUE);
	}
	m_TclFilesListBox.SetCurSel(loc);

}

void CFormViewShellView::OnBnClickedButtonPre()
{
	PreviewDlg prevDlg;
	prevDlg.m_EditText = m_ExcuteFilePath;
	prevDlg.DoModal();
}

void CFormViewShellView::OnBnClickedButtonPrelist()
{
	PreviewDlg prevDlg;
	prevDlg.m_EditText = m_AllCmdLnText;
	prevDlg.m_bIsModiFy = TRUE;
	prevDlg.m_TitleText = "명령줄";
	
	prevDlg.DoModal();

	CSize sizeTotal;   
	sizeTotal.cx = 0;   
	sizeTotal.cy = 0;   
	SetScrollSizes(MM_TEXT, sizeTotal);   
	SetScrollRange(SB_VERT,0,sizeTotal.cy);   
	SetScrollPos(SB_VERT,0);   
	  
	SetScrollRange(SB_HORZ,0,sizeTotal.cx);   
	SetScrollPos(SB_HORZ,0);

}

// 미리보기
void CFormViewShellView::OnBnClickedButtonPreview()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int lbTclFilesCnt = m_TclFilesListBox.GetCount();
	int forceDelay = 0;
	// 결과 텍스트 초기화
	m_ExcuteFilePath = "";
	m_bIsPreview = TRUE;
	OnBnClickedExecappl();

	//m_OperPlus = (COperPlus*)AfxBeginThread(RUNTIME_CLASS(COperPlus),
	//THREAD_PRIORITY_NORMAL,
	//0, CREATE_SUSPENDED);
	//m_OperPlus->SetDialogPointer(this);
	//m_OperPlus->ResumeThread();

	m_bIsPreview = FALSE;
}


