#pragma once
#include "afxwin.h"
#include "PictureEx.h"
class CtermpView; // ������ Ŭ���� ����
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
	CtermpView *m_pView; // �� ��ü �ּ� ����
	CListBox m_list;
	CPictureEx m_Picture;
	CStatic m_temp;

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
