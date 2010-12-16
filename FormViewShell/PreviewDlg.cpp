// PreviewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PreviewDlg.h"


// PreviewDlg dialog

IMPLEMENT_DYNAMIC(PreviewDlg, CDialog)

PreviewDlg::PreviewDlg(CWnd* pParent /*=NULL*/)
	: CResizeDlg(PreviewDlg::IDD, pParent)
	,m_bIsModiFy(FALSE)
{
	
}

PreviewDlg::~PreviewDlg()
{
}

void PreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RESULT, m_resultEdit);
	
}




//================================���̾�α� ũ�⺯ȭ�� ���� ��Ʈ�� ���ġ ======================================================

BEGIN_MESSAGE_MAP(PreviewDlg, CResizeDlg)
	//{{AFX_MSG_MAP(CResizeDialogDlg)
	//ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	//ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// PreviewDlg message handlers

BOOL PreviewDlg::OnInitDialog()
{
	CResizeDlg::OnInitDialog();
/*
	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
*/
	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.


	// ����Ʈ �ڽ����� ���� ���� ���̾�α� ������ �������� ��ġ�� �����Ѵ�
	SetChildType(GetDlgItem(IDC_EDIT_RESULT), RE_INV_X2 | RE_INV_Y2);
	//SetChildType(GetDlgItem(IDC_EDIT2), RE_INV_X2);
	//SetChildType(GetDlgItem(IDC_EDIT3), RE_INV_X2);
/*
	// ���̾�α��� �� ��Ʈ�ѵ鿡 ����,
	// ���̾�α� ũ�� ��ȭ�� ���ġ �ɼ��� �����Ѵ�

	// �ؽ�Ʈ ����ƽ���� ���ġ���� �ʴ´�
	SetChildType(GetDlgItem(IDC_TEXT_STATIC1), RE_NONE);
	SetChildType(GetDlgItem(IDC_TEXT_STATIC2), RE_NONE);
	SetChildType(GetDlgItem(IDC_TEXT_STATIC3), RE_NONE);



	// �׷� �ڽ��� ���� ���� ���̾�α� ������ �������� ��ġ�� �����ϰ�
	// �Ʒ��� ���� ���̾�α� �Ʒ��� �������� ��ġ�� �����Ѵ�
	SetChildType(GetDlgItem(IDC_GROUP_STATIC), RE_INV_X2 | RE_INV_Y2);

	// ���� ������ ���� ���� ���̾�α� ������ �������� ��ġ�� �����Ѵ�
	SetChildType(GetDlgItem(IDC_MENT_STATIC), RE_INV_X2);

	// ������ ���� ���� ���̾�α� ������ �������� ��ġ�� �����Ѵ�
	SetChildType(GetDlgItem(IDC_LINE_STATIC), RE_INV_X2);
	
	// ���α׷����ٴ� ���� ���� ���̾�α� ������ �������� ��ġ�� �����ϰ�
	// �Ʒ��� ���� ���̾�α� �Ʒ��� �������� ��ġ�� �����Ѵ�
	SetChildType(GetDlgItem(IDC_PROGRESS1), RE_INV_X2 | RE_INV_Y2);
	
	// ��ư�� ���� ���� ���̾�α� �Ʒ��� �������� ��ġ�� �����ϰ�
	// ���� ���� ���̾�α� ������ �������� �����ϰ�
	// �Ʒ��� ���� ���̾�α� �Ʒ��� �������� �����Ѵ�
	SetChildType(GetDlgItem(IDC_BUTTON1), RE_INV_Y1 | RE_X2 | RE_INV_Y2);

	// �޺� �ڽ��� ���� ���� ������� ���� �������� �����ϰ�
	// ���� ���� ���̾�α� �Ʒ��� �������� �����ϰ�
	// ���� ���� ���̾�α� ������ �������� �����ϰ�
	// �Ʒ��� ���� ���̾�α� �Ʒ��� �������� �����Ѵ�
	SetChildType(GetDlgItem(IDC_COMBO1), RE_X1 | RE_INV_Y1 | RE_INV_X2 | RE_INV_Y2);

	// üũ �ڽ��� ���� ���� ���̾�α� ������ �������� ��ġ�� �����ϰ�
	// ���� ���� ���̾�α� �Ʒ��� �������� �����ϰ�
	// ���� ���� ���̾�α� ������ �������� ��ġ�� �����ϰ�
	// �Ʒ��� ���� ���̾�α� �Ʒ��� �������� �����Ѵ�
	SetChildType(GetDlgItem(IDC_CHECK1), RE_INV_ALL);
*/
	// Ȯ�� ��ư�� ���� ���� ���̾�α� ������ �������� ��ġ�� �����ϰ�
	// ���� ���� ���̾�α� �Ʒ��� �������� �����ϰ�
	// ���� ���� ���̾�α� ������ �������� �����ϰ�
	// �Ʒ��� ���� ���̾�α� �Ʒ��� �������� �����Ѵ�
	SetChildType(GetDlgItem(IDOK), RE_INV_ALL);
/*
	// ��� ��ư�� ���� ���� ���̾�α� ������ �������� ��ġ�� �����ϰ�
	// ���� ���� ���̾�α� �Ʒ��� �������� �����ϰ�
	// ���� ���� ���̾�α� ������ �������� �����ϰ�
	// �Ʒ��� ���� ���̾�α� �Ʒ��� �������� �����Ѵ�
	SetChildType(GetDlgItem(IDCANCEL), RE_INV_ALL);
*/
	if(m_bIsModiFy)
	{
		SetWindowPos(NULL,0,0,1280,138,SWP_NOMOVE);
		SetWindowTextA(m_TitleText);
		// ūũ���� ��Ʈ�� �����.
		m_bigfont.CreatePointFont(120, "����");
	}
	
	//m_EditText.Replace(" ", "��");
	SetDlgItemText(IDC_EDIT_RESULT, m_EditText);
	//m_EditText.Replace("��", " ");
	SetDlgItemText(IDC_EDIT_RESULT, m_EditText);
	// IDC_EDIT8 ��Ʈ�ѿ� �� ��Ʈ�� ����� �˷��ش�.
	GetDlgItem(IDC_EDIT_RESULT)->SetFont(&m_bigfont);

	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}

void PreviewDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CResizeDlg::OnPaint();
	}
}
/*
// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�. 
HCURSOR PreviewDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
*/
//==================================================
