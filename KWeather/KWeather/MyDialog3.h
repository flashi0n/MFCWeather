#pragma once
#include "afxwin.h"
#include "PictureEx.h"
class CKWeatherView; // ������ Ŭ���� ����
// CMyDialog3 ��ȭ �����Դϴ�.

class CMyDialog3 : public CDialog
{
	DECLARE_DYNAMIC(CMyDialog3)

public:
	CMyDialog3(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMyDialog3();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	CKWeatherView *m_pView; // �� ��ü �ּ� ����
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
