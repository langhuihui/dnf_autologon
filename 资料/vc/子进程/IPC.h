// IPC.h : main header file for the IPC application
//

#if !defined(AFX_IPC_H__81909895_804B_4B9C_9757_6F9D43960E30__INCLUDED_)
#define AFX_IPC_H__81909895_804B_4B9C_9757_6F9D43960E30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIPCApp:
// See IPC.cpp for the implementation of this class
//

class CIPCApp : public CWinApp
{
public:
	CIPCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIPCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPC_H__81909895_804B_4B9C_9757_6F9D43960E30__INCLUDED_)
