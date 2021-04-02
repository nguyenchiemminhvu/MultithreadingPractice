
// mfc_thread_practice.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// mfc_thread_practice_app:
// See mfc_thread_practice.cpp for the implementation of this class
//

class mfc_thread_practice_app : public CWinApp
{
public:
	mfc_thread_practice_app();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern mfc_thread_practice_app theApp;
