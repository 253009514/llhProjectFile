#define _CRT_SECURE_NO_WARNINGS

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include<fstream>
#include<direct.h>

#include <process.h>  
#include <windows.h>  
#include<iostream>
using namespace std;
#pragma comment(lib, "ws2_32.lib")


//�������Ӹ��߳�ʵ��  
#include <stdio.h>  

//���̺߳���  
unsigned int __stdcall ThreadFun(PVOID pM)
{
	static int i = 0;
	cout << i++ << endl;
	printf("�߳�ID��Ϊ%4d�����߳�˵��Hello World\n", GetCurrentThreadId());
	return 0;
}
//����������ν��������ʵ�������߳�ִ�еĺ�����  
int test()
{
	printf("     ����������߳�ʵ�� \n");
	printf(" -- by MoreWindows( http://blog.csdn.net/MoreWindows ) --\n\n");

	const int THREAD_NUM = 5;
	HANDLE handle[THREAD_NUM];
	for (int i = 0; i < THREAD_NUM; i++)
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, NULL, 0, NULL);
	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);
	getchar();
	return 0;
}


bool SendFile(SOCKET& sock, string &filename)
{
	//���ͻ���
	char sendBuf[BUFSIZ] = { 0 };
	
	FILE * fp;
	fp = fopen(filename.c_str(), "rb");
	if (!fp)
		return false;

	while (1)
	{
		memset(sendBuf, 0, BUFSIZ); 
		size_t len = fread(sendBuf, 1, BUFSIZ, fp);
		printf("sendBuf:%s\n", sendBuf);		
		
		send(sock, sendBuf, len, 0);
		if (len < 1)
			break;
	}
	cout << "Send File Finished" << endl;
	fclose(fp);
	return true;
}
unsigned int __stdcall TalkWithClient(PVOID pM)
{
	SOCKET sockConn = *(SOCKET*)pM;
	int testCount = 3;
	bool testPass = false;
	//�����֤
	while (testCount--)
	{
		char buff[BUFSIZ] = "�������û���";
		send(sockConn, buff, strlen(buff) + 1, 0);

		char uName[BUFSIZ];
		recv(sockConn, uName, BUFSIZ, 0);
		cout << "�û������û���Ϊ" << uName << endl;
		char uPass[BUFSIZ];
		recv(sockConn, uPass, BUFSIZ, 0);
		cout << "�û�����Ϊ" << uPass << endl;
		if (strcmp(uName, uPass) == 0)
		{
			testPass = true;
			break;
		}
	}

	if (!testPass)//�����֤ʧ��,�ر����ӡ�
	{
		char buff[BUFSIZ] = "�����֤ʧ�ܣ�";

		send(sockConn, buff, strlen(buff) + 1, 0);

		closesocket(sockConn);
		return 0;
	}

	//�����֤ͨ����������ʽ��������

	char sendBuff[BUFSIZ] = "�����֤�ɹ�����ӭʹ�ã�";
	send(sockConn, sendBuff, strlen(sendBuff) + 1, 0);

	Sleep(2000);
	char recvBuff[BUFSIZ];

	memset(recvBuff, 0, sizeof(recvBuff));
	recv(sockConn, recvBuff, BUFSIZ, 0);

	send(sockConn, "", 1, 0);
	if (strcmp(recvBuff, "�ļ���������") == 0)
	{
		//���͵�ǰ�������ļ��б�
		char   buffer[MAX_PATH];
		_getcwd(buffer, MAX_PATH);
		string curdir = buffer;
		string fileList = curdir + "\\FileList\\";
		_chdir(fileList.c_str());

		//memset(buffer, 0, sizeof(buffer));
		//_getcwd(buffer, MAX_PATH);
		printf("����ת����������Ŀ¼:   %s\n", buffer);

		//��ʾ�ļ��б�
		//ShowFileList(fileList);

		//�����������ļ���		
		memset(recvBuff, 0, sizeof(recvBuff));
		recv(sockConn, recvBuff, BUFSIZ, 0);
		cout << recvBuff << endl;		//�û������ļ�����

		string filename = fileList;
		filename += recvBuff;
		cout << recvBuff << endl;
		SendFile(sockConn, filename);
		closesocket(sockConn);
	}
	return 1;
}


const int THREAD_NUM = 5; //��������������


static bool Handle_connect(int sockSrv)
{
	SOCKADDR_IN addrClient;  //����ͻ��˵�ip��ַ
	int len = sizeof(SOCKADDR);

	//����ѭ��
	HANDLE handle[THREAD_NUM];
	int i = 0;
	while (i<THREAD_NUM)
	{
		//������ܽ��̻��������ȴ�����
		cout << "�������ȴ��ͻ�������" << endl;
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);  //���ܿͻ�������
		if (sockConn > 0) // ���ӳɹ�
		{
			cout << "��ǰ������Ϊ" << i + 1 << endl;
			handle[i++] = (HANDLE)_beginthreadex(NULL, 0,TalkWithClient, &sockConn, 0, NULL);
		}
	}
	WaitForMultipleObjects(THREAD_NUM, handle, true, INFINITE);
	return true;
}

int main()
{
	WORD wVersionRequested;  //typedef unsigned short WORD
	WSADATA wsaData;   //�ð���洢ϵͳ���صĹ���WinSocket������
	int err;  //�����ж����������Ƿ�����

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
		return -1;

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}

	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);   //AF_INET tcpip��Э��
	SOCKADDR_IN addrSrv;

	addrSrv.sin_addr.S_un.S_addr = inet_addr("172.16.89.39");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(1111);  //ftp server

	//���Լ���socket��ַ ��һ��socket����  ����˵�������Ϳͻ��˶�Ҫ���и�socket�� ����/�󶨣��ҷ�����ӰӦ���Ȱ�
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	//��Ƽ�����socket,��socket�Ѿ�����socket��ַ�����Է������Ӹ�socket�Ŀͻ��˶��ᱻ���뵽������
	listen(sockSrv, THREAD_NUM);  //���ü�������  ʹ������socket���ڼ���״̬

	//
	Handle_connect(sockSrv);
	closesocket(sockSrv);
	WSACleanup();
	system("pause");
	return 0;
}
