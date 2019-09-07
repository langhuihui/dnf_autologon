// YourFormDialog.cpp : implementation file
//

#include "stdafx.h"
#include "loginKey.h"
#include "YourFormDialog.h"
#include "ksreglib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYourFormDialog dialog


CYourFormDialog::CYourFormDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CYourFormDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CYourFormDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CYourFormDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYourFormDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CYourFormDialog, CDialog)
	//{{AFX_MSG_MAP(CYourFormDialog)
	ON_BN_CLICKED(IDC_btn1, Onbtn1)
	ON_BN_CLICKED(IDC_btn2, Onbtn2)
	ON_BN_CLICKED(IDC_btn3, Onbtn3)
	ON_BN_CLICKED(IDC_btn4, Onbtn4)
	ON_BN_CLICKED(IDC_btn5, Onbtn5)
	ON_BN_CLICKED(IDC_btn6, Onbtn6)
	ON_BN_CLICKED(IDC_btn7, Onbtn7)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYourFormDialog message handlers

//advapi����1
void CYourFormDialog::Onbtn1() 
{
	// TODO: Add your control notification handler code here
	
    MsgBox(advapi(__TEXT("v_geta")));  //��ʾ���Խ�� �����õ�ʾ���ӿ� v_geta ��������ֵ


}

void CYourFormDialog::Onbtn2() 
{
	// TODO: Add your control notification handler code here
    MsgBox(advapi(__TEXT("v_getb,100,200"))); //��ʾ���Խ�� �����õ�ʾ���ӿ� v_getb ��������ֵ
}

void CYourFormDialog::Onbtn3() 
{
	// TODO: Add your control notification handler code here
    ks_advapifree(__TEXT("v_getb,100,200"));
    MsgBox(advapi(__TEXT("v_getb,100,200"))); //��ʾ���Խ�� �����õ�ʾ���ӿ� v_getb ��������ֵ
}

void CYourFormDialog::Onbtn4() 
{
	// TODO: Add your control notification handler code here
      chkPass(0);
      MsgBox(__TEXT("��֤ͨ��"));
}

void CYourFormDialog::Onbtn5() 
{
	// TODO: Add your control notification handler code here
      chkPass(1);
      MsgBox(__TEXT("��֤ͨ��"));
	
}

void CYourFormDialog::Onbtn6() 
{
	// TODO: Add your control notification handler code here
    CString tresult(__TEXT("ks_GetData����������\n\r"));
	CString msg[20];
	msg[0] = __TEXT("����֤��������");
	msg[1] = __TEXT("��������ϢA����");
	msg[2] = __TEXT("��������ϢB����");
	msg[3] = __TEXT("���û������ԡ���");
	msg[4] = __TEXT("������Ϣ����");
	msg[5] = __TEXT("��ʣ��ʱ�䡿��");
	msg[6] = __TEXT("��������ص�ַ����");
	msg[7] = __TEXT("���û�ͨ��������");
	msg[8] = __TEXT("�����������汾����");
	msg[9] = __TEXT("��������־URL����");
	msg[10] = __TEXT("���û���������");
	msg[11] = __TEXT("�������ʱ��¾����");
	msg[12] = __TEXT("���û�ʣ���������");
	msg[13] = __TEXT("���û���ǩ����");
	msg[14] = __TEXT("���û���ֵ��������");
	msg[15] = __TEXT("�����ص���������");
	msg[16] = __TEXT("���Ƿ�ɵ����࿪����");
	msg[17] = __TEXT("���û�������");
	msg[18] = __TEXT("���Ƿ����ʺš���");
	msg[19] = __TEXT("���Ƿ����½��ʶ����");
	//
	CString s(__TEXT(""));
	for(int i = 0; i <= 19; ++i)
	{
		s.Format(__TEXT("ks_GetData(%d)%s%s\n\r"), i, msg[i], ks_GetData(i));
		tresult += s;
	}
    MsgBox(tresult);  //��ʾ���Խ��
}

//advapi����2
void CYourFormDialog::Onbtn7() 
{
	// TODO: Add your control notification handler code here
    _int32 tresult;
    CString errinfo;
    errinfo = __TEXT("");
    tresult = kpoints(3, &errinfo);  // ��¼��������Ǵ�ַ�ģ��۵����ʱ�ñ���ֵ���¼������Ϣ
    if(tresult == -1)   // �۵�ʧ��
    {
        MsgBox(errinfo);
        //�۵�ʧ�ܣ�����Ը��ݾݴ�����ʾ����������Ӧ�Ĵ���������ֻ�ǹرճ���
        myExitProcess();
    }
	CString lsS;
	lsS.Format(__TEXT("�۵�ɹ����û�ʣ�����%d"), tresult);
    MsgBox(lsS);  //��ʾ���Խ��
}

void CYourFormDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	
	//�˳������¼����������ks_exit();
	ks_exit(); ////�رյ�½��ʱһ��Ҫ����ks_exit();
	CDialog::OnClose();
}
