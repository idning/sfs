#include <event.h>
#include <evhttp.h>
#include <event2/rpc.h>
//#include <event2/rpc_struct.h>

#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "protocol.gen.h"
#include "protocol.h"
#include "log.h"
#include "cfg.h"

#include "cluster.h"

#define MAX_MACHINE_CNT 256

static struct machine machines[MAX_MACHINE_CNT];
static int machine_cnt = 0;
static uint32_t cluster_version = 0;

static int mds_arr[MAX_MACHINE_CNT];
static int mds_cnt = 0;
static int osd_arr[MAX_MACHINE_CNT];
static int osd_cnt = 0;

struct evrpc_pool *cmgr_conn_pool;
struct event_base *cmgr_ev_base;
struct machine self_machine;

void cluster_init();
struct machine * cluster_add(char *ip, int port, char type, int mid);
void cluster_remove(char *ip, int port);
void cluster_printf(int log_level, char *hint);
void cluster_dump();
static int cluster_uuid();
void sort_mds_arr();
int cluster_get_current_version(){
    return cluster_version;
}
static char *cluster_type_str(int type)
{
    switch (type) {
    case MACHINE_CMGR:
        return "MGR";
    case MACHINE_MDS:
        return "MDS";
    case MACHINE_OSD:
        return "OSD";
    case MACHINE_CLIENT:
        return "client";
    }
    return NULL;
}

//evrpc server side
void ping_handler(EVRPC_STRUCT(rpc_ping) * rpc, void *arg)
{
    DBG();

    struct ping *ping = rpc->request;
    struct pong *pong = rpc->reply;

    int ping_version = ping->version;
    //char * self_ip = ping->self_ip;
    char *remote_ip;            //= rpc->http_req->evcon->address;
    int remote_port;            //= rpc->http_req->evcon->address;
    uint16_t tmp_port;
    evhttp_connection_get_peer(rpc->http_req->evcon, &remote_ip, &tmp_port);
    remote_port = ping->self_port;
    int type = ping->self_type;
    int mid = ping->mid;
    logging(LOG_DEUBG, "ping_handler(ip=%s, port=%d, type=%s, load=%d)", remote_ip,
            remote_port, cluster_type_str(type), ping->load);

    if (ping->mid == 0) {
        mid = cluster_uuid();
        EVTAG_ASSIGN(pong, mid, mid);
    }else {
        EVTAG_ASSIGN(pong, mid, ping->mid);
    }

    struct machine * m = cluster_add(remote_ip, remote_port, type, mid);

    m->load = ping->load;
    if (ping_version == 0){ //MDS强制加version
        cluster_version++;
    }
    if (type == MACHINE_MDS)
        cluster_printf(LOG_WARN, "log load");

    if (cluster_version > ping_version) {   // new machine added to cluster
        EVTAG_ASSIGN(pong, version, cluster_version);
        int i;
        for (i = 0; i < machine_cnt; i++) {
            EVTAG_ARRAY_ADD(pong, machines);    // alloc space for machines
            pong_machines_assign(pong, i, machines + i);
        }
    } else {
        EVTAG_ASSIGN(pong, version, ping_version);  //version not change!
    }
    EVRPC_REQUEST_DONE(rpc);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//evrpc client side
void ping_cb(struct evrpc_status *status, struct ping *ping, struct pong *pong,
             void *arg)
{
    event_base_loopexit(cmgr_ev_base, NULL);
}


int ping_send_request_with_version(int req_version)
{
    DBG();
    struct ping *ping = ping_new();
    struct pong *pong = pong_new();

    EVTAG_ASSIGN(ping, version, req_version);
    EVTAG_ASSIGN(ping, self_ip, self_machine.ip);
    EVTAG_ASSIGN(ping, self_port, self_machine.port);
    EVTAG_ASSIGN(ping, self_type, self_machine.type);
    EVTAG_ASSIGN(ping, mid, self_machine.mid);
    EVTAG_ASSIGN(ping, load, self_machine.load);
    logging(LOG_DEUBG, "ping(version = %u, mid = %d)", req_version,
            self_machine.mid);

    EVRPC_MAKE_REQUEST(rpc_ping, cmgr_conn_pool, ping, pong, ping_cb, NULL);
    event_base_dispatch(cmgr_ev_base);

    int pong_version;
    int pong_mid;
    EVTAG_GET(pong, version, &pong_version);
    EVTAG_GET(pong, mid, &pong_mid);

    logging(LOG_DEUBG, "get pong (version = %u, mid = %d)", pong_version,
            pong_mid);

    if (pong_version == req_version) {
        logging(LOG_DEUBG, "cluster not change!");
        goto done;
    }
    logging(LOG_DEUBG, "cluster changed!");
    int cnt = EVTAG_ARRAY_LEN(pong, machines);
    int i;
    machine_cnt = cnt;
    cluster_version = pong->version;
    for (i = 0; i < cnt; i++) {
        struct machine *m;
        EVTAG_ARRAY_GET(pong, machines, i, &m);

        if (machines[i].ip)
            free(machines[i].ip);
        machines[i].ip = strdup(m->ip);
        machines[i].port = m->port;
        machines[i].type = m->type;
        machines[i].mid = m->mid;
        machines[i].load = m->load;
    }
    sort_mds_arr();
    cluster_printf(LOG_DEUBG, "after pong & sort::");
    cluster_dump();

  done:
    ping_free(ping);
    pong_free(pong);

    return pong_mid;
}


int ping_send_request(){
    return ping_send_request_with_version(cluster_version);

}

int ping_send_request_force_update(){
    return ping_send_request_with_version(0);
}


int get_self_machine_load(){
    return self_machine.load ;
}
void set_self_machine_load(int load){
    self_machine.load = load;
}

struct machine * get_self_machine(){
    return &self_machine;
}
//TODO: rename it to cmgr_client_setup or something
void rpc_client_setup(char *self_host, int self_port, int self_type)
{
    struct evhttp_connection *evcon;

    cmgr_ev_base = event_base_new();
    cmgr_conn_pool = evrpc_pool_new(cmgr_ev_base);

    int cluster_mid = cfg_getint32("CLUSTER_MID", 0);
    self_machine.mid = cluster_mid;
    self_machine.ip = strdup(self_host);
    self_machine.port = self_port;
    self_machine.type = self_type;

    char *host = cfg_getstr("CMGR_HOST", "127.0.0.1");
    int port = cfg_getint32("CMGR_PORT", 9527);
    int i;
    for (i = 0; i < 2; i++) {   // 2 connections
        evcon = evhttp_connection_new(host, port);
        evrpc_pool_add_connection(cmgr_conn_pool, evcon);
    }

    int new_mid = ping_send_request();
    if (cluster_mid == 0) {
        char tmp[32];
        sprintf(tmp, "CLUSTER_MID = %d", new_mid);
        cfg_append(tmp);
    }
    self_machine.mid = new_mid;
}

void cluster_printf(int log_level, char *hint)
{
    char tmp[36000];
    char *p = tmp;
    int i;
    if (log_level < LOG_LEVEL)
        return;

    for (i = 0; i < machine_cnt; i++) {
        sprintf(p, "%6s%20s:%-6d(%5d) LOAD: %d\n", cluster_type_str(machines[i].type)
                , machines[i].ip, machines[i].port, machines[i].mid, machines[i].load);
        while (*p)
            p++;
    }
    logging(log_level, " %s clusters: v%d \n--------------\n%s\n------------\n",
            hint, cluster_version, tmp);
}

void cluster_init()
{
    //pong_p = pong_new();
    cluster_dump();
}

void cluster_get_mds_arr(int **o_arr, int *o_cnt)
{
    *o_arr = mds_arr;
    *o_cnt = mds_cnt;
}

int cluster_get_osd_cnt()
{
    return osd_cnt;
}

void cluster_get_osd_arr(int **o_arr, int *o_cnt)
{
    *o_arr = osd_arr;
    *o_cnt = osd_cnt;
}

/*already sort */
struct machine *cluster_get_mds_with_max_load()
{
    int i;
    for (i = 0; i < machine_cnt; i++) {
        if ( (machines[i].type == MACHINE_MDS )){
            logging(LOG_INFO, "cluster_get_mds_with_lowest_load() return (%d)!!", machines[i].mid);
            return machines + i;
        }
    }
    return NULL;
}

struct machine *cluster_get_mds_with_lowest_load()
{
    logging(LOG_DEUBG, "cluster_get_mds_with_lowest_load()");
    struct machine * rst = machines;
    int i;
    for (i = 0; i < machine_cnt; i++) {
        if ( (machines[i].type == MACHINE_MDS )&& (machines[i].load < rst->load))
            rst = machines + i;
    }
    logging(LOG_INFO, "cluster_get_mds_with_lowest_load() return (%d)!!", rst->mid);
    return rst;
}

struct machine *cluster_get_machine_by_mid(int mid)
{
    logging(LOG_DEUBG, "cluster_get_machine_by_mid(%d)", mid);
    int i;
    for (i = 0; i < machine_cnt; i++) {
        if (machines[i].mid == mid)
            return machines + i;
    }
    logging(LOG_INFO, "cluster_get_machine_by_mid(%d) return NULL!!", mid);
    return NULL;
}

int select_osd()
{
    static int i = 0;
    i = (i + 1) % osd_cnt;
    return osd_arr[i];
}

static int cluster_lookup(int mid)
{
    int i;
    for (i = 0; i < machine_cnt; i++) {
        if (machines[i].mid == mid) //already in array
            return 1;
    }
    return 0;
}

static int cluster_uuid()
{
    int t = cluster_version;
    while (cluster_lookup(t))
        t++;
    return t;
}

struct machine *cluster_add(char *ip, int port, char type, int mid)
{
    DBG();
    cluster_printf(LOG_DEUBG, "before cluster_add");
    int i;
    for (i = 0; i < machine_cnt; i++) {
        if (machines[i].port == port && (0 == strcmp((char *)machines[i].ip, ip)))  //already in array
            return machines+i;
    }
    logging(LOG_DEUBG, "add machine %s:%d @ %d\n", ip, port, machine_cnt);

    machines[machine_cnt].ip = strdup(ip);
    machines[machine_cnt].port = port;
    machines[machine_cnt].type = type;
    machines[machine_cnt].mid = mid;
    machine_cnt++;
    cluster_printf(LOG_INFO, "after cluster_add, updated");
    //logging(LOG_DEUBG, "current machine_cnt : %d", machine_cnt);

    /*cluster_machine *m = cluster_machine_new(); */
    /*m -> ip = ip; */
    /*m -> port = port; */
    /*m -> type = type; */
    /*dlist_insert_tail(cluster_head, m); */
    cluster_version++;
    cluster_dump();
    return machines+(machine_cnt-1);
}

void cluster_remove(char *ip, int port)
{
    /*dlist_insert_tail(head, entries+i ); */

    cluster_version++;
    cluster_dump();
}

void cluster_dump()
{
    int i;
    mds_cnt = 0;
    osd_cnt = 0;
    for (i = 0; i < machine_cnt; i++) {
        if (machines[i].type == MACHINE_MDS)
            mds_arr[mds_cnt++] = machines[i].mid;
        else if (machines[i].type == MACHINE_OSD)
            osd_arr[osd_cnt++] = machines[i].mid;
    }

}

/*
load大的在前面.
*/
static int machines_load_cmp(const void *p1, const void *p2)
{
    return ((struct machine *) p2)->load - ((struct machine *) p1)->load ;
}

void sort_mds_arr(){
    qsort(machines, machine_cnt, sizeof(struct machine),
            machines_load_cmp);

    /*for (int i=0; i<machine_cnt;i++){*/

    
    /*}*/
}

/*void * cmgr_client_loop_func(void * ptr){*/
    /*while(1){*/
        /*logging(LOG_INFO, "ev_loop_func");*/
    /*}*/
/*}*/


/*pthread_mutex_t update_cluster_map_mut = PTHREAD_MUTEX_INITIALIZER;*/
/*pthread_cond_t update_cluster_map_cond = PTHREAD_COND_INITIALIZER;*/

//void cmgr_client_thread_start(){
//    pthread_t cmgr_thread;
//    /*int ret = pthread_create( &thread1, NULL, ev_loop_func, NULL);*/
//    
//
//}
