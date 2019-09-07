// loginUser.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "loginUser.h"
#include "loginUserDlg.h"
#include "IPCFormDLG.h"
#include "YourFormDLG.h"
#include "ksreglib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginUserApp

BEGIN_MESSAGE_MAP(CLoginUserApp, CWinApp)
	//{{AFX_MSG_MAP(CLoginUserApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginUserApp construction

CLoginUserApp::CLoginUserApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLoginUserApp object

CLoginUserApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLoginUserApp initialization

BOOL CLoginUserApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif









	ksLoadDLLs(); //★★★★★★★★★★★★加载DLL
	CLoginUserDlg dlg;
	//m_pMainWnd = &dlg;   这句需要注释掉！
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
		myISRunning = true; //代表已经正式登陆了,这样，一旦异常退出就要调用ks_exit来做一些工作
	    if(dlg.m_Tab01.m_LoginTypeID == 1)
		{
			CIPCFormDLG ipcDlg;
			m_pMainWnd = &ipcDlg;
			ipcDlg.DoModal();
		}
		else
		{
			CYourFormDLG yourDlg;
			m_pMainWnd = &yourDlg;
			yourDlg.DoModal();
		}
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	//
	ksFreeDLLs();//★★★★★★★★★★★★卸载DLL





	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
