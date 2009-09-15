// FormViewShellView.cpp : CFormViewShellView Ŭ������ ����
//

#include "stdafx.h"
#include "FormViewShell.h"

#include "FormViewShellDoc.h"
#include "FormViewShellView.h"
#include <shlwapi.h>

#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Ȯ������ �ִ���̴� 6���� ����				
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
ON_EN_CHANGE(IDC_EDIT6, &CFormViewShellView::OnEnChangeEdit6)
ON_EN_CHANGE(IDC_EDIT7, &CFormViewShellView::OnEnChangeEdit7)
ON_EN_CHANGE(IDC_EDIT8, &CFormViewShellView::OnEnChangeEdit8)
ON_LBN_SELCHANGE(IDC_LIST_OUT, &CFormViewShellView::OnLbnSelchangeListOut)
ON_EN_CHANGE(IDC_EDIT_OUT, &CFormViewShellView::OnEnChangeEditOut)
ON_BN_CLICKED(IDC_BUTTON_ADD_FOLDER, &CFormViewShellView::OnBnClickedButtonAddFolder)
ON_BN_CLICKED(IDC_BUTTON_DEL_FOLDER, &CFormViewShellView::OnBnClickedButtonDelFolder)
ON_WM_DROPFILES()
ON_BN_CLICKED(IDC_BUTTON2, &CFormViewShellView::OnBnClickedButton2)
// IDC_SPIN2�� ��ü
//ON_BN_CLICKED(IDC_BUTTON_UP, &CFormViewShellView::OnBnClickedButtonUp)
//ON_BN_CLICKED(IDC_BUTTON_DN, &CFormViewShellView::OnBnClickedButtonDn)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CFormViewShellView::OnDeltaposSpin2)
ON_LBN_SELCHANGE(IDC_LIST_TCL_FILES, &CFormViewShellView::OnLbnSelchangeListTclFiles)
//ON_BN_CLICKED(IDC_BUTTON3, &CFormViewShellView::OnBnClickedButton3)
ON_BN_CLICKED(IDC_DEL_MTCL_LIST, &CFormViewShellView::OnBnClickedDelMtclList)
END_MESSAGE_MAP()

// CFormViewShellView ����/�Ҹ�

extern "C" __declspec(dllimport) char* GetDateTime(BOOL& isTimeOver);

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
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

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

	DDX_Control(pDX, IDC_LIST3, m_ExFolderListBox);
	DDX_Text(pDX, IDC_EDIT_EX_FOLDER, m_ExFolderName);
	DDX_Control(pDX, IDC_LIST_TCL_FILES, m_TclFilesListBox);
	DDX_Control(pDX, IDC_BUTTON2, m_BtnMultiTCLExcute);
	DDX_Control(pDX, IDC_EXECAPPL, m_BtnExcute);
	DDX_Control(pDX, IDC_VIEWLIST, m_ViewList);
}

BOOL CFormViewShellView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	


	return CFormView::PreCreateWindow(cs);
}

void CFormViewShellView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	
	if(!m_IsToolTipInit)
	{
		// ���� ����
		m_toolTip.Create(this);
		m_toolTip.Activate(TRUE);
		m_toolTip.AddTool(GetDlgItem(IDC_EDIT4), "ex) *.txt, mainBG.nsc");
		m_toolTip.AddTool(GetDlgItem(IDC_EDIT_OUT), "ex) .NCGR,.NCER ... ");
		m_toolTip.AddTool(GetDlgItem(IDC_EDIT7), "����ó���� ����Ȯ���� ex) .nlz");
		m_toolTip.SetMaxTipWidth(500);
		m_IsToolTipInit = true;
	}

}


// CFormViewShellView ����

#ifdef _DEBUG
void CFormViewShellView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormViewShellView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFormViewShellDoc* CFormViewShellView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFormViewShellDoc)));
	return (CFormViewShellDoc*)m_pDocument;
}
#endif //_DEBUG


// CFormViewShellView �޽��� ó����

void CFormViewShellView::OnBnClickedSelfile2()
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
	BRinfo.lpszTitle = "������ �����ϼ���.";
	BRinfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_STATUSTEXT | BIF_UAHINT | BIF_RETURNFSANCESTORS | BIF_BROWSEINCLUDEFILES;
	// m_ExecFilePath�� ������ ��θ� ã�ư�
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
//	BRinfo.lpszTitle = "������ �����ϼ���.";
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
	BRinfo.lpszTitle = "������ �����ϼ���.";
	BRinfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_STATUSTEXT | BIF_UAHINT;
	// m_DestPath�� ������ ��θ� ã�ư�
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



void CFormViewShellView::OnBnClickedExecappl()
{
	// TODO: Add your control notification handler code here
	//GetDlgItemText(IDC_SRC_FILE, m_FullFileName);
	
	#define FILEEXISTCHECKCNT (2)
	
	// ���Ⱓ �ʰ� �˻�
	BOOL isTimeOver = FALSE;
	char* t = GetDateTime(isTimeOver);
	if(isTimeOver)
	{
		AfxMessageBox("�Ⱓ�ʰ�");
		return;
	}
	//�������� ����(�ڵ������ make��
	CFile myFile;
	CFileException e;
	//AfxMessageBox(m_IniFilePath);
	if(!myFile.Open(GetDocument()->m_IniFilePath + ".txt", CFile::modeCreate | CFile::modeWrite, &e))
	{
		TRACE(_T("File could not be opened %s : %d\n"), GetDocument()->m_IniFilePath + ".txt", e.m_cause);
	}
	CString refreshStr = m_SettingFilePath + "\r\n" + t;
	myFile.Write(refreshStr, refreshStr.GetLength());
	myFile.Close();

	BOOL bUseShellExCute = TRUE;
	BOOL isInternalCmd = FALSE;

	GetDlgItemText(IDC_SRC_FILE, m_FullFileName);
	GetDlgItemText(IDC_EXEC_FILE, m_ExecFilePath);
	GetDlgItemText(IDC_PATH, m_DestPath);
	

	if(!(m_ExecFilePath).Compare("command.com") || !(m_ExecFilePath).Compare("cmd.exe"))
	{
		isInternalCmd = TRUE;
	}

	if(m_FullFileName.IsEmpty() && !isInternalCmd) 
	{ 
		AfxMessageBox("��ȯ�� ������ �����ϼ���.");

		OnBnClickedSelfile();
		if(m_FullFileName.IsEmpty())
		{
			return;
		}
	}
	else if(m_ExecFilePath.IsEmpty())// && (fPath.Right(5)).Compare(".mtcl")) 
	{ 
		AfxMessageBox("���������� �����ϼ���.");

		OnBnClickedSelfile();
		if(m_ExecFilePath.IsEmpty())
		{
			return;
		}
	}
	// ���� ���� ���� �ɼ� �Ķ����
	CString execFirstArg;
	GetDlgItemText(IDC_EDIT6, execFirstArg);

	CString execArg;
	GetDlgItemText(IDC_EDIT3, execArg);

	CString midPath;
	GetDlgItemText(IDC_EDIT4, midPath);

	GetDlgItemText(IDC_PATH, m_DestPath);

	int fileCheckCnt = 0;
	fileCheckCnt = GetDlgItemInt(IDC_EDIT8);
	

	CString searchRootDirStr = m_FullFileName;
	std::vector<CString> vecSearchSubDirList;
	int iSuccessCnt = 0;
	// ���� ���� ���� �ʱ�ȭ
	m_SubDirCnt = 0;
	// �˻��� ���� �ʱ�ȭ
	m_SearchedFileCnt = 0;
	
	if(!isInternalCmd)
		SearchDir(m_FullFileName, vecSearchSubDirList);

	CString searchFileStr = midPath + "\0";

	std::vector<CString> vecFileList;

	m_ProgressCtrl.SetRange(0, 100);
	m_ProgressCtrl.SetPos(0);
	m_ExcuteFilePath = "";
	
	int progressCnt = 0;
	// ��ȯ�� ������ ������ ����. 
	for(int dirCnt = 0;dirCnt < m_SubDirCnt; dirCnt++) 
	{	
		int noUse = -1;
		progressCnt += SearchFile(searchRootDirStr + vecSearchSubDirList[dirCnt], searchFileStr, vecFileList, noUse);
	}
	
	m_EditProgCnt.ShowWindow(SW_SHOW);
	
	
	// ���� ��ɾ� ����
	if(isInternalCmd)
	{
		CString CMDLine = execFirstArg + "\"" + searchRootDirStr + "" + searchFileStr +  "\"" + " "+ "\"" + m_DestPath + execArg + "\"";

		ShellCommon(m_ExecFilePath, CMDLine, SW_SHOW);

		//fMask�� SEE_MASK_NOCLOSEPROCESS�� �����Ͽ��ٸ�
		//hProcesss ����� ���Ͽ� ���λ��� ���μ����� �ڵ��� ��ȯ ���� �� �ִ�.
		//m_SearchedFileCnt = m_SearchedFileCnt > 0 ? 1 : 0;
		progressCnt= 1;// �и� 0�̵Ǵ°� ����
		isInternalCmd = TRUE && m_SearchedFileCnt;

		//HWND hwnd = GetDlgItem(IDC_EDIT9)->m_hWnd;
		//CString CMDLine = execFirstArg + "\"" + searchRootDirStr + "\\" + searchFileStr +  "\"" + " "+ "\"" + m_DestPath + execArg + "\"";
		//ShellExecute(hwnd, "open", m_ExecFilePath, CMDLine, NULL, SW_SHOW);
		return;
	}

	for(int dirCnt = 0; dirCnt < m_SubDirCnt; dirCnt++) 
	{
		int starIndex = m_SearchedFileCnt;
		//������� �˻�
		SearchFile(searchRootDirStr + vecSearchSubDirList[dirCnt], searchFileStr, vecFileList, m_SearchedFileCnt);
		
		if(isInternalCmd)
			m_SearchedFileCnt = 1;

		for(int listCnt = starIndex; listCnt < m_SearchedFileCnt || isInternalCmd; listCnt++)
		{
			CString prgCnt;
			// ���Լ� ��ȯ��
			HINSTANCE ret = 0;
			
			m_ProgressCtrl.SetPos(((listCnt + 1) * 100) / progressCnt);
			prgCnt.Format( "[ %d / %d ]", listCnt+1, progressCnt);
			
			if(listCnt+1 == progressCnt)
				prgCnt += " Complet!!!";

			SetDlgItemText(IDC_EDIT2, prgCnt);
			
			GetDlgItem(IDC_EDIT2)->UpdateWindow();
			//UpdateWindow();
			
			CString testAllPath;// = execFirstArg + "\"" + vecFileList[listCnt] + m_PreCmdOptStr + m_DestPath + vecSearchSubDirList[dirCnt] +"\"" + execArg;
			INT nShowCmd = SW_HIDE;	
			
			if(m_DestPath.IsEmpty() || m_PreCmdOptStr.IsEmpty())
			{
				testAllPath = execFirstArg + "\"" + vecFileList[listCnt] + "\"" +  execArg;
			}
			else
			{
				testAllPath = execFirstArg + "\"" + vecFileList[listCnt] + m_PreCmdOptStr + m_DestPath + vecSearchSubDirList[dirCnt] +"\"" + execArg;
			}

			CString oExtStrTop;
			if(m_ListBox.GetCount())
				m_ListBox.GetText(0, oExtStrTop);
			


			if(0 <= oExtStrTop.Find('*',0))
			{
				// ���� ó���۾��� �Ѵ�.
				bUseShellExCute = FALSE;
				
				if(m_DestPath.IsEmpty())
					m_DestPath = m_FullFileName;
			}
			else
			{

				ShellCommon(m_ExecFilePath, testAllPath, SW_HIDE);

				//========g2dcvtr�� �ѱ������� ��������� ���Ҽ� ����?==========
				//ret = ShellExecute(NULL, "open", m_ExecFilePath, testAllPath, NULL, nShowCmd);
			}
			
			for(int lbIndex = 0; lbIndex < m_ListBox.GetCount(); lbIndex++)
			{
				// ����ó�� Ÿ�� ����
				int iExtType = 0;
			
				int sIndex = 0;
				CString oExtStr;
				//�������� ������ �����н�
				CString outPutResultPath = "";
				// �Է� ������ Ȯ����
				CString iExtStr = "";//searchFileStr;
				// Ȯ���ڸ� ����
				sIndex = testAllPath.Find('.', 0);
				iExtStr = testAllPath.Right(testAllPath.GetLength() - sIndex);
				iExtStr.Replace("\"", "");
				
				m_ListBox.GetText(lbIndex, oExtStr);

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
				
				//Ȯ���ڿ��� ����ó���� ���� ��Ʈ���� ����
				sIndex = oExtStr.Find('.', 0);
				oExtStr = oExtStr.Right(oExtStr.GetLength() - sIndex);


				//�������� ������ �����н� �����
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
				// ���� �̸��� �и��ϱ�
				//sIndex = tempOutPutStr.Find('.',tempOutPutStr.GetLength() - FILE_EXT_LENGTH_MAX);
				sIndex = tempOutPutStr.Find('.',0);
				CString sExte = tempOutPutStr.Right(tempOutPutStr.GetLength() - sIndex);
				// ��������̸��� Ȯ���� ���̱�
				tempOutPutStr.Replace(sExte, oExtStr);

				//tempOutPutStr.Replace(iExtStr, oExtStr);
				tempOutPutStr.Replace(searchRootDirStr, "");
				//CString movSrc = searchRootDirStr + tempOutPutStr;
				tempOutPutStr = m_DestPath + tempOutPutStr;
				// �̸� ���� �ٲٰ� �̵��ϱ�////////
				//CString movSrc = searchRootDirStr + vecSearchSubDirList[dirCnt] + "\\*" + oExtStr;//SE_ERR_ACCESSDENIED
				CString movDest = tempOutPutStr;
				//������ ���� ã������ ���� �н�
				CString movingStr = "";
				int findCnt = 0;
				
				BOOL bAtiveFileProc = TRUE;
				// �����õ� ������ ���� ������ �ִ� ������ ���� ���� ��� ��� ã��
				while(movingStr.IsEmpty())
				{	
					if(iExtType == FO_COPY)
					{
						SearchOneFile(NULL, outPutResultPath, &movingStr);
					}
					else if(iExtType == FO_DELETE)
					{
						// ������ ������ ���
						movingStr = tempOutPutStr;
						bAtiveFileProc = PathFileExists(tempOutPutStr);
						break;
					}
					else
					{
						CString extModify;
						GetDlgItemText(IDC_EDIT7, extModify);
						// �����̵�ó���� Ư��Ȯ���ڷ� ���� 
						if(!extModify.IsEmpty())
							movDest.Replace(oExtStr, iExtStr + extModify);
							//movDest += extModify;
						// �����̵� �ùٸ��� �Ϸ��� �ϳ��� ���ϸ� ���õǵ��� �ؾ���
						if(bUseShellExCute)
							SearchOneFile(searchRootDirStr + vecSearchSubDirList[dirCnt], oExtStr, &movingStr);
						else
							movingStr = outPutResultPath;
					}
					//Sleep(10);
					m_ExcuteFilePath +=  ".\r\n";
					findCnt++;
					// �������� ����(����׶�� ���÷�����)
					// �ǹ̰� ���� �Ǿ���
					fileCheckCnt = 0;
					if(findCnt > fileCheckCnt)
						break;
				}	
				// CMD ���
/*				CString testSTRING ="/c move " + movSrc + " " + movDest;
				HINSTANCE movRet = ShellExecute(NULL, "open", "cmd", testSTRING, NULL, SW_HIDE);
*/

//========================================= ��� ����ó��====================================================
				if(bAtiveFileProc)
				{

					if(m_ViewList.GetCheck() && !movingStr.IsEmpty())
					{
						//����ó�� ����Ʈ ����ϱ�
						CString fileOp[3]= {" �̵�", " ����", " ����"};
						if(iExtType == FO_DELETE)
						{
							m_ExcuteFilePath +=	movingStr + "��" + fileOp[iExtType - 1] +"\r\n";
						}
						else
						{
							m_ExcuteFilePath +=	movingStr + "��"+ "  \r\n"  + movDest +"��" + fileOp[iExtType - 1] + "\r\n";
						}

						SetDlgItemText(IDC_EDIT9, m_ExcuteFilePath);
						GetDlgItem(IDC_EDIT9)->UpdateWindow();
					}

					SHFILEOPSTRUCT shos;
					ZeroMemory(&shos, sizeof(SHFILEOPSTRUCT));
					// ����ó�� Ÿ�� ����
					shos.wFunc = iExtType;

					//movingStr = "\"" + movingStr + "\"";
					movingStr.Append("\0", 1);
					movDest.Append("\0", 1);
					shos.pFrom = NULL;
					shos.pTo = NULL;
					// �̷������� �־������!!!
					TCHAR        pszFrom[1024] = {0}; 
					strcpy_s(pszFrom , movingStr);
					shos.pFrom = pszFrom;//"D:\\WORK\\Macheon_Trial\\MC\\Program\\NC\\MainUi\\2d\\MainUI_bottom.NCER";

					TCHAR        pszTo[1024] = {0};
					//movDest = "\"" + movDest + "\"";
					strcpy_s(pszTo , movDest);
				
					if(iExtType != FO_DELETE)
						shos.pTo = pszTo;

					// �۾����� ������ ǥ�þ���
					shos.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI;

					int shRet = 0;
					int tryCount = 0;
					// ������� ������ ū ���������� SHFileOperation()�� ����� ��ġ�� �������� �ɶ������Ѵ�.
					do
					{
//===================================������ó��===========================================================
						shRet = SHFileOperation(&shos);
						//Sleep(10);
						if(tryCount > 0)
							m_ExcuteFilePath +=  "����ó�� ��";
						
						tryCount++;
					}
					while(tryCount < 2 && shRet != 0);

					//ó���ؾ��� ������ ���� �����޼����� ǥ���� �����̸�
					if(shRet && m_Check_EnableErrPop.GetCheck())
					{
						int popRet = MessageBox(movDest + " \n�۾��� ��ġ�� ���߽��ϴ�. ��� �Ͻðڽ��ϱ�?","�����̵� ����", MB_OKCANCEL);
						
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
					isDestFileExist = PathFileExists(movingStr);// ������ ������ ��ο� �ִ��� �˻�
				}

				int findMovedFileCnt = 0;
				while((iExtType == FO_DELETE && isDestFileExist) || (iExtType != FO_DELETE && !isDestFileExist))
				{
					isDestFileExist = PathFileExists(movDest);
					//Sleep(10);
					findMovedFileCnt++;
					
					if(findMovedFileCnt > FILEEXISTCHECKCNT)
						break;
				}
				// �����޼��� ǥ�ÿ���
				if(m_Check_EnableErrPop.GetCheck() && ((iExtType == FO_DELETE && isDestFileExist) || (iExtType != FO_DELETE && !isDestFileExist)))
				{
					int popRet = MessageBox(tempOutPutStr + " \n������ �۾��� ��ġ�� ���߽��ϴ�. \n��� �Ͻðڽ��ϱ�?","���� ����", MB_OKCANCEL);
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
			
			if(bUseShellExCute)
			{
				switch((int)ret)
				{
					case SE_ERR_NOASSOC:
					{
						MessageBox("������ ���� Ȯ���ڷ� ������ �� �ִ� ���α׷��� �����ϴ�.","���� Ȯ���� ����", MB_OKCANCEL);
					}
					break;
					case SE_ERR_FNF:
					{
						MessageBox("������ ������ ã���� �����ϴ�.","���� ����", MB_OKCANCEL);
					}
					break;
				}
			}			
		}
	}
	
	// ������ �����
	for(int dirCnt = m_SubDirCnt - 1; dirCnt >= 1; dirCnt--) // dirCnt >= 1 : �ֻ��� ������ ������������
	{
		//***������ �����̳� ������ ������ �ȵ�***
		WCHAR       wstring[1024];
		CString		delFolderPath;
		
		delFolderPath = m_FullFileName + vecSearchSubDirList[dirCnt];
		ZeroMemory(wstring, sizeof(wstring));
		int nLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, (LPCSTR)delFolderPath , delFolderPath.GetLength(), wstring, 1024 );

		_wrmdir(wstring);
		int tempErr = errno;//GetLastError();
		errno;
	}

	CString resultCnt;
	//resultCnt.Format( "����: %d", iSuccessCnt / m_ListBox.GetCount());
	resultCnt.Format( "����: %d", iSuccessCnt);
	m_Edit_Success_Fail.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_EDIT_SUCCESS_FAIL, resultCnt);
	GetDlgItem(IDC_EDIT_SUCCESS_FAIL)->UpdateWindow();
	//m_ProgressCtrl.SetPos(100);
	
}
int CFormViewShellView::SearchDir(CString sDirName, std::vector<CString> &sDirNameList)
{
	CFileFind dirFinder;
	BOOL bWorking = dirFinder.FindFile(sDirName + "\\*.*");
	int dCnt = 0;
	// �ֻ��� ���� �߰�
	if(!m_SubDirCnt)
	{
		//CString tempfolderPath = sDirName;
		//tempfolderPath.Replace(m_FullFileName, "");
		//sDirNameList[m_SubDirCnt++];// = tempfolderPath;
		m_SubDirCnt++;
		sDirNameList.push_back("");
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
			// �������� ������
			CString tempfolderPath = dirFinder.GetFilePath();
			CString exFolderName;

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
			// ���������� ����
			
			
			// Ÿ�� ������ �ִ°͸� ������ ��������
			// Ÿ�� ������ ���µ� �� ���������� �ִ� ��쵵 ������ �������� �����Ƿ� �ּ�ó��
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
			// ��ȯ�� ������ ������ ������Ŵ
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
			// ��ȯ�� ���ϐl ������ ������Ŵ
			fCnt++;
		}
	} 
	return fCnt;
}

// // Ŀ�ǵ� ��� ǥ��
void CFormViewShellView::DisplayCommand(BOOL modifyed)
{
	// ���� ���� �ٷ� ���� �ɼ�	
	CString execFirstArg;
	GetDlgItemText(IDC_EDIT6, execFirstArg);

	CString execSecondArg;
	GetDlgItemText(IDC_EDIT3, execSecondArg);

	GetDlgItemText(IDC_SRC_FILE, m_FullFileName);

	GetDlgItemText(IDC_EXEC_FILE, m_ExecFilePath);
	
	CString midPath;
	GetDlgItemText(IDC_EDIT4, midPath);


	GetDlgItemText(IDC_PATH, m_DestPath);
	
	GetDlgItemText(IDC_EDIT5, m_PreCmdOptStr);

	//CString testAllPath = /*m_ExecFileName */ m_ExecFilePath + " " + execFirstArg + m_FullFileName + " " + "[" + midPath +"]"+ m_PreCmdOptStr + " " + "[" + m_DestPath + "]" + execSecondArg;
	CString testAllPath = /*m_ExecFileName */ m_ExecFilePath + " " + execFirstArg + m_FullFileName + "\\" + midPath + m_PreCmdOptStr + " " + m_DestPath + execSecondArg;
	
	//testAllPath.Replace(" ", "��");

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
		//���õ� Ȯ���� �̵�
		case 0:
			m_ListData = "[M]" + m_ListData;
			break;
		//���õ� Ȯ���� ����
		case 1:
			m_ListData = "[C]" + m_ListData;
			break;
		//���õ� Ȯ���� ����
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
	BRinfo.lpszTitle = "������ �����ϼ���.";
	BRinfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_STATUSTEXT | BIF_UAHINT;
	// m_FullFileName�� ������ ��θ� ã�ư�
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
	//--xx �巡�׾� ����� ������ ���� ȹ��
	int iCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	int curCnt = m_TclFilesListBox.GetCount();
	
	//m_TclFilesListBox.ResetContent();

	for(int i=0; i<iCount; i++)
	{
		//--xx �巡�׾� ����� ������ ��������� �ϳ��� ȹ��
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
			GetDocument()->SetModifiedFlag(TRUE);
		}
	}

	curCnt = m_TclFilesListBox.GetCount();
	if(curCnt > 1)
	{
		CFrameWnd * pWnd = (CFrameWnd*)AfxGetMainWnd();
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

void CFormViewShellView::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	
	int lbTclFilesCnt = m_TclFilesListBox.GetCount();
	int forceDelay = 0;
	
	for(int lbIndex = 0; lbIndex < lbTclFilesCnt; lbIndex++)
	{
		CString fPath;	
		m_TclFilesListBox.GetText(lbIndex, fPath);
		
		if(!fPath.IsEmpty())
		{
			m_bMultiMode = TRUE;
			m_TclFilesListBox.SetCurSel(lbIndex);
			GetDocument()->OnOpenDocument(fPath);
			GetDocument()->SetPathName(fPath,0);
			UpdateWindow();
			OnBnClickedExecappl();
			forceDelay = GetDlgItemInt(IDC_EDIT_DELAY_TIME);
			if(lbIndex < lbTclFilesCnt - 1)
				Sleep(forceDelay * 10);
		}
	}
	// �ټ��� ���������� �ε� �߱⶧���� ó�� ���������� ��ε�		
	m_bMultiMode = FALSE;
	//GetDocument()->OnOpenDocument(m_SettingFilePath);
	GetDocument()->SetPathName(m_SettingFilePath,0);
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

void CFormViewShellView::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	if(pNMUpDown->iDelta < 0)
	{
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
	else if(pNMUpDown->iDelta > 0)
	{
		int loc;
		CString tempUp;
		CString tempDn;
		loc = m_TclFilesListBox.GetCurSel();
		UpdateData(TRUE);
		if(loc >= 0 && loc < m_TclFilesListBox.GetCount() - 1)
		{
			m_TclFilesListBox.GetText(loc, tempUp);
			m_TclFilesListBox.GetText(loc + 1, tempDn);
			m_TclFilesListBox.DeleteString(loc + 1);
			m_TclFilesListBox.InsertString(loc, tempDn);
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
	si.lpFile = __TEXT(excutteFile);           //��������
	si.lpParameters = CMDLine;                    //�������� ���ڰ�.
	// shellIP.lpDirectory =                        //�������� ���丮.
	si.nShow = isShow;                    //SW_HIDE
	si.fMask = SEE_MASK_NOCLOSEPROCESS|SEE_MASK_FLAG_DDEWAIT;

	BOOL isSuccess = ShellExecuteEx(&si);             //������ true

	if(!isSuccess)
	{
		DWORD err = GetLastError() ;
		switch(err)
		{
			case ERROR_FILE_NOT_FOUND:
			{

				MessageBox("������ ������ ã���� ����.");
			}
			break;
			case ERROR_DDE_FAIL:
			{

				MessageBox("���� ������ ��ȯ(DDE) Ʈ�����(transaction)�� �����߽��ϴ�.");
			}
			break;
			case ERROR_NO_ASSOCIATION:
			{

				MessageBox("�־��� ������ Ȯ���� ����Ǵ� ���� ���α׷��� ����.");
			}
			break;
			case ERROR_ACCESS_DENIED:
			{

				MessageBox("������ ���Ͽ� ���� �׼����� �ź� �Ǿ���.");
			}
			break;
			case ERROR_DLL_NOT_FOUND:
			{

				MessageBox("���� ���α׷��� �����ϴ� �� �ʿ��� �ϳ��� ���̺귯��(library)  ������ ã�� �������ϴ�.");
			}
			break;
			case ERROR_CANCELLED:
			{

				MessageBox("�Լ��� ����ڸ� ���� �߰������� �˷��־����� ����ڰ� ��û�� ��� �ߴ�.");
			}
			break;
			case ERROR_NOT_ENOUGH_MEMORY:
			{

				MessageBox("�װ��� ������ �۾��� �����ϱ� ���� ����� �޸𸮰� ����.");
			}
			break;
			case ERROR_SHARING_VIOLATION:
			{

				MessageBox("���� ������ �߻��ߴ�.");
			}
			break;
		}
	}

	// ���μ����� �Ϸ�ɶ����� ��ٸ�
	while(1)
	{
		::GetExitCodeProcess( si.hProcess, &dwExitCode );

		if( dwExitCode != STILL_ACTIVE )
		{
			//MessageBox("����");
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
	if(lbIdx <lbTclFilesCnt)
	{
		m_TclFilesListBox.GetText(lbIdx, fPath);
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
