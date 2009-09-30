// MainFrm.cpp : CMainFrame Ŭ������ ����
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
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
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
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	// TODO: ���� ������ ��ŷ�� �� ���� �Ϸ��� �� �� ���� �����Ͻʽÿ�.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// Ÿ��Ʋ���� "******" - FormViewShell -> "******" - TotalCommandLine Converter
	SetTitle("TotalCommandLine");
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.cx = 550;
	//cs.cy = 460;
	
	m_cs = cs;

   //�ִ�ȭ��ư ���ֱ�
    cs.style &=~WS_MAXIMIZEBOX;
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

	memcpy(&m_cs, &cs, sizeof(CREATESTRUCT));
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	//cs.cx = 550;
	//cs.cy = 460;
	return TRUE;
}






// CMainFrame ����

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


// CMainFrame �޽��� ó����




void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	//UpdateWindow();
	// TODO: Add your message handler code here
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	// ������ ũ������
	// �ִ� ũ��� ���ϴ� ��쿡�� �����ϼ���.
	if(m_cs.cx > 0 && m_cs.cy > 0)
	{
		lpMMI->ptMaxSize.x = m_cs.cx;
		lpMMI->ptMaxSize.y = m_cs.cy;

		lpMMI->ptMaxTrackSize.x = m_cs.cx;
		lpMMI->ptMaxTrackSize.y = m_cs.cy;
	}     

     lpMMI->ptMinTrackSize.x=200;       //< ������ �ּ� ũ�� ����
     lpMMI->ptMinTrackSize.y=150;  
     CFrameWnd::OnGetMinMaxInfo(lpMMI);
}
