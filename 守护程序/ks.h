void KSInit(){
	CString LicenseKey, v_rsamod, v_rsapubkey;
	CString v_pccode, s_notice;
	int v_haveclientid, v_haveunbind, v_cstime, v_is2svr;
	// �±�����������Ҫ�����Զ����������ϵ�ͨ���ؼ��ͽ��ť�Ƿ�ɼ�
	v_haveclientid = 1;   //�Ƿ��ж�ͨ����
	v_haveunbind = 1;   //�Ƿ�����ͻ�ͨ�����ť���
	// ��������������IPC����ͨѶģʽ���˲������Ϊ1
	v_isipcmode = 0;
	// �����������Զ����¹��ܣ����Ϊ0���ر��Զ����£���ks_setUVal�ӿں͹���ˡ�����������á���ġ��Զ����¡�ѡ���Ĳ�����ʵ���Զ������������
	v_softversion = 1;
	// ������������������֤ģ��ȡ�Ļ����벻����ȫ�򾭳��䶯��������Լ�д����ȡ���������������Ҫ����������ַ�������������
	v_pccode = __TEXT("");

	// ��������¼��־���ݵ��ļ� c:\kss.ini
	v_inipath = __TEXT("kss.ini");

	// ����������ӷ�������ʱĬ��Ϊ6000����
	v_cstime = 6000;

	// ������������װ����֤��������������1��������0
	v_is2svr = 0;


	// �������˲�������Ĺ���ˡ�����б���[������]��ֵ
	v_softcode = 1000102;

	// �������˲�������Ĺ���ˡ�����б���[�����Կ]��ֵ
	v_softkey = __TEXT("IpG971PHDj2psv4TX2SlL247");

	//������� 800��ҵ���û����±���������������μ� http://www.hphu.com/news_14.html
	//��Ԫ�����ߺ������û��벻Ҫ�޸�RSA����
	v_rsamod = __TEXT("65770992667108949439609301548660913131275548029934681330823001172159438539889");  // v_rsa mod = __TEXT("") ���ǹر�RSA���ܣ������ҲӦ�ر�RSA
	v_rsapubkey = __TEXT("65537");  //v_rsapubkey = __TEXT("")  ���ǹر�RSA���ܣ������ҲӦ�ر�RSA

	// ������� �����ݴӹ���˻�ȡ�����õ����������б�->���Ͻǡ�ȡǩ�����ݡ���ť�������ɻ�ȡ��
	signData = __TEXT("");
	signData += __TEXT("cdf68e38d62157c19c9c0e020638ce96e935a27839c8a76eae40995434cfc750931c2d42bf913e98fa4377cae72b59696e93");
	signData += __TEXT("ab4dae07ae74018423523fafad374224a59ad54a15e8e46731a18ce5a8cecdcc85dcccb32c46d090f38b8c0b7a8ecc205174");
	signData += __TEXT("f040cf3d3cb25c020e0261f8c631784da649137497c8a63a1ccc69018a401df3b50441b4b6c10c235a843b5a29b2fa26fc9a");
	signData += __TEXT("ba49980d98ecaf2311822bbd8ea55c144853b01958b04cae2f632cfea754f05d786920e94265e0344e24f395cc32ad4e3cef");
	signData += __TEXT("fe85fc7e651ef36cf395bd1b3a901fa60e1b572b92ad5feabf637b948d2b9672352faa624d01b26395759e68a7df17195d5d");
	signData += __TEXT("d8e447e8a97f92da7b6b55db75a20eefe2dab43228d315f79a24a5b720a82619c32a5914407f9f891b50d72f353f28a9273d");
	signData += __TEXT("c2c42ad3b8cdaa6438f20c309702a2be253f8c0cb2df9ba33971e53d377b36babae935c127c6c977ac0ddf7a9bf2a43c0d3d");
	signData += __TEXT("fe00cbc7a151efd10242176dbdca6d68abd339bf240cb4c92a2b812009fd6d1f0a9e0729cc533622d0759a44860968cef450");

	v_bdinfo = __TEXT("");           // �û��İ���Ϣ��
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
	ks_setExtVal(0, (LPSTR)(LPCTSTR)v_pccode, (LPSTR)(LPCTSTR)v_inipath, v_cstime, v_is2svr, (LPSTR)(LPCTSTR)v_rsamod, (LPSTR)(LPCTSTR)v_rsapubkey, __TEXT(""), __TEXT(""), __TEXT(""));


	if (v_haveclientid != 1) //�Ƿ��ж�ͨ�����������
	{
		//��ʱע�����   loginbtn.Left := 362;
		//��ʱע�����   loginbtn.Top := 13;
		//��ʱע�����   viewbtn.Left := 362;
		//��ʱע�����   viewbtn.Top := 41;
		//��ʱע�����   unbindButton1.Left := 278;
		//��ʱע�����   unbindButton1.Top := 41;
	}

	//if (v_haveunbind == 0)
	//	GetDlgItem(IDC_unbindButton)->ShowWindow(SW_HIDE);
	//else
	//	GetDlgItem(IDC_unbindButton)->ShowWindow(SW_SHOW);

	// ���۵��ö��ٴ�ks_GetData����������10000000����ֻ������һ�����磨���۲����Ƿ���ͬ��
	// �������д������������������Ծ����Ƿ���
	// s_isup = ks_GetData(v_softcode * 10 + 1);   //���������Ƿ�ǿ�Ƹ���
	// s_version = ks_GetData(v_softcode * 10 + 2);  //���������汾
	// s_downurl = ks_GetData(v_softcode * 10 + 3);   //�����������ص�ַ

}
bool KSLogin(int lsm_ks_clientid, LPSTR userName){
	_int32 i_rnd;
	CString v_msgstr;
	ks_setSoftVal(v_softcode, (LPSTR)(LPCTSTR)v_softkey);
	ks_setUserVal(lsm_ks_clientid, userName, _TEXT(""), (LPSTR)(LPCTSTR)v_bdinfo);
	srand((unsigned)time(NULL));
	i_rnd = (int)(49 * rand() / (RAND_MAX + 1.0));
	v_msgstr = ks_IdCheck((LPSTR)(LPCTSTR)signData.Mid(i_rnd * 8, 8), 1);
	return !iserrno(v_msgstr);
}