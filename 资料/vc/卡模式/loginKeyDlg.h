// loginKeyDlg.h : header file
//

#if !defined(AFX_LOGINKEYDLG_H__2B46C089_328B_4823_9995_ED90679BDB58__INCLUDED_)
#define AFX_LOGINKEYDLG_H__2B46C089_328B_4823_9995_ED90679BDB58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "YourFormDialog.h"
#include "IPCFormDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CLoginKeyDlg dialog

class CLoginKeyDlg : public CDialog
{
// Construction
public:
	CLoginKeyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLoginKeyDlg)
	enum { IDD = IDD_LOGINKEY_DIALOG };
	CStatic	m_lblclientid;
	CEdit	m_bdinfo3;
	CButton	m_Checkupdate;
	CButton	m_passCheck;
	CEdit	m_ks_clientid;
	CEdit	m_u11;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginKeyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLoginKeyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnunbindButton();
	afx_msg void Onviewbtn();
	afx_msg void Onloginbtn();
	afx_msg void OnbdinfoButton();
	afx_msg void OnCheckupdate();
	afx_msg void OnpassCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_LoginTypeID; //¼ÇÂ¼µÇÂ¼ÀàÐÍ£¡
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINKEYDLG_H__2B46C089_328B_4823_9995_ED90679BDB58__INCLUDED_)
