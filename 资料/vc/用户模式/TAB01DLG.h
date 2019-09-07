#if !defined(AFX_TAB01DLG_H__20CED0F6_81C7_4028_8C50_326CAEA5E709__INCLUDED_)
#define AFX_TAB01DLG_H__20CED0F6_81C7_4028_8C50_326CAEA5E709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TAB01DLG.h : header file
//

#include "New02Edit.h"


/////////////////////////////////////////////////////////////////////////////
// CTAB01DLG dialog

class CTAB01DLG : public CDialog
{
// Construction
public:
	CTAB01DLG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTAB01DLG)
	enum { IDD = IDD_TAB01 };
	CEdit	m_u11;
	CEdit	m_p11;
	CNew02Edit	m_K11;
	CButton	m_Checkupdate;
	CButton	m_passCheck;
	CEdit	m_ks_clientid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTAB01DLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTAB01DLG)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckupdate();
	afx_msg void Onviewbtn();
	afx_msg void OnunbindButton1();
	afx_msg void OnpassCheck();
	afx_msg void Onloginbtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	HWND m_ParentHandle;
	int m_LoginTypeID; //¼ÇÂ¼µÇÂ¼ÀàÐÍ£¡
	void OnCancel() {};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAB01DLG_H__20CED0F6_81C7_4028_8C50_326CAEA5E709__INCLUDED_)
