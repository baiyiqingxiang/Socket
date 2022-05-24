#include "basicfun.h"


ssize_t readn(int fd, void *buf, size_t count)
{ 
   
    // 剩余的字节数
    size_t nleft = count;
    // 读取的字节数
    ssize_t nread; 
	char * bufp = (char *)buf;
    while (nleft > 0)
    {
        nread = read(fd, bufp, nleft);
        if (nread  < 0 )
        {
            if (errno = EINTR) //读取 被中断    
                continue;
            else
                return -1;
        }
        else if(nread == 0) {
            // 对方已经关闭
            return count - nleft;
        }else{
            nleft -= nread;
            bufp += nread;
        }
        
    }
    return count;
}

ssize_t writen(int fd, const void *buf, size_t count)
{
    // 剩余的字节数
    size_t nleft = count;
    // 读取的字节数
    ssize_t nwitten; 
	char * bufp = (char *)buf;
    while (nleft > 0)
    {
        nwitten = write(fd, bufp, nleft);
        if (nwitten  < 0 )
        {
            if (errno = EINTR) 
                continue;//读取 被中断    
            else               
                return -1;
        }
        else if (nwitten == 0) {
            continue; // 对方已经关闭
        }
       else {
        nleft -= nwitten;
        bufp += nwitten;
       }
    }
    return count;
}

ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
    while (1)
    {
        ssize_t ret = recv(sockfd, buf, len, MSG_PEEK); // MSG_PEEK: 接收数据，但不从缓冲区移除数据
        if (ret == -1 && errno == EINTR)
        {
            continue;
        }
        return ret;
    }
}

ssize_t readline(int sockfd, void *buf, size_t maxline)
{
    ssize_t ret;
    char * bufp = buf;
    size_t nread;
    size_t nleft = maxline;
    while (1)
    {
        ret = recv_peek(sockfd, bufp, nleft);
        if (ret == -1) // 失败或者对方关闭了套接口
        {
            return ret;
        }
        if (ret == 0)
        {
            return ret;
        }
        nread = ret;
        for (size_t i = 0; i < nread; i++)
        {
            if (bufp[i] == '\n')
            {
                // 将数据从缓冲区移除
                ret = readn(sockfd, bufp,  i+1); 
                if (ret != i + 1)
                {
                    exit(EXIT_FAILURE);
                }
                return ret;
            }
            
        }
        if (nread > nleft) // 若已读的数据长度大于剩余的  则错误
        {
            exit(EXIT_FAILURE);
        }
        nleft -= nread; 
        // 读取数据并将数据从缓冲区移除
        ret = readn(sockfd, bufp, nread);
        if (ret != nread) // 若偷窥的数据长度的不等于剩余读取的数据 也错误
        {
            exit(EXIT_FAILURE);
        }
        bufp += nread;
    }
    return -1;
    
}































