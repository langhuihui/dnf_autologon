// loginKeyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "loginKey.h"
#include "loginKeyDlg.h"
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
// CLoginKeyDlg dialog

CLoginKeyDlg::CLoginKeyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginKeyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginKeyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoginKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginKeyDlg)
	DDX_Control(pDX, IDC_lblclientid, m_lblclientid);
	DDX_Control(pDX, IDC_bdinfo3, m_bdinfo3);
	DDX_Control(pDX, IDC_Checkupdate, m_Checkupdate);
	DDX_Control(pDX, IDC_passCheck, m_passCheck);
	DDX_Control(pDX, IDC_ks_clientid, m_ks_clientid);
	DDX_Control(pDX, IDC_u11, m_u11);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoginKeyDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginKeyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_unbindButton, OnunbindButton)
	ON_BN_CLICKED(IDC_viewbtn, Onviewbtn)
	ON_BN_CLICKED(IDC_loginbtn, Onloginbtn)
	ON_BN_CLICKED(IDC_bdinfoButton, OnbdinfoButton)
	ON_BN_CLICKED(IDC_Checkupdate, OnCheckupdate)
	ON_BN_CLICKED(IDC_passCheck, OnpassCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginKeyDlg message handlers

BOOL CLoginKeyDlg::OnInitDialog()
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

	m_LoginTypeID = 0;

	m_u11.SetWindowText(__TEXT("请输入注册卡号"));
	m_ks_clientid.SetWindowText(__TEXT("1"));
	//
    CString LicenseKey,v_rsamod,v_rsapubkey;
    CString v_pccode, s_notice;
    _int32 v_haveclientid, v_haveunbind, v_cstime, v_is2svr;
    // 下边两个参数主要用来自动调整界面上的通道控件和解绑按钮是否可见
    v_haveclientid = 1;   //是否有多通道卡
    v_haveunbind = 1;   //是否允许客户通过解绑按钮解绑
     // ★★★★★★如果你用IPC进程通讯模式，此参数请改为1
    v_isipcmode = 0;
    // ★★★★★★配合自动更新功能，如果为0则会关闭自动更新，与ks_setUVal接口和管理端【软件参数设置】里的【自动更新】选项卡里的参数可实现自动更新软件功能
    v_softversion = 1;
    // ★★★★★★如若你觉得验证模块取的机器码不够安全或经常变动，你可以自己写函数取机器码这个变量，要求最少五个字符，否则请留空
    v_pccode = __TEXT("");

    // ★★★★★★记录日志数据的文件 c:\kss.ini
    v_inipath = __TEXT("C:\\kss.ini");

    // ★★★★★★连接服务器超时默认为6000毫秒
    v_cstime = 6000;

    // ★★★★★★如果你装有验证备服，这里请填1，否则填0
    v_is2svr = 0;


    // ★★★★★★此参数请查阅管理端【软件列表】里[软件编号]的值
    v_softcode = 1000102;

    // ★★★★★★此参数请查阅管理端【软件列表】里[软件密钥]的值
    v_softkey = __TEXT("IpG971PHDj2psv4TX2SlL247");

	//★★★★★★ 800商业版用户，下边两个参数设置请参见 http://www.hphu.com/news_14.html
	//五元服作者和体验用户请不要修改RSA参数
	v_rsamod = __TEXT("65770992667108949439609301548660913131275548029934681330823001172159438539889");  // v_rsa mod = __TEXT("") 就是关闭RSA功能，服务端也应关闭RSA
	v_rsapubkey = __TEXT("65537");  //v_rsapubkey = __TEXT("")  就是关闭RSA功能，服务端也应关闭RSA

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

    v_bdinfo = __TEXT("");           // 用户的绑定信息。
    // 1、如果这里的值不为空，该数据会由服务端效验，也就是说如果服务端数据库里相应用户的绑定信息与该值不相同，会被服务端认为非法，也就是验证不能通过。
    // 2、如果这里的值为空，登陆成功后通过ks_GetData (4)取得服务器返回的用户绑定信息，也就是说你可以用ks_GetData (4)的值来处理你自己的应用。

	//★★★★★★ 800商业版用户，下边的参数里的字符串请修改为你自己的LicenseKey
	//五元服作者和体验用户请不要修改该参数
	LicenseKey = __TEXT("");
	LicenseKey += __TEXT("AbF6BZMFVaQJdXOJZ3cWdsm5Tnnoie3ZpjJYICVUOWfMj28ALOVtuEdfGM/6go4HJ/uzJwlZ90d1fBI4");
	LicenseKey += __TEXT("Lxxh4BBC8s16lTVUdtVxOpBCfjw3lnl3yW84qecrVJiog7CadhblEOJmk251jvB2bY2iCFF4TgfVQG6q");
	LicenseKey += __TEXT("bwHEb/n9iI+A38o0f23kBbBMIi8513DvrcyL2Ng7qChdbb0FreyGQy62L7CIKmFRnGSZqha9hKGSnX+k");
	LicenseKey += __TEXT("py6gAvBtZ8v9/wmpvCfZHqXB9L0W/Xw9CLliRLg33syfzf8xvDqDAtCSuOpxheGdj2lvG7Ymn4p6+j8I");
	LicenseKey += __TEXT("BMKKu7JkTK94yIErGwv3w1B3/yeLCHnlY1DKWrlKSYoHYGfjdCNXU9ncEFGD0rsM8wuV6HVgSh4015bx");
	LicenseKey += __TEXT("WtOwocQtX+Qji+pwKlCebpF3spe5H/aEorV3GiffAgoeFXyN3Ejq3K7YsZk99iQ22dudL0ebyk253r4x");
	LicenseKey += __TEXT("EDrO9TyQ+AsoDsED1DenhRPBHgoYXDL5lYq/Fs7qRQPI19r0qSLhUi1pNDhYKiA1nt6f81VgRNphKeV5");
	LicenseKey += __TEXT("edG1gd4t28DhJkqsxYy2Yq8FTAbserH0ZyRZyoRjXG39O7NpjfW24uOar4T3fy2IsVxRD9ZWgPi0lqKF");
	LicenseKey += __TEXT("MToc7Ym4nh+Czq4GTutINcUbma9H8udrBOlxqNRTxZjWRg5QziRgafRZdqfXgLfjy/Zwn/EqdlVnXLOu");
	LicenseKey += __TEXT("bUF3h5zQ48NpHqjkc64qLhwUct+vrfK5syE36sVXu9SDvWUsO/W7CtzjwMz9cLuNiWMl1dpBGYUZErGL");
	LicenseKey += __TEXT("+14y+MEKWyyR/e8bZfzpF8dD2aqyebcskFllNd00QFH/GXsIOTsy3rfGWASLmuSUFP5dxViecaWTIAtj");
	LicenseKey += __TEXT("j63D7JMFAPKn9iX2ZOoI5Q42DA4AWUp5wgt0aRza4WwETRGMnhLt91qVI1mDjryPU8tp0U97GVqpGugg");
	LicenseKey += __TEXT("F8ufE18GNzURgBr5Uj6G2ydS8Y1iab46Re3fZLsDHveZ1dM2iLBNyMpVT8y1Sh8elCNX3XAlAPeQCzoG");
	LicenseKey += __TEXT("TG/dkMT1htA3SnShUTVnWusHTbqA2gAT+4MSe60lkU0lZhEmSqeuIyNhUtJJLNTF4TQ1fov1fMSOpSm0");
	LicenseKey += __TEXT("Ek9YrSxsg9ESFR7zYbSI+s7OSup6Y+Z6fCuFyYz5P8n4hVIUMf+fdvmuok5sJZDaTx0qfYZJUBaXyWN3");
	LicenseKey += __TEXT("2+xI8XEcIjY/vAz5WNAHSX0VDJwzYBv2ezMF3VF00A1L8vgsOsyo6Ph8229Q4HbOSPM8hLzh8BoXIwmR");
	LicenseKey += __TEXT("tz7SRTxPfpJ5Ir7HQiQjtoK6my7p2UrA8aCoUJjYyua3QsSyjTI4zXdaYrGkWh84+WpnTijHED7kHcF/");
	LicenseKey += __TEXT("YjkhhMzGiZnDeQ6p8F9UG+Dy4B9XZ3QX5t/WtPg3QNuUrEukxMp/oUGRqKbKf4se1AHjv27PPbcZQcIv");
	LicenseKey += __TEXT("alMa4VYu8vQPnIG0U7lAFWy0lKuYoikH1qq4J+mNCJCS71AAdFNl99CMFUJF5U1EY6NI2xbJiRvPKQWl");
	LicenseKey += __TEXT("UouWu2fNpiRIEZyUAFKFg+t1ns306MyZECEH3bHYI8ToxQ4BPvZg0znWmtRzW/4povCiN3hW4LTtS43U");
	LicenseKey += __TEXT("05EgseJnLOZ3adxjmtKZl1lFroZuycDsai4wV7V3mg/jvZegJ9NzotZZbPmhUZgOZDuKo3QQGahPUltJ");
	LicenseKey += __TEXT("GcYTGmQ1oXiziAgTkalF3spYyOk=");

    //KS授权信息
    ks_setLicense((LPSTR)(LPCTSTR)LicenseKey);

    //初始化扩展参数，一般情况下无需修改
    ks_setExtVal(0, (LPSTR)(LPCTSTR)v_pccode, (LPSTR)(LPCTSTR)v_inipath, v_cstime, v_is2svr, (LPSTR)(LPCTSTR)v_rsamod,(LPSTR)(LPCTSTR)v_rsapubkey, __TEXT(""), __TEXT(""), __TEXT(""));

    if(r_ini(__TEXT("Soft_Config"), __TEXT("checkupdate")) != __TEXT("1"))
		m_Checkupdate.SetCheck(BST_UNCHECKED);
    else
		m_Checkupdate.SetCheck(BST_CHECKED);

    if(r_ini(__TEXT("Soft_Config"), __TEXT("Rememberaccount")) != __TEXT("1"))
	{
		m_passCheck.SetCheck(BST_UNCHECKED);
	}
    else
    {
		m_passCheck.SetCheck(BST_CHECKED);
        m_u11.SetWindowText(r_ini(__TEXT("Soft_Config"), __TEXT("card")));
    }

	if(v_haveclientid != 1) //是否有多通道卡界面控制
	{
		m_lblclientid.ShowWindow(SW_HIDE);
		m_ks_clientid.ShowWindow(SW_HIDE);
		m_ks_clientid.SetWindowText(__TEXT("1"));
		//暂时注释这段   loginbtn.Left := 362;
		//暂时注释这段   loginbtn.Top := 13;
		//暂时注释这段   viewbtn.Left := 362;
		//暂时注释这段   viewbtn.Top := 41;
		//暂时注释这段   unbindButton1.Left := 278;
		//暂时注释这段   unbindButton1.Top := 41;
	}

	if(v_haveunbind == 0)
		GetDlgItem(IDC_unbindButton)->ShowWindow(SW_HIDE);
	else
		GetDlgItem(IDC_unbindButton)->ShowWindow(SW_SHOW);

    // 无论调用多少次ks_GetData（参数大于10000000），只会连接一次网络（无论参数是否相同）
    // 以下五行代码视你具体需求，你可以决定是否保留
    // s_isup = ks_GetData(v_softcode * 10 + 1);   //服务端软件是否强制更新
    // s_version = ks_GetData(v_softcode * 10 + 2);  //服务端软件版本
    // s_downurl = ks_GetData(v_softcode * 10 + 3);   //服务端软件下载地址

    s_notice = ks_GetData(v_softcode * 10 + 4);  //服务端软件公告
    MsgBox(s_notice);
	//
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLoginKeyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// if you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLoginKeyDlg::OnPaint() 
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
HCURSOR CLoginKeyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLoginKeyDlg::OnunbindButton() 
{
	// TODO: Add your control notification handler code here
	CString tempstr;
	CString lsU11Text;
	m_u11.GetWindowText(lsU11Text);
	CString lsm_ks_clientid;
	m_ks_clientid.GetWindowText(lsm_ks_clientid);
    tempstr = ks_unbind(v_softcode, _ttoi((LPCTSTR)lsm_ks_clientid), (LPSTR)(LPCTSTR)lsU11Text, __TEXT(""));
    if(iserrno(tempstr)) tempstr = ks_GetMsg((LPSTR)(LPCTSTR)tempstr);
    MsgBox(tempstr);
}

void CLoginKeyDlg::Onviewbtn() 
{
	// TODO: Add your control notification handler code here
    CString tempstr;
    _int32 nkeypos;
	CString lsU11Text;
	m_u11.GetWindowText(lsU11Text);
    if(lsU11Text == __TEXT("请输入注册卡号"))
    {
        MsgBox(__TEXT("请输入注册卡号"));
        return;
    }

    tempstr = ks_viewinfo(v_softcode, (LPSTR)(LPCTSTR)lsU11Text);
    // 以下三行用来处理旧卡号
    nkeypos = tempstr.Find(__TEXT("《"));
    if(nkeypos > 0)
		m_u11.SetWindowText((LPSTR)(LPCTSTR)tempstr.Mid(nkeypos + 2, 32));
    if(iserrno(tempstr)) tempstr = ks_GetMsg((LPSTR)(LPCTSTR)tempstr);
    MsgBox(tempstr);
}

void CLoginKeyDlg::Onloginbtn() 
{
	// TODO: Add your control notification handler code here
	_int32 i_rnd;
    CString v_msgstr;
	GetDlgItem(IDC_loginbtn)->EnableWindow(false);
    ks_setSoftVal(v_softcode, (LPSTR)(LPCTSTR)v_softkey);

    //--  传递用户相关信息给DLL
	CString lsU11Text;
	m_u11.GetWindowText(lsU11Text);
	lsU11Text.TrimLeft();
	lsU11Text.TrimRight();
	CString lsm_ks_clientid;
	m_ks_clientid.GetWindowText(lsm_ks_clientid);
    ks_setUserVal(_ttoi((LPSTR)(LPCTSTR)lsm_ks_clientid), (LPSTR)(LPCTSTR)lsU11Text, _TEXT(""), (LPSTR)(LPCTSTR)v_bdinfo);

    //--  设置自动升级相关信息给DLL，不用自动升级功能的话，下边一段完全可以删掉
    //--  参数：1本登陆窗口的窗口句柄, 2当前软件的版本号, 3用户是否选择了检查更新
    ks_setUpVal((_int32)this->m_hWnd, v_softversion, (_int32)(m_Checkupdate.GetCheck()));

    //--  开始验证
	srand((unsigned)time(NULL));
    i_rnd = (int)(49 * rand() / (RAND_MAX + 1.0));
    v_msgstr = ks_IdCheck((LPSTR)(LPCTSTR)signData.Mid(i_rnd*8, 8), 1);
    if(iserrno(v_msgstr))
    {
		MsgBox(ks_GetMsg((LPSTR)(LPCTSTR)v_msgstr));
		GetDlgItem(IDC_loginbtn)->EnableWindow(true);
        return;
    }
    if(signData.Mid(i_rnd * 8 + 400, 8) != v_msgstr)
    {
		
		CString lsS = v_msgstr;
		lsS += __TEXT(",");
		lsS += signData.Mid(i_rnd * 8 + 400, 8);
        w_ini(__TEXT("signdata"), __TEXT("err"), lsS);
        myExitProcess(); //掐进程
    }


    if(r_ini(__TEXT("Soft_Config"), __TEXT("Rememberaccount")) = __TEXT("1"))
        w_ini(__TEXT("Soft_Config"), __TEXT("card"), (LPSTR)(LPCTSTR)lsU11Text);
    else
        w_ini(__TEXT("Soft_Config"), __TEXT("card"), __TEXT(""));

	GetDlgItem(IDC_loginbtn)->EnableWindow(true);
	m_LoginTypeID = v_isipcmode;

	OnOK();
}

void CLoginKeyDlg::OnbdinfoButton() 
{
	// TODO: Add your control notification handler code here
	CString lsU11Text;
	m_u11.GetWindowText(lsU11Text);
	lsU11Text.TrimLeft();
	lsU11Text.TrimRight();


	CString lsbdinfo3Text;
	m_bdinfo3.GetWindowText(lsbdinfo3Text);
	lsbdinfo3Text.TrimLeft();
	lsbdinfo3Text.TrimRight();
	CString tempstr = ks_edit(v_softcode, (LPSTR)(LPCTSTR)lsU11Text, __TEXT(""), __TEXT(""), (LPSTR)(LPCTSTR)lsbdinfo3Text);
    if(iserrno(tempstr)) tempstr = ks_GetMsg((LPSTR)(LPCTSTR)tempstr);
    MsgBox(tempstr);
}

void CLoginKeyDlg::OnCheckupdate() 
{
	// TODO: Add your control notification handler code here
    CString temp_aaa(_TEXT("0"));
    if(m_Checkupdate.GetCheck() == BST_CHECKED) temp_aaa = _TEXT("1");
    w_ini(_TEXT("Soft_Config"), _TEXT("checkupdate"),temp_aaa);
}

void CLoginKeyDlg::OnpassCheck() 
{
	// TODO: Add your control notification handler code here
    CString temp_aaa(_TEXT("0"));
    if(m_passCheck.GetCheck() == BST_CHECKED) temp_aaa = _TEXT("1");
    w_ini(_TEXT("Soft_Config"), _TEXT("Rememberaccount"),temp_aaa);
}
