
// KWeatherView.cpp : CKWeatherView 클래스의 구현
//
#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "KWeather.h"
#endif

#include "KWeatherDoc.h"
#include "KWeatherView.h"
#include "Markup.h"
#include "GetWeather.h"
#include <string>
#include "MyDialog3.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKWeatherView

IMPLEMENT_DYNCREATE(CKWeatherView, CView)

BEGIN_MESSAGE_MAP(CKWeatherView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_CONTEXTMENU()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKWeatherView 생성/소멸
KWeather_index index;
KWeather_info info;

CKWeatherView::CKWeatherView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_pDlg = NULL;
	nCount = FALSE;
	viewmode = 0;
	cursor = 0;
	rect[0].SetRect(125, 95, 125 + 40, 95 + 20);//seoul
	rect[1].SetRect(208, 238, 208 + 40, 238 + 20);//청주
	rect[2].SetRect(56, 509, 56 + 40, 509 + 20);//목포//
	rect[3].SetRect(85, 604, 85 + 40, 604 + 20);//제주//
	rect[4].SetRect(334, 245, 334 + 40, 245 + 20);//안동//
	rect[5].SetRect(60, 105, 60 + 40, 105 + 20);//인천
	rect[6].SetRect(110, 165, 110 + 40, 165 + 20);//수원
	rect[7].SetRect(395, 310, 395 + 40, 310 + 20);//포항//
	rect[8].SetRect(181, 283 ,181 + 40, 283 + 20);//대전//
	rect[9].SetRect(133, 371, 133 + 40, 371 + 20);//전주//
	rect[10].SetRect(289, 467, 289 + 40, 467 + 20);//창원//
	rect[11].SetRect(259, 76, 259 + 40, 76 + 20);//춘천//
	rect[12].SetRect(362, 439, 362 + 40, 439 + 20);//부산//
	rect[13].SetRect(301, 341, 301 + 40, 341 + 20);//대구//
	rect[14].SetRect(403, 390, 403 + 40, 390 + 20);//울산//
	rect[15].SetRect(105, 425, 105 + 40, 425 + 20);//광주
	rect[16].SetRect(84, 269, 84 + 40, 269 + 20);//홍성//
	rect[17].SetRect(347, 99, 347 + 40, 99 + 20);//강릉//
	rect[18].SetRect(200, 492, 200 + 40, 492 + 20);//
}

CKWeatherView::~CKWeatherView()
{
}
UINT Thread_Parsing(LPVOID arg) {
	CString Nx[19] = { _T("60"),_T("60"),_T("55"),_T("73"),_T("92"),_T("55"),_T("69"),_T("67"),_T("91"), _T("102"),_T("89"),_T("102"),_T("90"),_T("78"),_T("58"),_T("73"),_T("50"), _T("63"), _T("52") };   //좌표 받기
	CString Ny[19] = { _T("127"), _T("121"), _T("124"), _T("134"), _T("131"), _T("106"), _T("106"), _T("100"), _T("106"), _T("94"), _T("90"), _T("84"), _T("77"), _T("76"), _T("74"), _T("66"), _T("67"),_T("89"), _T("38") };
	//   CKWeatherView* tdata;   //view의 tdata(tiem) 객체 선언
	while (1) {


		ThreadArg *pArg = (ThreadArg *)arg;
		char curDate[10];
		char curTime[10];
		time_t cur;
		struct tm* ptm;
		int i_curTime;
		CString cs_curTime;
		CString i_Time;

		//스레드 파싱

		CMarkup xml;
		CMarkup xml2;
		CMarkup xml3;
		//시간 받기
		cur = time(NULL);
		ptm = localtime(&(cur));
		strftime(curDate, sizeof(curDate), "%Y%m%d", ptm);
		strftime(curTime, sizeof(curTime), "%H%M", ptm);

		i_curTime = stoi(curTime);
		int ii_curTime = i_curTime;
		i_curTime -= 100;
		i_Time.Format(_T("%d"), i_curTime);
		cs_curTime = i_Time;
		int i_curDate = stoi(curDate);
		int t_Date = i_curDate - 1;
		CString ex_Date;
		ex_Date.Format(_T("%d"), t_Date);

		while ((i_curTime - 1000) < 0) // 문자열 0 붙이기
		{
			if ((i_curTime - 100) < 0)
			{
				CString zero = _T("00");
				cs_curTime.Format(_T("%s%d"), zero, i_curTime);
				break;
			}
			CString zero = _T("0");
			cs_curTime.Format(_T("%s%d"), zero, i_curTime);
			break;
		}
		index.ch_baseDate = curDate;
		index.ch_baseTime = cs_curTime;
		index.do_baseDate = curDate;

		// 동네예보 받기 3시간 단위로 시간 나누기
		CString d_curTime;
		int t_curTime = (ii_curTime % 300) / 100;
		//int t_curTime = (ii_curTime / 100) % 3;
		if (t_curTime == 0) {
			if (ii_curTime - 100 >= 0) {
				ii_curTime = ii_curTime / 100;//분 없애기
				ii_curTime = ii_curTime * 100 - 90;
			}
			else
				ii_curTime = 2300;
		}
		else if (t_curTime == 1) {
			if (ii_curTime - 200 >= 0) {
				ii_curTime = ii_curTime / 100;
				ii_curTime = ii_curTime * 100 - 190;
			}
			else
				ii_curTime = 2300;
		}
		else if (t_curTime == 2) {
			if (ii_curTime - 300 >= 0) {
				ii_curTime = ii_curTime / 100;
				ii_curTime = ii_curTime * 100 - 290;
			}
			else
				ii_curTime = 2300;

		}
		if (ii_curTime == 2300)
			index.do_baseDate = ex_Date;

		d_curTime.Format(_T("%d"), ii_curTime);


		while ((ii_curTime - 1000) < 0)
		{
			if ((ii_curTime - 100) < 0)
			{
				CString zero = _T("00");
				d_curTime.Format(_T("%s%d"), zero, ii_curTime);
				break;
			}
			CString zero = _T("0");
			d_curTime.Format(_T("%s%d"), zero, ii_curTime);
			break;
		}

		index.do_baseTime = d_curTime;

		CString URL3 = _T("http://www.weather.go.kr/weather/forecast/mid-term-rss3.jsp?stnId=108");
		xml3.URLLoad(URL3, _T("jg.xml"));
		xml3.Load(_T("jg.xml"));

		pArg->wth_string = getWeather_jg(xml3);

		for (int i = 0; i < 19; i++) {
			CString URL1 = _T("http://newsky2.kma.go.kr/service/SecndSrtpdFrcstInfoService2/ForecastGrib?ServiceKey=7cqqyZ66TiNsXF3RF3b1yrwmXgSjoe%2BUA%2BaajA7MR9vQxlkQylJtQrrBF%2BmKTxIKSfxmmk7shprZSOU6OjIAxA%3D%3D&base_date=") + index.ch_baseDate + _T("&base_time=") + index.ch_baseTime + _T("&nx=") + Nx[i] + _T("&ny=") + Ny[i] + _T("&pageNo=1&numOfRows=8");//초단기
			CString URL2 = _T("http://newsky2.kma.go.kr/service/SecndSrtpdFrcstInfoService2/ForecastSpaceData?serviceKey=7cqqyZ66TiNsXF3RF3b1yrwmXgSjoe%2BUA%2BaajA7MR9vQxlkQylJtQrrBF%2BmKTxIKSfxmmk7shprZSOU6OjIAxA%3D%3D&base_date=") + index.do_baseDate + _T("&base_time=") + index.do_baseTime + _T("&nx=") + Nx[i] + _T("&ny=") + Ny[i] + _T("&pageNo=1&numOfRows=24");//동네예보
			xml.URLLoad(URL1, _T("ch.xml"));
			xml2.URLLoad(URL2, _T("do.xml"));

			xml.Load(_T("ch.xml"));
			xml2.Load(_T("do.xml"));

			pArg->tempA[i].T1H = getWeather_info(xml, 기온, 0);
			pArg->tempA[i].RN1 = getWeather_info(xml, 강수량, 0);
			pArg->tempA[i].REH = getWeather_info(xml, 습도, 0);
			pArg->tempA[i].VEC = getWeather_info(xml, 풍향, 0);
			pArg->tempA[i].WSD = getWeather_info(xml, 풍속, 0);
			pArg->tempA[i].SKY = getWeather_info(xml2, 하늘상태, 1);
			pArg->tempA[i].POP = getWeather_info(xml2, 강수확률, 1);
		}

		Sleep(36000000);      //60초 *60 ms
	}
	return 0;
}
UINT Thread_Time(LPVOID arg) {

	while (1) {

		ThreadArg *pArg = (ThreadArg *)arg;
		pArg->CcurDate = _T("");
		pArg->CcurTime = _T("");
		char curDate[10];
		char curTime[10];
		time_t cur;
		struct tm* ptm;
		//int i_curTime;
		//시간 받기
		cur = time(NULL);
		ptm = localtime(&(cur));
		strftime(curDate, sizeof(curDate), "%Y%m%d", ptm);
		strftime(curTime, sizeof(curTime), "%H%M%S", ptm);

		for (int i = 0; i < 8; i++) {
			pArg->CcurDate += curDate[i];
		}
		for (int i = 0; i < 6; i++) {
			pArg->CcurTime += curTime[i];
		}

		Sleep(1000);


	}

	return 0;
}

BOOL CKWeatherView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CKWeatherView 그리기

void CKWeatherView::OnDraw(CDC* pDC)
{		
	CKWeatherDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	CFont cfont;
	
	CDC dcmem;

	CDC mdcoffscreen;
	
	CBitmap bmpoffscreen;
	CBitmap *oldbitmap;
	CBitmap bitmap;
	CBitmap bitSeoul;//서울
	CBitmap bitSuwon;//수원
	CBitmap bitIncheon;//인천
	CBitmap bitChuncheon;//춘천
	CBitmap bitGangneung;//강릉
	CBitmap bitHong;//홍성
	CBitmap bitCheong;//청주
	CBitmap bitDaejeon;//대전
	CBitmap bitAndong;//안동

	CBitmap bitPohang;//포항
	CBitmap bitDaegu;//대구
	CBitmap bitUlsan;	//울산
	CBitmap bitChangwon;//창원
	CBitmap bitBusan;//부산
	CBitmap bitGwangju;//광주
	CBitmap bitYeosu;//여수
	CBitmap bitMokpo;//목포
	CBitmap bitJeonju;//전주
	CBitmap bitJeju;//제주

if (viewmode == 1)
{
	bitmap.LoadBitmapW(IDB_BITMAP2);
	text_x = 145;
	text_y = 115;
	wher.Format(_T("서울"));
}
else if (viewmode == 2)
{
	bitmap.LoadBitmapW(IDB_BITMAP4);
	text_x = 218;
	text_y = 248;
	wher.Format(_T("청주"));
}
else if (viewmode == 3)
{
	bitmap.LoadBitmapW(IDB_BITMAP9);
	wher.Format(_T("목포"));
	text_x = 66;
	text_y = 539;

}
else if (viewmode == 4)
{
	bitmap.LoadBitmapW(IDB_BITMAP10);
	text_x = 95;
	text_y = 634;
	wher.Format(_T("제주"));
}
else if (viewmode == 5)
{
	bitmap.LoadBitmapW(IDB_BITMAP5);
	wher.Format(_T("안동"));
	text_x = 344;
	text_y = 265;

}
else if (viewmode == 6)
{
	bitmap.LoadBitmapW(IDB_BITMAP12);
	text_x = 70;
	text_y = 115;
	wher.Format(_T("인천"));
}
else if (viewmode == 7)
{
	bitmap.LoadBitmapW(IDB_BITMAP11);
	text_x = 120;
	text_y = 175;
	wher.Format(_T("수원"));
}
else if (viewmode == 8)
{
	bitmap.LoadBitmapW(IDB_BITMAP5);
	wher.Format(_T("포항"));
	text_x = 405;
	text_y = 320;
}
else if (viewmode == 9)
{
	bitmap.LoadBitmapW(IDB_BITMAP13);
	text_x = 191;
	text_y = 293;
	wher.Format(_T("대전"));
}
else if (viewmode == 10)
{
	bitmap.LoadBitmapW(IDB_BITMAP7);
	text_x = 143;
	text_y = 381;
	wher.Format(_T("전주"));
}
else if (viewmode == 11)
{
	bitmap.LoadBitmapW(IDB_BITMAP8);
	text_x = 299;
	text_y = 477;
	wher.Format(_T("창원"));
}
else if (viewmode == 12)
{
	bitmap.LoadBitmapW(IDB_BITMAP3);

	wher.Format(_T("춘천"));
	text_x = 269;
	text_y = 86;
}
else if (viewmode == 13)
{
	bitmap.LoadBitmapW(IDB_BITMAP15);
	text_x = 372;
	text_y = 459;
	wher.Format(_T("부산"));
}
else if (viewmode == 14)
{
	bitmap.LoadBitmapW(IDB_BITMAP14);
	text_x = 311;
	text_y = 361;
	wher.Format(_T("대구"));
}
else if (viewmode == 15)
{
	bitmap.LoadBitmapW(IDB_BITMAP16);
	text_x = 413;
	text_y = 400;
	wher.Format(_T("울산"));
}
else if (viewmode == 16)
{
	bitmap.LoadBitmapW(IDB_BITMAP17);
	text_x = 115;
	text_y = 435;
	wher.Format(_T("광주"));
}
else if (viewmode == 17)
{
	bitmap.LoadBitmapW(IDB_BITMAP6);
	text_x = 94;
	text_y = 279;
	wher.Format(_T("홍성"));
}
else if (viewmode == 18)
{
	bitmap.LoadBitmapW(IDB_BITMAP3);
	wher.Format(_T("강릉"));
	text_x = 347;
	text_y = 99;
}
else if (viewmode == 19)
{
	bitmap.LoadBitmapW(IDB_BITMAP9);
	wher.Format(_T("여수"));
	text_x = 210;
	text_y = 512;
}
	else
	{
			bitmap.LoadBitmapW(IDB_BITMAP1);
	}

	for (int i = 0; i < 19; i++) {

		if (i == 0) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitSeoul.LoadBitmapW(IDB_BITMAP22);
				else bitSeoul.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitSeoul.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitSeoul.LoadBitmapW(IDB_BITMAP19);
			else
				bitSeoul.LoadBitmapW(IDB_BITMAP20);
		}
		else if (i == 1) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitCheong.LoadBitmapW(IDB_BITMAP22);
				else bitCheong.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitCheong.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitCheong.LoadBitmapW(IDB_BITMAP19);
			else
				bitCheong.LoadBitmapW(IDB_BITMAP20);
		}
		else if (i == 2) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitMokpo.LoadBitmapW(IDB_BITMAP22);
				else  bitMokpo.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitMokpo.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitMokpo.LoadBitmapW(IDB_BITMAP19);
			else
				bitMokpo.LoadBitmapW(IDB_BITMAP20);
		}
		else if (i == 3) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitJeju.LoadBitmapW(IDB_BITMAP22);
				else bitJeju.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitJeju.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitJeju.LoadBitmapW(IDB_BITMAP19);
			else
				bitJeju.LoadBitmapW(IDB_BITMAP20);
		}

		else if (i == 4) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitAndong.LoadBitmapW(IDB_BITMAP22);
				else  bitAndong.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitAndong.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitAndong.LoadBitmapW(IDB_BITMAP19);
			else
				bitAndong.LoadBitmapW(IDB_BITMAP20);
		}
		else if (i == 5) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitIncheon.LoadBitmapW(IDB_BITMAP22);
				else bitIncheon.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitIncheon.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitIncheon.LoadBitmapW(IDB_BITMAP19);
			else
				bitIncheon.LoadBitmapW(IDB_BITMAP20);
		}
		else if (i == 6) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitSuwon.LoadBitmapW(IDB_BITMAP22);
				else bitSuwon.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitSuwon.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitSuwon.LoadBitmapW(IDB_BITMAP19);
			else
				bitSuwon.LoadBitmapW(IDB_BITMAP20);
		}
		else if (i == 7) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitPohang.LoadBitmapW(IDB_BITMAP22);
				else bitPohang.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitPohang.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitPohang.LoadBitmapW(IDB_BITMAP19);
			else
				bitPohang.LoadBitmapW(IDB_BITMAP20);
		}
		else if (i == 8) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitDaejeon.LoadBitmapW(IDB_BITMAP22);
				else bitDaejeon.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitDaejeon.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitDaejeon.LoadBitmapW(IDB_BITMAP19);
			else
				bitDaejeon.LoadBitmapW(IDB_BITMAP20);
		}
		else if (i == 9) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitJeonju.LoadBitmapW(IDB_BITMAP22);
				else bitJeonju.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitJeonju.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitJeonju.LoadBitmapW(IDB_BITMAP19);
			else
				bitJeonju.LoadBitmapW(IDB_BITMAP20);
		}
		else if (i == 10) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitChangwon.LoadBitmapW(IDB_BITMAP22);
				else  bitChangwon.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitChangwon.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitChangwon.LoadBitmapW(IDB_BITMAP19);
			else
				bitChangwon.LoadBitmapW(IDB_BITMAP20);
		}
		else if (i == 11) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitChuncheon.LoadBitmapW(IDB_BITMAP22);
				else  bitChuncheon.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitChuncheon.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitChuncheon.LoadBitmapW(IDB_BITMAP19);
			else
				bitChuncheon.LoadBitmapW(IDB_BITMAP20);
		}
		else if (i == 12) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitBusan.LoadBitmapW(IDB_BITMAP22);
				else  bitBusan.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitBusan.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitBusan.LoadBitmapW(IDB_BITMAP19);
			else
				bitBusan.LoadBitmapW(IDB_BITMAP20);
		}
		else if (i == 13) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitDaegu.LoadBitmapW(IDB_BITMAP22);
				else bitDaegu.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitDaegu.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitDaegu.LoadBitmapW(IDB_BITMAP19);
			else
				bitDaegu.LoadBitmapW(IDB_BITMAP20);
		}
		else if (i == 14) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitUlsan.LoadBitmapW(IDB_BITMAP22);
				else bitUlsan.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitUlsan.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitUlsan.LoadBitmapW(IDB_BITMAP19);
			else
				bitUlsan.LoadBitmapW(IDB_BITMAP20);
		}
		else if (i == 15) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitGwangju.LoadBitmapW(IDB_BITMAP22);
				else bitGwangju.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitGwangju.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitGwangju.LoadBitmapW(IDB_BITMAP19);
			else
				bitGwangju.LoadBitmapW(IDB_BITMAP20);
		}
		else if (i == 16) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitHong.LoadBitmapW(IDB_BITMAP22);
				else bitHong.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitHong.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitHong.LoadBitmapW(IDB_BITMAP19);
			else
				bitHong.LoadBitmapW(IDB_BITMAP20);
		}
		else if (i == 17) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitGangneung.LoadBitmapW(IDB_BITMAP22);
				else  bitGangneung.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitGangneung.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitGangneung.LoadBitmapW(IDB_BITMAP19);
			else
				bitGangneung.LoadBitmapW(IDB_BITMAP20);
		}
		else if (i == 18) {
			if ((_ttoi(arg1.tempA[i].POP)) >= 80) {
				if (_ttoi(arg1.tempA[i].T1H) <= 0)
					bitYeosu.LoadBitmapW(IDB_BITMAP22);
				else  bitYeosu.LoadBitmapW(IDB_BITMAP21);
			}
			else if (arg1.tempA[i].SKY == _T("1")) {
				bitYeosu.LoadBitmapW(IDB_BITMAP18);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitYeosu.LoadBitmapW(IDB_BITMAP19);
			else
				bitYeosu.LoadBitmapW(IDB_BITMAP20);
		}

	}

	BITMAP bmpinfo;
	bitmap.GetBitmap(&bmpinfo);
	BITMAP bmpinfo1;
	bitSeoul.GetBitmap(&bmpinfo1);


	dcmem.CreateCompatibleDC(pDC);
	mdcoffscreen.CreateCompatibleDC(pDC);
	bmpoffscreen.CreateCompatibleBitmap(pDC, bmpinfo.bmWidth, bmpinfo.bmHeight);
	oldbitmap = mdcoffscreen.SelectObject(&bmpoffscreen);

	dcmem.SelectObject(&bitmap);
	mdcoffscreen.StretchBlt(0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 50; //0:투명 ~ 255:불투명
	bf.AlphaFormat = 0;

	dcmem.SelectObject(&bitSeoul);
	mdcoffscreen.TransparentBlt(135, 120, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//서울

	bitSuwon.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitSuwon);
	mdcoffscreen.TransparentBlt(110, 180, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));

	bitIncheon.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitIncheon);
	mdcoffscreen.TransparentBlt(60, 120, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//인천

	bitIncheon.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitChuncheon);
	mdcoffscreen.TransparentBlt(259, 91, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//춘천

	bitGangneung.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitGangneung);
	mdcoffscreen.TransparentBlt(337, 114, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//강

	bitHong.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitHong);
	mdcoffscreen.TransparentBlt(84, 284, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//홍성

	bitCheong.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitCheong);
	mdcoffscreen.TransparentBlt(215, 253, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//청

	bitDaejeon.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitDaejeon);
	mdcoffscreen.TransparentBlt(181, 298, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//대전

	bitAndong.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitAndong);
	mdcoffscreen.TransparentBlt(334, 266, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//안동   

	bitPohang.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitPohang);
	mdcoffscreen.TransparentBlt(395, 325, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//포항

	bitDaegu.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitDaegu);
	mdcoffscreen.TransparentBlt(301, 356, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//대구

	bitUlsan.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitUlsan);
	mdcoffscreen.TransparentBlt(403, 405, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//울

	bitChangwon.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitChangwon);
	mdcoffscreen.TransparentBlt(289, 482, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//창원

	bitBusan.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitBusan);
	mdcoffscreen.TransparentBlt(362, 454, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//부

	bitGwangju.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitGwangju);
	mdcoffscreen.TransparentBlt(105, 440, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//광

	bitYeosu.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitYeosu);
	mdcoffscreen.TransparentBlt(200, 507, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//여   

	bitMokpo.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitMokpo);
	mdcoffscreen.TransparentBlt(56, 524, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//목

	bitJeonju.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitJeonju);
	mdcoffscreen.TransparentBlt(133, 386, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//전주

	bitJeju.GetBitmap(&bmpinfo1);
	dcmem.SelectObject(&bitJeju);
	mdcoffscreen.TransparentBlt(85, 619, (int)bmpinfo1.bmWidth*0.7, bmpinfo1.bmHeight*0.7, &dcmem, 0, 0, (int)bmpinfo1.bmWidth, bmpinfo1.bmHeight, RGB(255, 255, 255));//제주
	
	
	mdcoffscreen.TextOut(347, 99, _T("강릉"));
	mdcoffscreen.TextOut(259, 76, _T("춘천"));
	mdcoffscreen.TextOut(403, 390, _T("울산"));
	mdcoffscreen.TextOut(362, 439, _T("부산"));
	mdcoffscreen.TextOut(289, 467, _T("창원"));
	mdcoffscreen.TextOut(133, 371, _T("전주"));
	mdcoffscreen.TextOut(181, 283, _T("대전"));
	mdcoffscreen.TextOut(84, 269, _T("홍성"));
	mdcoffscreen.TextOut(301, 341, _T("대구"));
	mdcoffscreen.TextOut(334, 245, _T("안동"));
	mdcoffscreen.TextOut(395, 310, _T("포항"));
	mdcoffscreen.TextOutW(85, 604, _T("제주"));
	mdcoffscreen.TextOut(105, 425, _T("광주"));
	mdcoffscreen.TextOut(56, 509, _T("목포"));
	mdcoffscreen.TextOut(200, 492, _T("여수"));
	mdcoffscreen.TextOut(208, 238, _T("청주"));
	mdcoffscreen.TextOut(60, 105, _T("인천"));
	mdcoffscreen.TextOut(125, 95, _T("서울"));
	mdcoffscreen.TextOut(110, 165, _T("수원"));
	
	mdcoffscreen.TextOut(340, 10,_T("날짜: ") + arg2.CcurDate);
	mdcoffscreen.TextOut(340, 40, _T("시간: ") + arg2.CcurTime);
	if (viewmode != 0)//선택한 도시 글씨 강조
	{
		cfont.CreateFont(20, NULL, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T(
			"굴림체"));
		mdcoffscreen.SelectObject(&cfont);
		mdcoffscreen.DrawTextW(wher, -1, rect[viewmode - 1], DT_NOCLIP);
	}

	pDC->StretchBlt(10, 10, bmpinfo.bmWidth, bmpinfo.bmHeight, &mdcoffscreen, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight,SRCCOPY);
	Invalidate(FALSE);
	dcmem.DeleteDC();
	mdcoffscreen.SelectObject(oldbitmap);
	mdcoffscreen.DeleteDC();
	bmpoffscreen.DeleteObject();
	
}

BOOL CKWeatherView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CKWeatherView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CKWeatherView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

// CKWeatherView 진단

#ifdef _DEBUG
void CKWeatherView::AssertValid() const
{
	CView::AssertValid();
}

void CKWeatherView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKWeatherDoc* CKWeatherView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKWeatherDoc)));
	return (CKWeatherDoc*)m_pDocument;
}
#endif //_DEBUG

// CKWeatherView 메시지 처리기

void CKWeatherView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	::GetCursorPos(&point);
	ScreenToClient(&point);
	Invalidate(FALSE);


	if (rect[0].PtInRect(point))
	{//서울
		viewmode = 1;
	}
	else if (rect[1].PtInRect(point))
	{//청주	
		viewmode = 2;
	}
	else if (rect[2].PtInRect(point))
	{//목포
		viewmode = 3;
	}
	else if (rect[3].PtInRect(point))
	{//제주
		viewmode = 4;
	}
	else if (rect[4].PtInRect(point))
	{//안동
		viewmode = 5;
	}
	else if (rect[5].PtInRect(point))
	{//인천
		viewmode = 6;
	}
	else if (rect[6].PtInRect(point))
	{//수원
		viewmode = 7;
	}
	else if (rect[7].PtInRect(point))
	{//포항
		viewmode = 8;
	}
	else if (rect[8].PtInRect(point))
	{//대전
		viewmode = 9;
	}
	else if (rect[9].PtInRect(point))
	{//전주
		viewmode = 10;
	}
	else if (rect[10].PtInRect(point))
	{//창원
		viewmode = 11;
	}
	else if (rect[11].PtInRect(point))
	{//춘천
		viewmode = 12;

	}
	else if (rect[12].PtInRect(point))
	{//부산
		viewmode = 13;
	}
	else if (rect[13].PtInRect(point))
	{//대구
		viewmode = 14;
	}
	else if (rect[14].PtInRect(point))
	{	//울산
		viewmode = 15;
	}
	else if (rect[15].PtInRect(point))
	{//광주
		viewmode = 16;
	}
	else if (rect[16].PtInRect(point))
	{//홍성
		viewmode = 17;
	}
	else if (rect[17].PtInRect(point))
	{//강릉
		viewmode = 18;
	}
	else if (rect[18].PtInRect(point))
	{//여수
		viewmode = 19;
	}
	else
	{
		viewmode = 0;
	}
	if(viewmode !=0){

	temp.Format(_T("%s"), arg1.tempA[viewmode - 1].T1H);
	rain.Format(_T("강수량:       %s mm"), arg1.tempA[viewmode - 1].RN1);
	humid.Format(_T("습도:         \t%s %%"), arg1.tempA[viewmode - 1].REH);
	wind.Format(_T("풍향:          \t%s °"), arg1.tempA[viewmode - 1].VEC);
	windv.Format(_T("풍속:         \t%s m/s"), arg1.tempA[viewmode - 1].WSD);
	sky.Format(_T("하늘상태    \t:%s"), arg1.tempA[viewmode - 1].SKY);
	rainP.Format(_T("강수확률\t :    %s %%"), arg1.tempA[viewmode - 1].POP);

	if (m_pDlg != NULL) {
			
		if (nCount == TRUE) {
			m_pDlg->m_list.ResetContent();
		}
	}
	else {
		m_pDlg = new CMyDialog3(); // 대화상자 객체를 생성한다.
		m_pDlg->m_pView = this; // 뷰 객체의 주소를 넘겨준다.
		m_pDlg->Create(IDD_DIALOG1); // 대화상자를 생성한다.		
		nCount = !nCount;

	}
	if ((_ttoi(arg1.tempA[viewmode-1].POP)) >= 80) {
		if (_ttoi(arg1.tempA[viewmode-1].T1H) <= 0)
			m_pDlg->m_Picture.Load(_T("snow.gif"));
		else m_pDlg->m_Picture.Load(_T("rain.gif"));
	}
	else if (arg1.tempA[viewmode - 1].SKY == "1")
		m_pDlg->m_Picture.Load(_T("sunny.gif"));// 파일에서 로드하는 방법입니다
	else if (arg1.tempA[viewmode - 1].SKY == "2")
		m_pDlg->m_Picture.Load(_T("partly cloudy.gif"));
	else if (arg1.tempA[viewmode - 1].SKY == "3")
		m_pDlg->m_Picture.Load(_T("cloudy.gif"));
	else if (arg1.tempA[viewmode - 1].SKY == "4")
		m_pDlg->m_Picture.Load(_T("cloudy.gif"));

	m_pDlg->m_Picture.Draw();
	m_pDlg->m_Picture3.Load(_T("res\\anounce.gif"));// 파일에서 로드하는 방법입니다 ~
	m_pDlg->m_Picture3.Draw();
	
	m_pDlg->m_list.AddString(rain); // IDC_STR 컨트롤의 초기값
	m_pDlg->m_list.AddString(humid); // IDC_STR 컨트롤의 초기값
	m_pDlg->m_list.AddString(wind); // IDC_STR 컨트롤의 초기값
	m_pDlg->m_list.AddString(windv); // IDC_STR 컨트롤의 초기값	
	
	m_pDlg->m_list.AddString(rainP); // IDC_STR 컨트롤의 초기값

	m_pDlg->m_tempInfo.SetWindowText(temp );
	m_pDlg->m_Text.SetWindowText(arg1.wth_string);

	m_pDlg->ShowWindow(SW_SHOW); // 대화상자가 보이게 한다.
	}
	Invalidate(FALSE);
}

BOOL CKWeatherView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nHitTest == HTCLIENT) {
		CPoint point;

		::GetCursorPos(&point);
		ScreenToClient(&point);
		
		if (rect[0].PtInRect(point) || rect[1].PtInRect(point) || rect[2].PtInRect(point) || rect[3].PtInRect(point) || rect[4].PtInRect(point)
			|| rect[5].PtInRect(point) || rect[6].PtInRect(point) || rect[7].PtInRect(point) || rect[8].PtInRect(point) || rect[9].PtInRect(point)
			|| rect[10].PtInRect(point) || rect[11].PtInRect(point) || rect[12].PtInRect(point) || rect[13].PtInRect(point) || rect[14].PtInRect(point)
			|| rect[15].PtInRect(point) || rect[16].PtInRect(point) || rect[17].PtInRect(point) || rect[18].PtInRect(point))
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
		else
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return TRUE;
	}
	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CKWeatherView::OnSetFocus(CWnd* pOldWnd)
{
	viewmode = 0;
	Invalidate(FALSE);
}


int CKWeatherView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	arg1.pWnd = this; // 뷰 객체의 주소
	arg2.pWnd = this; // 뷰 객체의 주소
	pThread1 = AfxBeginThread(Thread_Parsing, &arg1, THREAD_PRIORITY_TIME_CRITICAL, 0, CREATE_SUSPENDED);
	pThread2 = AfxBeginThread(Thread_Time, &arg2, THREAD_PRIORITY_HIGHEST, 0, CREATE_SUSPENDED);
	// 두 번째 스레드의 우선순위 레벨을 변경한다.
	//pThread1->SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	// 두 스레드의 실행을 재개한다.
	PlaySound(_T("res\\sample.wav"), NULL, SND_ASYNC | SND_LOOP);
	pThread1->ResumeThread();
	pThread2->ResumeThread();
	return 0;
}


BOOL CKWeatherView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CBrush backBrush(RGB(0, 102, 255)); // <- 흰색칼러로. 
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect; pDC->GetClipBox(&rect);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);

	return TRUE;
	return CView::OnEraseBkgnd(pDC);
}
