// TAB03DLG.cpp : implementation file
//

#include "stdafx.h"
#include "loginUser.h"
#include "TAB03DLG.h"
#include "ksreglib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTAB03DLG dialog


CTAB03DLG::CTAB03DLG(CWnd* pParent /*=NULL*/)
	: CDialog(CTAB03DLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTAB03DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTAB03DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTAB03DLG)
	DDX_Control(pDX, IDC_u31, m_u31);
	DDX_Control(pDX, IDC_p32, m_p32);
	DDX_Control(pDX, IDC_p31, m_p31);
	DDX_Control(pDX, IDC_bdinfo3, m_bdinfo3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTAB03DLG, CDialog)
	//{{AFX_MSG_MAP(CTAB03DLG)
	ON_BN_CLICKED(IDC_editbtn, Oneditbtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTAB03DLG message handlers

void CTAB03DLG::Oneditbtn() 
{
	// TODO: Add your control notification handler code here
	//ÐÞ¸Ä°´Å¥
	CString tempstr = ks_edit(v_softcode, (LPSTR)(LPCTSTR)myGetWindowTextTrim(m_u31), (LPSTR)(LPCTSTR)myGetWindowTextTrim(m_p31), (LPSTR)(LPCTSTR)myGetWindowTextTrim(m_p32), (LPSTR)(LPCTSTR)myGetWindowTextTrim(m_bdinfo3));
    if(iserrno(tempstr)) tempstr = ks_GetMsg((LPSTR)(LPCTSTR)tempstr);
    MsgBox(tempstr);
}

