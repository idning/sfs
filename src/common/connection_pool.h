#ifndef _CONN_POLL
#define _CONN_POLL
#include "hashtable.h"
#include "dlist.h"

/*
 * a map of string -> list[evhttp_connection]
 *
 * actually it's a mutiMap
 * */
typedef struct ConnectionPool {
    Hashtable *ht;              // map from machine id to conn;
} ConnectionPool;

/*
 * this is a list
 * */
struct PoolEntry {
    char host_port[25];

    struct evhttp_connection *conn;
    struct dlist_t dlist;
};

ConnectionPool *connection_pool_new();
ConnectionPool *connection_pool_free(ConnectionPool * pool);

void connection_pool_insert(ConnectionPool * pool, char *host, int port,
                            struct evhttp_connection *conn);

struct evhttp_connection *connection_pool_get_free_conn(ConnectionPool * pool,
                                                        char *host, int port);
struct evhttp_connection *connection_pool_get_or_create_conn(ConnectionPool *
                                                             pool, char *host,
                                                             int port);

#endif /* _CONN_POLL */
