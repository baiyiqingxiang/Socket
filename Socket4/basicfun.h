#include <unistd.h>
#include <errno.h>



ssize_t readn(int fd, void *buf, size_t count);

ssize_t writen(int fd, const void *buf, size_t count);