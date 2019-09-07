// loginUser.h : main header file for the LOGINUSER application
//

#if !defined(AFX_LOGINUSER_H__7FDA8CC1_B012_40E6_8E58_901EADD32E47__INCLUDED_)
#define AFX_LOGINUSER_H__7FDA8CC1_B012_40E6_8E58_901EADD32E47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLoginUserApp:
// See loginUser.cpp for the implementation of this class
//

class CLoginUserApp : public CWinApp
{
public:
	CLoginUserApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginUserApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLoginUserApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINUSER_H__7FDA8CC1_B012_40E6_8E58_901EADD32E47__INCLUDED_)
