
// termpView.cpp : CtermpView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_CONTEXTMENU()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CtermpView ����/�Ҹ�
KWeather_index index;
KWeather_info info;

CtermpView::CtermpView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	m_pDlg = NULL;
	nCount = FALSE;
	viewmode = 0;
}

CtermpView::~CtermpView()
{
}
UINT Thread_Parsing(LPVOID arg) {

	//   CtermpView* tdata;   //view�� tdata(tiem) ��ü ����
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

		//������ �Ľ�

		CString Nx[19] = { _T("60"),_T("60"),_T("55"),_T("73"),_T("92"),_T("55"),_T("69"),_T("67"),_T("91"), _T("102"),_T("89"),_T("102"),_T("90"),_T("78"),_T("58"),_T("73"),_T("50"), _T("63"), _T("52") };   //��ǥ �ޱ�
		CString Ny[19] = { _T("127"), _T("121"), _T("124"), _T("134"), _T("131"), _T("106"), _T("106"), _T("100"), _T("106"), _T("94"), _T("90"), _T("84"), _T("77"), _T("76"), _T("74"), _T("66"), _T("67"),_T("89"), _T("38") };

		CMarkup xml;
		CMarkup xml2;
		//�ð� �ޱ�
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

		while ((i_curTime - 1000) < 0) // ���ڿ� 0 ���̱�
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
		// ���׿��� �ޱ� 3�ð� ������ �ð� ������
		CString d_curTime;
		int t_curTime = (ii_curTime % 300) / 100;
		//int t_curTime = (ii_curTime / 100) % 3;
		if (t_curTime == 0) {
			if (ii_curTime - 100 >= 0) {
				ii_curTime = ii_curTime / 100;//�� ���ֱ�
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
			CString URL1 = _T("http://newsky2.kma.go.kr/service/SecndSrtpdFrcstInfoService2/ForecastGrib?ServiceKey=NXT2s%2FE5rIIbHIQq3tOaTquJFKHEaqQWfBruGyI%2FIr6%2FNB2s1jc%2BYC82T%2Bun%2BxuGvxdlbGFiCnZ%2BqXUpXsE%2Fjw%3D%3D&base_date=") + index.ch_baseDate + _T("&base_time=") + index.ch_baseTime + _T("&nx=") + Nx[i] + _T("&ny=") + Ny[i] + _T("&pageNo=1&numOfRows=8");//�ʴܱ�
			CString URL2 = _T("http://newsky2.kma.go.kr/service/SecndSrtpdFrcstInfoService2/ForecastSpaceData?serviceKey=VNtCb%2FWlpQHfMucBsHGSC%2BOYRnIx%2Fvy2DKjr4sXvh4qDgMUt9vcIxH77xVgVnshG%2Bd7eq1lSj5OHUS6FxonzVw%3D%3D&base_date=") + index.do_baseDate + _T("&base_time=") + index.do_baseTime + _T("&nx=") + Nx[i] + _T("&ny=") + Ny[i] + _T("&pageNo=1&numOfRows=24");//���׿���
			xml.URLLoad(URL1, 0);
			xml2.URLLoad(URL2, 1);

			xml.Load(_T("sample1.xml"));
			xml2.Load(_T("sample2.xml"));

			pArg->tempA[i].T1H = getWeather_info(xml, ���, 0);
			pArg->tempA[i].RN1 = getWeather_info(xml, ������, 0);
			pArg->tempA[i].REH = getWeather_info(xml, ����, 0);
			pArg->tempA[i].VEC = getWeather_info(xml, ǳ��, 0);
			pArg->tempA[i].WSD = getWeather_info(xml, ǳ��, 0);
			pArg->tempA[i].SKY = getWeather_info(xml2, �ϴû���, 1);
			pArg->tempA[i].POP = getWeather_info(xml2, ����Ȯ��, 1);
			//   pArg->tempA[i].TMN = getWeather_info(xml2, ��ħ�������, 1);
			//   pArg->tempA[i].TMX = getWeather_info(xml2, ���ְ���, 1);
		}

		Sleep(36000000);      //60�� *60 ms

		return 0;
	}
}
BOOL CtermpView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CtermpView �׸���

void CtermpView::OnDraw(CDC* pDC)
{		
	CtermpDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//CRect rect1()
	temp.Format(_T("���:%s"), arg1.tempA[viewmode].T1H);
	rain.Format(_T("������:%s"), arg1.tempA[viewmode].RN1);
	humid.Format(_T("����:%s"), arg1.tempA[viewmode].REH);
	wind.Format(_T("ǳ��:%s"), arg1.tempA[viewmode].VEC);
	windv.Format(_T("ǳ��:%s"), arg1.tempA[viewmode].WSD);
	sky.Format(_T("�ϴû���:%s"), arg1.tempA[viewmode].SKY);
	rainP.Format(_T("����Ȯ��:%s"), arg1.tempA[viewmode].POP);
	//tempL.Format(_T("��ħ �������:%s"), arg1.tempA[viewmode].TMN);
	//tempH.Format(_T("�� �ְ���:%s"), arg1.tempA[viewmode].TMX);

	CFont cfont;
	cfont.CreateFontW(12, NULL, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T(
		"����ü"));
	CDC dcmem;
	CBitmap bitmap;
	CBitmap bitSeoul;//����
	CBitmap bitSuwon;//����
	CBitmap bitIncheon;//��õ
	CBitmap bitChuncheon;//��õ
	CBitmap bitGangneung;//����
	CBitmap bitHong;//ȫ��
	CBitmap bitCheong;//û��
	CBitmap bitDaejeon;//����
	CBitmap bitAndong;//�ȵ�

	CBitmap bitPohang;//����
	CBitmap bitDaegu;//�뱸
	CBitmap bitUlsan;	//���
	CBitmap bitChangwon;//â��
	CBitmap bitBusan;//�λ�
	CBitmap bitGwangju;//����

	CBitmap bitYeosu;//����
	CBitmap bitMokpo;//����
	CBitmap bitJeonju;//����
	CBitmap bitJeju;//����


	pDC->SetBkMode(TRANSPARENT);
	
	if (viewmode == 1)
	{
		bitmap.LoadBitmapW(IDB_BITMAP2);
		text_x = 145;
		text_y = 125;
		wher.Format( _T("����"));
	}
	else if (viewmode == 2)
	{
		bitmap.LoadBitmapW(IDB_BITMAP4);
		text_x = 218;
		text_y = 248;
		wher.Format(_T("û��"));
	}
	else if (viewmode == 3)
	{
		bitmap.LoadBitmapW(IDB_BITMAP9);
		if (wher_x < 120) { wher.Format(_T("����")); text_x = 66; text_y = 539; }
		else { wher.Format(_T("����")); text_x = 210; text_y = 512; }
	}
	else if (viewmode == 4)
	{
		bitmap.LoadBitmapW(IDB_BITMAP10);
		text_x = 95;
		text_y = 634;
		wher.Format(_T("����"));
	}
	else if (viewmode == 5)
	{
		bitmap.LoadBitmapW(IDB_BITMAP5);
		if (wher_x < 380) { wher.Format(_T("�ȵ�")); text_x = 344; text_y = 265; }
		else { wher.Format(_T("����")); text_x = 405; text_y = 320; }
	}
	else if (viewmode == 6)
	{
		bitmap.LoadBitmapW(IDB_BITMAP12);
		text_x = 70;
		text_y = 115;
		wher.Format(_T("��õ"));
	}
	else if (viewmode == 7)
	{
		bitmap.LoadBitmapW(IDB_BITMAP11);
		text_x = 149;
		text_y = 175;
		wher.Format(_T("����"));
	}
	else if (viewmode == 8)
	{
		bitmap.LoadBitmapW(IDB_BITMAP17);
		text_x = 115;
		text_y = 435;
		wher.Format(_T("����"));
	}
	else if (viewmode == 9)
	{
		bitmap.LoadBitmapW(IDB_BITMAP13);
		text_x = 191;
		text_y = 293;
		wher.Format(_T("����"));	
	}
	else if (viewmode == 10)
	{
		bitmap.LoadBitmapW(IDB_BITMAP7);
		text_x = 143;
		text_y = 381;
		wher.Format(_T("����"));
	}
	else if (viewmode == 11)
	{
		bitmap.LoadBitmapW(IDB_BITMAP8);
		text_x = 299;
		text_y = 477;
		wher.Format(_T("â��"));
	}
	else if (viewmode == 12)
	{
		bitmap.LoadBitmapW(IDB_BITMAP3);
		if (wher_x >343) { wher.Format(_T("����")); text_x = 347; text_y = 99; }
		else { wher.Format(_T("��õ")); text_x = 269; text_y = 86; }
	}
	else if (viewmode == 13)
	{
		bitmap.LoadBitmapW(IDB_BITMAP15);
		text_x = 372;
		text_y = 459;
		wher.Format(_T("�λ�"));
	}
	else if (viewmode == 14)
	{
		bitmap.LoadBitmapW(IDB_BITMAP14);
		//pDC->StretchBlt(10, 10, bmpinfo.bmWidth, bmpinfo.bmHeight, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);
		text_x = 311;
		text_y = 361;
		wher.Format(_T("�뱸"));
	}
	else if (viewmode == 15)
	{
		bitmap.LoadBitmapW(IDB_BITMAP16);
		//pDC->StretchBlt(10, 10, bmpinfo.bmWidth, bmpinfo.bmHeight, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);
		text_x = 413;
		text_y = 400;
		wher.Format(_T("���"));
	}
	else if (viewmode == 16)
	{
		bitmap.LoadBitmapW(IDB_BITMAP17);
		//pDC->StretchBlt(10, 10, bmpinfo.bmWidth, bmpinfo.bmHeight, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);
		text_x = 115;
		text_y = 435;
		wher.Format(_T("����"));
	}
	else if (viewmode == 17)
	{
		bitmap.LoadBitmapW(IDB_BITMAP6);
		text_x = 94;
		text_y = 279;
		wher.Format(_T("ȫ��"));
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
	pDC->StretchBlt(145, 130, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);//����

	dcmem.SelectObject(&bitSuwon);
	pDC->StretchBlt(120, 190, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);

	dcmem.SelectObject(&bitIncheon);
	pDC->StretchBlt(70, 130, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);//��õ

	dcmem.SelectObject(&bitChuncheon);
	pDC->TransparentBlt(269, 101, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//��õ

	dcmem.SelectObject(&bitGangneung);
	pDC->TransparentBlt(347, 114, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//��

	dcmem.SelectObject(&bitHong);
	pDC->TransparentBlt(94, 294, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//ȫ��

	dcmem.SelectObject(&bitCheong);
pDC->TransparentBlt(225, 263, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//û

dcmem.SelectObject(&bitDaejeon);
pDC->TransparentBlt(191, 308, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//����

dcmem.SelectObject(&bitAndong);
pDC->TransparentBlt(344, 286, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//�ȵ�	

dcmem.SelectObject(&bitPohang);
pDC->TransparentBlt(405, 335, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//����

dcmem.SelectObject(&bitDaegu);
pDC->TransparentBlt(311, 376, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//�뱸

dcmem.SelectObject(&bitUlsan);
pDC->TransparentBlt(413, 415, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//��

dcmem.SelectObject(&bitChangwon);
pDC->TransparentBlt(299, 492, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//â��

dcmem.SelectObject(&bitBusan);
pDC->TransparentBlt(372, 474, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//��

dcmem.SelectObject(&bitGwangju);
pDC->TransparentBlt(115, 450, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//��

dcmem.SelectObject(&bitYeosu);
pDC->TransparentBlt(210, 527, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//��	

dcmem.SelectObject(&bitMokpo);
pDC->TransparentBlt(66, 554, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//��

dcmem.SelectObject(&bitJeonju);
pDC->TransparentBlt(143, 396, bmpinfo.bmWidth*0.05, bmpinfo1.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//����

dcmem.SelectObject(&bitJeju);
pDC->TransparentBlt(95, 649, bmpinfo.bmWidth*0.05, bmpinfo.bmHeight*0.05, &dcmem, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, RGB(0, 0, 0));//����



	if(viewmode!=0)pDC->TextOutW(text_x, text_y, wher);
		
	if (viewmode == 0)
	{
		pDC->TextOut(347, 99, _T("����"));
		pDC->TextOut(269, 86, _T("��õ"));
		pDC->TextOut(413, 400, _T("���"));
		pDC->TextOut(372, 459, _T("�λ�"));
		pDC->TextOut(299, 477, _T("â��"));
		pDC->TextOut(143, 381, _T("����"));
		pDC->TextOut(191, 293, _T("����"));
		pDC->TextOut(94, 279, _T("ȫ��"));
		pDC->TextOut(311, 361, _T("�뱸"));
		pDC->TextOut(344, 265, _T("�ȵ�"));
		pDC->TextOut(405, 320, _T("����"));
		pDC->TextOutW(95, 634, _T("����"));
		pDC->TextOut(115, 435, _T("����"));
		pDC->TextOut(66, 539, _T("����"));
		pDC->TextOut(210, 512, _T("����"));
		pDC->TextOut(218, 248, _T("û��"));
		pDC->TextOut(70, 115, _T("��õ"));
		pDC->TextOut(145, 125, _T("����"));
		pDC->TextOut(149, 175, _T("����"));


	/*	pDC->TextOut(600, 600, temp);
		pDC->TextOut(620, 620, sky);
		pDC->TextOut(610, 610, arg1.tempA[0].POP);
		pDC->TextOut(620, 620, arg1.tempA[1].SKY);
		pDC->TextOut(630, 630, arg1.tempA[1].T1H);*/
	}
	}
	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.


// CtermpView �μ�
BOOL CtermpView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CtermpView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CtermpView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

// CtermpView ����

#ifdef _DEBUG
void CtermpView::AssertValid() const
{
	CView::AssertValid();
}

void CtermpView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CtermpDoc* CtermpView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtermpDoc)));
	return (CtermpDoc*)m_pDocument;
}
#endif //_DEBUG

// CtermpView �޽��� ó����

void CtermpView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
		{//����
				index.NX = "55";
				index.NY = "127";
				viewmode = 1;
		}
	else if((R==63)&&(G==72)&&(B==204))
		{//���	
				index.NX = "69";
				index.NY = "107";
				viewmode = 2;
		}
			else if ((R == 163) && (G == 73) && (B == 164))
			{//����
				index.NX = "51";
				index.NY = "67";
				viewmode = 3;
				wher_x = point.x;
				wher_y = point.y;
			}
	else if ((R == 166) && (G == 252) && (B == 164))
		{//����
			index.NX = "52";
			index.NY = "38";
			viewmode = 4;
		}
		else if ((R ==237) && (G == 28) && (B == 36))
		{//���
			index.NX = "89";
			index.NY = "91";
			viewmode = 5;
			wher_x = point.x;
			wher_y = point.y;
		}
	else if ((239<=R&&R<=243)&&(237<=G&&G<=241)&&(189<=B&&B<=193))
	{//����
		index.NX = "60";
		index.NY = "120";
		viewmode = 7;
	}
	else if ((220<=R&&R<=224)&&(214<=G&&G<=218)&&(166<=B&&B<=170))
	{//��õ
		index.NX = "55";
		index.NY = "124";
		viewmode = 6;
	}
	else if ((R ==226) && (G == 237) && (B == 226))
	{//����
		index.NX = "58";
		index.NY = "74";
		viewmode = 16;
	}
	else if ((R == 212) && (G == 244) && (B == 242))
	{//����
		index.NX = "67";
		index.NY = "100";
		viewmode = 9;
	}
	else if ((R == 187) && (G == 186) && (B == 226))
	{//����
		index.NX = "63";
		index.NY = "89";
		viewmode = 10;
	}
	else if ((R == 229) && (G == 193) && (B == 226))
	{//�泲
		index.NX = "91";
		index.NY = "77";
		viewmode = 11;
	}
	
	else if ((R == 185) && (G == 122) && (B == 87))
	{
		//���
		index.NX = "102";
		index.NY = "84";
		viewmode = 15;
	}
		else if ((239<=R&&R<=243)&&(213<=G&&G<=217)&&(240<=B&&B<=244))
	{//�λ�
		index.NX = "59";
		index.NY = "65";
		viewmode = 13;
	}
	else if ((R == 255) && (G == 242) && (B == 0))
	{//�뱸
		index.NX = "89";
		index.NY = "90";
		viewmode = 14;
	}
	else if ((R == 174) && (G == 229) && (B == 227))
	{//ȫ��
		XX[0] = "67", YY[0] = "100";
		XX[1] = "55", YY[1] = "106";
		index.NX =XX[0];
		index.NY =YY[0];
		viewmode =	17;
	}
	else if ((R == 244) && (G == 179) && (B == 179))
	{//����
		index.NX = "73";
		index.NY = "134";
		viewmode = 12;
		wher_x = point.x;
		wher_y = point.y;
	}





	if (m_pDlg != NULL) {
		//m_pDlg->SetFocus(); // Ű���� ��Ŀ���� ��ȭ���ڷ� �ű��.	
		if (nCount == TRUE) {
		m_pDlg->m_list.ResetContent();
		}
	}
	else {
		m_pDlg = new CMyDialog3(); // ��ȭ���� ��ü�� �����Ѵ�.
		m_pDlg->m_pView = this; // �� ��ü�� �ּҸ� �Ѱ��ش�.
		m_pDlg->Create(IDD_DIALOG1); // ��ȭ���ڸ� �����Ѵ�.		
		nCount = !nCount;
		
	}

	

	m_pDlg->m_Picture.Load(_T("sunny.gif"));// ���Ͽ��� �ε��ϴ� ����Դϴ� ~
	m_pDlg->m_Picture.Draw();
	m_pDlg->m_list.AddString(temp); // IDC_LIST ��Ʈ���� �ʱⰪ ����Ʈ�� ���� ����
	m_pDlg->m_list.AddString(rain); // IDC_STR ��Ʈ���� �ʱⰪ
	m_pDlg->m_list.AddString(humid); // IDC_STR ��Ʈ���� �ʱⰪ
	m_pDlg->m_list.AddString(wind); // IDC_STR ��Ʈ���� �ʱⰪ
	m_pDlg->m_list.AddString(windv); // IDC_STR ��Ʈ���� �ʱⰪ	
	m_pDlg->m_list.AddString(sky); // IDC_STR ��Ʈ���� �ʱⰪ
	m_pDlg->m_list.AddString(rainP); // IDC_STR ��Ʈ���� �ʱⰪ
	//m_pDlg->m_list.AddString(tempL); // IDC_STR ��Ʈ���� �ʱⰪ
	//m_pDlg->m_list.AddString(tempH); // IDC_STR ��Ʈ���� �ʱⰪ	
	//CString strtemp, strtempL;

	//m_pDlg->m_tempT.GetWindowText(strtemp);
	//m_pDlg->m_tempLT.GetWindowText(strtempL);
	//m_pDlg->m_tempT.SetWindowText(temp);
	//m_pDlg->m_tempLT.SetWindowText(tempL);

	m_pDlg->ShowWindow(SW_SHOW); // ��ȭ���ڰ� ���̰� �Ѵ�.
	
	Invalidate(FALSE);
}

BOOL CtermpView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


int CtermpView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	arg1.pWnd = this; // �� ��ü�� �ּ�
	arg1.pos = 1;
	pThread1 = AfxBeginThread(Thread_Parsing, &arg1, THREAD_PRIORITY_TIME_CRITICAL, 0, CREATE_SUSPENDED);
	// �� ��° �������� �켱���� ������ �����Ѵ�.
	//pThread1->SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	// �� �������� ������ �簳�Ѵ�.
	pThread1->ResumeThread();
	return 0;
}
