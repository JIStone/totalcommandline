// FormViewShell.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN, &OnFileOpen)
END_MESSAGE_MAP()


// CFormViewShellApp ����

CFormViewShellApp::CFormViewShellApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CFormViewShellApp ��ü�Դϴ�.

CFormViewShellApp theApp;


// CFormViewShellApp �ʱ�ȭ

BOOL CFormViewShellApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	TCHAR *a0 = NULL;
	TCHAR *a1 = NULL;
	TCHAR *a2 = NULL;

	// commandline arguments ����
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

	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.
	// ���� ���α׷��� ���� ���ø��� ����մϴ�. ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CFormViewShellDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		RUNTIME_CLASS(CFormViewShellView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	
	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
	{
		return FALSE;
	}
		

	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	//  SDI ���� ���α׷������� ProcessShellCommand �Ŀ� �̷��� ȣ���� �߻��ؾ� �մϴ�.
	
	// �������� �ε�
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
		//AfxMessageBox("�׽�Ʈ.");
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
	// ����θ� ����
	TCHAR  Path[1024];
	GetModuleFileName(NULL, Path, 1024);

	//PathRemoveFileSpec(Path);
	CString currPath = Path;
	int i = currPath.ReverseFind('\\');//���� ���� �̸��� ����� ���ؼ� ���ʿ� �ִ� '/'�� ã�´�.
	currPath = currPath.Left(i);//�ڿ� �ִ� ���� ���� ���� �̸��� �����.
	// tcl��������(������������\tcl)
	mydoc->m_tclFilePath = currPath + _T("\\tcl");

	i = fPath.ReverseFind('\\');//���� ���� �̸��� ����� ���ؼ� ���ʿ� �ִ� '/'�� ã�´�.
	// ��������\����.Ȯ���� �� ����
	fPath = mydoc->m_tclFilePath + fPath.Right(fPath.GetLength() - i);//�ڿ� �ִ� ���� ���� ���� �̸��� �����.
	//�������� �������
	mydoc->m_IniFilePath = filePath;
	
	if(!fPath.IsEmpty())
	{
		mydoc->OnOpenDocument(fPath);
		mydoc->SetPathName(fPath,0);
		
	}
	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();	
	// ������ ����
	::SetCurrentDirectory(mydoc->m_tclFilePath);
	if(isExcute){

		if(!isMultiMode)
			pView->OnBnClickedExecappl();
		else
			pView->OnBnSeqExcuteButton();
	}

/*	
	// ������ �������� �ε�
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



// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CFormViewShellApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CFormViewShellApp �޽��� ó����


void CFormViewShellApp::OnFileOpen(void)
{
	//CWinApp::OnFileOpen();
	TCHAR szFileNameList[1024] = {0,};
	// 
	CFileDialog fileDlg( TRUE, NULL, NULL, NULL, _T("*.stcl|*.stcl|*.mtcl|*.mtcl|*.tcl|*.tcl|*.*|*.*||"));

	//fileDlg.m_ofn.lpstrFile   = szFileNameList;
	//fileDlg.m_ofn.nMaxFile   = sizeof( szFileNameList ) - 1;
	fileDlg.m_ofn.lpstrTitle  = _T("����");
	//fileDlg.m_ofn.lpstrInitialDir = "C:\\";

	if( fileDlg.DoModal() == IDOK )
	{
		POSITION pos = fileDlg.GetStartPosition( );

		while( pos )
		{
			CString strName = fileDlg.GetNextPathName( pos );
			//printf("name : %s\n", strName );
			//Serialize() �Լ��� ��� �Ϸ���, 
			AfxGetApp()->OpenDocumentFile(strName);

		}
	} 
	/*
	// ������ Ŭ�������� ����
	// Ÿ��Ʋ�� ���ڿ� ����
	CFrameWnd * pWnd = (CFrameWnd*)AfxGetMainWnd();
	CFormViewShellView* pView = (CFormViewShellView*)pWnd->GetActiveView();
	CFormViewShellDoc* mydoc = pView->GetDocument();	
	SetWindowText(*pWnd ,mydoc->m_TiltleString);
	*/
}


void CAboutDlg::OnStnClickedVersion()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
