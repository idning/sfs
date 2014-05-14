#include "sfs_common.h"
#include "mds_conn.h"

uint64_t uuid_min = 0, uuid_max = 0;
extern struct evrpc_pool *cmgr_conn_pool;
extern struct event_base *cmgr_ev_base;


void uuid_cb(struct evrpc_status *status, struct uuid_request *req, struct uuid_response *resp,
        void *arg)
{
    event_base_loopexit(cmgr_ev_base, NULL);
}

int uuid_send_request()
{
    DBG();

    struct uuid_request * request = uuid_request_new();
    struct uuid_response * response = uuid_response_new();

    EVTAG_ASSIGN(request, count , 100);

    EVRPC_MAKE_REQUEST(rpc_uuid, cmgr_conn_pool, request, response, uuid_cb, NULL);
    event_base_dispatch(cmgr_ev_base);

    EVTAG_GET(response, range_min, &uuid_min);
    EVTAG_GET(response, range_max, &uuid_max);

    logging(LOG_DEUBG, "get uuid range [%"PRIu64", %"PRIu64")", uuid_min, uuid_max);
    return 0;
}


int64_t cmgr_get_uuid(){
    if(uuid_max <= uuid_min){
        uuid_send_request();
    }
    return uuid_min++;
}

