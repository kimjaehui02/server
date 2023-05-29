/*
#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    512

struct MyStruct
{
    /// <summary>
    /// 0 ù ������ ����
    /// </summary>
    int type;

    char ip;
    int Port;

    char* data;

};

// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ProcessClient(LPVOID arg)
{
    SOCKET client_sock = (SOCKET)arg;
    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    // Ŭ���̾�Ʈ ���� ���
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    while (1) {
        // ������ �ޱ�
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

            printf("[TCP/%s:%d] IP�ּ�: %s, ��Ʈ��ȣ: %d\n", inet_ntoa(clientaddr.sin_addr),
                ntohs(clientaddr.sin_port), buf2->ip, buf2->Port);



            // ������ ������
            retval = send(client_sock, (char*)&buf2, sizeof(MyStruct), 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
        }
        else
        {
            // ���� ������ ���
            buf[retval] = '\0';
            printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
                ntohs(clientaddr.sin_port), buf);

            // ������ ������
            retval = send(client_sock, buf, retval, 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
        }


    }

    // closesocket()
    closesocket(client_sock);
    printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    return 0;
}

int main(int argc, char* argv[])
{
    int retval;

    // ���� �ʱ�ȭ
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

    // ������ ��ſ� ����� ����
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

        // ������ Ŭ���̾�Ʈ ���� ���
        printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        // ������ ����
        hThread = CreateThread(NULL, 0, ProcessClient,
            (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(hThread); }
    }

    // closesocket()
    closesocket(listen_sock);

    // ���� ����
    WSACleanup();
    return 0;
}
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
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

#define SERVERSTART "������ ���� �ּ�:"
#define SHAREFIRST "�����û"
#define SHARESTART "���:"

// �����Դϴ�

int share = 0;
int share2 = 0;


// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ProcessClient(LPVOID arg)
{
    SOCKET client_sock = (SOCKET)arg;
    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    // Ŭ���̾�Ʈ ���� ���
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
    share2++;
    //printf("�������� ����! %d �׸��� ����� %d \n", share2, share);

    char Ipdata[1000];

    while (1) {
        // ������ �ޱ�
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

        // ���� ������ ���
        buf[retval] = '\0';
        printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
            ntohs(clientaddr.sin_port), buf);

        char* ServerData = strstr(buf, SERVERSTART);
        char* ShareData = strstr(buf, SHARESTART);
        char* ShareDatafirst = strstr(buf, SHAREFIRST);
        //printf("\n %s \n", ServerData);
        //printf("\n %s \n", ShareData);
        //printf("\n %s \n", inet_ntoa(clientaddr.sin_addr));

#pragma region ������ �ۼ���
        // ���������� ����� ��� ���� �ƴϰ� �ȴ�
        if (ServerData != NULL)
        {
            // �����
            printf("[TCP/%s:%d] %d�� %d�� %d�� %d�� %d�� %d��\n", inet_ntoa(clientaddr.sin_addr),
                ntohs(clientaddr.sin_port), mytime->tm_year + 1900, mytime->tm_mon + 1, mytime->tm_mday, mytime->tm_hour, mytime->tm_min, mytime->tm_sec);
            printf("[TCP/%s:%d] Ip���� ���½��ϴ�\n", inet_ntoa(clientaddr.sin_addr),
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

#pragma region ������͸� �����޶�� ��û
        // ������� ��û�� ���� ��� ���� �ƴϰ� �ȴ�
        if (ShareDatafirst != NULL)
        {
            //printf("[TCP/%s:%d] ����� ��û�ؿԽ��ϴ� ���� ���: %d\n", inet_ntoa//(clientaddr.sin_addr),
            //    ntohs(clientaddr.sin_port), share);

            // ����� ���ڿ��� ��ȯ��
            char s1[1000];
            sprintf(s1, "%d", share);
            strcpy(buf, s1);
            //printf("���� ������ üũ %s", buf);
            // ��ȯ�� ����� Ŭ���̾�Ʈ�� ����
            retval = send(client_sock, buf, retval, 0);
        }
#pragma endregion

#pragma region ��� �ۼ���
        // ������� ����� ��� ���� �ƴϰ� �ȴ�
        if (ShareData != NULL)
        {
            //printf("[TCP/%s:%d] ����� �޾ҽ��ϴ�\n", inet_ntoa(clientaddr.sin_addr),
            //    ntohs(clientaddr.sin_port));

            // �˸��� ���� �ִٸ� ���� �ƴϰ� �ȴ�
            char* PlusShare = strstr(buf, ":") + 1;
            //printf("\n %s, %d \n", PlusShare, atoi(PlusShare));
            if (PlusShare != NULL)
            {
                //printf("\n %s, %d \n", PlusShare, atoi(PlusShare));
                // ������ ������ ��� �����Ѵ�
                share = atoi(PlusShare);
                //printf("\n����� %d", share);
            }

            retval = send(client_sock, buf, retval, 0);
        }
#pragma endregion


        // �ƹ��͵� �ƴ� ��
        if (ShareData == NULL && ServerData == NULL && ShareDatafirst == NULL)
        {

            retval = send(client_sock, buf, retval, 0);
        }

        // ������ ������
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
    //printf("�������� ����! %d �׸��� ����� %d ����� \n", share2, share);
    closesocket(client_sock);
    printf("[TCP ����] Ŭ���̾�Ʈ ����: =%s, ��Ʈ ��ȣ=%d\n",
        Ipdata, ntohs(clientaddr.sin_port));
        
        //inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    // �����
    printf("[TCP/%s:%d] %d�� %d�� %d�� %d�� %d�� %d��\n", inet_ntoa(clientaddr.sin_addr),
        ntohs(clientaddr.sin_port), mytime->tm_year + 1900, mytime->tm_mon + 1, mytime->tm_mday, mytime->tm_hour, mytime->tm_min, mytime->tm_sec);

    return 0;
}

int main(int argc, char* argv[])
{
    int retval;

    // ���� �ʱ�ȭ
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

    // ������ ��ſ� ����� ����
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

        // ������ Ŭ���̾�Ʈ ���� ���
        printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        // ������ ����
        hThread = CreateThread(NULL, 0, ProcessClient,
            (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(hThread); }
    }

    // closesocket()
    closesocket(listen_sock);

    // ���� ����
    WSACleanup();
    return 0;
}
