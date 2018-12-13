
// termpView.cpp : CtermpView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "termp.h"
#endif

#include "termpDoc.h"
#include "termpView.h"
#include "Markup.h"
#include "GetWeather.h"
#include <string>
#include "MyDialog3.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CtermpView

IMPLEMENT_DYNCREATE(CtermpView, CView)

BEGIN_MESSAGE_MAP(CtermpView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_CONTEXTMENU()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CtermpView 생성/소멸
KWeather_index index;
KWeather_info info;

CtermpView::CtermpView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_pDlg = NULL;
	nCount = FALSE;
	viewmode = 0;
}

CtermpView::~CtermpView()
{
}
UINT Thread_Parsing(LPVOID arg) {

	//   CtermpView* tdata;   //view의 tdata(tiem) 객체 선언
	while (1) {

		int i = 0;
		ThreadArg *pArg = (ThreadArg *)arg;
		char curDate[10];
		char curTime[10];
		time_t cur;
		struct tm* ptm;
		int i_curTime;
		CString cs_curTime;
		CString i_Time;

		//스레드 파싱

		CString Nx[19] = { _T("60"),_T("60"),_T("55"),_T("73"),_T("92"),_T("55"),_T("69"),_T("67"),_T("91"), _T("102"),_T("89"),_T("102"),_T("90"),_T("78"),_T("58"),_T("73"),_T("50"), _T("63"), _T("52") };   //좌표 받기
		CString Ny[19] = { _T("127"), _T("121"), _T("124"), _T("134"), _T("131"), _T("106"), _T("106"), _T("100"), _T("106"), _T("94"), _T("90"), _T("84"), _T("77"), _T("76"), _T("74"), _T("66"), _T("67"),_T("89"), _T("38") };

		CMarkup xml;
		CMarkup xml2;
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



		for (i = 0; i < 19; i++) {
			CString URL1 = _T("http://newsky2.kma.go.kr/service/SecndSrtpdFrcstInfoService2/ForecastGrib?ServiceKey=NXT2s%2FE5rIIbHIQq3tOaTquJFKHEaqQWfBruGyI%2FIr6%2FNB2s1jc%2BYC82T%2Bun%2BxuGvxdlbGFiCnZ%2BqXUpXsE%2Fjw%3D%3D&base_date=") + index.ch_baseDate + _T("&base_time=") + index.ch_baseTime + _T("&nx=") + Nx[i] + _T("&ny=") + Ny[i] + _T("&pageNo=1&numOfRows=8");//초단기
			CString URL2 = _T("http://newsky2.kma.go.kr/service/SecndSrtpdFrcstInfoService2/ForecastSpaceData?serviceKey=VNtCb%2FWlpQHfMucBsHGSC%2BOYRnIx%2Fvy2DKjr4sXvh4qDgMUt9vcIxH77xVgVnshG%2Bd7eq1lSj5OHUS6FxonzVw%3D%3D&base_date=") + index.do_baseDate + _T("&base_time=") + index.do_baseTime + _T("&nx=") + Nx[i] + _T("&ny=") + Ny[i] + _T("&pageNo=1&numOfRows=24");//동네예보
			xml.URLLoad(URL1, 0);
			xml2.URLLoad(URL2, 1);

			xml.Load(_T("sample1.xml"));
			xml2.Load(_T("sample2.xml"));

			pArg->tempA[i].T1H = getWeather_info(xml, 기온, 0);
			pArg->tempA[i].RN1 = getWeather_info(xml, 강수량, 0);
			pArg->tempA[i].REH = getWeather_info(xml, 습도, 0);
			pArg->tempA[i].VEC = getWeather_info(xml, 풍향, 0);
			pArg->tempA[i].WSD = getWeather_info(xml, 풍속, 0);
			pArg->tempA[i].SKY = getWeather_info(xml2, 하늘상태, 1);
			pArg->tempA[i].POP = getWeather_info(xml2, 강수확률, 1);
			//   pArg->tempA[i].TMN = getWeather_info(xml2, 아침최저기온, 1);
			//   pArg->tempA[i].TMX = getWeather_info(xml2, 낮최고기온, 1);
		}

		Sleep(36000000);      //60초 *60 ms

		return 0;
	}
}
BOOL CtermpView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CtermpView 그리기

void CtermpView::OnDraw(CDC* pDC)
{		
	CtermpDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//CRect rect1()
	temp.Format(_T("기온:%s"), arg1.tempA[viewmode].T1H);
	rain.Format(_T("강수량:%s"), arg1.tempA[viewmode].RN1);
	humid.Format(_T("습도:%s"), arg1.tempA[viewmode].REH);
	wind.Format(_T("풍향:%s"), arg1.tempA[viewmode].VEC);
	windv.Format(_T("풍속:%s"), arg1.tempA[viewmode].WSD);
	sky.Format(_T("하늘상태:%s"), arg1.tempA[viewmode].SKY);
	rainP.Format(_T("강수확률:%s"), arg1.tempA[viewmode].POP);
	//tempL.Format(_T("아침 최저기온:%s"), arg1.tempA[viewmode].TMN);
	//tempH.Format(_T("낮 최고기온:%s"), arg1.tempA[viewmode].TMX);

	CFont cfont;
	cfont.CreateFontW(12, NULL, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T(
		"굴림체"));
	CDC dcmem;
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


	pDC->SetBkMode(TRANSPARENT);
	
	if (viewmode == 1)
	{
		bitmap.LoadBitmapW(IDB_BITMAP2);
		text_x = 145;
		text_y = 125;
		wher.Format( _T("서울"));
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
		if (wher_x < 120) { wher.Format(_T("목포")); text_x = 66; text_y = 539; }
		else { wher.Format(_T("여수")); text_x = 210; text_y = 512; }
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
		if (wher_x < 380) { wher.Format(_T("안동")); text_x = 344; text_y = 265; }
		else { wher.Format(_T("포항")); text_x = 405; text_y = 320; }
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
		text_x = 149;
		text_y = 175;
		wher.Format(_T("수원"));
	}
	else if (viewmode == 8)
	{
		bitmap.LoadBitmapW(IDB_BITMAP17);
		text_x = 115;
		text_y = 435;
		wher.Format(_T("광주"));
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
		if (wher_x >343) { wher.Format(_T("강릉")); text_x = 347; text_y = 99; }
		else { wher.Format(_T("춘천")); text_x = 269; text_y = 86; }
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
		//pDC->StretchBlt(10, 10, bmpinfo.bmWidth, bmpinfo.bmHeight, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);
		text_x = 311;
		text_y = 361;
		wher.Format(_T("대구"));
	}
	else if (viewmode == 15)
	{
		bitmap.LoadBitmapW(IDB_BITMAP16);
		//pDC->StretchBlt(10, 10, bmpinfo.bmWidth, bmpinfo.bmHeight, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);
		text_x = 413;
		text_y = 400;
		wher.Format(_T("울산"));
	}
	else if (viewmode == 16)
	{
		bitmap.LoadBitmapW(IDB_BITMAP17);
		//pDC->StretchBlt(10, 10, bmpinfo.bmWidth, bmpinfo.bmHeight, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);
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
	else
	{
			bitmap.LoadBitmapW(IDB_BITMAP1);
	}
	
	BITMAP bmpinfo;
	bitmap.GetBitmap(&bmpinfo);
	dcmem.CreateCompatibleDC(pDC);
	dcmem.SelectObject(&bitmap);
	pDC->StretchBlt(10, 10, bmpinfo.bmWidth, bmpinfo.bmHeight, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);

	//

//	arg1.tempA[1].SKY = _T("2");
//	arg1.tempA[1].SKY = _T("1");


	for (int i = 0; i < 19; i++){
		
		if (i == 0){
			if (arg1.tempA[i].SKY == _T("1")){
				bitSeoul.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitSeoul.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("3"))
				bitSeoul.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("4"))
				bitSeoul.LoadBitmapW(IDB_BITMAP21);

		}
		else if (i == 1){
			if (arg1.tempA[i].SKY == _T("1"))
				bitSuwon.LoadBitmapW(IDB_BITMAP19);
			else if (arg1.tempA[i].SKY == _T("2"))
				bitSuwon.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("3"))
				bitSuwon.LoadBitmapW(IDB_BITMAP20);
			else  if (arg1.tempA[i].SKY == _T("4"))
				bitSuwon.LoadBitmapW(IDB_BITMAP21);
		}
		else if (i == 2){
			if (arg1.tempA[i].SKY == _T("1")){
				bitIncheon.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitIncheon.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("3"))
				bitIncheon.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("4"))
				bitIncheon.LoadBitmapW(IDB_BITMAP21);
		}
		else if (i == 3){
			if (arg1.tempA[i].SKY == _T("1")){
				bitChuncheon.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitChuncheon.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("3"))
				bitChuncheon.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("4"))
				bitChuncheon.LoadBitmapW(IDB_BITMAP21);
		}
		else if (i == 4){
			if (arg1.tempA[i].SKY == _T("1")){
				bitChuncheon.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitChuncheon.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("3"))
				bitChuncheon.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("4"))
				bitChuncheon.LoadBitmapW(IDB_BITMAP21);
		}
		else if (i == 5){
			if (arg1.tempA[i].SKY == _T("1")){
				bitGangneung.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitGangneung.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("3"))
				bitGangneung.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("4"))
				bitGangneung.LoadBitmapW(IDB_BITMAP21);
		}
		else if (i == 6){
			if (arg1.tempA[i].SKY == _T("1")){
				bitHong.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitHong.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("3"))
				bitHong.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("4"))
				bitHong.LoadBitmapW(IDB_BITMAP21);
		}
		else if (i == 7){
			if (arg1.tempA[i].SKY == _T("1")){
				bitCheong.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitCheong.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("3"))
				bitCheong.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("4"))
				bitCheong.LoadBitmapW(IDB_BITMAP21);

		}
		else if (i == 8){
			if (arg1.tempA[i].SKY == _T("1")){
				bitDaejeon.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitDaejeon.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("3"))
				bitDaejeon.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("4"))
				bitDaejeon.LoadBitmapW(IDB_BITMAP21);

		}
		else if (i == 9){
			if (arg1.tempA[i].SKY == _T("1")){
				bitAndong.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitAndong.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("3"))
				bitAndong.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("4"))
				bitAndong.LoadBitmapW(IDB_BITMAP21);

		}
		else if (i == 10){
			if (arg1.tempA[i].SKY == _T("1")){
				bitPohang.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitPohang.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("3"))
				bitPohang.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("4"))
				bitPohang.LoadBitmapW(IDB_BITMAP21);

		}
		else if (i == 11){
			if (arg1.tempA[i].SKY == _T("1")){
				bitDaegu.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitDaegu.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("3"))
				bitDaegu.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("4"))
				bitDaegu.LoadBitmapW(IDB_BITMAP21);

		}
		else if (i == 12){
			if (arg1.tempA[i].SKY == _T("1")){
				bitUlsan.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitUlsan.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("3"))
				bitUlsan.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("4"))
				bitUlsan.LoadBitmapW(IDB_BITMAP21);

		}
		else if (i == 13){
			if (arg1.tempA[i].SKY == _T("1")){
				bitChangwon.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitChangwon.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("3"))
				bitChangwon.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("4"))
				bitChangwon.LoadBitmapW(IDB_BITMAP21);

		}
		else if (i == 14){
			if (arg1.tempA[i].SKY == _T("1")){
				bitBusan.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("2"))
				bitBusan.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("3"))
				bitBusan.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("4"))
				bitBusan.LoadBitmapW(IDB_BITMAP21);

		}
		else if (i == 15){
			if (arg1.tempA[i].SKY == _T("1")){
				bitGwangju.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("1"))
				bitGwangju.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("1"))
				bitGwangju.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("1"))
				bitGwangju.LoadBitmapW(IDB_BITMAP21);

		}
		else if (i == 16){
			if (arg1.tempA[i].SKY == _T("1")){
				bitYeosu.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("1"))
				bitYeosu.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("1"))
				bitYeosu.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("1"))
				bitYeosu.LoadBitmapW(IDB_BITMAP21);

		}
		else if (i == 17){
			if (arg1.tempA[i].SKY == _T("1")){
				bitMokpo.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("1"))
				bitMokpo.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("1"))
				bitMokpo.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("1"))
				bitMokpo.LoadBitmapW(IDB_BITMAP21);

		}
		else if (i == 18){
			if (arg1.tempA[i].SKY == _T("1")){
				bitJeonju.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("1"))
				bitJeonju.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("1"))
				bitJeonju.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("1"))
				bitJeonju.LoadBitmapW(IDB_BITMAP21);

		}
		else if (i == 19){
			if (arg1.tempA[i].SKY == _T("1")){
				bitJeju.LoadBitmapW(IDB_BITMAP19);
			}
			else if (arg1.tempA[i].SKY == _T("1"))
				bitJeju.LoadBitmapW(IDB_BITMAP18);
			else if (arg1.tempA[i].SKY == _T("1"))
				bitJeju.LoadBitmapW(IDB_BITMAP20);
			else if (arg1.tempA[i].SKY == _T("1"))
				bitJeju.LoadBitmapW(IDB_BITMAP21);

		}

	}
	BITMAP bmpinfo1;
	bitmap.GetBitmap(&bmpinfo1);


	dcmem.SelectObject(&bitSeoul);
	pDC->StretchBlt(145, 130, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);//서울

	dcmem.SelectObject(&bitSuwon);
	pDC->StretchBlt(120, 190, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);

	dcmem.SelectObject(&bitIncheon);
	pDC->StretchBlt(70, 130, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);//인천

	dcmem.SelectObject(&bitChuncheon);
	pDC->TransparentBlt(269, 101, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//춘천

	dcmem.SelectObject(&bitGangneung);
	pDC->TransparentBlt(347, 114, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//강

	dcmem.SelectObject(&bitHong);
	pDC->TransparentBlt(94, 294, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//홍성

	dcmem.SelectObject(&bitCheong);
pDC->TransparentBlt(225, 263, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//청

dcmem.SelectObject(&bitDaejeon);
pDC->TransparentBlt(191, 308, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//대전

dcmem.SelectObject(&bitAndong);
pDC->TransparentBlt(344, 286, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//안동	

dcmem.SelectObject(&bitPohang);
pDC->TransparentBlt(405, 335, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//포항

dcmem.SelectObject(&bitDaegu);
pDC->TransparentBlt(311, 376, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//대구

dcmem.SelectObject(&bitUlsan);
pDC->TransparentBlt(413, 415, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//울

dcmem.SelectObject(&bitChangwon);
pDC->TransparentBlt(299, 492, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//창원

dcmem.SelectObject(&bitBusan);
pDC->TransparentBlt(372, 474, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//부

dcmem.SelectObject(&bitGwangju);
pDC->TransparentBlt(115, 450, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//광

dcmem.SelectObject(&bitYeosu);
pDC->TransparentBlt(210, 527, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//여	

dcmem.SelectObject(&bitMokpo);
pDC->TransparentBlt(66, 554, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//목

dcmem.SelectObject(&bitJeonju);
pDC->TransparentBlt(143, 396, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//전주

dcmem.SelectObject(&bitJeju);
pDC->TransparentBlt(95, 649, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//제주



	if(viewmode!=0)pDC->TextOutW(text_x, text_y, wher);
		
	if (viewmode == 0)
	{
		pDC->TextOut(347, 99, _T("강릉"));
		pDC->TextOut(269, 86, _T("춘천"));
		pDC->TextOut(413, 400, _T("울산"));
		pDC->TextOut(372, 459, _T("부산"));
		pDC->TextOut(299, 477, _T("창원"));
		pDC->TextOut(143, 381, _T("전주"));
		pDC->TextOut(191, 293, _T("대전"));
		pDC->TextOut(94, 279, _T("홍성"));
		pDC->TextOut(311, 361, _T("대구"));
		pDC->TextOut(344, 265, _T("안동"));
		pDC->TextOut(405, 320, _T("포항"));
		pDC->TextOutW(95, 634, _T("제주"));
		pDC->TextOut(115, 435, _T("광주"));
		pDC->TextOut(66, 539, _T("목포"));
		pDC->TextOut(210, 512, _T("여수"));
		pDC->TextOut(218, 248, _T("청주"));
		pDC->TextOut(70, 115, _T("인천"));
		pDC->TextOut(145, 125, _T("서울"));
		pDC->TextOut(149, 175, _T("수원"));


	/*	pDC->TextOut(600, 600, temp);
		pDC->TextOut(620, 620, sky);
		pDC->TextOut(610, 610, arg1.tempA[0].POP);
		pDC->TextOut(620, 620, arg1.tempA[1].SKY);
		pDC->TextOut(630, 630, arg1.tempA[1].T1H);*/
	}
	}
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.


// CtermpView 인쇄
BOOL CtermpView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CtermpView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CtermpView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

// CtermpView 진단

#ifdef _DEBUG
void CtermpView::AssertValid() const
{
	CView::AssertValid();
}

void CtermpView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CtermpDoc* CtermpView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtermpDoc)));
	return (CtermpDoc*)m_pDocument;
}
#endif //_DEBUG

// CtermpView 메시지 처리기

void CtermpView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	::GetCursorPos(&point);
	ScreenToClient(&point);
	Invalidate(FALSE);
	
	COLORREF color;
	CClientDC dc(this);
	color = dc.GetPixel(point);
	R = GetRValue(color);
	G = GetGValue(color);
	B = GetBValue(color);
	/*CString str;
	str.Format(_T("%d, %d, %d, %d, %d"),point.x,point.y,R,G,B);
	::AfxMessageBox(str);*/

	if ((R==196)&&(G==196)&&(B==196))
		{//서울
				index.NX = "55";
				index.NY = "127";
				viewmode = 1;
		}
	else if((R==63)&&(G==72)&&(B==204))
		{//충북	
				index.NX = "69";
				index.NY = "107";
				viewmode = 2;
		}
			else if ((R == 163) && (G == 73) && (B == 164))
			{//전남
				index.NX = "51";
				index.NY = "67";
				viewmode = 3;
				wher_x = point.x;
				wher_y = point.y;
			}
	else if ((R == 166) && (G == 252) && (B == 164))
		{//제주
			index.NX = "52";
			index.NY = "38";
			viewmode = 4;
		}
		else if ((R ==237) && (G == 28) && (B == 36))
		{//경북
			index.NX = "89";
			index.NY = "91";
			viewmode = 5;
			wher_x = point.x;
			wher_y = point.y;
		}
	else if ((239<=R&&R<=243)&&(237<=G&&G<=241)&&(189<=B&&B<=193))
	{//수원
		index.NX = "60";
		index.NY = "120";
		viewmode = 7;
	}
	else if ((220<=R&&R<=224)&&(214<=G&&G<=218)&&(166<=B&&B<=170))
	{//인천
		index.NX = "55";
		index.NY = "124";
		viewmode = 6;
	}
	else if ((R ==226) && (G == 237) && (B == 226))
	{//광주
		index.NX = "58";
		index.NY = "74";
		viewmode = 16;
	}
	else if ((R == 212) && (G == 244) && (B == 242))
	{//대전
		index.NX = "67";
		index.NY = "100";
		viewmode = 9;
	}
	else if ((R == 187) && (G == 186) && (B == 226))
	{//전북
		index.NX = "63";
		index.NY = "89";
		viewmode = 10;
	}
	else if ((R == 229) && (G == 193) && (B == 226))
	{//경남
		index.NX = "91";
		index.NY = "77";
		viewmode = 11;
	}
	
	else if ((R == 185) && (G == 122) && (B == 87))
	{
		//울산
		index.NX = "102";
		index.NY = "84";
		viewmode = 15;
	}
		else if ((239<=R&&R<=243)&&(213<=G&&G<=217)&&(240<=B&&B<=244))
	{//부산
		index.NX = "59";
		index.NY = "65";
		viewmode = 13;
	}
	else if ((R == 255) && (G == 242) && (B == 0))
	{//대구
		index.NX = "89";
		index.NY = "90";
		viewmode = 14;
	}
	else if ((R == 174) && (G == 229) && (B == 227))
	{//홍성
		XX[0] = "67", YY[0] = "100";
		XX[1] = "55", YY[1] = "106";
		index.NX =XX[0];
		index.NY =YY[0];
		viewmode =	17;
	}
	else if ((R == 244) && (G == 179) && (B == 179))
	{//강원
		index.NX = "73";
		index.NY = "134";
		viewmode = 12;
		wher_x = point.x;
		wher_y = point.y;
	}





	if (m_pDlg != NULL) {
		//m_pDlg->SetFocus(); // 키보드 포커스를 대화상자로 옮긴다.	
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

	

	m_pDlg->m_Picture.Load(_T("sunny.gif"));// 파일에서 로드하는 방법입니다 ~
	m_pDlg->m_Picture.Draw();
	m_pDlg->m_list.AddString(temp); // IDC_LIST 컨트롤의 초기값 리스트에 값들 저장
	m_pDlg->m_list.AddString(rain); // IDC_STR 컨트롤의 초기값
	m_pDlg->m_list.AddString(humid); // IDC_STR 컨트롤의 초기값
	m_pDlg->m_list.AddString(wind); // IDC_STR 컨트롤의 초기값
	m_pDlg->m_list.AddString(windv); // IDC_STR 컨트롤의 초기값	
	m_pDlg->m_list.AddString(sky); // IDC_STR 컨트롤의 초기값
	m_pDlg->m_list.AddString(rainP); // IDC_STR 컨트롤의 초기값
	//m_pDlg->m_list.AddString(tempL); // IDC_STR 컨트롤의 초기값
	//m_pDlg->m_list.AddString(tempH); // IDC_STR 컨트롤의 초기값	
	//CString strtemp, strtempL;

	//m_pDlg->m_tempT.GetWindowText(strtemp);
	//m_pDlg->m_tempLT.GetWindowText(strtempL);
	//m_pDlg->m_tempT.SetWindowText(temp);
	//m_pDlg->m_tempLT.SetWindowText(tempL);

	m_pDlg->ShowWindow(SW_SHOW); // 대화상자가 보이게 한다.
	
	Invalidate(FALSE);
}

BOOL CtermpView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nHitTest == HTCLIENT) {
		CPoint point;

		::GetCursorPos(&point);
		ScreenToClient(&point);
		COLORREF color;
		CClientDC dc(this);
		color = dc.GetPixel(point);
		R = GetRValue(color);
		G = GetGValue(color);
		B = GetBValue(color);
		if (R==255&&G==255)
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		else 
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
		return TRUE;
	}
	return CView::OnSetCursor(pWnd, nHitTest, message);
}



void CtermpView::OnSetFocus(CWnd* pOldWnd)
{
	//CView::OnSetFocus(pOldWnd);
	viewmode = 0;
	Invalidate(FALSE);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


int CtermpView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	arg1.pWnd = this; // 뷰 객체의 주소
	arg1.pos = 1;
	pThread1 = AfxBeginThread(Thread_Parsing, &arg1, THREAD_PRIORITY_TIME_CRITICAL, 0, CREATE_SUSPENDED);
	// 두 번째 스레드의 우선순위 레벨을 변경한다.
	//pThread1->SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	// 두 스레드의 실행을 재개한다.
	pThread1->ResumeThread();
	return 0;
}
