// IPCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IPC.h"
#include "IPCDlg.h"
#include "ksreglib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIPCDlg dialog

CIPCDlg::CIPCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIPCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIPCDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIPCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIPCDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIPCDlg, CDialog)
	//{{AFX_MSG_MAP(CIPCDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btn7, Onbtn7)
	ON_BN_CLICKED(IDC_btn6, Onbtn6)
	ON_BN_CLICKED(IDC_btn5, Onbtn5)
	ON_BN_CLICKED(IDC_btn4, Onbtn4)
	ON_BN_CLICKED(IDC_btn3, Onbtn3)
	ON_BN_CLICKED(IDC_btn2, Onbtn2)
	ON_BN_CLICKED(IDC_btn1, Onbtn1)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIPCDlg message handlers

BOOL CIPCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	ksLoadDLLs(); //★★★★★★★★★★★★加载DLL


    _int32 sockPort = 12345;                 // sock服务端口号
    
    // ★★★★★★ 该数据从管理端获取后设置到这里：【软件列表】->右上角【取签名数据】按钮单击即可获取。
	signData = __TEXT("");
	signData += __TEXT("cdf68e38d62157c19c9c0e020638ce96e935a27839c8a76eae40995434cfc750931c2d42bf913e98fa4377cae72b59696e93");
	signData += __TEXT("ab4dae07ae74018423523fafad374224a59ad54a15e8e46731a18ce5a8cecdcc85dcccb32c46d090f38b8c0b7a8ecc205174");
	signData += __TEXT("f040cf3d3cb25c020e0261f8c631784da649137497c8a63a1ccc69018a401df3b50441b4b6c10c235a843b5a29b2fa26fc9a");
	signData += __TEXT("ba49980d98ecaf2311822bbd8ea55c144853b01958b04cae2f632cfea754f05d786920e94265e0344e24f395cc32ad4e3cef");
	signData += __TEXT("fe85fc7e651ef36cf395bd1b3a901fa60e1b572b92ad5feabf637b948d2b9672352faa624d01b26395759e68a7df17195d5d");
	signData += __TEXT("d8e447e8a97f92da7b6b55db75a20eefe2dab43228d315f79a24a5b720a82619c32a5914407f9f891b50d72f353f28a9273d");
	signData += __TEXT("c2c42ad3b8cdaa6438f20c309702a2be253f8c0cb2df9ba33971e53d377b36babae935c127c6c977ac0ddf7a9bf2a43c0d3d");
	signData += __TEXT("fe00cbc7a151efd10242176dbdca6d68abd339bf240cb4c92a2b812009fd6d1f0a9e0729cc533622d0759a44860968cef450");

    if(IPC_Connect(sockPort) == 0)
	{
        MsgBox(__TEXT("连接登陆器失败!"));
		::PostQuitMessage(0);
	}
	//-- Delphi源代码到这里结束
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIPCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIPCDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIPCDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CIPCDlg::Onbtn7() 
{
	// TODO: Add your control notification handler code here
    _int32 tresult;
    CString errinfo = __TEXT("");
    tresult = kpoints(3, &errinfo);  // 记录错误变量是传址的，扣点出错时该变量值会记录错误信息
    if(tresult == -1)   // 扣点失败
    {
        MsgBox(errinfo);
        //扣点失败，你可以根据据错误提示，来进行相应的处理，我这里只是关闭程序
        myExitProcess();
    }
	//
	CString lsMSG;
	lsMSG.Format(__TEXT("扣点成功，用户剩余点数%d"), tresult);
    MsgBox(lsMSG);  //显示测试结果
}

void CIPCDlg::Onbtn6() 
{
	// TODO: Add your control notification handler code here
    CString tresult(__TEXT("IPC_GetData所有序数据\n\r"));
	CString msg[20];
	msg[0] = __TEXT("【验证次数】：");
	msg[1] = __TEXT("【返回信息A】：");
	msg[2] = __TEXT("【返回信息B】：");
	msg[3] = __TEXT("【用户附属性】：");
	msg[4] = __TEXT("【绑定信息】：");
	msg[5] = __TEXT("【剩余时间】：");
	msg[6] = __TEXT("【软件下载地址】：");
	msg[7] = __TEXT("【用户通道数】：");
	msg[8] = __TEXT("【服务端软件版本】：");
	msg[9] = __TEXT("【更新日志URL】：");
	msg[10] = __TEXT("【用户天数】：");
	msg[11] = __TEXT("【服务端时间戮】：");
	msg[12] = __TEXT("【用户剩余点数】：");
	msg[13] = __TEXT("【用户标签】：");
	msg[14] = __TEXT("【用户充值次数】：");
	msg[15] = __TEXT("【返回的域名】：");
	msg[16] = __TEXT("【是否可单机多开】：");
	msg[17] = __TEXT("【用户名】：");
	msg[18] = __TEXT("【是否公用帐号】：");
	msg[19] = __TEXT("【是否检查登陆标识】：");
	//
	CString s(__TEXT(""));
	for(int i = 0; i <= 19; ++i)
	{
		s.Format(__TEXT("IPC_GetData(%d)%s%s\n\r"), i, msg[i], IPC_GetData(i));
		tresult += s;
	}
    MsgBox(tresult);  //显示测试结果
}

void CIPCDlg::Onbtn5() 
{
	// TODO: Add your control notification handler code here
      chkPass(1);
      MsgBox(__TEXT("验证通过"));
}

void CIPCDlg::Onbtn4() 
{
	// TODO: Add your control notification handler code here
      chkPass(0);
      MsgBox(__TEXT("验证通过"));
	
}

void CIPCDlg::Onbtn3() 
{
	// TODO: Add your control notification handler code here
    IPC_advapifree(__TEXT("v_getb,100,200"));
    MsgBox(advapi(__TEXT("v_getb,100,200"))); //显示测试结果 这里用的示例接口 v_getb 弹出返回值
	
}

void CIPCDlg::Onbtn2() //advapi测试2 
{
	// TODO: Add your control notification handler code here
    MsgBox(advapi(__TEXT("v_getb,100,200"))); //显示测试结果 这里用的示例接口 v_getb 弹出返回值
	
}

void CIPCDlg::Onbtn1() //advapi测试1 
{
	// TODO: Add your control notification handler code here
    MsgBox(advapi(__TEXT("v_geta")));  //显示测试结果 这里用的示例接口 v_geta 弹出返回值
}

void CIPCDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	ksFreeDLLs();//★★★★★★★★★★★★卸载DLL

	CDialog::OnClose();
}
