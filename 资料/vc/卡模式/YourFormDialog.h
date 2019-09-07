#if !defined(AFX_YOURFORMDIALOG_H__2F770A1F_9BDB_4925_8D81_60E1A9699EC3__INCLUDED_)
#define AFX_YOURFORMDIALOG_H__2F770A1F_9BDB_4925_8D81_60E1A9699EC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YourFormDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CYourFormDialog dialog

class CYourFormDialog : public CDialog
{
// Construction
public:
	CYourFormDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CYourFormDialog)
	enum { IDD = IDD_YourForm };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYourFormDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CYourFormDialog)
	afx_msg void Onbtn1();
	afx_msg void Onbtn2();
	afx_msg void Onbtn3();
	afx_msg void Onbtn4();
	afx_msg void Onbtn5();
	afx_msg void Onbtn6();
	afx_msg void Onbtn7();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YOURFORMDIALOG_H__2F770A1F_9BDB_4925_8D81_60E1A9699EC3__INCLUDED_)
