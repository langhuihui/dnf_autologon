// loginKey.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "loginKey.h"
#include "loginKeyDlg.h"
#include "ksreglib.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginKeyApp

BEGIN_MESSAGE_MAP(CLoginKeyApp, CWinApp)
	//{{AFX_MSG_MAP(CLoginKeyApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginKeyApp construction

CLoginKeyApp::CLoginKeyApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLoginKeyApp object

CLoginKeyApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLoginKeyApp initialization

BOOL CLoginKeyApp::InitInstance()
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


	ksLoadDLLs(); //����������������DLL
	CLoginKeyDlg dlg;
	//m_pMainWnd = &dlg;   �����Ҫע�͵���
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK

		myISRunning = true; //�����Ѿ���ʽ��½��,������һ���쳣�˳���Ҫ����ks_exit����һЩ����
	    if(dlg.m_LoginTypeID == 1)
		{
			IPCFormDialog ipcDlg;
			m_pMainWnd = &ipcDlg;
			ipcDlg.DoModal();
		}
		else
		{
			CYourFormDialog yourDlg;
			m_pMainWnd = &yourDlg;
			yourDlg.DoModal();
		}
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	ksFreeDLLs();//�������������ж��DLL



	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
