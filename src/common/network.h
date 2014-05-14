
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>

#include <stdint.h>

int server_socket(char *host, char *port);
int client_socket(char *host, char *port);

int tcpnonblock(int sock);
int tcpreuseaddr(int sock);
int tcpnodelay(int sock);

int tcpaccept(int sock);
int tcpclose(int sock);
int tcptoaccept(int sock, uint32_t msecto);
int tcpgetstatus(int sock);

int32_t tcptoread(int sock, void *buff, uint32_t leng, uint32_t msecto);
int32_t tcptowrite(int sock, const void *buff, uint32_t leng, uint32_t msecto);
