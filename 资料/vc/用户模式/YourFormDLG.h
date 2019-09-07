#if !defined(AFX_YOURFORMDLG_H__ED58A96F_890B_44FE_BA79_2F5ECE933C07__INCLUDED_)
#define AFX_YOURFORMDLG_H__ED58A96F_890B_44FE_BA79_2F5ECE933C07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YourFormDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CYourFormDLG dialog

class CYourFormDLG : public CDialog
{
// Construction
public:
	CYourFormDLG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CYourFormDLG)
	enum { IDD = IDD_YourForm };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYourFormDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CYourFormDLG)
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

#endif // !defined(AFX_YOURFORMDLG_H__ED58A96F_890B_44FE_BA79_2F5ECE933C07__INCLUDED_)
