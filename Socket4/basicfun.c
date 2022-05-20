#include "basicfun.h"
#include <stdio.h>
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
