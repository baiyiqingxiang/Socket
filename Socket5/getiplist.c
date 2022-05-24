#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main() 
{
    char hostname[128];
    int getres = gethostname(hostname, sizeof(hostname));
    printf("getres:%d ----- hostname: %s\n", getres, hostname);

    struct  hostent * host = gethostbyname(hostname);
    if (host == NULL)
    {
        printf("gethostbyname");
        return  0;
    }
    int i = 0;
    while (host->h_addr_list[i] != NULL)
    {
        printf("len:%d, ip:%s \n", host->h_length,inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));
         printf("len:%d, ip:%s \n", host->h_length,inet_ntoa(*(struct in_addr *)host->h_addr));
        i++;
    }
    return 0;
}