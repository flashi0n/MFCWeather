#pragma once

#include "stdafx.h"
#include "Markup.h"

#define 지역이름 _T("Region")
#define X좌표 _T("NX")
#define Y좌표 _T("NY")
//#define 발표일자 _T("baseDate")
//#define 발표시간 _T("baseTime")
#define 기온 _T("T1H")
#define 강수량 _T("RN1")
#define 동서바람 _T("UUU")
#define 남북바람 _T("VVV")
#define 습도 _T("REH")
#define 강수형태 _T("PTY")
#define 풍향 _T("VEC")
#define 풍속 _T("WSD")
#define 하늘상태 _T("SKY")
#define 강수확률 _T("POP")
#define 아침최저기온 _T("TMN")
#define 낮최고기온 _T("TMX")


struct KWeather_index {
	CString Region;			// 지역 이름
	CString NX;				// x좌표
	CString NY;				 // y좌표
	CString ch_baseDate;
	CString ch_baseTime;
	CString do_baseDate;
	CString do_baseTime;
};

struct KWeather_info {
	CString T1H;				// 기온
	CString RN1;				// 1시간 강수량
	CString UUU;			// 동서바람성분
	CString VVV;				// 남북바람성분
	CString REH;				// 습도
	CString PTY;				 // 강수형태
	CString VEC;				// 풍향
	CString WSD;			// 풍속
	CString SKY;			//하늘상태
	CString POP;		//강수확률
	CString TMN;			//아침최저기온
	CString TMX;		//낮최고기온
};

using namespace std;

class Date
{
	int year;
	int month;
	int day;
public:
	void setDate(const int& year_input, const int& month_input, const int& day_input)
	{
		year = year_input;
		month = month_input;
		day = day_input;
	}
	
};


CString getWeather_info(CMarkup xml, MCD_CSTR factor, char mode);