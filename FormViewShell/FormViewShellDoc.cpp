// FormViewShellDoc.cpp : CFormViewShellDoc Ŭ������ ����
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


// CFormViewShellDoc ����/�Ҹ�

CFormViewShellDoc::CFormViewShellDoc()
: m_IniFilePath(_T(""))
, m_TiltleString(_T(""))
, m_bIsFirstLoad(TRUE)
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
	if(pWnd == NULL)
		return;

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

		// ��ƼĿ�ǵ���� ���� ������
		pView->GetDlgItemText(IDC_EDIT_DELAY_TIME, execArg);
		ar << execArg;


		// ��ƼĿ�ǵ���� ��� ����
		int lbTclFilesCnt = pView->m_TclFilesListBox.GetCount();
		ar << lbTclFilesCnt;
		for(int lbIndex = 0; lbIndex < lbTclFilesCnt; lbIndex++)
		{
			pView->m_TclFilesListBox.GetText(lbIndex, execArg);
			ar << execArg;
			pView->m_SettingFilePath = ar.m_strFileName;
			// ���� ��API ȣ��� â Ȯ��
			((CMainFrame*)(pWnd->GetActiveFrame()))->SetWindowPos(NULL,0,0,1300,700,SWP_NOMOVE);
		}
		// ��ƼĿ�ǵ���� ü�׹ڽ�����
		for(int lbIndex = 0; lbIndex < lbTclFilesCnt; lbIndex++)
		{
			ar << pView->m_TclFilesListBox.GetCheck(lbIndex);;
		}
		// �������� �˻� ����
		ar << pView->m_ChkSubFolder.GetCheck();
		ar << pView->m_ViewList.GetCheck();
		//===================== ������ �����͸� ���Ͽ� ����=====================
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.

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
		// ���ο� ������Ұ� �߰��Ǿ� �Ͼ���϶� ���� ���������� �˻����ش�
		isEmpty = ar.IsBufferEmpty();
		
		if(isEmpty)
		{
			// ���� ��������Ʈ �ε�
			pView->m_ExFolderListBox.ResetContent();
			// ���������� �⺻������ svn������ �������� �� ����
			pView->m_ExFolderListBox.AddString(".svn");
		}
		else
		{
			// ���� ��������Ʈ �ε�
			int lbExFolderCnt = 0;
			pView->m_ExFolderListBox.ResetContent();
			ar >> lbExFolderCnt;
			for(int lbIndex = 0; lbIndex < lbExFolderCnt; lbIndex++)
			{
				ar >> execArg;
				pView->m_ExFolderListBox.AddString(execArg);
			}
		}

		// ��ƼĿ�ǵ���� ���� ������
		if(!ar.IsBufferEmpty())
		{	
			ar >> execArg;
			pView->SetDlgItemText(IDC_EDIT_DELAY_TIME, execArg);
		}
		int lbTclFilesCnt = 0;
		if(!ar.IsBufferEmpty())
			ar >> lbTclFilesCnt;
		// �о�� ������ �ְ�, ��Ƽ�������̾ �ε��ϴ� ���� �ƴϸ�
		if((!isEmpty && !pView->m_bMultiMode) || isMultiCommand)
		{
			
			pView->m_TclFilesListBox.ResetContent();
			CREATESTRUCT cs;
			cs = ((CMainFrame*)(pWnd->GetActiveFrame()))->m_cs;
			
			if(lbTclFilesCnt)
			{
				// ���� ��API ȣ��� â Ȯ��
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
				
				// ���� ���� �ʱ�ȭ ǥ��
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


				// ���� ���� �ʱ�ȭ ǥ��
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
		// �������� ã������
		if(!ar.IsBufferEmpty())
		{	
			ar >> nCheck;
			pView->m_ChkSubFolder.SetCheck(nCheck);	
		}
		else
		{
			pView->m_ChkSubFolder.SetCheck(TRUE);	
		}

		// �������� ã������
		if(!ar.IsBufferEmpty())
		{	
			ar >> nCheck;
			pView->m_ViewList.SetCheck(nCheck);	
		}
		else
		{
			pView->m_ViewList.SetCheck(FALSE);	
		}
		//===================== �ҷ����ϵ����͸� ���Ͽ� ����=====================
	}
	
	m_bIsFirstLoad = FALSE;
	
	//========================!!!!�������� ����!!!===============================
	CFile myFile;
	CFileException e;
	//AfxMessageBox(m_IniFilePath);
	if(!myFile.Open(m_IniFilePath, CFile::modeCreate | CFile::modeWrite, &e))
	{
		TRACE(_T("File could not be opened %s : %d\n"), m_IniFilePath, e.m_cause);
	}
	myFile.Write(pView->m_SettingFilePath, pView->m_SettingFilePath.GetLength());
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


// CFormViewShellDoc ����