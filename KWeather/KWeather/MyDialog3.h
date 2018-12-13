#pragma once
#include "afxwin.h"
#include "PictureEx.h"
class CKWeatherView; // 참조할 클래스 선언
// CMyDialog3 대화 상자입니다.

class CMyDialog3 : public CDialog
{
	DECLARE_DYNAMIC(CMyDialog3)

public:
	CMyDialog3(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMyDialog3();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	CKWeatherView *m_pView; // 뷰 객체 주소 저장
	CListBox m_list;
	CPictureEx m_Picture;
	

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CStatic m_tempInfo;
	CPictureEx m_Picture3;
	CFont m_font;
	CFont m_font2;
	CStatic m_Celcius;
	CStatic m_Curtemp;
	CStatic m_Text;
};
