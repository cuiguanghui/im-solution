
// IMRichEditTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CIMRichEditTestApp:
// �йش����ʵ�֣������ IMRichEditTest.cpp
//

class CIMRichEditTestApp : public CWinApp
{
public:
	CIMRichEditTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CIMRichEditTestApp theApp;