// MyDialog3.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "termp.h"
#include "MyDialog3.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CMyDialog3 대화 상자입니다.

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


// CMyDialog3 메시지 처리기입니다.



BOOL CMyDialog3::OnInitDialog()
{
	CDialog::OnInitDialog();
	//m_Picture.Load(_T("test.gif"));// 파일에서 로드하는 방법입니다 ~
	//m_Picture.Load(MAKEINTRESOURCE(IDB_BITMAP1), _T("BIN")); // 등록 된 리소스에서 로드하는 방법입니다 ~

	//	m_Picture.Draw();
	CPoint cpos;
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CRect rect;
	pFrame->GetWindowRect(&rect);
	cpos.x = rect.left + 650;
	cpos.y = rect.top;

	m_temp.Create(_T("날씨"), WS_CAPTION || WS_VISIBLE || BS_TEXT, CRect(20, 20, 160,90), this, 101);

	SetWindowPos(NULL, cpos.x, cpos.y, 650, 900, NULL);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CMyDialog3::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(249, 249, 249));//다이얼로그 배경색 지정
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}
