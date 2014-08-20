
// Monte_Carlo_GUI.h : main header file for the Monte_Carlo_GUI application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMonte_Carlo_GUIApp:
// See Monte_Carlo_GUI.cpp for the implementation of this class
//

class CMonte_Carlo_GUIApp : public CWinAppEx
{
public:
	CMonte_Carlo_GUIApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMonte_Carlo_GUIApp theApp;
