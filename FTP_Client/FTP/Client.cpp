////�ͻ���
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define	_CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include<iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")
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

	SOCKET socketClient = socket(AF_INET, SOCK_STREAM, 0);   //AF_INET tcpip��Э��
															 //��ʼ������

	SOCKADDR_IN addrSrv;  //�������ĵ�ַ
	addrSrv.sin_addr.S_un.S_addr = inet_addr("172.16.89.39");
	addrSrv.sin_family = AF_INET;  //ʹ�õ���TCP/IP 
	addrSrv.sin_port = htons(1111);  //תΪ������  ���ö˿ں�


									 //���ӵ������� ʹ��SOCKET�������ӷ�����,����������ĵ�ַ��Ϣ  ǿת
	if (connect(socketClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) < 0)  //Э�����  �׽��ֲ���
	{
		printf("connction faild!");
		closesocket(socketClient);
		return 0;
	}

	//������ӵ���  �Ǿͽ���
	char buff[BUFSIZ] = { 0 };
	char recvBuf[BUFSIZ] = { 0 };

	///�������

	int len = recv(socketClient, buff, BUFSIZ, 0);
	printf("%s\n", buff);
	//�������Ϊ�����֤

	char Username[BUFSIZ] = "liang";
	send(socketClient, Username, strlen(Username) + 1, 0);
	char Pass[BUFSIZ] = "liang";
	send(socketClient, Pass, strlen(Pass) + 1, 0);

	memset(buff, 0, sizeof(buff));
	recv(socketClient, buff, BUFSIZ, 0);
	cout << buff << endl;

	char req[BUFSIZ] = "�ļ���������";
	send(socketClient, req, strlen(req) + 1, 0);
	recv(socketClient, recvBuf, BUFSIZ, 0);

	////////////////////////////////////////////////
	//�����ļ��б�


	//ѡ�������ļ���
	char filename[BUFSIZ] = "tt1.txt";

	send(socketClient, filename, strlen(filename) + 1, 0);
	//���ղ��ұ�����ļ�������Ŀ¼
	FILE* fp = fopen("E:\\tt1.txt", "wb");

	if (!fp)
		return 0;

	//д���ļ�
	while (1)
	{
		int len = recv(socketClient, recvBuf, BUFSIZ, 0);
		fwrite(recvBuf, 1, len, fp);
		printf("%s\n", recvBuf);
		memset(recvBuf, 0, BUFSIZ);

		if (len < 1)
			break;
	}
	fclose(fp);
	closesocket(socketClient);  //�ر�socket����

	WSACleanup();

	printf("Client exit!");
	system("pause");
	return 0;

}

