// loginUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "loginUser.h"
#include "loginUserDlg.h"
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
// CLoginUserDlg dialog

CLoginUserDlg::CLoginUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginUserDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoginUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginUserDlg)
	DDX_Control(pDX, IDC_TAB1, m_TabContr);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoginUserDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginUserDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginUserDlg message handlers

BOOL CLoginUserDlg::OnInitDialog()
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




    //ΪTab Control��������ҳ��
    m_TabContr.InsertItem(0, __TEXT(" ��¼ | ��ѯ "));
    m_TabContr.InsertItem(1, __TEXT(" ע�� | ��ֵ "));	
    m_TabContr.InsertItem(2, __TEXT("  �޸� ����  "));
	
	//�趨��Tab����ʾ�ķ�Χ
	CRect rc;
    m_TabContr.GetClientRect(rc);
    rc.top += 23 + 8;
    rc.bottom -= 8;
    rc.left += 8;
    rc.right -= 8;

	//������ҳ��
	m_Tab01.Create(IDD_TAB01, GetDlgItem(IDC_TAB1));
	m_Tab02.Create(IDD_TAB02, GetDlgItem(IDC_TAB1));
	m_Tab03.Create(IDD_TAB03, GetDlgItem(IDC_TAB1));
    m_Tab01.MoveWindow(&rc);
    m_Tab02.MoveWindow(&rc);
    m_Tab03.MoveWindow(&rc);

    //�ѶԻ������ָ�뱣������
    pDialog[0] = &m_Tab01;
    pDialog[1] = &m_Tab02;
    pDialog[2] = &m_Tab03;

    //��ʾ��ʼҳ��
    pDialog[0]->ShowWindow(SW_SHOW);
    pDialog[1]->ShowWindow(SW_HIDE);
    pDialog[2]->ShowWindow(SW_HIDE);

    //���浱ǰѡ��
    m_CurSelTab = 0;


    CString LicenseKey;
    CString v_pccode, s_notice,v_rsamod,v_rsapubkey;
    _int32 v_haveclientid, v_haveunbind, v_cstime, v_is2svr;
    // �±�����������Ҫ�����Զ����������ϵ�ͨ���ؼ��ͽ��ť�Ƿ�ɼ�
    v_haveclientid = 1;   //�Ƿ��ж�ͨ����
    v_haveunbind = 1;   //�Ƿ�����ͻ�ͨ�����ť���
     // ��������������IPC����ͨѶģʽ���˲������Ϊ1
    v_isipcmode = 1;
    // �����������Զ����¹��ܣ����Ϊ0���ر��Զ����£���ks_setUVal�ӿں͹���ˡ�����������á���ġ��Զ����¡�ѡ���Ĳ�����ʵ���Զ������������
    v_softversion = 1;
    // ������������������֤ģ��ȡ�Ļ����벻����ȫ�򾭳��䶯��������Լ�д����ȡ���������������Ҫ����������ַ�������������
    v_pccode = __TEXT("");

    // ��������¼��־���ݵ��ļ� c:\kss.ini
    v_inipath = __TEXT("C:\\kss.ini");

    // ����������ӷ�������ʱĬ��Ϊ6000����
    v_cstime = 6000;

    // ������������װ����֤��������������1��������0
    v_is2svr = 0;


    // �������˲�������Ĺ���ˡ�����б���[������]��ֵ
    v_softcode = 1000101;

    // �������˲�������Ĺ���ˡ�����б���[�����Կ]��ֵ
    v_softkey = __TEXT("67iRh5471AX12K111HUHtAmK");

	//������� 800��ҵ���û����±���������������μ� http://www.hphu.com/news_14.html
	//��Ԫ�����ߺ������û��벻Ҫ�޸�RSA����
	v_rsamod = __TEXT("65770992667108949439609301548660913131275548029934681330823001172159438539889");  // v_rsa mod = __TEXT("") ���ǹر�RSA���ܣ������ҲӦ�ر�RSA
	v_rsapubkey = __TEXT("65537");  //v_rsapubkey = __TEXT("")  ���ǹر�RSA���ܣ������ҲӦ�ر�RSA


    // ������� �����ݴӹ���˻�ȡ�����õ����������б�->���Ͻǡ�ȡǩ�����ݡ���ť�������ɻ�ȡ��
	signData = __TEXT("");
	signData += __TEXT("847c18cf2d40d14d5b0ac9f99c7a7282606a1b0bd20b728163af39c3df421c4b8811ddff9bb0716e60cd37921bef08e65319");
	signData += __TEXT("916035458ae0c923e2fce03fea2c8cbe31e2fb7cb61f4bd32bd24ba029235d294cc9c765d4535884421e1db644bce70cf82e");
	signData += __TEXT("fad90d55db55dae989d21ecf32d2333956e05757684b67bfb83cf4e9f2ce2ffbedd5b8aad7c0962c6c6dbc3a256f654e8ac8");
	signData += __TEXT("6611e0a19aacaca531a5f1cc231840a8457b3818e81be1fc51e9598f09869d0d093d7cd45a993da3c1d862ab25ea3200fefe");
	signData += __TEXT("796e85634d8bc824cd1cc9176a027d7780b252ac72d152593bff94d032c0ac8b36b9d4753afdf5fcd1863204ddaeff992ae8");
	signData += __TEXT("c3b5c9aec2296750dab0f713d8e06360a8c1e052b81a38069d763816ffa6aab12befe5cfd54e2c39f149aaef8aae5afab179");
	signData += __TEXT("40f9df52e2c7672870a5d57ee2538720f4823b0661e2588717cd0838d82f7f4484138dafb75ee73c974fa258eac3b72fab14");
	signData += __TEXT("13dfedde3c493f9f1ba163d821ad67fc9e6f150ec3f1987b911a999bc0e6dd34210275ad5c940f0f240d588194a6b0df929c");
    
	v_bdinfo = __TEXT("");	// �û��İ���Ϣ��
    // 1����������ֵ��Ϊ�գ������ݻ��ɷ����Ч�飬Ҳ����˵�����������ݿ�����Ӧ�û��İ���Ϣ���ֵ����ͬ���ᱻ�������Ϊ�Ƿ���Ҳ������֤����ͨ����
    // 2����������ֵΪ�գ���½�ɹ���ͨ��ks_GetData (4)ȡ�÷��������ص��û�����Ϣ��Ҳ����˵�������ks_GetData (4)��ֵ���������Լ���Ӧ�á�


	//������� 800��ҵ���û����±ߵĲ�������ַ������޸�Ϊ���Լ���LicenseKey
	//��Ԫ�����ߺ������û��벻Ҫ�޸ĸò���
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

    //KS��Ȩ��Ϣ
    ks_setLicense((LPSTR)(LPCTSTR)LicenseKey);



    //��ʼ����չ������һ������������޸�
    ks_setExtVal(0, (LPSTR)(LPCTSTR)v_pccode, (LPSTR)(LPCTSTR)v_inipath, v_cstime, v_is2svr, (LPSTR)(LPCTSTR)v_rsamod,(LPSTR)(LPCTSTR)v_rsapubkey, __TEXT(""), __TEXT(""), __TEXT(""));

    if(r_ini(__TEXT("Soft_Config"), __TEXT("checkupdate")) != __TEXT("1"))
		m_Tab01.m_Checkupdate.SetCheck(BST_UNCHECKED);
    else
		m_Tab01.m_Checkupdate.SetCheck(BST_CHECKED);

    if(r_ini(__TEXT("Soft_Config"), __TEXT("Rememberaccount")) != __TEXT("1"))
	{
		m_Tab01.m_passCheck.SetCheck(BST_UNCHECKED);
	}
    else
    {
		m_Tab01.m_passCheck.SetCheck(BST_CHECKED);
        m_Tab01.m_u11.SetWindowText(r_ini(__TEXT("Soft_Config"), __TEXT("username")));
        m_Tab01.m_p11.SetWindowText(r_ini(__TEXT("Soft_Config"), __TEXT("password")));
    }






    if(v_haveunbind != 1)
		m_Tab01.GetDlgItem(IDC_unbindButton1)->ShowWindow(SW_HIDE);

	if(v_haveclientid != 1) //�Ƿ��ж�ͨ�����������
	{
		m_Tab01.GetDlgItem(IDC_ks_clientid)->ShowWindow(SW_HIDE);
		m_Tab01.m_ks_clientid.SetWindowText(_TEXT("1"));
		m_Tab01.GetDlgItem(IDC_lblclientid)->ShowWindow(SW_HIDE);
	}

    // ���۵��ö��ٴ�ks_GetData����������10000000����ֻ������һ�����磨���۲����Ƿ���ͬ��
    // �������д������������������Ծ����Ƿ���
    // s_isup = ks_GetData(v_softcode * 10 + 1);   //���������Ƿ�ǿ�Ƹ���
    // s_version = ks_GetData(v_softcode * 10 + 2);  //���������汾
    // s_downurl = ks_GetData(v_softcode * 10 + 3);   //�����������ص�ַ
    s_notice = ks_GetData(v_softcode * 10 + 4);  //������������
    MsgBox(s_notice);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLoginUserDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLoginUserDlg::OnPaint() 
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
HCURSOR CLoginUserDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLoginUserDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    switch (m_TabContr.GetCurSel())
    {
      case 0:
            m_Tab01.ShowWindow(SW_SHOW);
            m_Tab02.ShowWindow(SW_HIDE);
            m_Tab03.ShowWindow(SW_HIDE);
            break;
       case 1:
            m_Tab01.ShowWindow(SW_HIDE);
            m_Tab02.ShowWindow(SW_SHOW);
            m_Tab03.ShowWindow(SW_HIDE);
            break;
       case 2:
            m_Tab01.ShowWindow(SW_HIDE);
            m_Tab02.ShowWindow(SW_HIDE);
            m_Tab03.ShowWindow(SW_SHOW);
            break;
      default:
            break;
    }
	
	*pResult = 0;
}

void CLoginUserDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Tab01.m_ParentHandle = this->m_hWnd;
}

void CLoginUserDlg::myOnOK()
{
	OnOK();
}
