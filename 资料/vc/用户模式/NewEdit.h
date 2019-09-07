#if !defined(AFX_NEWEDIT_H__9CEECDF0_719B_48D2_BFFC_7C7AAFB08DCD__INCLUDED_)
#define AFX_NEWEDIT_H__9CEECDF0_719B_48D2_BFFC_7C7AAFB08DCD__INCLUDED_

#include "KeyFormDLG.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewEdit window

class CNewEdit : public CEdit
{
// Construction
public:
	CNewEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	void mySetText(LPSTR tranText);
	virtual ~CNewEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNewEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillfocus();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CBrush m_Brush;
public:
	CString m_Text;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWEDIT_H__9CEECDF0_719B_48D2_BFFC_7C7AAFB08DCD__INCLUDED_)
