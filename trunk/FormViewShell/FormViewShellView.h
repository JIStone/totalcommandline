// FormViewShellView.h : CFormViewShellView Ŭ������ �������̽�
//


#pragma once
#include "afxwin.h"
#include "afxcmn.h"


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

	int SearchDir(CString sDirName, CString *sDirNameList);
	int SearchFile(CString sDirName, CString sFileName,CString *sFileNameList);
	int SearchOneFile(CString sDirName, CString sFileName, CString *sFileNameList);
	// // Ŀ�ǵ� ��� ǥ��
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

	afx_msg void OnBnClickedRadio1Move();
	afx_msg void OnBnClickedRadio2Copy();
	afx_msg void OnBnClickedRadio3Delete();
	CEdit m_Edit_Success_Fail;
	ITEMIDLIST m_InputPathiIdList;
	ITEMIDLIST m_OutputPathiIdList;
};

#ifndef _DEBUG  // FormViewShellView.cpp�� ����� ����
inline CFormViewShellDoc* CFormViewShellView::GetDocument() const
   { return reinterpret_cast<CFormViewShellDoc*>(m_pDocument); }
#endif

