#include "sfs_common.h"

uint64_t g_inode_allocated= 1000;
/**
 * clinet会向uuid server请求1个或Ngeuuid,用于接下来创建inode的inode 号,
 * 服务器可以实现自己的uuid算法，
 * 这里通过在配置文件中保存哪些uuid被分配过而保证分配唯一的uuid
 */
void uuid_handler(EVRPC_STRUCT(rpc_uuid) * rpc, void *arg)
{
    DBG();

    struct uuid_request *req = rpc->request;
    struct uuid_response *response = rpc->reply;

    int req_count = req->count;
    //TODO: add lock
    EVTAG_ASSIGN(response, range_min, g_inode_allocated);
    EVTAG_ASSIGN(response, range_max, g_inode_allocated+req_count);
    g_inode_allocated+= req_count;


    EVRPC_REQUEST_DONE(rpc);
}

static void rpc_setup()
{
    struct evhttp *http = NULL;
    struct evrpc_base *base = NULL;

    char *listen_host = cfg_getstr("CMGR_LISTEN_HOST", "*");
    int port = cfg_getint32("CMGR_LISTEN_PORT", 9000);

    http = evhttp_start(listen_host, port);
    /*http = evhttp_start("192.168.1.102", port); */
    if (!http) {
        perror("can't start server!");
        exit(-1);
    }
    printf("Start cmgr at %s:%d\n", listen_host, port);

    base = evrpc_init(http);

    EVRPC_REGISTER(base, rpc_ping, ping, pong, ping_handler, NULL);
    EVRPC_REGISTER(base, rpc_uuid, uuid_request, uuid_response, uuid_handler, NULL);

}

void usage(const char *appname)
{

}

void onexit()
{
    char s[28];
    sprintf(s, "%"PRIu64, g_inode_allocated);
    cfg_add_and_write("INODE_ALLOCATED", s);

}

int main(int argc, char **argv)
{
    event_init();
    init_app(argc, argv, "cmgr");
    g_inode_allocated= cfg_getint32("INODE_ALLOCATED", 1000);

    rpc_setup();
    event_dispatch();
    return 0;
}
