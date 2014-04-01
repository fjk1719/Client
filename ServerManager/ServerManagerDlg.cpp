// ServerManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ServerManager.h"
#include "ServerManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerManagerDlg �Ի���




CServerManagerDlg::CServerManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CServerManagerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_RESTARTPUBSERVER, &CServerManagerDlg::OnBnClickedBtnRestartpubserver)
	ON_BN_CLICKED(IDC_BBTN_RESTARTXSSERVER, &CServerManagerDlg::OnBnClickedBbtnRestartxsserver)
	ON_BN_CLICKED(IDC_BTN_RESTARTXSCLIENT, &CServerManagerDlg::OnBnClickedBtnRestartxsclient)
END_MESSAGE_MAP()


// CServerManagerDlg ��Ϣ�������

BOOL CServerManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_SHOWNORMAL);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServerManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CServerManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CServerManagerDlg::OnBnClickedBtnRestartpubserver()
{
	SetDlgItemText(IDC_EDIT1,"");
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str_pubwin_server_ip;
	CString str_Current_Pathfile;
	GetModuleFileName(NULL,str_Current_Pathfile.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	str_Current_Pathfile=str_Current_Pathfile.Left(str_Current_Pathfile.ReverseFind('\\'))+_T("\\config.ini");

	if (!PathFileExists(str_Current_Pathfile))
	{
		::MessageBox(NULL,_T("�����ļ��������Ѿ���������,����������Ϣ�Ƿ���ȷ!!! �����˳�"),NULL,MB_OK|MB_ICONERROR);
		::WritePrivateProfileString(_T("server"),_T("serverIp"),_T("10.30.9.43"),str_Current_Pathfile);
		return ;
	}
	::GetPrivateProfileString(_T("server"),_T("serverIp"),NULL,str_pubwin_server_ip.GetBufferSetLength(MAX_PATH+1),MAX_PATH,str_Current_Pathfile);
	if (str_pubwin_server_ip== "")
	{
		::WritePrivateProfileString(_T("server"),_T("serverIp"),_T("10.30.9.43"),str_Current_Pathfile);
		::MessageBox(NULL,_T("�����ļ���ȡʧ��"),NULL,MB_OK);
		return ;
	}
	WSADATA wsadata;
	WORD ver=MAKEWORD(2,2);
	WSAStartup(ver,&wsadata);
	SOCKET server;
	server =socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN serveraddr;
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(12345);

	serveraddr.sin_addr.S_un.S_addr=inet_addr(str_pubwin_server_ip.GetBuffer());
	if(connect(server, (SOCKADDR*)&serveraddr, sizeof(serveraddr))<0)
	{
		::MessageBox(NULL,_T("�޷����ӷ�����"),NULL,MB_OK);
		return ;
	}
	else
	{
		char Buff[100];
		int ret = recv(server, Buff, sizeof(Buff), 0);
		if(ret != 0)
		{
			if(strcmp(Buff, "Password") ==0)
			{
				//char pass[100];
				//strcpy(pass, "Pubwin");
				if(send(server, "Pubwin", 10, 0) == SOCKET_ERROR)
				{
					::MessageBox(NULL,_T("��������ʧ��!"),NULL,MB_OK);
					closesocket(server);
				}
				else
				{
					ret=recv(server,Buff,sizeof(Buff),0);
					if(ret != 0)
					{

						SetDlgItemText(IDC_EDIT1,Buff);
						closesocket(server);
					}

				}
			}


		}




	}
	closesocket(server);
	return ;

}
//xs
void CServerManagerDlg::OnBnClickedBbtnRestartxsserver()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemText(IDC_EDIT1,"");
	
	CString str_pubwin_server_ip;
	CString str_Current_Pathfile;
	GetModuleFileName(NULL,str_Current_Pathfile.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	str_Current_Pathfile=str_Current_Pathfile.Left(str_Current_Pathfile.ReverseFind('\\'))+_T("\\config.ini");

	if (!PathFileExists(str_Current_Pathfile))
	{
		::MessageBox(NULL,_T("�����ļ��������Ѿ���������,����������Ϣ�Ƿ���ȷ!!! �����˳�"),NULL,MB_OK|MB_ICONERROR);
		::WritePrivateProfileString(_T("XSserver"),_T("XSserverIp"),_T("10.30.9.45"),str_Current_Pathfile);
		return ;
	}
	::GetPrivateProfileString(_T("XSserver"),_T("XSserverIp"),NULL,str_pubwin_server_ip.GetBufferSetLength(MAX_PATH+1),MAX_PATH,str_Current_Pathfile);
	if (str_pubwin_server_ip=="")
	{
		::WritePrivateProfileString(_T("XSserver"),_T("XSserverIp"),_T("10.30.9.45"),str_Current_Pathfile);
		::MessageBox(NULL,_T("�����ļ���ȡʧ��"),NULL,MB_OK);
		return ;
	}
	WSADATA wsadata;
	WORD ver=MAKEWORD(2,2);
	WSAStartup(ver,&wsadata);
	SOCKET server;
	server =socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN serveraddr;
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(12345);

	serveraddr.sin_addr.S_un.S_addr=inet_addr(str_pubwin_server_ip.GetBuffer());
	if(connect(server, (SOCKADDR*)&serveraddr, sizeof(serveraddr))<0)
	{
		::MessageBox(NULL,_T("�޷����ӷ�����"),NULL,MB_OK);
		return ;
	}
	else
	{
		char Buff[100];
		int ret = recv(server, Buff, sizeof(Buff), 0);
		if(ret != 0)
		{
			if(strcmp(Buff, "Password") ==0)
			{
				//char pass[100];
				//strcpy(pass, "Pubwin");
				if(send(server, "xunshan", 10, 0) == SOCKET_ERROR)
				{
					::MessageBox(NULL,_T("��������ʧ��!"),NULL,MB_OK);
					closesocket(server);
				}
				else
				{
					ret=recv(server,Buff,sizeof(Buff),0);
					if(ret != 0)
					{

						SetDlgItemText(IDC_EDIT1,Buff);
						closesocket(server);
					}

				}
			}


		}




	}
	closesocket(server);
	return ;
}

void CServerManagerDlg::OnBnClickedBtnRestartxsclient()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemText(IDC_EDIT1,"");

	CString str_pubwin_server_ip;
	CString str_Current_Pathfile;
	GetModuleFileName(NULL,str_Current_Pathfile.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	str_Current_Pathfile=str_Current_Pathfile.Left(str_Current_Pathfile.ReverseFind('\\'))+_T("\\config.ini");

	if (!PathFileExists(str_Current_Pathfile))
	{
		::MessageBox(NULL,_T("�����ļ��������Ѿ���������,����������Ϣ�Ƿ���ȷ!!! �����˳�"),NULL,MB_OK|MB_ICONERROR);
		::WritePrivateProfileString(_T("XSclient"),_T("XSclientIp"),_T("10.30.9.72"),str_Current_Pathfile);
		return ;
	}
	::GetPrivateProfileString(_T("XSclient"),_T("XSclientIp"),NULL,str_pubwin_server_ip.GetBufferSetLength(MAX_PATH+1),MAX_PATH,str_Current_Pathfile);
	if (str_pubwin_server_ip=="")
	{
		::WritePrivateProfileString(_T("XSclient"),_T("XSclientIp"),_T("10.30.9.72"),str_Current_Pathfile);
		::MessageBox(NULL,_T("�����ļ���ȡʧ��"),NULL,MB_OK);
		return ;
	}
	WSADATA wsadata;
	WORD ver=MAKEWORD(2,2);
	WSAStartup(ver,&wsadata);
	SOCKET server;
	server =socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN serveraddr;
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(12345);

	serveraddr.sin_addr.S_un.S_addr=inet_addr(str_pubwin_server_ip.GetBuffer());
	if(connect(server, (SOCKADDR*)&serveraddr, sizeof(serveraddr))<0)
	{
		::MessageBox(NULL,_T("�޷����ӷ�����"),NULL,MB_OK);
		return ;
	}
	else
	{
		char Buff[100];
		int ret = recv(server, Buff, sizeof(Buff), 0);
		if(ret != 0)
		{
			if(strcmp(Buff, "Password") ==0)
			{
				//char pass[100];
				//strcpy(pass, "Pubwin");
				if(send(server, "XSclient", 10, 0) == SOCKET_ERROR)
				{
					::MessageBox(NULL,_T("��������ʧ��!"),NULL,MB_OK);
					closesocket(server);
				}
				else
				{
					ret=recv(server,Buff,sizeof(Buff),0);
					if(ret != 0)
					{

						SetDlgItemText(IDC_EDIT1,Buff);
						closesocket(server);
					}

				}
			}


		}




	}
	closesocket(server);
	return ;
}
