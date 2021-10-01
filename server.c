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