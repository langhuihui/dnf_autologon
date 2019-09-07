#if !defined(AFX_IPCFORMDLG_H__01D9770D_EA1A_4432_9AEC_B82A8540FC79__INCLUDED_)
#define AFX_IPCFORMDLG_H__01D9770D_EA1A_4432_9AEC_B82A8540FC79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IPCFormDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIPCFormDLG dialog

class CIPCFormDLG : public CDialog
{
// Construction
public:
	CIPCFormDLG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIPCFormDLG)
	enum { IDD = IDD_IPCForm };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPCFormDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIPCFormDLG)
	afx_msg void OnButton1();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	//afx_msg LRESULT onCallIPCAnswer(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPCFORMDLG_H__01D9770D_EA1A_4432_9AEC_B82A8540FC79__INCLUDED_)
