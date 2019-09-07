; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLoginKeyDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "loginKey.h"

ClassCount=5
Class1=CLoginKeyApp
Class2=CLoginKeyDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_IPCFormDIALOG
Class4=IPCFormDialog
Resource4=IDD_LOGINKEY_DIALOG
Class5=CYourFormDialog
Resource5=IDD_YourForm

[CLS:CLoginKeyApp]
Type=0
HeaderFile=loginKey.h
ImplementationFile=loginKey.cpp
Filter=N

[CLS:CLoginKeyDlg]
Type=0
HeaderFile=loginKeyDlg.h
ImplementationFile=loginKeyDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CLoginKeyDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=loginKeyDlg.h
ImplementationFile=loginKeyDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_LOGINKEY_DIALOG]
Type=1
Class=CLoginKeyDlg
ControlCount=14
Control1=IDC_unbindButton,button,1342242816
Control2=IDC_viewbtn,button,1342242816
Control3=IDC_loginbtn,button,1342242816
Control4=IDC_bdinfoButton,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_lblclientid,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,button,1342177287
Control9=IDC_u11,edit,1350631552
Control10=IDC_ks_clientid,edit,1350631552
Control11=IDC_passCheck,button,1342242819
Control12=IDC_Checkupdate,button,1342242819
Control13=IDC_bdinfo3,edit,1350631552
Control14=IDC_STATIC,button,1342177287

[DLG:IDD_IPCFormDIALOG]
Type=1
Class=IPCFormDialog
ControlCount=1
Control1=IDC_BUTTON1,button,1342242816

[CLS:IPCFormDialog]
Type=0
HeaderFile=IPCFormDialog.h
ImplementationFile=IPCFormDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BUTTON1

[DLG:IDD_YourForm]
Type=1
Class=CYourFormDialog
ControlCount=7
Control1=IDC_btn1,button,1342242816
Control2=IDC_btn2,button,1342242816
Control3=IDC_btn3,button,1342242816
Control4=IDC_btn4,button,1342242816
Control5=IDC_btn5,button,1342242816
Control6=IDC_btn6,button,1342242816
Control7=IDC_btn7,button,1342242816

[CLS:CYourFormDialog]
Type=0
HeaderFile=YourFormDialog.h
ImplementationFile=YourFormDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

