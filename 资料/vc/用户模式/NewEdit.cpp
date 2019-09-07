// NewEdit.cpp : implementation file
//

#include "stdafx.h"
#include "loginUser.h"
#include "NewEdit.h"
#include "KeyFormDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewEdit

CNewEdit::CNewEdit()
{
}

CNewEdit::~CNewEdit()
{
}


BEGIN_MESSAGE_MAP(CNewEdit, CEdit)
	//{{AFX_MSG_MAP(CNewEdit)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewEdit message handlers

void CNewEdit::mySetText(LPSTR tranText)
{
	m_Text = tranText;
	SetWindowText(tranText);
}

int CNewEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_Text = "";

	return 0;
}

void CNewEdit::OnKillfocus() 
{
	// TODO: Add your control notification handler code here
	SetWindowText(m_Text);
}


void CNewEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CEdit::OnLButtonUp(nFlags, point);
	CRect rect;
	GetWindowRect(&rect);

	ScreenToClient(&rect);
	//在下面来判断
	if (PtInRect(rect, point))
	{
			//弹出管理对话框
			CKeyFormDLG lsKeyFormDLG;
			lsKeyFormDLG.m_pInOutEdit = this;
			lsKeyFormDLG.DoModal();
	}
	this->SetSel(0);
}

void CNewEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	return;
	
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CNewEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	return;

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CNewEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	return;
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CNewEdit::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	return;
	CEdit::OnRButtonDown(nFlags, point);
}

void CNewEdit::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	return;
	CEdit::OnRButtonUp(nFlags, point);
}
