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

/** 服务端流程
 * socket函数创建监听套接字listenfd
 * 为监听套接字listenfd去bind本地的ip及port
 * 调用listen函数启动监听
 * 调用accept函数返回一个已连接的套接字 conn
 * 正常读写数据
 */
void err_exit(char * msg)
{
    do {
		perror(msg);
		exit(EXIT_FAILURE); 

    }while (0);	
}

void handler(int sig)
{
    printf("recv a singal = %d\n", sig);
    exit(EXIT_SUCCESS);
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
    int conn;
    if((conn = accept(listenfd,(struct sockaddr *)&peeraddr, &peerlen))< 0)
    {
        err_exit("accept");
    }
    pid_t pid;
    pid = fork();
    if (pid == -1)
    {
        err_exit("fock");
    }
    if (pid == 0) // 子进程发送数据
    {
        signal(SIGUSR1, handler);
        char sendbuf[1024];
        while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
        {
            write(conn, sendbuf, strlen(sendbuf));
            memset(sendbuf, 0, strlen(sendbuf));
        }
        exit(EXIT_SUCCESS);
        printf("exit after");
        close(conn);

    }
    else  // 父进程接收数据
    {
        char recvbuf[1024];
        while (1)
        {
            int ret = read(conn, recvbuf, sizeof(recvbuf));
            if (ret == 0)
            {
                printf("peer close");
                break;
            }
            if (ret == -1)
            {
                err_exit("read");
            }
            fputs(recvbuf,stdout);
        }
        kill(pid, SIGUSR1);
        printf("kill after\n");
        close(conn);
    } 
    close(listenfd);

	return 0;
}




