#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>


ssize_t readn(int fd, void *buf, size_t count);

ssize_t writen(int fd, const void *buf, size_t count);

ssize_t recv_peek(int sockfd, void *buf, size_t len);

ssize_t readline(int sockfd, void *buf, size_t maxline);