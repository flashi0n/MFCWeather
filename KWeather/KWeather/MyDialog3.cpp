// MyDialog3.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KWeather.h"
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
	DDX_Control(pDX, IDC_MYPICTURE2, m_Picture3);
	DDX_Control(pDX, IDC_temp_info, m_tempInfo);
	DDX_Control(pDX, IDC_celcius, m_Celcius);
	DDX_Control(pDX, IDC_Curtemp, m_Curtemp);
	DDX_Control(pDX, IDC_text, m_Text);
}


BEGIN_MESSAGE_MAP(CMyDialog3, CDialog)

	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CMyDialog3 메시지 처리기입니다.



BOOL CMyDialog3::OnInitDialog()
{
	CDialog::OnInitDialog();
	CPoint cpos;
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CRect rect;
	pFrame->GetWindowRect(&rect);
	cpos.x = rect.left + 550;
	cpos.y = rect.top;

	SetWindowPos(NULL, cpos.x, cpos.y, 550, 800, NULL);

	m_font.CreateFont(40, // nHeight 
		20, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		1, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		DEFAULT_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		_T("돋움")); // lpszFacename 

	m_font2.CreateFont(24, // nHeight 
		12, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		1, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		DEFAULT_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		_T("돋움")); // lpszFacename 


	m_tempInfo.SetFont(&m_font, TRUE);
	m_Celcius.SetFont(&m_font2, TRUE);
	m_Curtemp.SetFont(&m_font, TRUE);


	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CMyDialog3::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(240 ,240, 240));//다이얼로그 배경색 지정

	return TRUE;
}
