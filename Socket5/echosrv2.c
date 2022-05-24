
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include "basicfun.h"
#include <unistd.h>

void err_exit(char * msg)
{
    do {
		perror(msg);
		exit(EXIT_FAILURE); 

    }while (0);	
}

void do_recvservices(int conn)
{
    char recvbuf[1024];
    while (1)
    {	
        memset(recvbuf,0, sizeof(recvbuf));
        // 从已连接的套接字中读取数据
        int nread = readline(conn, recvbuf, sizeof(recvbuf)); 
	    if (nread == -1)
		   err_exit("readline error");
		if (nread == 0)
		{
		   printf("cli close");
	       break;
		}
       
        // 将数据同步到标准输出
        fputs(recvbuf, stdout);
        // 将数据回射回已连接的套接字
        writen(conn, recvbuf, strlen(recvbuf));    
     }

}

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
	int on = 1;
	// 设置地址重复利用SO_REUSEADDR，解除Socket的 TIME_WAIT 状态
	if (setsockopt(listenfd,SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
	{
		err_exit("setsockopt");
	}
			
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
	// 输入输出参数，客户端信息保存在此参数
	struct sockaddr_in peeraddr;
	// peerlen 一定要有初始值，否则会accept失败
	socklen_t peerlen = sizeof(peeraddr);
	// 返回已连接队列的第一个连接
    pid_t pid;
    while (1)
    {
        int conn;
        if((conn = accept(listenfd,(struct sockaddr *)&peeraddr, &peerlen))< 0)
        {
            err_exit("accept");
        }
		struct  sockaddr_in paddr;
		socklen_t plen = sizeof(paddr);
        int getres = getpeername(conn,(struct sockaddr *)&paddr,  &plen);
	    printf("res = %d, ip = %s, port = %d \n", getres, inet_ntoa(paddr.sin_addr),htons(paddr.sin_port));
        pid = fork();
        if (pid == -1)
        {
            err_exit("fock");
        }
        if (pid == 0) // 子进程
        {
            close(listenfd);
            do_recvservices(conn);
            exit(EXIT_SUCCESS);
        }
        else 
        {
            close(conn);
        } 
    }
    
	return 0;
}
