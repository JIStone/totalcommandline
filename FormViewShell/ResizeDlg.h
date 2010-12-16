#if !defined(AFX_RESIZEDLG_H__1BC3A745_2245_404D_A3DB_1BA0FBEB9D73__INCLUDED_)
#define AFX_RESIZEDLG_H__1BC3A745_2245_404D_A3DB_1BA0FBEB9D73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResizeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResizeDlg dialog

// CArray 사용을 위한 헤더 파일
#include <afxtempl.h>

// 자식 윈도우들을 재배치하는 옵션 선언
#define RE_NONE		0			// 재배치 하지 않음
#define RE_X1		1			// 좌측 변을 비율대로 재배치
#define RE_Y1		2			// 위쪽 변을 비율대로 재배치
#define RE_X2		4			// 우측 변을 비율대로 재배치
#define RE_Y2		8			// 아래쪽 변을 비율대로 재배치
#define RE_ALL		15			// 모든 변을 비율대로 재배치. RE_ALL = RE_X1 | RE_Y1 | RE_X2 | RE_Y2
#define RE_INV_X1	16			// 좌측 변을 다이얼로그 우측 기준으로 고정
#define RE_INV_Y1	32			// 위쪽 변을 다이얼로그 아래쪽 기준으로 고정
#define RE_INV_X2	64			// 우측 변을 다이얼로그 우측 기준으로 고정
#define RE_INV_Y2	128			// 아래쪽 변을 다이얼로그 아래쪽 기준으로 고정
#define RE_INV_ALL	240			// 모든 변을 다이얼로그 우측/아래쪽 기준으로 고정. RE_INV_ALL = RE_INV_X1 | RE_INV_Y1 | RE_INV_X2 | RE_INV_Y2

// 자식 윈도우에 대한 재배치 옵션 저장 구조체
typedef struct tagCHILDRESIZE
{
	HWND	hChild;
	double	dPosX1;
	double	dPosY1;
	double	dPosX2;
	double	dPosY2;

	UINT	nResizeType;
} CHILDRESIZE, *LPCHILDRESIZE;


class CResizeDlg : public CDialog
{
// Construction
public:
	CResizeDlg(UINT nResourceID, CWnd* pParent = NULL);   // standard constructor
	virtual ~CResizeDlg();

// Dialog Data
	//{{AFX_DATA(CResizeDlg)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CRect										m_rcClient;			// 다이얼로그의 기본 크기 저장 변수
	CArray<LPCHILDRESIZE, LPCHILDRESIZE>		m_ChildArray;		// 자식 윈도우들에 대한 재배치 옵션 저장 배열

	BOOL Reset();											// 모든 자식 윈도우들을 재배치하지 않도록 초기화
	BOOL SetAll();											// 모든 자식 윈도우들을 비율대로 재배치하도록 초기화
	BOOL SetChildType(HWND hWnd, UINT nType);				// 특정 자식 윈도우의 재배치 옵션 설정 함수
	BOOL SetChildType(CWnd *pWnd, UINT nType);				// 특정 자식 윈도우의 재배치 옵션 설정 함수

	void RepositionAll();									// 모든 자식 윈도우들을 재배치하는 함수
	void RepositionAt(int nChild, CRect *pClientRect);		// 특정 자식 윈도우를 재배치하는 함수

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CResizeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESIZEDLG_H__1BC3A745_2245_404D_A3DB_1BA0FBEB9D73__INCLUDED_)
