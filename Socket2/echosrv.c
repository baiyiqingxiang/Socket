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

/** 服务端流程
 * socket函数创建监听套接字listenfd
 * 为监听套接字listenfd去bind本地的ip及port
 * 调用listen函数启动监听
 * 调用accept函数返回一个已连接的套接字 conn
 * 正常读写数据
 */
int main() 
{
	int listenfd;
	listenfd = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
	// 0 自动选择协议
	//listenfd = socket(AF_INET, SOCK_STREAM,0);
	if(listenfd < 0) err_exit("socket");
    struct sockaddr_in servaddr;
	memset(&servaddr, 0 , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5188);
	// 指定本机的任意地址
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//servaddr.sin_addr.s_addr = inet_addr("127.0.0.1)";
	//inet_aton("127.0.0.1",&servaddr.sin_addr);			
    if( bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0) 
	{
		 err_exit("bind");
	}
	// SOMAXCONN 表示队列（正在握手的队列和已完成连接的队列）的最大值 
	// 调用listen函数会将套接字转为被动套接字，之后需要调用accept函数接受连接，默认创建的套接字为制动套接字
	if(listen(listenfd, SOMAXCONN) < 0 )
	{
		err_exit("listen");
	}
	struct sockaddr_in peeraddr;
	// peerlen 一定要有初始值，否则会accept失败
	socklen_t peerlen = sizeof(peeraddr);
	// 返回已连接队列的第一个连接
	int conn;
	if((conn = accept(listenfd,(struct sockaddr *)&peeraddr, &peerlen))< 0)
	{
		err_exit("accept");
	}
	char recvbuf[1024];

	while (1)
	{	
		memset(recvbuf,0, sizeof(recvbuf));
		// 从已连接的套接字中读取数据
		read(conn, recvbuf, sizeof(recvbuf));
		// 将数据同步到标准输出
		fputs(recvbuf, stdout);
		// 将数据回射回已连接的套接字
		write(conn,recvbuf, strlen(recvbuf));
		
	}
	return 0;
}
