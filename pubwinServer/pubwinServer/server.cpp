
#pragma once

//#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
//#include <windows.h>
#include <afx.h>
#include <afxwin.h>
#include <shellapi.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的
#include<WinSock2.h>



#include <atlbase.h>
#include <atlstr.h>
#include <Windows.h>
#include <stdio.h>
#pragma  comment(lib,"Ws2_32.lib")
void CreateRun();
int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	//启动项添加
	CreateRun();

	CString str_pubwin_server_ip;
	CString str_Current_Pathfile;
	GetModuleFileName(NULL,str_Current_Pathfile.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	str_Current_Pathfile=str_Current_Pathfile.Left(str_Current_Pathfile.ReverseFind('\\'))+"\\config.ini";

	CString str_PubwinBatPath,str_xsBatPath,str_xsClientPath;
	

//判断配置文件不存在 就生成默认配置文件
	if (!PathFileExists(str_Current_Pathfile))
	{
		MessageBox(NULL,_T("配置文件不存在已经重新生成,请检查配置信息是否正确!!! 程序退出"),NULL,MB_OK|MB_ICONERROR);
		str_PubwinBatPath=str_Current_Pathfile.Left(str_Current_Pathfile.ReverseFind('\\'))+"\\pubwin.bat";
		::WritePrivateProfileString(_T("Pubwin"),_T("Pubin_bat"),str_PubwinBatPath,str_Current_Pathfile);

		str_xsBatPath=str_Current_Pathfile.Left(str_Current_Pathfile.ReverseFind('\\'))+_T("\\xs_bat.bat");
		::WritePrivateProfileString(_T("XSServer"),_T("XS_bat"),str_xsBatPath,str_Current_Pathfile);
		str_xsClientPath=str_Current_Pathfile.Left(str_Current_Pathfile.ReverseFind('\\'))+_T("\\xsc_bat.bat");

		::WritePrivateProfileString(_T("XSClient"),_T("XSC_bat"),str_xsClientPath,str_Current_Pathfile);
		return 0;
	}
// 	::GetPrivateProfileString(_T("Pubwin"),_T("Pubin_bat"),NULL,str_PubwinBatPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH,str_Current_Pathfile);
// 	if (str_PubwinBatPath=="")
// 	{
// 		::WritePrivateProfileString(_T("Pubwin"),_T("Pubin_bat"),str_xsBatPath,str_Current_Pathfile);
// 		::WritePrivateProfileString(_T("XSServer"),_T("XS_bat"),str_xsBatPath,str_Current_Pathfile);
// 		MessageBox(NULL,_T("配置文件读取失败"),NULL,MB_OK);
// 		return 0;
// 	}


	WSADATA wsadata;
	WORD ver=MAKEWORD(2,2);
	WSAStartup(ver,&wsadata);
	SOCKET server;
	server =socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN serveraddr;
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(12345);
	serveraddr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	bind(server,(SOCKADDR*)&serveraddr,sizeof(serveraddr));
	listen(server,10);
	SOCKET client;
	char password[100];
	while(true)
	{
		if(INVALID_SOCKET!=(client=accept(server,0,0)))
		{
			if(SOCKET_ERROR!=send(client,"Password",sizeof("Password"),0))
			{
				//cout<<"有客户端请求连接，正等待客户输入密码!\n";

			}

//ag:  
			if(SOCKET_ERROR==recv(client,password,sizeof(password),0))
	  {
		  //cout<<"接收客户端密码失败\n";
		  return 0;
	  }
	  else
	  {
		  if (0==strcmp(password,"Pubwin"))
		  {
			  ::GetPrivateProfileString(_T("Pubwin"),_T("Pubin_bat"),NULL,str_PubwinBatPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH,str_Current_Pathfile);
			  if (str_PubwinBatPath=="")
			  {

				  str_PubwinBatPath=str_Current_Pathfile.Left(str_Current_Pathfile.ReverseFind('\\'))+"\\pubwin.bat";
				  ::WritePrivateProfileString(_T("Pubwin"),_T("Pubin_bat"),str_PubwinBatPath,str_Current_Pathfile);
				 // MessageBox(NULL,"已重新生成缺失配置文件,请检查配置文件重新运行程序",NULL,MB_OK);
				  send(client,"Pubwin执行文件路径不存在",sizeof("Pubwin执行文件路径不存在"),0);
				  
				  return 0;


			  }

			  if (!PathFileExists(str_PubwinBatPath))
			  {
				
				  send(client,"puwin执行文件不存在 程序退出",sizeof("puwin执行文件不存在 程序退出"),0);
				  return 0;

			  }
			  
			  if(ShellExecute(NULL,_T("open"),str_PubwinBatPath,NULL,NULL,SW_MAXIMIZE)<=(HINSTANCE)32)
			  {
				 // strcpy((char*)cmd.lparam,"文件执行失败");
				   
				  
				  send(client,"Pubwin服务器重启失败",sizeof("Pubwin服务器重启失败"),0);
			  }
			  else
			  {
				   send(client,"Pubwin服务器重启成功",sizeof("Pubwin服务器重启成功"),0);

			  }
		  }
		if (0==strcmp(password,"xunshan"))
			 {
				  ::GetPrivateProfileString(_T("XSServer"),_T("XS_bat"),NULL,str_xsBatPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH,str_Current_Pathfile);
				  if (str_xsBatPath=="")
				  {
					 
					  str_xsBatPath=str_Current_Pathfile.Left(str_Current_Pathfile.ReverseFind('\\'))+_T("\\xs_bat.bat");
					  ::WritePrivateProfileString(_T("XSServer"),_T("XS_bat"),str_xsBatPath,str_Current_Pathfile);
					 //  MessageBox(NULL,"已重新生成缺失配置文件,请检查配置文件重新运行程序",NULL,MB_OK);
					  send(client,"已重新生成缺失配置文件,请检查配置文件重新运行程序",sizeof("已重新生成缺失配置文件,请检查配置文件重新运行程序"),0);
					 


				  }
				  if (!PathFileExists(str_xsBatPath))
				  {
					  AfxMessageBox("迅闪执行文件不存在 程序退出");
					  send(client,"迅闪执行文件不存在 程序退出",sizeof("迅闪执行文件不存在 程序退出"),0);

				

				  }

				  if(ShellExecute(NULL,_T("open"),str_xsBatPath,NULL,NULL,SW_MAXIMIZE)<=(HINSTANCE)32)
				  {
					  // strcpy((char*)cmd.lparam,"文件执行失败");


					  send(client,"xs服务器重启失败",sizeof("xs服务器重启失败"),0);
				  }
				  else
				  {
					  send(client,"xs服务器重启成功",sizeof("xs服务器重启成功"),0);

				  }
		}
				  if (0==strcmp(password,"XSclient"))
				  {
					  ::GetPrivateProfileString(_T("XSClient"),_T("XSC_bat"),NULL,str_xsClientPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH,str_Current_Pathfile);
					  if (str_xsClientPath=="")
					  {

						  str_xsClientPath=str_Current_Pathfile.Left(str_Current_Pathfile.ReverseFind('\\'))+_T("\\xsc_bat.bat");

						  ::WritePrivateProfileString(_T("XSClient"),_T("XSC_bat"),str_xsClientPath,str_Current_Pathfile);
						 // MessageBox(NULL,"已重新生成缺失配置文件,请检查配置文件重新运行程序",NULL,MB_OK);
						  send(client,"已重新生成缺失配置文件,请检查配置文件重新运行程序",sizeof("已重新生成缺失配置文件,请检查配置文件重新运行程序"),0);
					


					  }
					  if (!PathFileExists(str_xsClientPath))
					  {
						 // AfxMessageBox("迅闪执行文件不存在 程序退出");
						  send(client,"迅闪Client执行文件不存在 程序退出",sizeof("迅闪Client执行文件不存在 程序退出"),0);
						

					  }

					  if(ShellExecute(NULL,_T("open"),str_xsClientPath,NULL,NULL,SW_MAXIMIZE)<=(HINSTANCE)32)
					  {
						  // strcpy((char*)cmd.lparam,"文件执行失败");
						  send(client,"迅闪Client重启失败",sizeof("迅闪Client重启失败"),0);
					  }
					  else
					  {
						  send(client,"迅闪Client重启成功",sizeof("迅闪Client重启成功"),0);
					  }
				  }

			  

		  }
		  

	  
		}
	}
}
void CreateRun()
{	
	HKEY   RegKey;   	
	CString   sPath;   	
	CString fileName;	
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer();   	// 获取文件名字	
	fileName=sPath.Right(sPath.GetLength()-sPath.ReverseFind('\\')-1);
	fileName=fileName.Left(fileName.ReverseFind('.'));
	CFileFind   fFind;   	
	BOOL   bSuccess;   	
	bSuccess=fFind.FindFile(sPath);  
	fFind.Close();   
	if(bSuccess)   
	{   	
		CString   fullName;  
		fullName=sPath;   		
		RegKey=NULL;   	
		///////////////////////////		// 打开注册表 失败退出		
		int iRegipen=RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&RegKey);	
		if (iRegipen!=ERROR_SUCCESS)
		{			
			AfxMessageBox("注册表打开失败,启动项添加失败");		
			return ;		
		}		
		/////////////////////////////
		//判断键项目存在 则判断启动项字符串和程序路径是否一致 不一致添加启动项 一致退出
		LPBYTE owner_Get=new BYTE[80];	
		DWORD type_1=REG_SZ;	
		DWORD cbData_1=80;		
		int ret1=::RegQueryValueEx(RegKey,fileName,NULL,&type_1,owner_Get,&cbData_1);
		if(ret1!=ERROR_SUCCESS)	
		{			
			AfxMessageBox("查询的键不存在！");
			RegSetValueEx(RegKey,fileName,0,REG_SZ,(const   unsigned   char*)(LPCTSTR)fullName,fullName.GetLength());
			//这里加上你需要在注册表中注册的内容   		

		}		
		else		
		{			
			CString cstr_get;
			cstr_get=owner_Get;		
			if (cstr_get!=fullName)	
			{				
				RegSetValueEx(RegKey,fileName,0,REG_SZ,(const   unsigned   char*)(LPCTSTR)fullName,fullName.GetLength());//这里加上你需要在注册表中注册的内容

			}	
		}	
	}  
	else  
	{   		
		//theApp.SetMainSkin(); 
		::AfxMessageBox("没找到执行程序，启动项加载失败"); 
		return; 
	}   	
	RegCloseKey(RegKey);
	return ;
} 
