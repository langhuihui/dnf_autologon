// New02Edit.cpp : implementation file
//

#include "stdafx.h"
#include "loginUser.h"
#include "New02Edit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNew02Edit

CNew02Edit::CNew02Edit()
{
}

CNew02Edit::~CNew02Edit()
{
}


BEGIN_MESSAGE_MAP(CNew02Edit, CEdit)
	//{{AFX_MSG_MAP(CNew02Edit)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNew02Edit message handlers

void CNew02Edit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetWindowRect(&rect);

	ScreenToClient(&rect);
	//���������ж�
	if (PtInRect(rect, point))
	{
			//��ѯ����򵥻��¼�
			CString lsS;
			this->GetWindowText(lsS);
			if(lsS == _TEXT("�������û�����ע�Ῠ��")) this->SetWindowText(__TEXT(""));
	}
	
	CEdit::OnLButtonDown(nFlags, point);
}
