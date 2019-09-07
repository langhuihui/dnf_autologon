#if !defined(AFX_TAB02DLG_H__CB07F14A_E068_4EF4_B90A_59F0926416CF__INCLUDED_)
#define AFX_TAB02DLG_H__CB07F14A_E068_4EF4_B90A_59F0926416CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TAB02DLG.h : header file
//

#include "NewEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CTAB02DLG dialog

class CTAB02DLG : public CDialog
{
// Construction
public:
	CTAB02DLG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTAB02DLG)
	enum { IDD = IDD_TAB02 };
	CButton	m_Radio1reg;
	CButton	m_Radio2cz;
	CEdit	m_u21;
	CEdit	m_parentuser;
	CEdit	m_p22;
	CEdit	m_p21;
	CEdit	m_bdinfo2;
	CNewEdit	m_k21;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTAB02DLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTAB02DLG)
	afx_msg void Onregorczbtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1reg();
	afx_msg void OnRadio2cz();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void OnCancel() {};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAB02DLG_H__CB07F14A_E068_4EF4_B90A_59F0926416CF__INCLUDED_)
