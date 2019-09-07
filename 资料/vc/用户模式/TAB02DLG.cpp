// TAB02DLG.cpp : implementation file
//

#include "stdafx.h"
#include "loginUser.h"
#include "TAB02DLG.h"
#include "ksreglib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTAB02DLG dialog


CTAB02DLG::CTAB02DLG(CWnd* pParent /*=NULL*/)
	: CDialog(CTAB02DLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTAB02DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTAB02DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTAB02DLG)
	DDX_Control(pDX, IDC_Radio1reg, m_Radio1reg);
	DDX_Control(pDX, IDC_Radio2cz, m_Radio2cz);
	DDX_Control(pDX, IDC_u21, m_u21);
	DDX_Control(pDX, IDC_parentuser, m_parentuser);
	DDX_Control(pDX, IDC_p22, m_p22);
	DDX_Control(pDX, IDC_p21, m_p21);
	DDX_Control(pDX, IDC_bdinfo2, m_bdinfo2);
	DDX_Control(pDX, IDC_k21, m_k21);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTAB02DLG, CDialog)
	//{{AFX_MSG_MAP(CTAB02DLG)
	ON_BN_CLICKED(IDC_regorczbtn, Onregorczbtn)
	ON_BN_CLICKED(IDC_Radio1reg, OnRadio1reg)
	ON_BN_CLICKED(IDC_Radio2cz, OnRadio2cz)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTAB02DLG message handlers

void CTAB02DLG::Onregorczbtn() 
{
	// TODO: Add your control notification handler code here
	CString tempstr;
    if(m_Radio1reg.GetCheck() == 1)
        tempstr = ks_reguser(v_softcode, (LPSTR)(LPCTSTR)myGetWindowTextTrim(m_u21), (LPSTR)(LPCTSTR)myGetWindowTextTrim(m_p21), (LPSTR)(LPCTSTR)myGetWindowTextTrim(m_p22), (LPSTR)(LPCTSTR)myGetWindowTextTrim(m_k21), (LPSTR)(LPCTSTR)myGetWindowTextTrim(m_bdinfo2), (LPSTR)(LPCTSTR)myGetWindowTextTrim(m_parentuser));
    else
        tempstr = ks_prepaid(v_softcode, (LPSTR)(LPCTSTR)myGetWindowTextTrim(m_u21), (LPSTR)(LPCTSTR)myGetWindowTextTrim(m_k21));
    if(iserrno(tempstr))  tempstr = ks_GetMsg((LPSTR)(LPCTSTR)tempstr);
    MsgBox(tempstr);
}

BOOL CTAB02DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Radio1reg.SetCheck(1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTAB02DLG::OnRadio1reg() 
{
	// TODO: Add your control notification handler code here
	m_p21.SetReadOnly(false);
	m_p22.SetReadOnly(false);
	m_bdinfo2.SetReadOnly(false);
	m_parentuser.SetReadOnly(false);
	GetDlgItem(IDC_regorczbtn)->SetWindowText(__TEXT("◊¢≤·’ ∫≈"));
}

void CTAB02DLG::OnRadio2cz() 
{
	// TODO: Add your control notification handler code here
	m_p21.SetReadOnly(true);
	m_p22.SetReadOnly(true);
	m_bdinfo2.SetReadOnly(true);
	m_parentuser.SetReadOnly(true);
	GetDlgItem(IDC_regorczbtn)->SetWindowText(__TEXT("’ ∫≈≥‰÷µ"));
}
