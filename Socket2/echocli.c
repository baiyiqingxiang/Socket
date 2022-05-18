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


void err_exit(char * msg)
{
    do 
	{
		perror(msg);
		exit(EXIT_FAILURE); 

	}while (0);	
}
/** 客户端流程
 * socket函数创建监听套接字sock
 * 为套接字sock去指定服务端ip及port
 * 调用connect函数
 * 正常读写数据
 */
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
	char sendbuf[1024];
	char recvbuf[1024];
	// 从标准输入中读取数据
	while(fgets(sendbuf,sizeof(sendbuf), stdin) != NULL)
	{
		// 将读取的数据写入套套接字
		write(sock, sendbuf, strlen(sendbuf));
		// 接受服务端的回射数据
		read(sock, recvbuf, sizeof(recvbuf));
		// 将数据同步到标准输出
		fputs(recvbuf, stdout);
		memset(sendbuf, 0, strlen(sendbuf));
		memset(recvbuf, 0, strlen(recvbuf));
	}	
	close(sock);
	return 0;
}
