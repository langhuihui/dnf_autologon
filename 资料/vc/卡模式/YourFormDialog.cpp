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

//advapi测试1
void CYourFormDialog::Onbtn1() 
{
	// TODO: Add your control notification handler code here
	
    MsgBox(advapi(__TEXT("v_geta")));  //显示测试结果 这里用的示例接口 v_geta 弹出返回值


}

void CYourFormDialog::Onbtn2() 
{
	// TODO: Add your control notification handler code here
    MsgBox(advapi(__TEXT("v_getb,100,200"))); //显示测试结果 这里用的示例接口 v_getb 弹出返回值
}

void CYourFormDialog::Onbtn3() 
{
	// TODO: Add your control notification handler code here
    ks_advapifree(__TEXT("v_getb,100,200"));
    MsgBox(advapi(__TEXT("v_getb,100,200"))); //显示测试结果 这里用的示例接口 v_getb 弹出返回值
}

void CYourFormDialog::Onbtn4() 
{
	// TODO: Add your control notification handler code here
      chkPass(0);
      MsgBox(__TEXT("验证通过"));
}

void CYourFormDialog::Onbtn5() 
{
	// TODO: Add your control notification handler code here
      chkPass(1);
      MsgBox(__TEXT("验证通过"));
	
}

void CYourFormDialog::Onbtn6() 
{
	// TODO: Add your control notification handler code here
    CString tresult(__TEXT("ks_GetData所有序数据\n\r"));
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
		s.Format(__TEXT("ks_GetData(%d)%s%s\n\r"), i, msg[i], ks_GetData(i));
		tresult += s;
	}
    MsgBox(tresult);  //显示测试结果
}

//advapi测试2
void CYourFormDialog::Onbtn7() 
{
	// TODO: Add your control notification handler code here
    _int32 tresult;
    CString errinfo;
    errinfo = __TEXT("");
    tresult = kpoints(3, &errinfo);  // 记录错误变量是传址的，扣点出错时该变量值会记录错误信息
    if(tresult == -1)   // 扣点失败
    {
        MsgBox(errinfo);
        //扣点失败，你可以根据据错误提示，来进行相应的处理，我这里只是关闭程序
        myExitProcess();
    }
	CString lsS;
	lsS.Format(__TEXT("扣点成功，用户剩余点数%d"), tresult);
    MsgBox(lsS);  //显示测试结果
}

void CYourFormDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	
	//退出程序事件，必须调用ks_exit();
	ks_exit(); ////关闭登陆器时一定要调用ks_exit();
	CDialog::OnClose();
}
