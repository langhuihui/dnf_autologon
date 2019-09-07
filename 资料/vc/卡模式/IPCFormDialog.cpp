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
    //干些你自己想干的事，
}



BOOL IPCFormDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    _int32 sockPort,clientNum;

    sockPort = 12345;        // ★★★★★★ sock服务端口号，建议填写10000-20000之间的整数
    clientNum = 1;          // ★★★★★★ 允许连接的子进程数量



    if(IPC_Start(sockPort,clientNum) == 0)
    {
        MsgBox(__TEXT("进程通讯服务启动失败！"));
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
	tresult = ks_exit();   //关闭登陆器时一定要调用ks_exit();
	if(tresult == __TEXT("eno199")) MsgBox(_TEXT("退出软件：离线成功"));
	
	CDialog::OnClose();
}
