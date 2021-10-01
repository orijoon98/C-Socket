# C언어 소켓 프로그래밍

## 목표

- 간단한 소켓 프로그래밍으로 네트워크 서버 클라이언트 모델 이해해보기

## 네트워크 서버 클라이언트 모델

- 서버는 정보처리와 정보 창고의 역할을 하며, 클라이언트가 정보를 요청할 경우 이를 서비스한다.
- 하나의 서버는 다수의 클라이언트의 요청을 처리 가능하다.
- 서버는 정보를 독점적으로 처리해야 하기에 강력한 컴퓨팅 파워와 거대한 저장공간을 필요로 한다.

## 소켓

- 몇 개의 간단한 함수만으로도 인터넷 애플리케이션을 만들 수 있도록 도와주는 함수 모음이다.
- 인터넷 연결, 종료, 데이터 전송, 도메인 이름 변환, 주소 변환등과 관련된 주요 함수들을 제공한다.
- 운영체제에서 관리하는 객체로 소프트웨어를 인터넷에 연결해주는 접점 역할을 한다.

## 소프트웨어 연결에 필요한 것들

- IP 주소와 PORT 번호로 전체 인터넷 상에서 유일한 소프트웨어를 찾아서 연결할 수 있다.

## 서버 프로그램의 흐름

1. 소켓 만들기 : 소켓을 만들어서 소프트웨어간 통신을 가능하게 한다.
2. binding : IP와 포트번호를 프로그램에 묶어주는 작업을 한다.
3. 연결 대기열 만들기 : 클라이언트가 소켓을 통하여 접속 요청을 하면, 이 요청은 연결 대기열로 들어간다. 대기열은 원활한 서비스를 위해서 필요하다. 만약 대기열이 없다면, 클라이언트는 접속 거부가 될 것이다.
4. 연결 대기열에서 클라이언트 요청 가져오기 : 서버는 연결 대기열에서 맨 앞에 있는 클라이언트 요청을 가져온다.
5. 클라이언트와 연결이 이루어졌다면, 데이터 통신을 하고 데이터를 처리한다.

### 소켓 만들기

다음은 socket 함수의 원형이다.

```c
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```

- domain : 소켓이 사용될 주소 영역을 지정하기 위해서 사용한다. TCP/IP 기반의 인터넷 주소영역을 사용하는 프로그램이면 AF_INET을 지정한다.
- type : 통신에 사용될 패킷 타입을 지정하기 위해서 사용한다. 연결 지향 소켓인 SOCK_STREAM 과 데이터 그램 지향 소켓인 SOCK_DGRAM 이 사용된다.
- protocol : 통신에 사용될 프로토콜을 지정하기 위해서 사용한다. 보통 type과 쌍으로 사용되는데 SOCK_STREAM 이라면 IPPROTO_TCP, SOCK_DGRAM 이라면 IPPROTO_UDP 가 사용된다.

### binding

다음은 bind 함수의 원형이다.

```c
#include <sys/types.h>
#include <sys/socket.h>

int bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
```

다음은 사용 방법이다.

```c
struct sockaddr_in sock_addr;

sock_addr.sin_family = AF_INET;
sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
sock_addr.sin_port = htons(12345);
```

### 연결 대기열 만들기

listen 함수를 이용해서 연결 대기열을 만들 수 있다.

```c
#include <sys/socket.h>

int listen(int s, int backlog);
```

- s : 연결 대기열을 가질 소켓이다. 이 소켓은 연결 대기열에 클라이언트 연결 요청이 있는지 검사하는 일을 한다.
- backlog : 연결 대기열의 크기이다.

### 연결 대기열 가져오기

연결 대기열에 있는 클라이언트 연결 요청은 accept 함수로 가져온다.

```c
#include <sys/types.h>
#include <sys/socket.h>

int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
```

- s : 듣기 소켓으로 연결 대기열에서 클라이언트 요청을 가져온다.
- addr : 가져온 클라이언트의 주소 정보를 넘긴다.
- addrlen : addr의 크기

함수가 성공적으로 수행되면, 0보다 큰 소켓 지정 번호를 반환한다. 이걸 연결 소켓이라고 부른다. 서버는 이 연결 소켓을 이용해서 클라이언트와 통신한다.

### 데이터 통신

accept 함수 호출로 만들어진 연결 소켓을 이용해서 클라이언트와 통신을 한다.

리눅스 소켓은 read, write 함수를 이용해서 통신할 수 있다. 혹은 BSD 소켓 함수인 recv, send 함수로 통신할 수도 있다.

## 클라이언트 프로그램의 흐름

1. 소켓 만들기 : 서버와 마찬가지로 socket 함수로 만든다.
2. 서버에 연결 요청하기 : 서버로의 연결 요청은 connect 함수를 이용한다.
3. 서버와의 데이터 통신 : 서버와의 데이터 통신은 recv 혹은 send 함수를 이용한다.

### 소켓 만들기

서버 소켓 만들기와 동일하다.

### 서버에 연결 요청하기

connect 함수를 이용한다.

```c
#include <sys/types.h>
#include <sys/socket.h>

int  connect(int sockfd, const struct sockaddr *serv_addr,
         socklen_t addrlen);
```

소켓 지정 번호 sockfd 를 이용해서 sockaddr 에 지정된 IP와 포트번호로 연결을 시도한다. 성공하면 0을 반환하고 실패하면 -1을 반환한다.

### 서버와의 데이터 통신

서버와의 데이터 통신은 recv 혹은 send 함수를 이용한다.

## 서버 코드

```c
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){
    int serv_sock;
    int clint_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clint_addr;
    socklen_t clnt_addr_size;

    if(argc != 2){
        printf("%s <port>\n", argv[0]);
        exit(1);
    }

    //소켓 만들기
    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serv_sock == -1)
        printf("socket error\n");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    //프로세스를 소켓에 바인딩하기
    if(bind(serv_sock,(struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        printf("bind error\n");

    //연결 대기열 만들기
    if(listen(serv_sock,5)==-1)
        printf("listen error\n");

    //연결 대기열 가져오기
    clnt_addr_size = sizeof(clint_addr);
    clint_sock = accept(serv_sock,(struct sockaddr*)&clint_addr,&clnt_addr_size);
    if(clint_sock == -1)
        printf("accept error\n");

		//데이터 통신하기
    char message[] = "hello client";
    write(clint_sock, message, sizeof(message));
    close(serv_sock);
    close(clint_sock);
    return 0;
}
```

## 클라이언트 코드

```c
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char* argv[])
{
    int my_sock;
    struct sockaddr_in serv_addr;
    int str_len;
    if(argc != 3)
    {
        printf("%s <IP> <PORT>\n", argv[0]);
        exit(1);
    }

    //소켓 만들기
    my_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(my_sock == -1)
        printf("socket error \n");
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));

    //서버에 연결 요청하기
    if(connect(my_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        printf("connect error\n");

		//데이터 통신하기
    char message[15];
    str_len = read(my_sock,message,sizeof(message)-1);
    if(str_len==-1)
        printf("read error\n");
    printf("서버에서 : %s \n", message);
    close(my_sock);
    return 0;
}
```

## 결과

![스크린샷 2021-10-01 오전 10 21 51](https://user-images.githubusercontent.com/74812188/135551734-ac2118a9-1449-43ba-912b-0f9296124731.png)

![스크린샷 2021-10-01 오전 10 22 58](https://user-images.githubusercontent.com/74812188/135551754-87ff4ea7-133c-4855-b214-ce9e0235e839.png)

![스크린샷 2021-10-01 오전 10 23 31](https://user-images.githubusercontent.com/74812188/135551763-f82bdc08-f941-4dc1-b75a-1a825254448a.png)
