#if !defined(AFX_IPCFORMDIALOG_H__2FB8AF52_0034_4C7B_89B8_2413097217C7__INCLUDED_)
#define AFX_IPCFORMDIALOG_H__2FB8AF52_0034_4C7B_89B8_2413097217C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IPCFormDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// IPCFormDialog dialog

class IPCFormDialog : public CDialog
{
// Construction
public:
	IPCFormDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(IPCFormDialog)
	enum { IDD = IDD_IPCFormDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(IPCFormDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(IPCFormDialog)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	//afx_msg LRESULT onCallIPCAnswer(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPCFORMDIALOG_H__2FB8AF52_0034_4C7B_89B8_2413097217C7__INCLUDED_)
