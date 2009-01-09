// FormViewShellView.cpp : CFormViewShellView Ŭ������ ����
//

#include "stdafx.h"
#include "FormViewShell.h"

#include "FormViewShellDoc.h"
#include "FormViewShellView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define NUM_OF_CVTR_FILE_MAX (500)
#define NUM_OF_CVTR_FOLDER_MAX (200)

// Ȯ������ �ִ���̴� 6���� ����				
#define FILE_EXT_LENGTH_MAX (6)

// CFormViewShellView

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

// CFormViewShellView ����/�Ҹ�

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
	BRinfo.ulFlags = BIF_RETURNONLYFSDIRS;
	BRinfo.lpfn = NULL;
	BRinfo.lParam = 0;

	pidlBrowse = SHBrowseForFolder(&BRinfo);
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
		AfxMessageBox("��ȯ�� ������ �����ϼ���.");

		OnBnClickedSelfile();
		if(m_FullFileName.IsEmpty())
		{
			return;
		}
	}
	else if(m_ExecFilePath.IsEmpty()) 
	{ 
		AfxMessageBox("���������� �����ϼ���.");

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
	
	// ���� ���� ���� �ʱ�ȭ
	m_SubDirCnt = 0;
	// �˻��� ���� �ʱ�ȭ
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
		//������� �˻�
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
			// ������� �ɼ��� ������
			if(m_DestPath.IsEmpty() || m_PreCmdOptStr.IsEmpty())
			{
				testAllPath = searchedFileList[listCnt] + execArg;
			}

			//========g2dcvtr�� �ѱ������� ��������� ���Ҽ� ����?==========
			HINSTANCE ret = ShellExecute(NULL, "open", m_ExecFilePath, testAllPath, NULL, SW_HIDE);
			
			
			for(int lbIndex = 0; lbIndex < m_ListBox.GetCount(); lbIndex++)
			{
				int sIndex = 0;
				CString oExtStr;
				//�������� ������ �����н�
				CString outPutResultPath = "";
				CString iExtStr = searchFileStr;
			
				// ����ó�� Ÿ�� ����
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
				
				//Ȯ���ڿ��� ����ó���� ���� ��Ʈ���� ����
				sIndex = oExtStr.Find('.', 0);
				oExtStr = oExtStr.Right(oExtStr.GetLength() - sIndex);


				//�������� ������ �����н� �����
				sIndex = testAllPath.Find('.',testAllPath.GetLength() - FILE_EXT_LENGTH_MAX);
				outPutResultPath = testAllPath.Left(sIndex) + oExtStr;

				CString tempOutPutStr = searchedFileList[listCnt];
				// ���� �̸��� �и��ϱ�
				sIndex = tempOutPutStr.Find('.',tempOutPutStr.GetLength() - FILE_EXT_LENGTH_MAX);
				CString sExte = tempOutPutStr.Right(tempOutPutStr.GetLength() - sIndex);
				// ��������̸��� Ȯ���� ���̱�
				tempOutPutStr.Replace(sExte, oExtStr);

				//tempOutPutStr.Replace(iExtStr, oExtStr);
				tempOutPutStr.Replace(searchRootDirStr, "");
				//CString movSrc = searchRootDirStr + tempOutPutStr;
				tempOutPutStr = m_DestPath + tempOutPutStr;
				// �̸� ���� �ٲٰ� �̵��ϱ�////////
				CString movSrc = searchRootDirStr + searchSubDirList[dirCnt] + "\\*" + oExtStr;//SE_ERR_ACCESSDENIED
				CString movDest = tempOutPutStr;
				//������ ���� ã������ ���� �н�
				CString movingStr = "";
				int findCnt = 0;
				
				// �����õ� ������ ���� ���� ��� ��� ã��
				while(movingStr.IsEmpty() )
				{					
					SearchOneFile(NULL, outPutResultPath, &movingStr);
					Sleep(10);

					findCnt++;
					// �������� ����(����׶�� ���÷�����)
					if(findCnt > 10)
						break;
				}	
				// CMD ���
/*				CString testSTRING ="/c move " + movSrc + " " + movDest;
				HINSTANCE movRet = ShellExecute(NULL, "open", "cmd", testSTRING, NULL, SW_HIDE);

*/				//=========�����̵�================
				SHFILEOPSTRUCT shos;
				ZeroMemory(&shos, sizeof(SHFILEOPSTRUCT));
				// ����ó�� Ÿ�� ����
				shos.wFunc = iExtType;
				movingStr.Append("\0", 1);
				movDest.Append("\0", 1);
				shos.pFrom = NULL;
				shos.pTo = NULL;
				// �̷������� �־������!!!
				TCHAR        pszFrom[1024] = {0}; 
				strcpy_s(pszFrom , movingStr);
				shos.pFrom = pszFrom;//"D:\\WORK\\Macheon_Trial\\MC\\Program\\NC\\MainUi\\2d\\MainUI_bottom.NCER";

				TCHAR        pszTo[1024] = {0}; 
				strcpy_s(pszTo , movDest);				
				shos.pTo = pszTo;
				// �۾����� ������ ǥ�þ���
				shos.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI;
	
				int shRet = SHFileOperation(&shos);
				//�̵���ų ������ ���� �����޼����� ǥ���� �����̸�
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

				///////////////////////////////////
				BOOL isExist = FALSE;
				int findMovedFileCnt = 0;
				while(!isExist)
				{
					isExist = PathFileExists(tempOutPutStr);

					Sleep(10);
					findMovedFileCnt++;
					if(findMovedFileCnt > 10)
						break;
				}
				// �����޼��� ǥ�ÿ���
				if(!isExist && m_Check_EnableErrPop.GetCheck())
				{
					//int popRet = MessageBox(searchedFileList[listCnt] + " \n������ �۾��� ��ġ�� ���߽��ϴ�. \n��� �Ͻðڽ��ϱ�?","���� ����", MB_OKCANCEL);
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

			}

			switch((int)ret)
			{
				case SE_ERR_NOASSOC:
				{
					MessageBox("������ ���� Ȯ���ڷ� ������ �� �ִ� ���α׷��� �����ϴ�.",
					"���� Ȯ���� ����", MB_OKCANCEL);
				}
				break;
				case SE_ERR_FNF:
				{
					MessageBox("������ ������ ã���� �����ϴ�.",
					"���� ����", MB_OKCANCEL);
				}
				break;
			}
		}

	}

	//m_ProgressCtrl.SetPos(100);
}
int CFormViewShellView::SearchDir(CString sDirName, CString *sDirNameList)
{
	CFileFind dirFinder;
	BOOL bWorking = dirFinder.FindFile(sDirName + "\\*.*");
	int dCnt = 0;
	// �ֻ��� ���� �߰�
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
			// �������� ������
			CString tempfolderPath = dirFinder.GetFilePath();
			tempfolderPath.Replace(m_FullFileName, "");
			sDirNameList[m_SubDirCnt++] = tempfolderPath;
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
			// ��ȯ�� ���ϐl ������ ������Ŵ
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
	CString execArg;
	GetDlgItemText(IDC_EDIT3, execArg);

	GetDlgItemText(IDC_SRC_FILE, m_FullFileName);

	GetDlgItemText(IDC_EXEC_FILE, m_ExecFilePath);
	
	CString midPath;
	GetDlgItemText(IDC_EDIT4, midPath);


	GetDlgItemText(IDC_PATH, m_DestPath);
	
	GetDlgItemText(IDC_EDIT5, m_PreCmdOptStr);

	CString testAllPath = m_ExecFileName + " " + m_FullFileName + " " + "[" + midPath +"]"+ m_PreCmdOptStr + " " + "[" + m_DestPath + "]" + execArg;
	
	testAllPath.Replace(" ", "��");

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

	LPITEMIDLIST pidlBrowse;

	BROWSEINFO BRinfo;
	BRinfo.hwndOwner = GetSafeHwnd();
	BRinfo.pidlRoot = NULL;
	BRinfo.pszDisplayName = m_FullFileName.GetBuffer(MAX_PATH);
	BRinfo.lpszTitle = "������ �����ϼ���.";
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
