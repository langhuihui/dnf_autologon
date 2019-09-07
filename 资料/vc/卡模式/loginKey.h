// loginKey.h : main header file for the LOGINKEY application
//

#if !defined(AFX_LOGINKEY_H__3B44F10E_E59F_4463_8A40_4FEDD854F20F__INCLUDED_)
#define AFX_LOGINKEY_H__3B44F10E_E59F_4463_8A40_4FEDD854F20F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLoginKeyApp:
// See loginKey.cpp for the implementation of this class
//

class CLoginKeyApp : public CWinApp
{
public:
	CLoginKeyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginKeyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLoginKeyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINKEY_H__3B44F10E_E59F_4463_8A40_4FEDD854F20F__INCLUDED_)
