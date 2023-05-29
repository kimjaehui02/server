/*
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    512

struct MyStruct
{
    /// <summary>
    /// 0 첫 서버의 접속
    /// </summary>
    int type;

    char ip;
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

// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
    SOCKET client_sock = (SOCKET)arg;
    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    // 클라이언트 정보 얻기
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    while (1) {
        // 데이터 받기
        retval = recv(client_sock, buf, BUFSIZE, 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        MyStruct* buf2;
        buf2 = ((MyStruct*)buf);
        //printf("\n %d \n", buf2->type);

        if(buf2->type == 0)
        {

            printf("[TCP/%s:%d] IP주소: %s, 포트번호: %d\n", inet_ntoa(clientaddr.sin_addr),
                ntohs(clientaddr.sin_port), buf2->ip, buf2->Port);



            // 데이터 보내기
            retval = send(client_sock, (char*)&buf2, sizeof(MyStruct), 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
        }
        else
        {
            // 받은 데이터 출력
            buf[retval] = '\0';
            printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
                ntohs(clientaddr.sin_port), buf);

            // 데이터 보내기
            retval = send(client_sock, buf, retval, 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
        }


    }

    // closesocket()
    closesocket(client_sock);
    printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    return 0;
}

int main(int argc, char* argv[])
{
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket()
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    // 데이터 통신에 사용할 변수
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    HANDLE hThread;

    while (1) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        // 접속한 클라이언트 정보 출력
        printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        // 스레드 생성
        hThread = CreateThread(NULL, 0, ProcessClient,
            (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(hThread); }
    }

    // closesocket()
    closesocket(listen_sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <time.h> 
#define SERVERPORT 9000
#define BUFSIZE    512

#define SERVERSTART "보내진 서버 주소:"
#define SHAREFIRST "쉐어값요청"
#define SHARESTART "쉐어값:"

// 서버입니다

int share = 0;
int share2 = 0;


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

// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
    SOCKET client_sock = (SOCKET)arg;
    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    // 클라이언트 정보 얻기
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
    share2++;
    //printf("접속중인 숫자! %d 그리고 쉐어값은 %d \n", share2, share);

    char Ipdata[1000];

    while (1) {
        // 데이터 받기
        retval = recv(client_sock, buf, BUFSIZE, 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;



        time_t times;
        tm* mytime;
        times = time(NULL);
        mytime = localtime(&times);

        // 받은 데이터 출력
        buf[retval] = '\0';
        printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
            ntohs(clientaddr.sin_port), buf);

        char* ServerData = strstr(buf, SERVERSTART);
        char* ShareData = strstr(buf, SHARESTART);
        char* ShareDatafirst = strstr(buf, SHAREFIRST);
        //printf("\n %s \n", ServerData);
        //printf("\n %s \n", ShareData);
        //printf("\n %s \n", inet_ntoa(clientaddr.sin_addr));

#pragma region 서버값 송수신
        // 서버데이터 양식일 경우 널이 아니게 된다
        if (ServerData != NULL)
        {
            // 년월일
            printf("[TCP/%s:%d] %d년 %d월 %d일 %d시 %d분 %d초\n", inet_ntoa(clientaddr.sin_addr),
                ntohs(clientaddr.sin_port), mytime->tm_year + 1900, mytime->tm_mon + 1, mytime->tm_mday, mytime->tm_hour, mytime->tm_min, mytime->tm_sec);
            printf("[TCP/%s:%d] Ip값을 보냈습니다\n", inet_ntoa(clientaddr.sin_addr),
                ntohs(clientaddr.sin_port));
            int i2 = 0;
            //printf("\n ? ");
            while (true)
            {
                if (i2 == retval)
                {
                    break;
                }
                //printf("%c", buf[i2]);
                Ipdata[i2] = buf[i2];
                i2++;
            }
            Ipdata[retval] = '\0';

            //printf("\n ? ");
            retval = send(client_sock, buf, retval, 0);
        }
#pragma endregion

#pragma region 쉐어데이터를 보내달라는 요청
        // 쉐어데이터 요청을 받을 경우 널이 아니게 된다
        if (ShareDatafirst != NULL)
        {
            //printf("[TCP/%s:%d] 쉐어값을 요청해왔습니다 현재 쉐어값: %d\n", inet_ntoa//(clientaddr.sin_addr),
            //    ntohs(clientaddr.sin_port), share);

            // 쉐어값을 문자열로 변환함
            char s1[1000];
            sprintf(s1, "%d", share);
            strcpy(buf, s1);
            //printf("쉐어 보내짐 체크 %s", buf);
            // 변환한 쉐어값을 클라이언트에 보냄
            retval = send(client_sock, buf, retval, 0);
        }
#pragma endregion

#pragma region 쉐어값 송수신
        // 쉐어데이터 양식일 경우 널이 아니게 된다
        if (ShareData != NULL)
        {
            //printf("[TCP/%s:%d] 쉐어값을 받았습니다\n", inet_ntoa(clientaddr.sin_addr),
            //    ntohs(clientaddr.sin_port));

            // 알맞은 값이 있다면 널이 아니게 된다
            char* PlusShare = strstr(buf, ":") + 1;
            //printf("\n %s, %d \n", PlusShare, atoi(PlusShare));
            if (PlusShare != NULL)
            {
                //printf("\n %s, %d \n", PlusShare, atoi(PlusShare));
                // 증가된 값으로 쉐어를 수정한다
                share = atoi(PlusShare);
                //printf("\n쉐어는 %d", share);
            }

            retval = send(client_sock, buf, retval, 0);
        }
#pragma endregion


        // 아무것도 아닐 때
        if (ShareData == NULL && ServerData == NULL && ShareDatafirst == NULL)
        {

            retval = send(client_sock, buf, retval, 0);
        }

        // 데이터 보내기
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }
    }

    time_t times;
    tm* mytime;
    times = time(NULL);
    mytime = localtime(&times);
    // closesocket()
    share2--;
    //printf("접속중인 숫자! %d 그리고 쉐어값은 %d 퇴장용 \n", share2, share);
    closesocket(client_sock);
    printf("[TCP 서버] 클라이언트 종료: =%s, 포트 번호=%d\n",
        Ipdata, ntohs(clientaddr.sin_port));
        
        //inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    // 년월일
    printf("[TCP/%s:%d] %d년 %d월 %d일 %d시 %d분 %d초\n", inet_ntoa(clientaddr.sin_addr),
        ntohs(clientaddr.sin_port), mytime->tm_year + 1900, mytime->tm_mon + 1, mytime->tm_mday, mytime->tm_hour, mytime->tm_min, mytime->tm_sec);

    return 0;
}

int main(int argc, char* argv[])
{
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket()
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    // 데이터 통신에 사용할 변수
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    HANDLE hThread;

    while (1) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        // 접속한 클라이언트 정보 출력
        printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        // 스레드 생성
        hThread = CreateThread(NULL, 0, ProcessClient,
            (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(hThread); }
    }

    // closesocket()
    closesocket(listen_sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}
