// MyDialog3.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "termp.h"
#include "MyDialog3.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CMyDialog3 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMyDialog3, CDialog)
//IMPLEMENT_DYNAMIC(CMyDialog3, CDialogEx)
CMyDialog3::CMyDialog3(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG1, pParent)
{

}

CMyDialog3::~CMyDialog3()
{
}

void CMyDialog3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_MYPICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CMyDialog3, CDialog)

	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CMyDialog3 �޽��� ó�����Դϴ�.



BOOL CMyDialog3::OnInitDialog()
{
	CDialog::OnInitDialog();
	//m_Picture.Load(_T("test.gif"));// ���Ͽ��� �ε��ϴ� ����Դϴ� ~
	//m_Picture.Load(MAKEINTRESOURCE(IDB_BITMAP1), _T("BIN")); // ��� �� ���ҽ����� �ε��ϴ� ����Դϴ� ~

	//	m_Picture.Draw();
	CPoint cpos;
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CRect rect;
	pFrame->GetWindowRect(&rect);
	cpos.x = rect.left + 650;
	cpos.y = rect.top;

	m_temp.Create(_T("����"), WS_CAPTION || WS_VISIBLE || BS_TEXT, CRect(20, 20, 160,90), this, 101);

	SetWindowPos(NULL, cpos.x, cpos.y, 650, 900, NULL);
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL CMyDialog3::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(249, 249, 249));//���̾�α� ���� ����
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}
