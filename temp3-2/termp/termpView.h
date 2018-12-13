
// termpView.h : CtermpView Ŭ������ �������̽�
//

#pragma once
#include "PictureEx.h"
#include "GetWeather.h"
class CMyDialog3; // ������ Ŭ���� ����

struct ThreadArg		// �������� ���� ����
{
	CWnd *pWnd; // ����� ��� ������ ��ü�� �ּҴ�.
	int pos; // 1�̸� ���ʿ�, 2�� �Ʒ��ʿ� ����Ѵ�. ����
	KWeather_info tempA[20];	//���� ���� ���� ����ü
};
class CtermpView : public CView
{
protected: // serialization������ ��������ϴ�.
	CtermpView();
	DECLARE_DYNCREATE(CtermpView)

// Ư���Դϴ�.
public:
	CtermpDoc* GetDocument() const;
	CMyDialog3 *m_pDlg;
	CPictureEx m_Picture;
// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CtermpView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
	//afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	int R,G,B;
	char curDate[10];
	char curTime[10];
	time_t cur;
	struct tm* ptm;
	int i_curTime;
	int viewmode;
	int wher_y;
	int wher_x;
	int text_x;
	int text_y;
	CString wher;
	CString XX[3], YY[3];
	CString zero;
	CString cs_curTime;
	CString i_Time;
	CString temp, rain, humid, wind, windv, sky, rainP, tempL, tempH;
	//CMyDialog3 *m_pDlg; // ��ȭ���� ��ü �ּ� ����
	CListBox m_list;
	BOOL nCount;
	CWinThread  *pThread1, *pThread2; // ������ ��ü �ּ�
	ThreadArg  arg1, arg2; // ������ ���� ����	
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // termpView.cpp�� ����� ����
inline CtermpDoc* CtermpView::GetDocument() const
   { return reinterpret_cast<CtermpDoc*>(m_pDocument); }
#endif

