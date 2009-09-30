// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "FormViewShell.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// 타이틀바의 "******" - FormViewShell -> "******" - TotalCommandLine Converter
	SetTitle("TotalCommandLine");
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.cx = 550;
	//cs.cy = 460;
	
	m_cs = cs;

   //최대화버튼 없애기
    cs.style &=~WS_MAXIMIZEBOX;
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

	memcpy(&m_cs, &cs, sizeof(CREATESTRUCT));
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	//cs.cx = 550;
	//cs.cy = 460;
	return TRUE;
}






// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 메시지 처리기




void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	//UpdateWindow();
	// TODO: Add your message handler code here
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	// 윈도우 크기제한
	// 최대 크기는 원하는 경우에만 변경하세요.
	if(m_cs.cx > 0 && m_cs.cy > 0)
	{
		lpMMI->ptMaxSize.x = m_cs.cx;
		lpMMI->ptMaxSize.y = m_cs.cy;

		lpMMI->ptMaxTrackSize.x = m_cs.cx;
		lpMMI->ptMaxTrackSize.y = m_cs.cy;
	}     

     lpMMI->ptMinTrackSize.x=200;       //< 윈도우 최소 크기 설정
     lpMMI->ptMinTrackSize.y=150;  
     CFrameWnd::OnGetMinMaxInfo(lpMMI);
}
