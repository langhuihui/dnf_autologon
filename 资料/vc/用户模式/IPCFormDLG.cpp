// IPCFormDLG.cpp : implementation file
//

#include "stdafx.h"
#include "loginUser.h"
#include "IPCFormDLG.h"
#include "ksreglib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIPCFormDLG dialog


CIPCFormDLG::CIPCFormDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CIPCFormDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIPCFormDLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIPCFormDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIPCFormDLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIPCFormDLG, CDialog)
	//{{AFX_MSG_MAP(CIPCFormDLG)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	//ON_MESSAGE(Wm_IPCAnswer, onCallIPCAnswer)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIPCFormDLG message handlers

void CIPCFormDLG::OnButton1() 
{
	// TODO: Add your control notification handler code here
   //��Щ���Լ���ɵ��£�
}

void CIPCFormDLG::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CString tresult;
	tresult = ks_exit();   //�رյ�½��ʱһ��Ҫ����ks_exit();
	if(tresult == __TEXT("eno199")) MsgBox(__TEXT("�˳���������߳ɹ�"));
	//
	CDialog::OnClose();
}




BOOL CIPCFormDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    _int32 sockPort,clientNum;

    sockPort = 12345;        // ������� sock����˿ںţ�������д10000-20000֮�������
    clientNum = 1;          // ������� �������ӵ��ӽ�������



    if(IPC_Start(sockPort,clientNum) == 0)
    {
        MsgBox(__TEXT("����ͨѶ��������ʧ�ܣ�"));
        myExitProcess();
    }
	//
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
