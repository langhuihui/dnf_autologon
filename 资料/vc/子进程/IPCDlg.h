// IPCDlg.h : header file
//

#if !defined(AFX_IPCDLG_H__A4289D35_2B8E_4F43_BBF4_0010E75C2772__INCLUDED_)
#define AFX_IPCDLG_H__A4289D35_2B8E_4F43_BBF4_0010E75C2772__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CIPCDlg dialog

class CIPCDlg : public CDialog
{
// Construction
public:
	CIPCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CIPCDlg)
	enum { IDD = IDD_IPC_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CIPCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void Onbtn7();
	afx_msg void Onbtn6();
	afx_msg void Onbtn5();
	afx_msg void Onbtn4();
	afx_msg void Onbtn3();
	afx_msg void Onbtn2();
	afx_msg void Onbtn1();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPCDLG_H__A4289D35_2B8E_4F43_BBF4_0010E75C2772__INCLUDED_)
