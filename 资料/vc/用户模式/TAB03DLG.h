#if !defined(AFX_TAB03DLG_H__63374298_54B4_4A72_9830_698BE3AA7D53__INCLUDED_)
#define AFX_TAB03DLG_H__63374298_54B4_4A72_9830_698BE3AA7D53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TAB03DLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTAB03DLG dialog

class CTAB03DLG : public CDialog
{
// Construction
public:
	CTAB03DLG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTAB03DLG)
	enum { IDD = IDD_TAB03 };
	CEdit	m_u31;
	CEdit	m_p32;
	CEdit	m_p31;
	CEdit	m_bdinfo3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTAB03DLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTAB03DLG)
	afx_msg void Oneditbtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void OnCancel() {};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAB03DLG_H__63374298_54B4_4A72_9830_698BE3AA7D53__INCLUDED_)
