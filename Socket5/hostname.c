#include <unistd.h>
#include <stdio.h>
#include <string.h>


int main() 
{
    char hostname[128];
    int getres = gethostname(hostname, sizeof(hostname));
    printf("getres:%d ----- hostname: %s", getres, hostname);
    return 0;
}