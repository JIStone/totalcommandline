#pragma once


// PreviewDlg dialog
#include "resource.h" 
#include "afxwin.h"
#include "ResizeDlg.h"

class PreviewDlg : public CResizeDlg
{
	DECLARE_DYNAMIC(PreviewDlg)

public:
	PreviewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~PreviewDlg();

// Dialog Data
	enum { IDD = IDD_PREVIEW_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

//==============================다이얼로그 크기변화에 따른 컨트를 재배치 =================================================
// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	//{{AFX_MSG(CResizeDialogDlg)
	virtual BOOL OnInitDialog();
	//afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//======================================================================================================================

public:
	CEdit m_resultEdit;
	CString m_EditText;
	BOOL m_bIsModiFy;
	CString m_TitleText;

	CFont m_bigfont;
	CFont m_stylefont;
	afx_msg void OnBnClickedOk();
};
