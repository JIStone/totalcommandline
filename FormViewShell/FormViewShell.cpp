// FormViewShell.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "FormViewShell.h"
#include "MainFrm.h"

#include "FormViewShellDoc.h"
#include "FormViewShellView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFormViewShellApp

BEGIN_MESSAGE_MAP(CFormViewShellApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CFormViewShellApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN, &OnFileOpen)
END_MESSAGE_MAP()


// CFormViewShellApp 생성

CFormViewShellApp::CFormViewShellApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CFormViewShellApp 개체입니다.

CFormViewShellApp theApp;


// CFormViewShellApp 초기화

BOOL CFormViewShellApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	TCHAR *a0 = NULL;
	TCHAR *a1 = NULL;
	TCHAR *a2 = NULL;

	// commandline arguments 수정
	if (__targv) {
		if(__targv[0])
			a0 = __targv[0];
		if (__targv[1])
			a1 = __targv[1];
		if (__targv[2])
			a2 = __targv[2];

		//AfxMessageBox(CString(a0));
		//AfxMessageBox(CString(a1));
		//AfxMessageBox(CString(a2));
	}
		 
//char *a0 = __argv[0];
//char *a1 = __argv[1];
//char *a2 = __argv[2];


	CWinApp::InitInstance();

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.
	// 응용 프로그램의 문서 템플릿을 등록합니다. 문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CFormViewShellDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CFormViewShellView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	
	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
	{
		return FALSE;
	}
		

	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	//  SDI 응용 프로그램에서는 ProcessShellCommand 후에 이러한 호출이 발생해야 합니다.
	
	// 설정파일 로드
	CFile fIni;
	CFileException e;
	CString fPath;
	CString filePath;
	CString	argumentExcute;

	BOOL isMultiMode = false;
	BOOL isExcute = false;



	if(a1)
	{
		fPath = a1;
		argumentExcute = a2;
		fIni.Open(_T(".\\totalcvtr.cur"), CFile::modeRead);
		filePath = fIni.GetFilePath();
		//AfxMessageBox("테스트.");
		fIni.Close();

		isMultiMode = !(fPath.Right(5)).Compare(_T(".mtcl"));
		isExcute = !(argumentExcute).Compare(_T("-go"));
	}
	else if(!fIni.Open(_T(".\\totalcvtr.cur"), CFile::modeRead))
	{
		filePath = fIni.GetFilePath();
		TRACE(_T("File could not be opened %d\n"), e.m_cause);

	}
	else
	{
		BYTE buffer[1024] = {0,};
		int fileLen = (int)fIni.GetLength();
		fIni.Read(buffer, fileLen);
		
		CString bufstr((TCHAR*)buffer, fileLen);
		//memcpy(&fPath, buffer, fileLen);
		fPath = bufstr;
		
		filePath = fIni.GetFilePath();
		fIni.Close();
	}
	
	CFrameWnd * pWnd = (CFrameWnd*)AfxGetMainWnd();
	CFormViewShellView* pView = (CFormViewShellView*)pWnd->GetActiveView();
	CFormViewShellDoc* mydoc = pView->GetDocument();
	// 상대경로를 위해
	TCHAR  Path[1024];
	GetModuleFileName(NULL, Path, 1024);

	//PathRemoveFileSpec(Path);
	CString currPath = Path;
	int i = currPath.ReverseFind('\\');//실행 파일 이름을 지우기 위해서 왼쪽에 있는 '/'를 찾는다.
	currPath = currPath.Left(i);//뒤에 있는 현재 실행 파일 이름을 지운다.
	// tcl전용폴더(실행파일폴더\tcl)
	mydoc->m_tclFilePath = currPath + _T("\\tcl");

	i = fPath.ReverseFind('\\');//실행 파일 이름을 지우기 위해서 왼쪽에 있는 '/'를 찾는다.
	// 전용폴더\파일.확장자 로 저장
	fPath = mydoc->m_tclFilePath + fPath.Right(fPath.GetLength() - i);//뒤에 있는 현재 실행 파일 이름을 지운다.
	//설정파일 경로저장
	mydoc->m_IniFilePath = filePath;
	
	if(!fPath.IsEmpty())
	{
		mydoc->OnOpenDocument(fPath);
		mydoc->SetPathName(fPath,0);
		
	}
	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();	
	// 현재경로 고정
	::SetCurrentDirectory(mydoc->m_tclFilePath);
	if(isExcute){

		if(!isMultiMode)
			pView->OnBnClickedExecappl();
		else
			pView->OnBnSeqExcuteButton();
	}

/*	
	// 컨버터 셋팅파일 로드
	CFile myFile;

	if(!myFile.Open(fPath, CFile::modeRead))
	{
		TRACE(_T("File could not be opened %d\n"), e.m_cause);
	}
	else
	{
		CArchive ar(&myFile, CArchive::load);
		ar.m_pDocument = mydoc;

		// Serialize the document to the archive.
		if (ar.m_pDocument != NULL)
		{
		   ar.m_pDocument->Serialize(ar);
		   ar.Close();

		}
		myFile.Close();
	}
*/


	return TRUE;
}



// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedVersion();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_STN_CLICKED(IDC_VERSION, &CAboutDlg::OnStnClickedVersion)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CFormViewShellApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CFormViewShellApp 메시지 처리기


void CFormViewShellApp::OnFileOpen(void)
{
	//CWinApp::OnFileOpen();
	TCHAR szFileNameList[1024] = {0,};
	// 
	CFileDialog fileDlg( TRUE, NULL, NULL, NULL, _T("*.stcl|*.stcl|*.mtcl|*.mtcl|*.tcl|*.tcl|*.*|*.*||"));

	//fileDlg.m_ofn.lpstrFile   = szFileNameList;
	//fileDlg.m_ofn.nMaxFile   = sizeof( szFileNameList ) - 1;
	fileDlg.m_ofn.lpstrTitle  = _T("열기");
	//fileDlg.m_ofn.lpstrInitialDir = "C:\\";

	if( fileDlg.DoModal() == IDOK )
	{
		POSITION pos = fileDlg.GetStartPosition( );

		while( pos )
		{
			CString strName = fileDlg.GetNextPathName( pos );
			//printf("name : %s\n", strName );
			//Serialize() 함수를 사용 하려면, 
			AfxGetApp()->OpenDocumentFile(strName);

		}
	} 
	/*
	// 프레임 클레스에서 대응
	// 타이틀바 문자열 수정
	CFrameWnd * pWnd = (CFrameWnd*)AfxGetMainWnd();
	CFormViewShellView* pView = (CFormViewShellView*)pWnd->GetActiveView();
	CFormViewShellDoc* mydoc = pView->GetDocument();	
	SetWindowText(*pWnd ,mydoc->m_TiltleString);
	*/
}


void CAboutDlg::OnStnClickedVersion()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
