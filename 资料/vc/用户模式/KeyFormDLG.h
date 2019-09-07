#if !defined(AFX_KEYFORMDLG_H__3FA13817_D55C_48D7_9711_2D034ABDD7CD__INCLUDED_)
#define AFX_KEYFORMDLG_H__3FA13817_D55C_48D7_9711_2D034ABDD7CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyFormDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeyFormDLG dialog

class CKeyFormDLG : public CDialog
{
// Construction
public:
	CEdit *m_pInOutEdit;
	CKeyFormDLG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CKeyFormDLG)
	enum { IDD = IDD_keyForm };
	CEdit	m_Edit5;
	CEdit	m_Edit4;
	CEdit	m_Edit3;
	CEdit	m_Edit2;
	CEdit	m_Edit1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyFormDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKeyFormDLG)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton2();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CEdit *m_KeyList[5];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYFORMDLG_H__3FA13817_D55C_48D7_9711_2D034ABDD7CD__INCLUDED_)
