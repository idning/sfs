#include<stdio.h>
#include<unistd.h>
#include<assert.h>

#include "network.h"
#include "log.h"

int client();
int server();

int main()
{
    pid_t pid;
    if ((pid = fork()) < 0) {
        logging(LOG_ERROR, "fork error");
    } else if (pid == 0) {      //child
        usleep(1);
        client();
    } else {                    //parent
        server();
        usleep(1);
    }
    return 0;
}

int server()
{
    char buffer[1024];
    int ss = server_socket("127.0.0.1", "9991");
    if (ss < 0) {
        logging(LOG_ERROR, "socket error!");
        return -1;
    }
    int ns = tcptoaccept(ss, 100000);
    if (ns < 0) {
        logging(LOG_ERROR, "accept error!");
        return -1;
    }
    tcpnonblock(ns);
    tcpnodelay(ns);
    tcptoread(ns, buffer, 10, 3000);
    /*printf("read ok %d !!\n", n); */
    /*printf("read data: \n%s\n", buffer); */
    assert(strcmp("helloworl", buffer) == 0);
    tcpclose(ns);
    tcpclose(ss);
    return 0;
}

int client()
{
    int cs = client_socket("127.0.0.1", "9991");
    tcptowrite(cs, "helloworl", 10, 1000);
    /*printf("write ok %d \n", n); */
    tcpclose(cs);
    return 0;
}
