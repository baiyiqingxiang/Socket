#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>


void err_exit(char * msg)
{
    do 
	{
		perror(msg);
		exit(EXIT_FAILURE); 

	}while (0);	
}

void handler(int sig)
{
    printf("recv a sig = %d\n", sig);
    exit(EXIT_SUCCESS);
}


int main() 
{
	int sock;
	sock = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
	// 0 自动选择协议
	//listenfd = socket(AF_INET, SOCK_STREAM,0);
	if(sock < 0) err_exit("socket");
    struct sockaddr_in servaddr;
	memset(&servaddr, 0 , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5188);
	// 指定服务端的地址
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		err_exit("connect");
	}
    pid_t pid;
    pid = fork();
    if (pid == -1)
    {
        err_exit("fork");
    }
    if (pid == 0) // 子进程
    {
        char recvbuf[1024];
        while (1)
        {
            int ret  = read(sock, recvbuf, sizeof(recvbuf));
            if(ret == 0)
            {
                printf("peer close\n");
                break;
            }
            if(ret == -1)
            {
                err_exit("read");
            }
            fputs(recvbuf, stdout);
        }
        kill(getppid(), SIGUSR1);
        close(sock);
        
    }
    else  // 父进程发送数据
    {
        signal(SIGUSR1, handler);
        char sendbuf[1024];
        while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
        {
            write(sock, sendbuf, strlen(sendbuf));
            memset(sendbuf, 0, strlen(sendbuf));
        }
        exit(EXIT_SUCCESS);
        close(sock);
    }

	
	return 0;
}
