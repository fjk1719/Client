// Client.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Client.h"
#include<WinSock2.h>
#pragma  comment(lib,"Ws2_32.lib")

#define MAX_LOADSTRING 100




int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	CString str_pubwin_server_ip;
	CString str_Current_Pathfile;
	GetModuleFileName(NULL,str_Current_Pathfile.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	str_Current_Pathfile=str_Current_Pathfile.Left(str_Current_Pathfile.ReverseFind('\\'))+_T("\\config.ini");

	if (!PathFileExists(str_Current_Pathfile))
	{
		MessageBox(NULL,_T("配置文件不存在已经重新生成,请检查配置信息是否正确!!! 程序退出"),NULL,MB_OK|MB_ICONERROR);
		::WritePrivateProfileString(_T("server"),_T("serverIp"),_T("10.30.9.43"),str_Current_Pathfile);
		return 0;
	}
	::GetPrivateProfileString(_T("server"),_T("serverIp"),NULL,str_pubwin_server_ip.GetBufferSetLength(MAX_PATH+1),MAX_PATH,str_Current_Pathfile);
	if (str_pubwin_server_ip.IsEmpty())
	{
		MessageBox(NULL,_T("配置文件读取失败"),NULL,MB_OK);
		return 0;
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
		MessageBox(NULL,_T("套字节初始化失败"),NULL,MB_OK);
		return 0;
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
					MessageBox(NULL,_T("发送密码失败!"),NULL,MB_OK);
					closesocket(server);
				}
			}


	    }



	   
       }
	 return 0;




}