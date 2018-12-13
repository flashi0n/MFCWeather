
#include "stdafx.h"
#include "GetWeather.h"

CString getWeather_info(CMarkup xml, MCD_CSTR factor, char mode) {

	CString cs;

	xml.FindElem(_T("response"));
	xml.IntoElem();
	xml.FindElem(_T("body"));
	xml.IntoElem();
	xml.FindElem(_T("items"));
	xml.IntoElem();



	while (1)
	{
		xml.FindElem(_T("item"));
		xml.IntoElem();
		xml.FindElem(_T("category"));
		cs = xml.GetData();
		if (cs == factor)
		{
			if (mode == 0)
				xml.FindElem(_T("obsrValue"));      //초단기
			else if (mode == 1)
				xml.FindElem(_T("fcstValue"));      //동네

			cs = xml.GetData();
			break;
		}
		xml.OutOfElem();
	}


	return cs;
}

