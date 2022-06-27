
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include "basicfun.h"
#include <unistd.h>
#include <signal.h>


void err_exit(char * msg)
{
    do 
	{
		perror(msg);
		exit(EXIT_FAILURE); 

	}while (0);	
}

void echo_cli(int sock)
{
	char sendbuf[1024];
	char recvbuf[1024];
	memset(sendbuf, 0, sizeof(sendbuf));
	memset(recvbuf, 0, sizeof(recvbuf));
	// 从标准输入中读取数据
	while(fgets(sendbuf,sizeof(sendbuf), stdin) != NULL)
	{
		/*
		// 将读取的数据写入套套接字
	    writen(sock, sendbuf, strlen(sendbuf));
		*/ 
	    //已收到对段的FIN分节时，此时在发送数据给对端会收到对端发送的RST分节 
	    writen(sock, sendbuf, 1);
		// 收到对端的RST分节时再发送数据就会收到SIG_PIPE信号 可以SIG_IGN处理 
		writen(sock, sendbuf + 1, strlen(sendbuf) -1);
		// 接受服务端的回射数据
		// 先获取4字节的信息长度
       int nread = readline(sock, recvbuf, sizeof(recvbuf)); 
	   if (nread == -1)
	   {	
		   err_exit("readline  error");

	   }
	   if (nread == 0)
	   {
		   printf("srv close\n");
		   break;
	   }
	   

	    fputs(recvbuf, stdout);
		memset(sendbuf, 0, sizeof(sendbuf));
		memset(recvbuf, 0, sizeof(recvbuf));
	}	
	close(sock);
}


void sigpipe_handle (int sig)
{
    printf("recv a sig:%d\n", sig);
}

int main() 
{

	//signal(SIGPIPE,sigpipe_handle);
	signal(SIGPIPE, SIG_IGN);
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

	struct  sockaddr_in sockaddr;
	socklen_t socklen = sizeof(sockaddr);
	int getres = getsockname(sock, (struct sockaddr *)&sockaddr, &socklen);
	printf("res = %d, ip = %s, port = %d \n", getres, inet_ntoa(sockaddr.sin_addr),htons(sockaddr.sin_port));
	
	echo_cli(sock);
	return 0;
}
