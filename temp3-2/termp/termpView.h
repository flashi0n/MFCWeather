
// termpView.h : CtermpView 클래스의 인터페이스
//

#pragma once
#include "PictureEx.h"
#include "GetWeather.h"
class CMyDialog3; // 참조할 클래스 선언

struct ThreadArg		// 스레드의 관련 변수
{
	CWnd *pWnd; // 출력할 대상 윈도우 객체의 주소다.
	int pos; // 1이면 위쪽에, 2면 아래쪽에 출력한다. 방향
	KWeather_info tempA[20];	//날씨 정보 받을 구조체
};
class CtermpView : public CView
{
protected: // serialization에서만 만들어집니다.
	CtermpView();
	DECLARE_DYNCREATE(CtermpView)

// 특성입니다.
public:
	CtermpDoc* GetDocument() const;
	CMyDialog3 *m_pDlg;
	CPictureEx m_Picture;
// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CtermpView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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
	//CMyDialog3 *m_pDlg; // 대화상자 객체 주소 저장
	CListBox m_list;
	BOOL nCount;
	CWinThread  *pThread1, *pThread2; // 스레드 객체 주소
	ThreadArg  arg1, arg2; // 스레드 전달 인자	
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // termpView.cpp의 디버그 버전
inline CtermpDoc* CtermpView::GetDocument() const
   { return reinterpret_cast<CtermpDoc*>(m_pDocument); }
#endif

