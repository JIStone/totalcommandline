// FormViewShellView.h : CFormViewShellView Ŭ������ �������̽�
//


#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>

class CFormViewShellView : public CFormView
{
protected: // serialization������ ��������ϴ�.
	CFormViewShellView();
	DECLARE_DYNCREATE(CFormViewShellView)

public:
	enum{ IDD = IDD_FORMVIEWSHELL_FORM };

// Ư���Դϴ�.
public:
	CFormViewShellDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CFormViewShellView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSelfile2();
	CString m_ExecFilePath;
//	afx_msg void OnBnClickedSelfile();
	CString m_FullFileName;
	afx_msg void OnBnClickedSelpath();
	CString m_DestPath;
//	afx_msg void OnEnChangeEdit4();
	afx_msg void OnBnClickedCheck1();
	CButton m_Check_EnableErrPop;
//	afx_msg void OnBnClickedCheck2();
	//CButton m_Check_Bmp;
	afx_msg void OnBnClickedCheck3();
	//CButton m_ExtName;
	afx_msg void OnBnClickedExecappl();
	int m_SubDirCnt;
	int m_SearchedFileCnt;
	// ����
	//CToolTipCtrl m_tooltip;

	int SearchDir(CString sDirName, std::vector<CString> &sDirNameList);
	int SearchFile(CString sDirName, CString sFileName, std::vector<CString> &vecFileList, int &SearchedFileCnt);
	int SearchOneFile(CString sDirName, CString sFileName, CString *sFileNameList);
	// // Ŀ�ǵ� ���� ǥ��
	void DisplayCommand(BOOL modifyed = TRUE);
	CProgressCtrl m_ProgressCtrl;
//	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangePath();
	afx_msg void OnEnChangeSrcFile();
	afx_msg void OnEnChangeExecFile();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	// ���α׷���ī��Ʈ ǥ��
	CEdit m_EditProgCnt;
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnEnChangeEdit2();
//	CButton m_Check_WildDotExt;
	CString m_ExecFileName;
//	afx_msg void OnBnClickedCheck4();
	CString m_PreCmdOptStr;
	afx_msg void OnEnChangeEdit5();
	CString m_AllCmdLnText;
	BOOL m_FirstLoaded;
//	afx_msg void OnBnClickedOk();
//	CString m_ListData;
	CListBox m_ListBox;
	afx_msg void OnBnClickedBtnOutAdd();
	afx_msg void OnBnClickedBtnOutDel();
	CString m_ListData;
//	afx_msg void OnLbnSelchangeListOut();
	afx_msg void OnBnClickedSelfile();

	CToolTipCtrl m_toolTip;
	BOOL PreTranslateMessage(MSG* pMsg);
	bool m_IsToolTipInit;
	// Ȯ���� �߰��� Ÿ�� ����
	int m_AddExtTypeRadio;

	CString m_kDir;
	// �����н� ǥ�ÿ�
	CString m_ExcuteFilePath;
	
	afx_msg void OnBnClickedRadio1Move();
	afx_msg void OnBnClickedRadio2Copy();
	afx_msg void OnBnClickedRadio3Delete();
	CEdit m_Edit_Success_Fail;
	ITEMIDLIST m_InputPathiIdList;
	ITEMIDLIST m_OutputPathiIdList;
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnEnChangeEdit7();
	afx_msg void OnEnChangeEdit8();
	afx_msg void OnLbnSelchangeListOut();
	afx_msg void OnEnChangeEditOut();
	afx_msg void OnBnClickedButtonAddFolder();

	// ������ ��������Ʈ
	CListBox m_ExFolderListBox;
	// �߰��� ����������
	CString m_ExFolderName;
	afx_msg void OnBnClickedButtonDelFolder();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	// TCL ���� ����Ʈ
	//CListBox m_TclFilesListBox;
	CCheckListBox m_TclFilesListBox;
	//
	BOOL m_bMultiMode;
	CString m_SettingFilePath;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDn();
	// ��ν��� ��ư
	CButton m_BtnMultiTCLExcute;
	// �����ư
	CButton m_BtnExcute;
	//std::vector<CString> vecSearchedFileList;
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	void ShellCommon(CString excutteFile, CString CMDLine, int isShow);
	CButton m_ViewList;
	afx_msg void OnLbnSelchangeListTclFiles();
//	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedDelMtclList();
	afx_msg void OnBnClickedButtonPre();
	afx_msg void OnBnClickedButtonPrelist();
};

#ifndef _DEBUG  // FormViewShellView.cpp�� ����� ����
inline CFormViewShellDoc* CFormViewShellView::GetDocument() const
   { return reinterpret_cast<CFormViewShellDoc*>(m_pDocument); }
#endif
