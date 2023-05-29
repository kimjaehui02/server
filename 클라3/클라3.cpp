/*
#pragma comment(lib, "ws2_32")
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.11"
#define SERVERPORT 9000
#define BUFSIZE    512

struct MyStruct
{
	/// <summary>
	/// 0 첫 서버의 접속
	/// </summary>
	int type;

	char* ip;
	int Port;

	char* data;
};

// 소켓 함수 오류 출력 후 종료
void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

int main(int argc, char* argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE + 1];
	int len;

#pragma region 개시용


	MyStruct buf2 = { 0};
	buf2.ip = SERVERIP;
	buf2.Port = SERVERPORT;

	retval = send(sock, (char*)&buf2, sizeof(MyStruct), 0);
	printf("\n[ip주소 보냄] ");
	retval = recvn(sock, (char*)&buf2, sizeof(MyStruct), 0);
#pragma endregion

	printf("\n[ip주소 보냄] 2");
	// 서버와 데이터 통신
	while (1) {
		printf("\n[보낼 데이터] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		// '\n' 문자 제거
		len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		// 데이터 보내기
		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		// 데이터 받기
		retval = recvn(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
		printf("[받은 데이터] %s\n", buf);
	}

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
*/

#pragma comment(lib, "ws2_32")
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SERVERIP   "127.0.0.143"
#define SERVERIP2   "보내진 서버 주소:127.0.0.143"
#define SERVERPORT 9000
#define BUFSIZE    512

#define SHAREFIRST "쉐어값요청"
#define SHARESTART "쉐어값:"

int share = 0;

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

int main(int argc, char* argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE + 1];
	int len;
	//printf("? %s", inet_ntoa(serveraddr.sin_addr));
#pragma region MyRegion

#pragma region 서버값 송수신
	// 데이터 보내기
	retval = send(sock, SERVERIP2, strlen(SERVERIP2), 0);
	printf("\n[ip주소 보냄] ");
	retval = recvn(sock, buf, strlen(SERVERIP2), 0);
	buf[retval] = '\0';
	printf("\n[ip주소 대답 받음] %s", buf);
#pragma endregion

#pragma region 쉐어데이터를 보내달라는 요청
	//printf("\n[쉐어값 받을 준비] ");
	retval = send(sock, SHAREFIRST, strlen(SHAREFIRST), 0);
#pragma endregion

#pragma region 쉐어값 송수신
	retval = recvn(sock, buf, strlen(SHAREFIRST), 0);
	//printf("\n\n 잘 온지 확인합니다. %d", atoi(buf));
	buf[retval] = '\0';
	//printf("\n[쉐어값 대답 받음] %s", buf);

	// 쉐어값을 증가한다
	share = atoi(buf) + 10;

	// 증가된 쉐어값을 양식이랑 합쳐서 다시 문자열로 바꾸고 보낸다
	char s1[1000];

	// 합치기 위해 문자열로 변환
	sprintf(s1, "%d", share);
	char s2[] = SHARESTART;
	// 합침
	strcat(s2, s1);

	// 합친걸 보내줌
	retval = send(sock, s2, strlen(s2), 0);

	retval = recvn(sock, buf, strlen(s2), 0);

	//printf("\n[쉐어값 증가시켜 재송신] %s", buf);
#pragma endregion




#pragma endregion



	// 서버와 데이터 통신
	while (1) {
		// 데이터 입력
		printf("\n[보낼 데이터] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		// '\n' 문자 제거
		len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		// 데이터 보내기
		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		// 데이터 받기
		retval = recvn(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
		printf("[받은 데이터] %s\n", buf);
	}

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}