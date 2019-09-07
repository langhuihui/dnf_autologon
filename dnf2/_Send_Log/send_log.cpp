#include <iostream>
using namespace std;

#include "SendMail.h"

int main()
{

	CSendMail send_mail;
	sMailInfo mail_info;
	mail_info.m_pcUserName = "gcc2236";
	mail_info.m_pcUserPassWord = "";

	mail_info.m_pcSender = "gcc2236@gmail.com";
	mail_info.m_pcReceiver = "";

	mail_info.m_pcTitle = "";
	mail_info.m_pcBody = "";

	mail_info.m_pcIPName = "";


	send_mail.SendMail( mail_info);

	return 0;
}