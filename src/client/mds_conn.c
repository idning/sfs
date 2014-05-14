#include "sfs_common.h"
#include "mds_conn.h"
#include "evrpc_pool_holder.h"
#include <pthread.h>

ConnectionPool *conn_pool = NULL;
EVrpcPoolHolder * pool_holder = NULL; //evrpc_pool_holder_new();

typedef void (*marshal_func) (struct evbuffer *, void *);
typedef int (*unmarshal_func) (void *, struct evbuffer *);

static void rpc_grneral_request(char *ip, int port, const char *rpcname,
                                void *req, marshal_func req_marshal,
                                void *resp, unmarshal_func resp_unmarshal);

static void rpc_rpc_grneral_requestuest_cb(struct evhttp_request *req,
                                           void *arg);
static void file_stat_copy(struct file_stat *dst, struct file_stat *src);
///////////////////////////////////////////////////////////////////////////////////////////
/*
在一个group里面的rpc需要都完成，才向上返回.
*/
struct rpc_group{
    int cnt;
};


static void stat_cb(struct evrpc_status *status, struct stat_request *request , struct stat_response * response , void *arg) {
    struct mds_req_ctx * ctx = (struct mds_req_ctx *) arg;
    struct file_stat *o_stat = file_stat_new();
    struct file_stat *stat;
    EVTAG_ARRAY_GET(response, stat_arr, 0, &stat);
    file_stat_copy(o_stat, stat);
    if (ctx->cb(ctx, o_stat) == 0)
        event_loopexit(NULL);
}

/*static void ls_cb(struct evrpc_status *status, struct ls_request *request , struct ls_response * response , void *arg){*/
    /*event_loopexit(NULL);*/
/*}*/

/*static void lookup_cb(struct evrpc_status *status, struct lookup_request *request , struct lookup_response * response , void *arg){*/
    /*event_loopexit(NULL);*/
/*}*/

/*static void unlink_cb(struct evrpc_status *status, struct unlink_request *request , struct unlink_response * response , void *arg){*/
    /*event_loopexit(NULL);*/
/*}*/

/*static void setattr_cb(struct evrpc_status *status, struct setattr_request *request , struct setattr_response * response , void *arg){*/
    /*event_loopexit(NULL);*/
/*}*/

static void mknod_cb(struct evrpc_status *status, struct mknod_request *request , struct mknod_response * response , void *arg){

    struct mds_req_ctx * ctx = (struct mds_req_ctx *) arg;
    struct file_stat *o_stat = file_stat_new();
    struct file_stat *stat;
    EVTAG_ARRAY_GET(response, stat_arr, 0, &stat);
    file_stat_copy(o_stat, stat);
    if (ctx->cb(ctx, o_stat) == 0)
        event_loopexit(NULL);
}

/*static void statfs_cb(struct evrpc_status *status, struct statfs_request *request , struct statfs_response* response , void *arg){*/
    /*event_loopexit(NULL);*/
/*}*/




///////////////////////////////////////////////////////////////////////////////////////////
int setattr_send_request(char *ip, int port, struct file_stat *stat_arr)
{
    DBG();
    struct setattr_request *req = setattr_request_new();
    struct setattr_response *response = setattr_response_new();

    struct file_stat *t = EVTAG_ARRAY_ADD(req, stat_arr);
    EVTAG_ASSIGN(t, ino, stat_arr->ino);
    EVTAG_ASSIGN(t, size, stat_arr->size);

    rpc_grneral_request(ip, port, "/.rpc.rpc_setattr",
                        req, (marshal_func) setattr_request_marshal,
                        response, (unmarshal_func) setattr_response_unmarshal);

    int rst = response->rst_code;
    if (rst!=0){
        logging(LOG_WARN, "setattr_send_request("PRIu64") return rst != 0", stat_arr->ino);
        goto done;
    }
    /*int cnt = EVTAG_ARRAY_LEN(response, stat_arr);*/
    /*if (cnt != 1) {*/
        /*logging(LOG_ERROR, "setattr_send_request return cnt != 1, cnt= %d",*/
                /*cnt);*/
        /*ret = -1;*/
        /*goto done;*/
    /*}*/
  done:
    setattr_request_free(req);
    setattr_response_free(response);
    return rst;

}

int stat_send_request_async(char *ip, int port, uint64_t * ino_arr, int len, struct mds_req_ctx * ctx)
{
    DBG();
    struct stat_request *req = stat_request_new();
    struct stat_response *response = stat_response_new();
    int i;
    for (i = 0; i < len; i++) {
        EVTAG_ARRAY_ADD_VALUE(req, ino_arr, ino_arr[i]);
    }

    struct evrpc_pool * pool  = evrpc_pool_holder_get(pool_holder, ip, port);
    EVRPC_MAKE_REQUEST(rpc_stat, pool, req, response,  stat_cb, ctx);
    return 0;
}

int stat_send_request(char *ip, int port, uint64_t * ino_arr, int len,
                      struct file_stat *stat_arr)
{
    logging(LOG_DEUBG, "stat_send_request, %s:%d, %"PRIu64"", ip, port ,ino_arr[0]);

    int rst = 0;
    struct stat_request *req = stat_request_new();
    struct stat_response *response = stat_response_new();
    int i;
    for (i = 0; i < len; i++) {
        EVTAG_ARRAY_ADD_VALUE(req, ino_arr, ino_arr[i]);
    }

    rpc_grneral_request(ip, port, "/.rpc.rpc_stat",
                        req, (marshal_func) stat_request_marshal,
                        response, (unmarshal_func) stat_response_unmarshal);

    int cnt = EVTAG_ARRAY_LEN(response, stat_arr);
    rst = response->rst_code;
    if (cnt != len || rst!= 0) {
        logging(LOG_DEUBG, "stat_send_request, %s:%d, %"PRIu64" return null!!", ip, port ,ino_arr[0]);
        goto done;
    }
    struct file_stat *stat;
    for (i = 0; i < len; i++) {
        EVTAG_ARRAY_GET(response, stat_arr, i, &stat);
        if (stat->ino == 0) {
            rst = -1;
            goto done;
        }
        file_stat_copy(stat_arr + i, stat);
        log_file_stat("stat get file stat: ", stat);
    }
  done:
    stat_request_free(req);
    stat_response_free(response);
    return rst;
}

//seams same as stat_send_request
int ls_send_request(char *ip, int port, uint64_t ino,
                    struct file_stat ***o_stat_arr, int *o_cnt)
{
    DBG();
    struct ls_request *req = ls_request_new();
    struct ls_response *response = ls_response_new();

    EVTAG_ARRAY_ADD_VALUE(req, ino_arr, ino);

    /*int mid = get_mid_of_ino(ino); */
    /*struct machine * m = cluster_get_machine_by_mid(mid); */

    rpc_grneral_request(ip, port, "/.rpc.rpc_ls",
                        req, (marshal_func) ls_request_marshal,
                        response, (unmarshal_func) ls_response_unmarshal);
    int rst = response->rst_code;
    if (rst!=0){
        logging(LOG_WARN, "ls_send_request return rst != 0");
        goto done;
    }
    int cnt = EVTAG_ARRAY_LEN(response, stat_arr);

    int i;
    struct file_stat **stat_arr = malloc(sizeof(void *) * cnt);
    // calloc(sizeof(struct file_stat), cnt);
    for (i = 0; i < cnt; i++)
        stat_arr[i] = file_stat_new();

    struct file_stat *stat;

    for (i = 0; i < cnt; i++) {
        EVTAG_ARRAY_GET(response, stat_arr, i, &stat);
        file_stat_copy(stat_arr[i], stat);
        /*log_file_stat("ls get : ", stat);*/
    }
done:
    ls_request_free(req);
    ls_response_free(response);

    *o_stat_arr = stat_arr;
    *o_cnt = cnt;

    return rst;
}

/*struct mknod_reqeust{*/
    /*char *ip;*/
    /*int port;*/
    /*uint64_t parent_ino; */
    /*const char *name; */
    /*int type;*/
    /*int mode; */
    /*struct file_stat *o_stat;*/
/*}*/


int mknod_send_request_async(char *ip, int port,
                       uint64_t parent_ino, uint64_t ino, 
                       const char *name, int type,
                       int mode, struct mds_req_ctx * ctx)
{
    DBG();
    struct mknod_request *req = mknod_request_new();

    struct mknod_response *response = mknod_response_new();
    EVTAG_ASSIGN(req, parent_ino, parent_ino);
    EVTAG_ASSIGN(req, ino, ino);
    EVTAG_ASSIGN(req, name, name);
    EVTAG_ASSIGN(req, type, type);
    EVTAG_ASSIGN(req, mode, mode);

    /*int mid = get_mid_of_ino(parent_ino); */
    /*struct machine * m = cluster_get_machine_by_mid(mid); */

    /*rpc_grneral_request(ip, port, "/.rpc.rpc_mknod",*/
                        /*req, (marshal_func) mknod_request_marshal,*/
                        /*response, (unmarshal_func) mknod_response_unmarshal);*/




    struct evrpc_pool * pool  = evrpc_pool_holder_get(pool_holder, ip, port);
    EVRPC_MAKE_REQUEST(rpc_mknod, pool, req, response,  mknod_cb, ctx);
    return 0;
}


int mknod_send_request(char *ip, int port,
                       uint64_t parent_ino, uint64_t ino, 
                       const char *name, int type,
                       int mode, struct file_stat *o_stat)
{
    DBG();
    struct mknod_request *req = mknod_request_new();

    struct mknod_response *response = mknod_response_new();
    EVTAG_ASSIGN(req, parent_ino, parent_ino);
    EVTAG_ASSIGN(req, ino, ino);
    EVTAG_ASSIGN(req, name, name);
    EVTAG_ASSIGN(req, type, type);
    EVTAG_ASSIGN(req, mode, mode);

    /*int mid = get_mid_of_ino(parent_ino); */
    /*struct machine * m = cluster_get_machine_by_mid(mid); */

    rpc_grneral_request(ip, port, "/.rpc.rpc_mknod",
                        req, (marshal_func) mknod_request_marshal,
                        response, (unmarshal_func) mknod_response_unmarshal);

    int rst = response->rst_code;
    if (rst!=0){
        logging(LOG_WARN, "mknod_send_request(%"PRIu64", name = %s, parent = %"PRIu64") return rst != 0", ino, name, parent_ino);
        goto done;
    }
    struct file_stat *stat;
    EVTAG_ARRAY_GET(response, stat_arr, 0, &stat);
    file_stat_copy(o_stat, stat);

done:
    mknod_request_free(req);
    mknod_response_free(response);
    return rst;
}

int symlink_send_request(char *ip, int port, uint64_t parent_ino, uint64_t ino, 
                         const char *name, const char *path,
                         struct file_stat *o_stat)
{
    DBG();
    struct symlink_request *req = symlink_request_new();

    struct symlink_response *response = symlink_response_new();
    EVTAG_ASSIGN(req, parent_ino, parent_ino);
    EVTAG_ASSIGN(req, ino, ino);
    EVTAG_ASSIGN(req, name, name);
    EVTAG_ASSIGN(req, path, path);

    /*int mid = get_mid_of_ino(parent_ino); */
    /*struct machine * m = cluster_get_machine_by_mid(mid); */

    rpc_grneral_request(ip, port, "/.rpc.rpc_symlink",
                        req, (marshal_func) symlink_request_marshal,
                        response, (unmarshal_func) symlink_response_unmarshal);

    int rst = response->rst_code;
    if (rst!=0){
        logging(LOG_WARN, "symlink_send_request return rst != 0");
        goto done;
    }
    struct file_stat *stat;
    EVTAG_GET(response, stat, &stat);
    file_stat_copy(o_stat, stat);

done:
    symlink_request_free(req);
    symlink_response_free(response);
    return rst;
}
//FIXME 应该返回int
const char *readlink_send_request(char *ip, int port, uint64_t ino)
{
    DBG();
    struct readlink_request *req = readlink_request_new();
    struct readlink_response *response = readlink_response_new();

    EVTAG_ASSIGN(req, ino, ino);

    /*int mid = get_mid_of_ino(ino); */
    /*struct machine * m = cluster_get_machine_by_mid(mid); */

    rpc_grneral_request(ip, port, "/.rpc.rpc_readlink",
                        req, (marshal_func) readlink_request_marshal,
                        response, (unmarshal_func) readlink_response_unmarshal);

    int rst = response->rst_code;
    if (rst!=0){
        logging(LOG_WARN, "readlink_send_request return rst != 0");
        goto done;
    }
    logging(LOG_DEUBG, "readlink , test : %s", response->path);


    char *p = NULL;
done:
    p = strdup(response->path);
    readlink_request_free(req);
    readlink_response_free(response);
    return p;
}

int lookup_send_request(char *ip, int port, uint64_t parent_ino,
                        const char *name, struct file_stat *o_stat)
{
    DBG();
    struct lookup_request *req = lookup_request_new();

    /*int mid = get_mid_of_ino(parent_ino); */
    /*struct machine * m = cluster_get_machine_by_mid(mid); */

    struct lookup_response *response = lookup_response_new();
    EVTAG_ASSIGN(req, parent_ino, parent_ino);
    EVTAG_ASSIGN(req, name, name);

    rpc_grneral_request(ip, port, "/.rpc.rpc_lookup",
                        req, (marshal_func) lookup_request_marshal,
                        response, (unmarshal_func) lookup_response_unmarshal);

    int rst = response->rst_code;
    if (rst!=0){
        logging(LOG_WARN, "lookup_send_request return rst != 0");
        goto done;
    }
    struct file_stat *stat;
    EVTAG_ARRAY_GET(response, stat_arr, 0, &stat);
    file_stat_copy(o_stat, stat);

done:
    lookup_request_free(req);
    lookup_response_free(response);
    
    return rst;
}

int unlink_send_request(char *ip, int port, uint64_t parent_ino,
                        const char *name)
{
    DBG();
    struct unlink_request *req = unlink_request_new();
    struct unlink_response *response = unlink_response_new();
    EVTAG_ASSIGN(req, parent_ino, parent_ino);
    EVTAG_ASSIGN(req, name, name);

    /*int mid = get_mid_of_ino(parent_ino); */
    /*struct machine * m = cluster_get_machine_by_mid(mid); */

    rpc_grneral_request(ip, port, "/.rpc.rpc_unlink",
                        req, (marshal_func) unlink_request_marshal,
                        response, (unmarshal_func) unlink_response_unmarshal);
    int rst = response->rst_code;
    unlink_request_free(req);
    unlink_response_free(response);
    return rst;
}

int statfs_send_request(char *ip, int port, int *total_space, int *avail_space,
                        int *inode_cnt)
{
    DBG();
    struct statfs_request *req = statfs_request_new();
    struct statfs_response *response = statfs_response_new();

    EVTAG_ASSIGN(req, nothing, 1);

    /*cluster_get_mds_arr(&mds, &mds_cnt); */
    /*struct machine * m = cluster_get_machine_by_mid(mds[0]); // TODO: currently it's  a random one */

    rpc_grneral_request(ip, port, "/.rpc.rpc_statfs",
                        req, (marshal_func) statfs_request_marshal,
                        response, (unmarshal_func) statfs_response_unmarshal);

    EVTAG_GET(response, total_space, total_space);
    EVTAG_GET(response, avail_space, avail_space);
    EVTAG_GET(response, inode_cnt, inode_cnt);
    statfs_request_free(req);
    statfs_response_free(response);
    return 0;
}

int mkfs_send_request(int mds1, int mds2)
{
    DBG();
    struct mkfs_request *req = mkfs_request_new();
    struct mkfs_response *response = mkfs_response_new();

    EVTAG_ARRAY_ADD_VALUE(req, pos_arr, mds1);
    EVTAG_ARRAY_ADD_VALUE(req, pos_arr, mds2);

    struct machine *m = cluster_get_machine_by_mid(mds1);

    rpc_grneral_request(m->ip, m->port, "/.rpc.rpc_mkfs",
                        req, (marshal_func) mkfs_request_marshal,
                        response, (unmarshal_func) mkfs_response_unmarshal);

    m = cluster_get_machine_by_mid(mds2);

    rpc_grneral_request(m->ip, m->port, "/.rpc.rpc_mkfs",
                        req, (marshal_func) mkfs_request_marshal,
                        response, (unmarshal_func) mkfs_response_unmarshal);

    mkfs_request_free(req);
    mkfs_response_free(response);
    return 0;
}

static void rpc_grneral_request(char *ip, int port, const char *rpcname,
                                void *req, marshal_func req_marshal,
                                void *resp, unmarshal_func resp_unmarshal)
{
    struct evhttp_connection *evcon =
        connection_pool_get_or_create_conn(conn_pool, ip, port);
    struct evhttp_request *evreq =
        evhttp_request_new(rpc_rpc_grneral_requestuest_cb, NULL);
    evhttp_request_own(evreq);  // this means that I should free it my self

    req_marshal(evreq->output_buffer, req);
    if (evhttp_make_request(evcon, evreq, EVHTTP_REQ_POST, rpcname))
        logging(LOG_ERROR, "error on make_request");

    event_dispatch();//TODO 
    if (resp_unmarshal(resp, evreq->input_buffer)) {
        logging(LOG_ERROR, "error on statfs_response_unmarshal");
    }
    connection_pool_insert(conn_pool, ip, port, evcon);
    evhttp_request_free(evreq);
}

static void rpc_rpc_grneral_requestuest_cb(struct evhttp_request *req,
                                           void *arg)
{
    if (req->response_code != HTTP_OK) {
        fprintf(stderr, "FAILED (response code)\n");
        exit(1);
    }
    event_loopexit(NULL);
}


void * ev_loop_func(void * ptr){
    while(1){
        event_dispatch();
        logging(LOG_INFO, "ev_loop_func");
    }
}

void mds_conn_init()
{
    DBG();
    event_init();
    rpc_client_setup("client", 0, MACHINE_CLIENT);

    //struct machine *mds = cluster_get_machine_of_type(MACHINE_MDS);

    conn_pool = connection_pool_new();
    pool_holder = evrpc_pool_holder_new();
    /*pthread_t thread1;*/
    /*int ret = pthread_create( &thread1, NULL, ev_loop_func, NULL);*/



}

static void file_stat_copy(struct file_stat *dst, struct file_stat *src)
{
    dst->size = src->size;
    dst->ino = src->ino;
    dst->type = src->type;
    dst->mode = src->mode;
    dst->parent_ino = src->parent_ino;
    if (src->name)
        dst->name = strdup(src->name);  //FIXME : free me!

    int len = EVTAG_ARRAY_LEN(src, pos_arr);
    int i = 0, pos;
    for (i = 0; i < len; i++) {
        EVTAG_ARRAY_GET(src, pos_arr, i, &pos);
        EVTAG_ARRAY_ADD_VALUE(dst, pos_arr, pos);
    }
}
