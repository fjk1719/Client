// ServerManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ServerManager.h"
#include "ServerManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerManagerDlg 对话框




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


// CServerManagerDlg 消息处理程序

BOOL CServerManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_SHOWNORMAL);

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CServerManagerDlg::OnBnClickedBtnRestartpubserver()
{
	SetDlgItemText(IDC_EDIT1,"");
	// TODO: 在此添加控件通知处理程序代码
	CString str_pubwin_server_ip;
	CString str_Current_Pathfile;
	GetModuleFileName(NULL,str_Current_Pathfile.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	str_Current_Pathfile=str_Current_Pathfile.Left(str_Current_Pathfile.ReverseFind('\\'))+_T("\\config.ini");

	if (!PathFileExists(str_Current_Pathfile))
	{
		::MessageBox(NULL,_T("配置文件不存在已经重新生成,请检查配置信息是否正确!!! 程序退出"),NULL,MB_OK|MB_ICONERROR);
		::WritePrivateProfileString(_T("server"),_T("serverIp"),_T("10.30.9.43"),str_Current_Pathfile);
		return ;
	}
	::GetPrivateProfileString(_T("server"),_T("serverIp"),NULL,str_pubwin_server_ip.GetBufferSetLength(MAX_PATH+1),MAX_PATH,str_Current_Pathfile);
	if (str_pubwin_server_ip== "")
	{
		::WritePrivateProfileString(_T("server"),_T("serverIp"),_T("10.30.9.43"),str_Current_Pathfile);
		::MessageBox(NULL,_T("配置文件读取失败"),NULL,MB_OK);
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
		::MessageBox(NULL,_T("无法连接服务器"),NULL,MB_OK);
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
					::MessageBox(NULL,_T("发送密码失败!"),NULL,MB_OK);
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
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT1,"");
	
	CString str_pubwin_server_ip;
	CString str_Current_Pathfile;
	GetModuleFileName(NULL,str_Current_Pathfile.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	str_Current_Pathfile=str_Current_Pathfile.Left(str_Current_Pathfile.ReverseFind('\\'))+_T("\\config.ini");

	if (!PathFileExists(str_Current_Pathfile))
	{
		::MessageBox(NULL,_T("配置文件不存在已经重新生成,请检查配置信息是否正确!!! 程序退出"),NULL,MB_OK|MB_ICONERROR);
		::WritePrivateProfileString(_T("XSserver"),_T("XSserverIp"),_T("10.30.9.45"),str_Current_Pathfile);
		return ;
	}
	::GetPrivateProfileString(_T("XSserver"),_T("XSserverIp"),NULL,str_pubwin_server_ip.GetBufferSetLength(MAX_PATH+1),MAX_PATH,str_Current_Pathfile);
	if (str_pubwin_server_ip=="")
	{
		::WritePrivateProfileString(_T("XSserver"),_T("XSserverIp"),_T("10.30.9.45"),str_Current_Pathfile);
		::MessageBox(NULL,_T("配置文件读取失败"),NULL,MB_OK);
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
		::MessageBox(NULL,_T("无法连接服务器"),NULL,MB_OK);
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
					::MessageBox(NULL,_T("发送密码失败!"),NULL,MB_OK);
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
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT1,"");

	CString str_pubwin_server_ip;
	CString str_Current_Pathfile;
	GetModuleFileName(NULL,str_Current_Pathfile.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	str_Current_Pathfile=str_Current_Pathfile.Left(str_Current_Pathfile.ReverseFind('\\'))+_T("\\config.ini");

	if (!PathFileExists(str_Current_Pathfile))
	{
		::MessageBox(NULL,_T("配置文件不存在已经重新生成,请检查配置信息是否正确!!! 程序退出"),NULL,MB_OK|MB_ICONERROR);
		::WritePrivateProfileString(_T("XSclient"),_T("XSclientIp"),_T("10.30.9.72"),str_Current_Pathfile);
		return ;
	}
	::GetPrivateProfileString(_T("XSclient"),_T("XSclientIp"),NULL,str_pubwin_server_ip.GetBufferSetLength(MAX_PATH+1),MAX_PATH,str_Current_Pathfile);
	if (str_pubwin_server_ip=="")
	{
		::WritePrivateProfileString(_T("XSclient"),_T("XSclientIp"),_T("10.30.9.72"),str_Current_Pathfile);
		::MessageBox(NULL,_T("配置文件读取失败"),NULL,MB_OK);
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
		::MessageBox(NULL,_T("无法连接服务器"),NULL,MB_OK);
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
					::MessageBox(NULL,_T("发送密码失败!"),NULL,MB_OK);
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
