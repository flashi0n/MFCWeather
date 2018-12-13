// MyDialog3.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KWeather.h"
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
	DDX_Control(pDX, IDC_MYPICTURE2, m_Picture3);
	DDX_Control(pDX, IDC_temp_info, m_tempInfo);
	DDX_Control(pDX, IDC_celcius, m_Celcius);
	DDX_Control(pDX, IDC_Curtemp, m_Curtemp);
	DDX_Control(pDX, IDC_text, m_Text);
}


BEGIN_MESSAGE_MAP(CMyDialog3, CDialog)

	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CMyDialog3 �޽��� ó�����Դϴ�.



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
		_T("����")); // lpszFacename 

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
		_T("����")); // lpszFacename 


	m_tempInfo.SetFont(&m_font, TRUE);
	m_Celcius.SetFont(&m_font2, TRUE);
	m_Curtemp.SetFont(&m_font, TRUE);


	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL CMyDialog3::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(240 ,240, 240));//���̾�α� ���� ����

	return TRUE;
}
