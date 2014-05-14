#include <evhttp.h>
#include "connection_pool.h"
#include "dlist.h"
#include "log.h"

ConnectionPool *connection_pool_new()
{
    ConnectionPool *pool = (ConnectionPool *) malloc(sizeof(ConnectionPool));
    Hashtable *ht =
        hashtable_new(hash_str_hash_func, hash_str_equal_func, 1024);
    pool->ht = ht;
    return pool;
};

ConnectionPool *connection_pool_free(ConnectionPool * pool)
{
    assert(0);

}

void connection_pool_insert(ConnectionPool * pool, char *host, int port,
                            struct evhttp_connection *conn)
{
    logging(LOG_CONN, "connection_pool_insert on %s:%d", host, port);
    struct PoolEntry *e = malloc(sizeof(struct PoolEntry));
    e->conn = conn;
    sprintf(e->host_port, "%s:%d", host, port);
    dlist_t *pl;

    struct PoolEntry *old = hashtable_lookup(pool->ht, e->host_port);
    if (NULL == old) {
        struct PoolEntry *head = malloc(sizeof(struct PoolEntry));
        sprintf(head->host_port, "%s:%d", host, port);
        pl = &(head->dlist);

        dlist_init(pl);
        old = head;

        hashtable_insert(pool->ht, head->host_port, head);
    }
    pl = &(e->dlist);

    dlist_t *headpl = &(old->dlist);
    dlist_insert_head(headpl, pl);
}

struct evhttp_connection *connection_pool_get_free_conn(ConnectionPool * pool,
                                                        char *host, int port)
{
    logging(LOG_DEUBG, "connection_pool_get_free_conn on %s:%d", host, port);
    char str[25];
    sprintf(str, "%s:%d", host, port);

    struct PoolEntry *old = hashtable_lookup(pool->ht, str);
    if (NULL == old) {
        logging(LOG_CONN,
                "_connection_pool_get_free_conn on %s:%d return NULL!!!!!",
                host, port);
        return NULL;
    }

    struct PoolEntry *p;
    dlist_t *head = &(old->dlist);
    dlist_t *pl;
    for (pl = head->next; pl != head; pl = pl->next) {
        p = dlist_data(pl, struct PoolEntry, dlist);
        dlist_remove(pl);
        struct evhttp_connection *conn = p->conn;
        free(p);

        return conn;
    }
    logging(LOG_CONN,
            "connection_pool_get_free_conn on %s:%d return NULL!!!!!", host,
            port);
    return NULL;
}

struct evhttp_connection *connection_pool_get_or_create_conn(ConnectionPool *
                                                             pool, char *host,
                                                             int port)
{
    struct evhttp_connection *rst;
    rst = connection_pool_get_free_conn(pool, host, port);
    if (NULL == rst)
        rst = evhttp_connection_new(host, port);
    return rst;
}
