#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include<iostream>
#include<fstream>
using namespace std;
#pragma comment(lib, "ws2_32.lib")



#include<iostream>
#include<stdlib.h>
#include<windows.h>
#include<mmsystem.h>  
#include<string>
#include<time.h>
#include<fstream>
#include<stdio.h>
#include<vector>
#include<string>
#pragma comment (lib, "winmm.lib")


void ShowFileList(string filename, SOCKET sockConn)
{

	WIN32_FIND_DATAA p;
	HANDLE h = FindFirstFileA(filename.c_str(), &p);
	if (string(p.cFileName) != "." || string(p.cFileName) != "..")
	{
		send(sockConn, p.cFileName, 260, 0);
	}
	while (FindNextFileA(h, &p))
	{
		if (string(p.cFileName) != "." || string(p.cFileName) != "..")
		{
			cout << p.cFileName << endl;
			send(sockConn, p.cFileName, 260, 0);
		}
	}
	char end[260] = "transform end";
	send(sockConn, "transform end", 260, 0);
	/*for (int i = 0; i < filelist.size(); i++)
	{
	cout << filelist[i] << endl;
	}*/
}

void ShowLastFileList(string & filepath, SOCKET sockConn)
{
	string filepath2 = filepath;
	string tmp = "../";
	tmp += filepath2;
	filepath = tmp;
	ShowFileList(tmp, sockConn);
}


void ShowSelectFileList(string &filepath, SOCKET sockConn)
{
	char Path[260] = { 0 };
	recv(sockConn, Path, 260, 0);
	cout << "Path:" << Path << endl;
	string yourchoose(Path);
	yourchoose += '/';
	string filepath2 = filepath;
	yourchoose += filepath2;
	ShowFileList(yourchoose, sockConn);
}



string ExecuteCommand(SOCKET sockConn)
{
	string filepath = "";
	string FileName = "*.*";
	string FilePath = filepath + FileName;
	while (1)
	{
		char *command = new char(0);
		recv(sockConn, command, 1, 0);
		cout << "command" << command << endl;
		if (*command == '4')
		{
			char filename[260] = { 0 };
			recv(sockConn, filename, 260, 0);
			cout << "FilePath1:  " << FilePath << endl;
			FilePath.erase(FilePath.size() - 3, 3);
			cout << "FilePath2:  " << FilePath << endl;
			FilePath += filename;
			break;
		}
		switch (*command)																  //���ȷ��ļ���
		{
		case '1':
			ShowFileList(FilePath, sockConn);
			break;
		case '2':
			ShowLastFileList(FilePath, sockConn);
			break;
		case '3':
			ShowSelectFileList(FilePath, sockConn);
			break;
		case '5':
			closesocket(sockConn);
			exit(0);
			break;
		}
	}
	return FilePath;
}





int main()
{
	WORD wVersionRequested;  //typedef unsigned short WORD
	WSADATA wsaData;   //�ð���洢ϵͳ���صĹ���WinSocket������
	int err;  //�����ж����������Ƿ�����

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}

	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);   //AF_INET tcpip��Э��

	SOCKADDR_IN addrSrv;

	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8001);  //ftp server

	//���Լ���socket��ַ ��һ��socket����  ����˵�������Ϳͻ��˶�Ҫ���и�socket�� ����/�󶨣��ҷ�����ӰӦ���Ȱ�
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	//��Ƽ�����socket,��socket�Ѿ�����socket��ַ�����Է������Ӹ�socket�Ŀͻ��˶��ᱻ���뵽������
	listen(sockSrv, 5);  //���ü�������  ʹ������socket���ڼ���״̬

	SOCKADDR_IN addrClient;  //����ͻ��˵�ip��ַ
	int len = sizeof(SOCKADDR);

	//����ѭ��
	while (1)
	{
		//������ܽ��̻��������ȴ�����
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);  //���ܿͻ�������
		//���յ�һ������ʱ ���͸����ļ�


		//���Ƚ���һ������
		string FilePath = ExecuteCommand(sockConn);
		cout << "return FilePath:  " << FilePath << endl;
		//	char filename[256] = { "test2.txt " };  //������255���ַ�

		//		send(sockConn, filename, strlen(filename) + 1, 0);

		//send(sockConn, "hello", 6, 0);
		//Sleep(2000);
		FILE * fp;
		fp = fopen(FilePath.c_str(), "rb");
		if (!fp)
			return 0;


		//���ͻ���
		char sendBuf[512] = { 0 };  //һ��512B ����

		//FILE * fp = fopen("F:\\1.pdf", "rb");
		int count = 0;


		while (1)
		{
			memset(sendBuf, 0, 512);
			int len = fread(sendBuf, 1, 512, fp);
			send(sockConn, sendBuf, len, 0);
			//	printf("sendBuf:%c\n",* sendBuf);
			//	printf("len:%d\n", len);
			count++;
			if (len<1)
				break;
		}
		//memset(sendBuf, 0, 1);
		//strcpy(sendBuf, "end");
		//send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);
		//printf("Finished!\n%d",count);
		closesocket(sockConn);
	}

	closesocket(sockSrv);
	WSACleanup();
	system("pause");
	return 0;
}

//�ͻ���

#define	_CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>;
#include<vector>
#include<string>
using namespace std;

void MainMenu()
{
	printf("��ѡ�����\n");
	printf("1.��ʾ��ǰ�ļ��е������ļ�\n");
	printf("2.������һ��\n");
	printf("3.ѡ���ļ���\n");
	printf("4.����Ҫ���ص��ļ�\n");
	printf("5.�˳�\n");
}

void Showfilelist(vector<string>& filelist)
{
	for (int i = 0; i < filelist.size(); ++i)
	{
		cout << filelist[i] << endl;
	}

}

string ExecuteCommand(SOCKET &socketClient)
{
	string filename;
	char *command = new char(0);
	while (1)
	{
		system("CLS");
		MainMenu();
		scanf("%d", command);
		getchar();
		*command += '0';
		send(socketClient, command, 1, 0);
		vector<string> filelist;
		//Ȼ��͵ý���
		string Path;
		if (*command == '3')
		{
			cin >> Path;
			send(socketClient, (char *)Path.c_str(), Path.size(), 0);
		}

		if (*command == '4')
		{

			cin >> filename;
			send(socketClient, (char *)filename.c_str(), filename.size(), 0);

			break;
		}
		char Buf[260] = { 0 };
		while (1)
		{
			recv(socketClient, Buf, 260, 0);
			filelist.push_back(Buf);
			memset(Buf, 0, 100);
			if (filelist.back() == "transform end")
			{
				filelist.pop_back();
				break;
			}

		}
		Showfilelist(filelist);
		system("pause");
	}
	return filename;
}

#pragma comment(lib, "ws2_32.lib")
int main()
{
	WORD wVersionRequested;  //typedef unsigned short WORD
	WSADATA wsaData;   //�ð���洢ϵͳ���صĹ���WinSocket������
	int err;  //�����ж����������Ƿ�����

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}

	SOCKET socketClient = socket(AF_INET, SOCK_STREAM, 0);   //AF_INET tcpip��Э��
	//��ʼ������
	SOCKADDR_IN addrSrv;  //�������ĵ�ַ
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;  //ʹ�õ���TCP/IP 
	addrSrv.sin_port = htons(8001);  //תΪ������  ���ö˿ں�

	//���ӵ������� ʹ��SOCKET�������ӷ�����,����������ĵ�ַ��Ϣ  ǿת
	if (connect(socketClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) < 0)  //Э�����  �׽��ֲ��� 
	{
		printf("connction faild!");
		closesocket(socketClient);
		return 0;
	}


	//������ӵ���  �Ǿ��ȷ���һ������
	string filename = ExecuteCommand(socketClient);

	//char filename[256] = { 0 };
	char recvBuf[512] = { 0 };

	//int len = recv(socketClient, filename, 256, 0);

	//printf("%d\n", len);


	//printf("%s\n", filename);
	//�����ļ�
	FILE* fp = fopen(filename.c_str(), "wb");
	cout << "�ɹ����ļ�" << filename << endl;
	if (!fp)
		return 0;


	//д���ļ�
	while (1)
	{
		cout << "������" << endl;
		int len = recv(socketClient, recvBuf, 512, 0);
		cout << "����" << endl;
		fwrite(recvBuf, 1, len, fp);
		cout << "��д" << endl;
		memset(recvBuf, 0, 512);
		cout << "����" << endl;
		if (len < 1)
		{
			cout << "�˳�" << endl;
			break;
		}
	}
	printf("�ļ��������\n");




	//char recvBuf[50] = {0};

	//int size = 10;
	//int i = 0;
	//while (i<100)
	//{
	//	//recv(socketClient, recvBuf, 50, 0);  //socket�����Ѿ����յ����ݣ����ڿ�ʼ��on����������ȡ����
	//    //printf("%s\n", recvBuf);
	//	send(socketClient, "123456789" , 50, 0);
	//	//Sleep(100);
	//}



	closesocket(socketClient);  //�ر�socket����

	WSACleanup();

	printf("Client exit!");
	system("pause");
	return 0;

}
