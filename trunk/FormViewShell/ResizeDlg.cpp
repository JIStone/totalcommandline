// ResizeDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "resizedialog.h"
#include "ResizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizeDlg dialog


CResizeDlg::CResizeDlg(UINT nResourceID, CWnd* pParent /*=NULL*/)
	: CDialog(nResourceID, pParent)
{
	//{{AFX_DATA_INIT(CResizeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CResizeDlg::~CResizeDlg()
{
	// 자식 윈도우들에 대한 옵션 소멸
	int i, nSize = m_ChildArray.GetSize();
	for(i = 0; i < nSize; i++)
	{
		delete m_ChildArray.GetAt(i);
	}
	m_ChildArray.RemoveAll();
}

void CResizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResizeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResizeDlg, CDialog)
	//{{AFX_MSG_MAP(CResizeDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizeDlg message handlers

// 자식 윈도우에 대한 옵션 초기화 함수
BOOL CALLBACK EnumChildProc(HWND hChild, LPARAM lParam)
{
	CResizeDlg *pParent = (CResizeDlg*)lParam;
	LPCHILDRESIZE pChildType = new CHILDRESIZE;

	CRect rcChild;
	::GetWindowRect(hChild, &rcChild);
	pParent->ScreenToClient(&rcChild);

	// 초기 비율 저장
	pChildType->hChild		= hChild;
	pChildType->dPosX1		= double(rcChild.left) / pParent->m_rcClient.Width();
	pChildType->dPosY1		= double(rcChild.top) / pParent->m_rcClient.Height();
	pChildType->dPosX2		= double(rcChild.right) / pParent->m_rcClient.Width();
	pChildType->dPosY2		= double(rcChild.bottom) / pParent->m_rcClient.Height();
	pChildType->nResizeType = RE_NONE;

	// 배열에 추가
	pParent->m_ChildArray.Add(pChildType);

	return TRUE;
}

BOOL CResizeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 초기 다이얼로그 크기 저장
	GetClientRect(&m_rcClient);

	// 자식 윈도우들을 찾아가며 옵션 초기화
	::EnumChildWindows(m_hWnd, EnumChildProc, (LPARAM)this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 모든 자식 윈도우들을 재배치하지 않도록 초기화
BOOL CResizeDlg::Reset()
{
	int i, nSize = m_ChildArray.GetSize();
	LPCHILDRESIZE pChild;

	// 모든 자식 윈도우들에 대해
	for(i = 0; i < nSize; i++)
	{
		pChild = m_ChildArray.GetAt(i);
		// 재배치 않음 속성 설정
		pChild->nResizeType = RE_NONE;
	}

	// 화면에 재배치
	if(::IsWindow(m_hWnd))		RepositionAll();

	return TRUE;
}

// 모든 자식 윈도우들을 비율대로 재배치하도록 초기화
BOOL CResizeDlg::SetAll()
{
	int i, nSize = m_ChildArray.GetSize();
	LPCHILDRESIZE pChild;

	// 모든 자식 윈도우들에 대해
	for(i = 0; i < nSize; i++)
	{
		pChild = m_ChildArray.GetAt(i);
		// 비율대로 재배치 속성 설정
		pChild->nResizeType = RE_ALL;
	}

	// 화면에 재배치
	if(::IsWindow(m_hWnd))		RepositionAll();

	return TRUE;
}

// 특정 자식 윈도우의 재배치 옵션 설정 함수
BOOL CResizeDlg::SetChildType(HWND hWnd, UINT nType)
{
	int i, nSize = m_ChildArray.GetSize();
	LPCHILDRESIZE pChild;

	for(i = 0; i < nSize; i++)
	{
		pChild = m_ChildArray.GetAt(i);

		// 윈도우 핸들이 동일한 항목 찾기
		if(pChild->hChild == hWnd)
		{
			// 옵션 재설정
			pChild->nResizeType = nType;
			// 화면에 재배치
			if(::IsWindow(hWnd))		RepositionAt(i, NULL);

			return TRUE;
		}
	}

	return FALSE;
}

// 특정 자식 윈도우의 재배치 옵션 설정 함수
BOOL CResizeDlg::SetChildType(CWnd *pWnd, UINT nType)
{
	ASSERT(pWnd);

	return SetChildType(pWnd->m_hWnd, nType);
}

// 모든 자식 윈도우들을 재배치하는 함수
void CResizeDlg::RepositionAll()
{
	int i, nSize = m_ChildArray.GetSize();
	CRect rcClient;

	GetClientRect(&rcClient);

	for(i = 0; i < nSize; i++)
	{
		RepositionAt(i, &rcClient);
	}
}

// 특정 자식 윈도우를 재배치하는 함수
void CResizeDlg::RepositionAt(int nChild, CRect *pClientRect)
{
	CRect rcClient, rcChild;
	LPCHILDRESIZE pChild;
	pChild = m_ChildArray.GetAt(nChild);

	if(pClientRect == NULL)
	{
		GetClientRect(&rcClient);
		pClientRect = &rcClient;
	}

	if(pChild->nResizeType & RE_X1)
		// 좌측 변을 비율대로 재배치
		rcChild.left		= (int)(pClientRect->Width() * pChild->dPosX1);
	else
		if(pChild->nResizeType & RE_INV_X1)
			// 좌측 변을 다이얼로그 우측 기준으로 고정
			rcChild.left	= (int)(pClientRect->right - (m_rcClient.Width() * (1.0 - pChild->dPosX1)));
		else
			rcChild.left	= (int)(m_rcClient.Width() * pChild->dPosX1);

	if(pChild->nResizeType & RE_Y1)
		// 위쪽 변을 비율대로 재배치
		rcChild.top			= (int)(pClientRect->Height() * pChild->dPosY1);
	else
		if(pChild->nResizeType & RE_INV_Y1)
			// 위쪽 변을 다이얼로그 아래쪽 기준으로 고정
			rcChild.top		= (int)(pClientRect->bottom - (m_rcClient.Height() * (1.0 - pChild->dPosY1)));
		else
			rcChild.top		= (int)(m_rcClient.Height() * pChild->dPosY1);

	if(pChild->nResizeType & RE_X2)
		// 우측 변을 비율대로 재배치
		rcChild.right		= (int)(pClientRect->Width() * pChild->dPosX2);
	else
		if(pChild->nResizeType & RE_INV_X2)
			// 우측 변을 다이얼로그 우측 기준으로 고정
			rcChild.right	= (int)(pClientRect->right - (m_rcClient.Width() * (1.0 - pChild->dPosX2)));
		else
			rcChild.right	= (int)(m_rcClient.Width() * pChild->dPosX2);

	if(pChild->nResizeType & RE_Y2)
		// 아래쪽 변을 비율대로 재배치
		rcChild.bottom	= (int)(pClientRect->Height() * pChild->dPosY2);
	else
		if(pChild->nResizeType & RE_INV_Y2)
			// 아래쪽 변을 다이얼로그 아래쪽 기준으로 고정
			rcChild.bottom	= (int)(pClientRect->bottom - (m_rcClient.Height() * (1.0 - pChild->dPosY2)));
		else
			rcChild.bottom	= (int)(m_rcClient.Height() * pChild->dPosY2);

	::SetWindowPos(pChild->hChild, NULL, rcChild.left, rcChild.top,
		rcChild.Width(), rcChild.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);
	::InvalidateRect(pChild->hChild, NULL, FALSE);
}

void CResizeDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// 다이얼로그 크기가 변경되면 컨트롤들을 재배치한다
	if(::IsWindow(m_hWnd))		RepositionAll();
}
