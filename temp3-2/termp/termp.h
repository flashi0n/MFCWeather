
// termp.h : termp ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CtermpApp:
// �� Ŭ������ ������ ���ؼ��� termp.cpp�� �����Ͻʽÿ�.
//

class CtermpApp : public CWinApp
{
public:
	CtermpApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CtermpApp theApp;
