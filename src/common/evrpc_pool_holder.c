//#include <evrpc.h>
#include "evrpc_pool_holder.h"
#include "log.h"

#include <event.h>
#include <evhttp.h>
#include <evutil.h>
#include <event2/rpc.h>

struct PoolEntry {
    char host_port[25];
    struct evrpc_pool * pool;
};

EVrpcPoolHolder *evrpc_pool_holder_new()
{
    EVrpcPoolHolder *holder = (EVrpcPoolHolder *) malloc(sizeof(EVrpcPoolHolder));
    Hashtable *ht =
        hashtable_new(hash_str_hash_func, hash_str_equal_func, 1024);
    holder->ht = ht;
    return holder;
};

void evrpc_pool_holder_free(EVrpcPoolHolder * holder)
{
    assert(0);
}

static void evrpc_pool_holder_insert(EVrpcPoolHolder * holder, char *host, int port,
                            struct evrpc_pool *pool)
{
    logging(LOG_DEUBG, "evrpc_pool_holder_insert on %s:%d", host, port);
    struct PoolEntry *e = malloc(sizeof(struct PoolEntry));
    e->pool = pool;
    sprintf(e->host_port, "%s:%d", host, port);
    hashtable_insert(holder->ht, e->host_port, e);
}
struct evrpc_pool * evrpc_pool_holder_get(EVrpcPoolHolder * holder, char *host, int port)
{
    logging(LOG_DEUBG, "evrpc_pool_holder_get on %s:%d", host, port);
    char str[25];
    sprintf(str, "%s:%d", host, port);

    struct PoolEntry *p= hashtable_lookup(holder->ht, str);
    if (NULL == p) {
        struct evrpc_pool * pool = evrpc_pool_new(NULL);
        struct evhttp_connection *evcon = evhttp_connection_new(host, port);
        evrpc_pool_add_connection(pool, evcon);

        evrpc_pool_holder_insert(holder, host, port, pool);
        return pool;

    }

    return p-> pool;
}

