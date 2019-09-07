// TAB01DLG.cpp : implementation file
//

#include "stdafx.h"
#include "loginUser.h"
#include "TAB01DLG.h"
#include "ksreglib.h"
#include "loginUserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTAB01DLG dialog


CTAB01DLG::CTAB01DLG(CWnd* pParent /*=NULL*/)
	: CDialog(CTAB01DLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTAB01DLG)
	//}}AFX_DATA_INIT
}


void CTAB01DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTAB01DLG)
	DDX_Control(pDX, IDC_u11, m_u11);
	DDX_Control(pDX, IDC_p11, m_p11);
	DDX_Control(pDX, IDC_k11, m_K11);
	DDX_Control(pDX, IDC_Checkupdate, m_Checkupdate);
	DDX_Control(pDX, IDC_passCheck, m_passCheck);
	DDX_Control(pDX, IDC_ks_clientid, m_ks_clientid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTAB01DLG, CDialog)
	//{{AFX_MSG_MAP(CTAB01DLG)
	ON_BN_CLICKED(IDC_Checkupdate, OnCheckupdate)
	ON_BN_CLICKED(IDC_viewbtn, Onviewbtn)
	ON_BN_CLICKED(IDC_unbindButton1, OnunbindButton1)
	ON_BN_CLICKED(IDC_passCheck, OnpassCheck)
	ON_BN_CLICKED(IDC_loginbtn, Onloginbtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTAB01DLG message handlers

BOOL CTAB01DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_K11.SetWindowText(__TEXT("�������û�����ע�Ῠ��"));
	m_ks_clientid.SetWindowText(__TEXT("1"));

	m_LoginTypeID = 0;


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTAB01DLG::OnCheckupdate() 
{
	// TODO: Add your control notification handler code here
    CString temp_aaa(__TEXT("0"));
	
    if(m_Checkupdate.GetCheck() == 1) temp_aaa = __TEXT("1");
    w_ini(__TEXT("Soft_Config"), __TEXT("checkupdate"), (LPSTR)(LPCTSTR)temp_aaa);

}

void CTAB01DLG::Onviewbtn() 
{
	// TODO: Add your control notification handler code here

    CString tempstr;
    _int32 nkeypos;

    if(myGetWindowText(m_K11) == __TEXT("�������û�����ע�Ῠ��"))
    {
        MsgBox(__TEXT("�������û�����ע�Ῠ��"));
        return;
    }

    tempstr = ks_viewinfo(v_softcode, (LPSTR)(LPCTSTR)myGetWindowText(m_K11));

    // ����������������ɿ���
    nkeypos = tempstr.Find(__TEXT("��"));
    if(nkeypos > 0)
		m_u11.SetWindowText((LPSTR)(LPCTSTR)tempstr.Mid(nkeypos + 2, 32));
    if(iserrno(tempstr)) tempstr = ks_GetMsg((LPSTR)(LPCTSTR)tempstr);
    MsgBox(tempstr);
}

void CTAB01DLG::OnunbindButton1() 
{
	// TODO: Add your control notification handler code here
	CString tempstr = ks_unbind(v_softcode, _ttoi((LPSTR)(LPCTSTR)myGetWindowText(m_ks_clientid)), (LPSTR)(LPCTSTR)myGetWindowTextTrim(m_u11), (LPSTR)(LPCTSTR)myGetWindowTextTrim(m_p11));
    if(iserrno(tempstr)) tempstr = ks_GetMsg((LPSTR)(LPCTSTR)tempstr);
    MsgBox(tempstr);
}

void CTAB01DLG::OnpassCheck() 
{
	// TODO: Add your control notification handler code here

    CString temp_aaa(__TEXT("0"));
    if(m_passCheck.GetCheck() == 1) temp_aaa = __TEXT("1");
    w_ini(__TEXT("Soft_Config"), __TEXT("Rememberaccount"), (LPSTR)(LPCTSTR)temp_aaa);
}

void CTAB01DLG::Onloginbtn() 
{
	// TODO: Add your control notification handler code here

	_int32 i_rnd;
    CString v_msgstr;
	GetDlgItem(IDC_loginbtn)->EnableWindow(false);


    ks_setSoftVal(v_softcode, (LPSTR)(LPCTSTR)v_softkey);



    //--  �����û������Ϣ��DLL
	ks_setUserVal(_ttoi((LPSTR)(LPCTSTR)myGetWindowText(m_ks_clientid)), (LPSTR)(LPCTSTR)myGetWindowTextTrim(m_u11), (LPSTR)(LPCTSTR)myGetWindowTextTrim(m_p11), (LPSTR)(LPCTSTR)v_bdinfo);



    //--  �����Զ����������Ϣ��DLL�������Զ��������ܵĻ����±�һ����ȫ����ɾ��
    //--  ������1����½���ڵĴ��ھ��, 2��ǰ����İ汾��, 3�û��Ƿ�ѡ���˼�����

    ks_setUpVal((_int32)m_ParentHandle, v_softversion, (_int32)(m_Checkupdate.GetCheck()));

    //--  ��ʼ��֤
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
        myExitProcess(); //������
    }
    if(r_ini(__TEXT("Soft_Config"), __TEXT("Rememberaccount")) = __TEXT("1"))
	{
        w_ini(__TEXT("Soft_Config"), __TEXT("username"), (LPSTR)(LPCTSTR)myGetWindowText(m_u11));
        w_ini(__TEXT("Soft_Config"), __TEXT("password"), (LPSTR)(LPCTSTR)myGetWindowText(m_p11));
	}
    else
	{
        w_ini(__TEXT("Soft_Config"), __TEXT("username"), __TEXT(""));
        w_ini(__TEXT("Soft_Config"), __TEXT("password"), __TEXT(""));
	}

	GetDlgItem(IDC_loginbtn)->EnableWindow(true);
	m_LoginTypeID = v_isipcmode;
	
	CWnd * pCWnd;
	pCWnd=FromHandle(m_ParentHandle);
	((CLoginUserDlg *)pCWnd)->myOnOK();
}
