


//该窗体是在登陆窗口的所有充值卡号单击事件显示的，让用户输入充值卡号。
//有需要你可以稍改动一下界面UI和文字说明。





// KeyFormDLG.cpp : implementation file
//

#include "stdafx.h"
#include "loginUser.h"
#include "KeyFormDLG.h"
#include "ksreglib.h"
#include "NewEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyFormDLG dialog


CKeyFormDLG::CKeyFormDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyFormDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyFormDLG)
	//}}AFX_DATA_INIT
}


void CKeyFormDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyFormDLG)
	DDX_Control(pDX, IDC_EDIT5, m_Edit5);
	DDX_Control(pDX, IDC_EDIT4, m_Edit4);
	DDX_Control(pDX, IDC_EDIT3, m_Edit3);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeyFormDLG, CDialog)
	//{{AFX_MSG_MAP(CKeyFormDLG)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyFormDLG message handlers

void CKeyFormDLG::OnButton1() 
{
	// TODO: Add your control notification handler code here
	//清空卡号
	for(int i = 0; i <= 4; ++i)	
	{
		m_KeyList[i]->SetWindowText("");
	}
	//清空父窗口的Edit
	((CNewEdit*)m_pInOutEdit)->mySetText(__TEXT(""));
}

BOOL CKeyFormDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_KeyList[0] = &m_Edit1;
	m_KeyList[1] = &m_Edit2;
	m_KeyList[2] = &m_Edit3;
	m_KeyList[3] = &m_Edit4;
	m_KeyList[4] = &m_Edit5;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKeyFormDLG::OnButton2() 
{
	// TODO: Add your control notification handler code here

	//将输入的卡号传给登陆窗口的输入框
	CString lsE = __TEXT("");
	CString lsS = __TEXT("");
	CString lsT = __TEXT("");
	for(int i = 0; i < 5; ++i)
	{
		if(!myGetWindowTextTrim(*m_KeyList[i]).IsEmpty())
		{
			if(!lsT.IsEmpty())
			{
				lsT += __TEXT("|");
			}
			lsT += myGetWindowTextTrim(*m_KeyList[i]);
			if(myGetWindowTextTrim(*m_KeyList[i]).GetLength() != 32)
			{
				lsS.Format("第%d行卡号位数有错！\n\r", (i + 1));
				lsE += lsS;
				lsE += myGetWindowTextTrim(*m_KeyList[i]);
			}
			for(int j = (i - 1); j >= 0; --j)
			{
				if(myGetWindowTextTrim(*m_KeyList[i]) == myGetWindowTextTrim(*m_KeyList[j]))
				{
					lsS.Format("第%d行和第%d行卡号相同，重复了。\n\r", (i + 1), (j + 1));
					lsE += lsS;
				}
			}
		}
	}
	if(lsE == __TEXT(""))
	{
		((CNewEdit*)m_pInOutEdit)->mySetText((LPSTR)(LPCTSTR)lsT);
		OnOK();
	}
	else
	{
		MsgBox(lsE);
	}
}

void CKeyFormDLG::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	CString lsS = ((CNewEdit*)m_pInOutEdit)->m_Text;
	CString lsKey;
	int i = 0;
	while(lsS.Find(__TEXT("|")) != -1)
	{
		int lsp = lsS.Find(__TEXT("|"));
		lsKey = lsS.Left(lsp);
		if(lsKey.GetLength() == 32)
		{
			m_KeyList[i]->SetWindowText((LPSTR)(LPCTSTR)lsKey);
			++i;
		}
		lsKey = lsS.Mid(lsp + 1, lsS.GetLength() - lsp - 1);
		lsS = lsKey;
	}
	if(lsS.GetLength() == 32) 
	{
		m_KeyList[i]->SetWindowText((LPSTR)(LPCTSTR)lsS);
	}
}
