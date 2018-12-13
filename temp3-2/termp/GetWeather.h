#pragma once

#include "stdafx.h"
#include "Markup.h"

#define �����̸� _T("Region")
#define X��ǥ _T("NX")
#define Y��ǥ _T("NY")
//#define ��ǥ���� _T("baseDate")
//#define ��ǥ�ð� _T("baseTime")
#define ��� _T("T1H")
#define ������ _T("RN1")
#define �����ٶ� _T("UUU")
#define ���Ϲٶ� _T("VVV")
#define ���� _T("REH")
#define �������� _T("PTY")
#define ǳ�� _T("VEC")
#define ǳ�� _T("WSD")
#define �ϴû��� _T("SKY")
#define ����Ȯ�� _T("POP")
#define ��ħ������� _T("TMN")
#define ���ְ��� _T("TMX")


struct KWeather_index {
	CString Region;			// ���� �̸�
	CString NX;				// x��ǥ
	CString NY;				 // y��ǥ
	CString ch_baseDate;
	CString ch_baseTime;
	CString do_baseDate;
	CString do_baseTime;
};

struct KWeather_info {
	CString T1H;				// ���
	CString RN1;				// 1�ð� ������
	CString UUU;			// �����ٶ�����
	CString VVV;				// ���Ϲٶ�����
	CString REH;				// ����
	CString PTY;				 // ��������
	CString VEC;				// ǳ��
	CString WSD;			// ǳ��
	CString SKY;			//�ϴû���
	CString POP;		//����Ȯ��
	CString TMN;			//��ħ�������
	CString TMX;		//���ְ���
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