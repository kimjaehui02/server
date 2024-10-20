## 1. 프로젝트 소개
이 프로젝트는 사용자가 서버와 데이터를 주고받을 수 있는 기능을 학습하기 위해 개발되었습니다.

## 2. 기술 스택
- **프로그래밍 언어**: C, C++
- **소켓 프로그래밍**: Winsock API를 이용한 TCP/IP 소켓 통신

## 3. 작동 원리
- **서버와의 연결**: 클라이언트가 서버에 TCP/IP 연결을 요청하여 데이터를 송신 및 수신합니다.
- **데이터 송수신**: 사용자가 입력한 데이터를 서버에 전송하고, 서버에서 반환된 데이터를 수신하여 출력합니다.

## 4. 주요 기능
- **TCP/IP 소켓 통신**:
  - `socket()`, `connect()`, `send()`, `recv()` 함수로 서버와의 연결 및 데이터 통신 수행
  - 데이터 전송 후 수신하는 과정을 반복하여 사용자와 서버 간의 상호작용 구현
- **사용자 정의 데이터 구조**:
  - 사용자 정보를 담기 위한 구조체 정의 및 활용 (예: IP 주소, 포트, 데이터 등)



## 5. 실행 방법
- Winsock 초기화는 `WSAStartup` 함수를 사용하여 수행합니다.
- 소켓을 생성하고 서버에 연결하기 위해 `socket()`과 `connect()` 함수를 사용합니다.
- 사용자가 입력한 데이터를 서버에 전송할 때는 `send()` 함수를 사용하고, 서버로부터 데이터를 수신할 때는 `recv()` 함수를 사용합니다.
- 프로그램 종료 시, `WSACleanup` 함수를 호출하여 Winsock을 정리합니다.


## 6. 기능별 코드 구현

<details>
2. 멀티스레드 코드
</details>

<details>

<summary>소켓 통신 코드</summary>

```c
#include <winsock2.h>
#include <stdio.h>

void socketCommunication() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server;
    char *message, server_reply[2000];

    // Winsock 초기화
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    // 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 서버에 연결
    connect(sock, (struct sockaddr *)&server, sizeof(server));

    // 데이터 전송
    message = "Hello Server";
    send(sock, message, strlen(message), 0);

    // 서버로부터 응답 수신
    recv(sock, server_reply, sizeof(server_reply), 0);
    printf("Server reply: %s\n", server_reply);

    // 소켓 종료
    closesocket(sock);
    WSACleanup();
}
</details>

<details>
2. 멀티스레드 코드
</details>

2. 멀티스레드 코드

<details>
```c
<summary>멀티스레드 코드</summary>

#include <pthread.h>
#include <stdio.h>

void* threadFunction(void* arg) {
    int thread_id = *((int*)arg);
    printf("Thread %d is running.\n", thread_id);
    // 스레드 작업 수행
    return NULL;
}

void multiThreading() {
    pthread_t threads[5];
    int thread_ids[5];

    for (int i = 0; i < 5; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, threadFunction, (void*)&thread_ids[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
}

</details>
