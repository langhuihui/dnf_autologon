// IPCFormDialog.cpp : implementation file
//

#include "stdafx.h"
#include "loginKey.h"
#include "IPCFormDialog.h"
#include "ksreglib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// IPCFormDialog dialog


IPCFormDialog::IPCFormDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IPCFormDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(IPCFormDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void IPCFormDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(IPCFormDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(IPCFormDialog, CDialog)
	//{{AFX_MSG_MAP(IPCFormDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	//ON_MESSAGE(Wm_IPCAnswer, onCallIPCAnswer)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// IPCFormDialog message handlers

void IPCFormDialog::OnButton1() 
{
	// TODO: Add your control notification handler code here
    //��Щ���Լ���ɵ��£�
}



BOOL IPCFormDialog::OnInitDialog() 
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

void IPCFormDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CString tresult;
	tresult = ks_exit();   //�رյ�½��ʱһ��Ҫ����ks_exit();
	if(tresult == __TEXT("eno199")) MsgBox(_TEXT("�˳���������߳ɹ�"));
	
	CDialog::OnClose();
}
