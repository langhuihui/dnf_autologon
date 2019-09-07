// loginUserDlg.h : header file
//

#if !defined(AFX_LOGINUSERDLG_H__43ED3F06_421F_46FD_BD86_079F6431EE5A__INCLUDED_)
#define AFX_LOGINUSERDLG_H__43ED3F06_421F_46FD_BD86_079F6431EE5A__INCLUDED_

#include "TAB01DLG.h"	// Added by ClassView
#include "TAB02DLG.h"	// Added by ClassView
#include "TAB03DLG.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLoginUserDlg dialog

class CLoginUserDlg : public CDialog
{
// Construction
public:
	CLoginUserDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLoginUserDlg)
	enum { IDD = IDD_LOGINUSER_DIALOG };
	CTabCtrl	m_TabContr;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginUserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLoginUserDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_CurSelTab; //��ǰѡ���Tabҳ

	CDialog * pDialog[3]; //����ȫ��Tabҳָ��
public:
	void myOnOK();
	CTAB01DLG m_Tab01;
	CTAB02DLG m_Tab02;
	CTAB03DLG m_Tab03;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINUSERDLG_H__43ED3F06_421F_46FD_BD86_079F6431EE5A__INCLUDED_)
