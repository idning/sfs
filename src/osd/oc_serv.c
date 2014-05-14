#include "sfs_common.h"
#include "hdd.h"

static void reply_error(struct evhttp_request *req, int err_code, char *fmt,
                        ...)
{
    DBG();
    char buf[1024];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf) - 30, fmt, ap);
    va_end(ap);

    logging(LOG_ERROR, "reply_error : %s", buf);
    evhttp_send_error(req, err_code, buf);
}

void shutdown_handler(struct evhttp_request *req, void *arg)
{
    DBG();

    if (evhttp_request_get_command(req) != EVHTTP_REQ_GET) {
        reply_error(req, HTTP_BADREQUEST, "should call with GET");
        return;
    }
    exit(0);
}

void write_chunk(uint64_t chunkid, struct evhttp_request *req)
{
    DBG();
    struct evbuffer *input;
    struct evbuffer *evb = evbuffer_new();

    if (evhttp_request_get_command(req) != EVHTTP_REQ_POST) {
        reply_error(req, HTTP_BADREQUEST, "should call write with POST");
        return;
    }
    uint64_t start = 0, end;
    const char *range = evhttp_find_header(req->input_headers, "Range");

    logging(LOG_DEUBG, "write Range Header: %s", range);
    if (range) {
        sscanf(range, "bytes=%" SCNu64 "-%" SCNu64, &start, &end);
    }

    input = req->input_buffer;
    hdd_chunk *chunk = hdd_create_chunk(chunkid, 0);    //TODO

    int fd = open(chunk->path, O_WRONLY | O_CREAT, 0755);
    logging(LOG_DEUBG, "write seek to : %" PRIu64 "", start);
    logging(LOG_DEUBG, "evbuffer_get_length(input) = %d",
            evbuffer_get_length(input));
    lseek(fd, start, SEEK_SET);

    if (-1 == fd) {
        reply_error(req, HTTP_INTERNAL, "could not open file : %s",
                    chunk->path);
        return;
    }

    int rst = 0;
    while (evbuffer_get_length(input) && (rst = evbuffer_write(input, fd)) > 0) {
        ;
    }

    /*evbuffer_write(input, fd); */
    close(fd);

    evbuffer_add(evb, "success", strlen("success"));
    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);
}

/*
 * request start - end 
 * if file_size < end;
 * return start - file_size
 * */
void read_chunk(uint64_t chunkid, struct evhttp_request *req)
{
    DBG();
    struct evbuffer *evb = evbuffer_new();
    hdd_chunk *chunk = chunk_hashtable_get(chunkid);

    if (chunk == NULL) {
        reply_error(req, HTTP_NOTFOUND, "not found chunk %" PRIx64 ";",
                    chunkid);
        return;
    }

    hdd_chunk_printf(chunk);

    uint64_t start = 0, end = 0;
    const char *range = evhttp_find_header(req->input_headers, "Range");
    struct stat st;

    logging(LOG_DEUBG, "get range : %s", range);
    int fd = open(chunk->path, O_RDONLY);
    if (fstat(fd, &st) < 0) {
        reply_error(req, HTTP_NOTFOUND, "file not exist : %s", chunk->path);
        return;
    }
    logging(LOG_DEUBG, "st.st_size = : %d", st.st_size);

    if (range) {
        sscanf(range, "bytes=%" SCNu64 "-%" SCNu64, &start, &end);
        //假设文件st_size = 2
        //if end = 0, 应该返回1个字节           end=end
        //if end = 1, 应该返回0,1 共2个字节.    end = st_size - 1  || end = end
        //if end = 2, 还是应该2个字节.          end = st_size - 1
        if (st.st_size <= end)
            end = st.st_size - 1;
    } else {
        start = 0;
        end = st.st_size - 1;
    }
    logging(LOG_DEUBG, "get return range : %" PRIu64 " - %" PRIu64, start, end);
    logging(LOG_DEUBG, "d : %" PRIu64, end - start + 1);

    lseek(fd, start, SEEK_SET);
    evbuffer_add_file(evb, fd, (int)start, end - start + 1);    //如果编译的时候加上 -D_FILE_OFFSET_BITS=64 ,，evbuffer认为length = 0

    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);
}

void gen_handler(struct evhttp_request *req, void *arg)
{
    DBG();
    /*struct evbuffer *evb = evbuffer_new(); */

    const char *uri = evhttp_request_get_uri(req);
    struct evhttp_uri *decoded_uri = NULL;
    const char *path;
    char *decoded_path;

    /* Decode the URI */
    decoded_uri = evhttp_uri_parse(uri);
    if (!decoded_uri) {
        reply_error(req, HTTP_BADREQUEST, "Bad URI: %s", uri);
        return;
    }

    path = evhttp_uri_get_path(decoded_uri);
    if (!path) {
        logging(LOG_INFO, "request path is nil, replace it as /");
        path = "/";
    }

    decoded_path = evhttp_uridecode(path, 0, NULL);

    uint64_t chunkid;
    char *slash = strchr(path + 1, '/');
    *slash = '\0';
    const char *op = path + 1;
    sscanf(slash + 1, "%" SCNu64, &chunkid);

    logging(LOG_INFO, "%s, %" PRIu64, op, chunkid);

    if (strcmp(op, "put") == 0) {
        write_chunk(chunkid, req);
    } else if (strcmp(op, "get") == 0) {
        read_chunk(chunkid, req);
    } else {
        reply_error(req, HTTP_NOTFOUND, "not found: %s", uri);
        return;
    }
}

void usage(const char *appname)
{

}

void onexit()
{

}

static void update_clustermap_from_cmgr_on_timer_cb(evutil_socket_t fd, short what, void *arg)
{ 
    ping_send_request();
}

int main(int argc, char **argv)
{
    init_app(argc, argv, "osd");

    event_init();

    char *hdd_cfg = cfg_getstr("HDD_CONF_FILENAME", "etc/hdd.conf");
    hdd_init(hdd_cfg);

    char *self_host = cfg_getstr("OSD2CLIENT_LISTEN_HOST", "*");
    int self_port = cfg_getint32("OSD2CLIENT_LISTEN_PORT", 9527);
    rpc_client_setup(self_host, self_port, MACHINE_OSD);

    struct evhttp *httpd;

    char *listen_host = cfg_getstr("OSD2CLIENT_LISTEN_HOST", "*");
    int port = cfg_getint32("OSD2CLIENT_LISTEN_PORT", 9527);

    httpd = evhttp_start(listen_host, port);
    if (httpd == NULL) {
        logging(LOG_ERROR, "start server error %m");
        exit(1);
    } else {
        printf("Start osd at %s:%d\n", listen_host, port);
    }
    evhttp_set_cb(httpd, "/shutdown", shutdown_handler, NULL);
    evhttp_set_gencb(httpd, gen_handler, NULL);


    struct timeval five_seconds = {2,0};
    struct event *update_clustermap_event= event_new(NULL, -1, EV_PERSIST, update_clustermap_from_cmgr_on_timer_cb, NULL);
    event_add(update_clustermap_event, &five_seconds);

    event_dispatch();

    evhttp_free(httpd);
    return 0;
}
