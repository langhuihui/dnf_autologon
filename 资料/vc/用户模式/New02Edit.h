#if !defined(AFX_NEW02EDIT_H__5DD64120_E9AD_43B1_9C03_D6EEE0814C28__INCLUDED_)
#define AFX_NEW02EDIT_H__5DD64120_E9AD_43B1_9C03_D6EEE0814C28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// New02Edit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNew02Edit window

class CNew02Edit : public CEdit
{
// Construction
public:
	CNew02Edit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNew02Edit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNew02Edit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNew02Edit)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEW02EDIT_H__5DD64120_E9AD_43B1_9C03_D6EEE0814C28__INCLUDED_)
