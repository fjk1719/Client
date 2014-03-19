
#pragma once

//#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
//#include <windows.h>
#include <afx.h>
#include <afxwin.h>
#include <shellapi.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��
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
	//���������
	CreateRun();

	CString str_pubwin_server_ip;
	CString str_Current_Pathfile;
	GetModuleFileName(NULL,str_Current_Pathfile.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	str_Current_Pathfile=str_Current_Pathfile.Left(str_Current_Pathfile.ReverseFind('\\'))+"\\config.ini";

	CString str_PubwinBatPath,str_xsBatPath,str_xsClientPath;
	

//�ж������ļ������� ������Ĭ�������ļ�
	if (!PathFileExists(str_Current_Pathfile))
	{
		MessageBox(NULL,_T("�����ļ��������Ѿ���������,����������Ϣ�Ƿ���ȷ!!! �����˳�"),NULL,MB_OK|MB_ICONERROR);
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
// 		MessageBox(NULL,_T("�����ļ���ȡʧ��"),NULL,MB_OK);
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
				//cout<<"�пͻ����������ӣ����ȴ��ͻ���������!\n";

			}

//ag:  
			if(SOCKET_ERROR==recv(client,password,sizeof(password),0))
	  {
		  //cout<<"���տͻ�������ʧ��\n";
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
				 // MessageBox(NULL,"����������ȱʧ�����ļ�,���������ļ��������г���",NULL,MB_OK);
				  send(client,"Pubwinִ���ļ�·��������",sizeof("Pubwinִ���ļ�·��������"),0);
				  
				  return 0;


			  }

			  if (!PathFileExists(str_PubwinBatPath))
			  {
				
				  send(client,"puwinִ���ļ������� �����˳�",sizeof("puwinִ���ļ������� �����˳�"),0);
				  return 0;

			  }
			  
			  if(ShellExecute(NULL,_T("open"),str_PubwinBatPath,NULL,NULL,SW_MAXIMIZE)<=(HINSTANCE)32)
			  {
				 // strcpy((char*)cmd.lparam,"�ļ�ִ��ʧ��");
				   
				  
				  send(client,"Pubwin����������ʧ��",sizeof("Pubwin����������ʧ��"),0);
			  }
			  else
			  {
				   send(client,"Pubwin�����������ɹ�",sizeof("Pubwin�����������ɹ�"),0);

			  }
		  }
		if (0==strcmp(password,"xunshan"))
			 {
				  ::GetPrivateProfileString(_T("XSServer"),_T("XS_bat"),NULL,str_xsBatPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH,str_Current_Pathfile);
				  if (str_xsBatPath=="")
				  {
					 
					  str_xsBatPath=str_Current_Pathfile.Left(str_Current_Pathfile.ReverseFind('\\'))+_T("\\xs_bat.bat");
					  ::WritePrivateProfileString(_T("XSServer"),_T("XS_bat"),str_xsBatPath,str_Current_Pathfile);
					 //  MessageBox(NULL,"����������ȱʧ�����ļ�,���������ļ��������г���",NULL,MB_OK);
					  send(client,"����������ȱʧ�����ļ�,���������ļ��������г���",sizeof("����������ȱʧ�����ļ�,���������ļ��������г���"),0);
					 


				  }
				  if (!PathFileExists(str_xsBatPath))
				  {
					  AfxMessageBox("Ѹ��ִ���ļ������� �����˳�");
					  send(client,"Ѹ��ִ���ļ������� �����˳�",sizeof("Ѹ��ִ���ļ������� �����˳�"),0);

				

				  }

				  if(ShellExecute(NULL,_T("open"),str_xsBatPath,NULL,NULL,SW_MAXIMIZE)<=(HINSTANCE)32)
				  {
					  // strcpy((char*)cmd.lparam,"�ļ�ִ��ʧ��");


					  send(client,"xs����������ʧ��",sizeof("xs����������ʧ��"),0);
				  }
				  else
				  {
					  send(client,"xs�����������ɹ�",sizeof("xs�����������ɹ�"),0);

				  }
		}
				  if (0==strcmp(password,"XSclient"))
				  {
					  ::GetPrivateProfileString(_T("XSClient"),_T("XSC_bat"),NULL,str_xsClientPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH,str_Current_Pathfile);
					  if (str_xsClientPath=="")
					  {

						  str_xsClientPath=str_Current_Pathfile.Left(str_Current_Pathfile.ReverseFind('\\'))+_T("\\xsc_bat.bat");

						  ::WritePrivateProfileString(_T("XSClient"),_T("XSC_bat"),str_xsClientPath,str_Current_Pathfile);
						 // MessageBox(NULL,"����������ȱʧ�����ļ�,���������ļ��������г���",NULL,MB_OK);
						  send(client,"����������ȱʧ�����ļ�,���������ļ��������г���",sizeof("����������ȱʧ�����ļ�,���������ļ��������г���"),0);
					


					  }
					  if (!PathFileExists(str_xsClientPath))
					  {
						 // AfxMessageBox("Ѹ��ִ���ļ������� �����˳�");
						  send(client,"Ѹ��Clientִ���ļ������� �����˳�",sizeof("Ѹ��Clientִ���ļ������� �����˳�"),0);
						

					  }

					  if(ShellExecute(NULL,_T("open"),str_xsClientPath,NULL,NULL,SW_MAXIMIZE)<=(HINSTANCE)32)
					  {
						  // strcpy((char*)cmd.lparam,"�ļ�ִ��ʧ��");
						  send(client,"Ѹ��Client����ʧ��",sizeof("Ѹ��Client����ʧ��"),0);
					  }
					  else
					  {
						  send(client,"Ѹ��Client�����ɹ�",sizeof("Ѹ��Client�����ɹ�"),0);
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
	sPath.ReleaseBuffer();   	// ��ȡ�ļ�����	
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
		///////////////////////////		// ��ע��� ʧ���˳�		
		int iRegipen=RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&RegKey);	
		if (iRegipen!=ERROR_SUCCESS)
		{			
			AfxMessageBox("ע����ʧ��,���������ʧ��");		
			return ;		
		}		
		/////////////////////////////
		//�жϼ���Ŀ���� ���ж��������ַ����ͳ���·���Ƿ�һ�� ��һ����������� һ���˳�
		LPBYTE owner_Get=new BYTE[80];	
		DWORD type_1=REG_SZ;	
		DWORD cbData_1=80;		
		int ret1=::RegQueryValueEx(RegKey,fileName,NULL,&type_1,owner_Get,&cbData_1);
		if(ret1!=ERROR_SUCCESS)	
		{			
			AfxMessageBox("��ѯ�ļ������ڣ�");
			RegSetValueEx(RegKey,fileName,0,REG_SZ,(const   unsigned   char*)(LPCTSTR)fullName,fullName.GetLength());
			//�����������Ҫ��ע�����ע�������   		

		}		
		else		
		{			
			CString cstr_get;
			cstr_get=owner_Get;		
			if (cstr_get!=fullName)	
			{				
				RegSetValueEx(RegKey,fileName,0,REG_SZ,(const   unsigned   char*)(LPCTSTR)fullName,fullName.GetLength());//�����������Ҫ��ע�����ע�������

			}	
		}	
	}  
	else  
	{   		
		//theApp.SetMainSkin(); 
		::AfxMessageBox("û�ҵ�ִ�г������������ʧ��"); 
		return; 
	}   	
	RegCloseKey(RegKey);
	return ;
} 
