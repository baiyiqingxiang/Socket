#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "basicfun.h"
#include <unistd.h>

struct  packet
{
	int len; // 消息的实际长度
	char buf[1024]; // 消息体
};

/******************************************************************/

void err_exit(char * msg)
{
    do 
	{
		perror(msg);
		exit(EXIT_FAILURE); 

	}while (0);	
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
	struct packet sendbuf;
	struct packet recvbuf;
	memset(&sendbuf, 0, sizeof(sendbuf));
	memset(&recvbuf, 0, sizeof(recvbuf));
	// 从标准输入中读取数据
	while(fgets(sendbuf.buf,sizeof(sendbuf.buf), stdin) != NULL)
	{
		int n = strlen(sendbuf.buf);
		sendbuf.len = htonl(n);
		// 将读取的数据写入套套接字
	    writen(sock, &sendbuf, n + 4); // 数据的长度 + 4字节的头
		// 接受服务端的回射数据
		// 先获取4字节的信息长度
       int nread = readn(sock, &(recvbuf.len), 4); 
	   if (nread == -1)
	   {	
		   err_exit("read error");

	   }
		else if (nread < 4)
		{
		   printf("srv close\n");
	       break;
		}
		uint32_t len = ntohl(recvbuf.len);
        nread = readn(sock, recvbuf.buf, len);
		if (nread ==-1)
		   err_exit("read error");
		else if (nread < len)
		{
		   printf("srv close\n");
	       break;
		}
		// 将数据同步到标准输出
		fputs(recvbuf.buf, stdout);
		memset(&sendbuf, 0, sizeof(sendbuf));
		memset(&recvbuf, 0, sizeof(recvbuf));
	}	
	close(sock);
	return 0;
}
