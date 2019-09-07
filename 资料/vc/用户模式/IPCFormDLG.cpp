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
   //干些你自己想干的事，
}

void CIPCFormDLG::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CString tresult;
	tresult = ks_exit();   //关闭登陆器时一定要调用ks_exit();
	if(tresult == __TEXT("eno199")) MsgBox(__TEXT("退出软件：离线成功"));
	//
	CDialog::OnClose();
}




BOOL CIPCFormDLG::OnInitDialog() 
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
