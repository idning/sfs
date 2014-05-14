#ifndef __POOL_HOLDER
#define __POOL_HOLDER
#include "hashtable.h"

/*
 * a map of string -> struct evrpc_pool *
 * this is different from connection_pool.c
 *
 * */
typedef struct EVrpcPoolHolder{
    Hashtable *ht;              // map from machine id to conn;
} EVrpcPoolHolder;


EVrpcPoolHolder *evrpc_pool_holder_new();
void evrpc_pool_holder_free(EVrpcPoolHolder* holder);

/*不需要return to the pool, evrpc自己会处理的.
 * */
//void evrpc_pool_holder_insert(EVrpcPoolHolder * holder, char *host, int port,
                            //struct evrpc_pool * pool);

struct evrpc_pool * evrpc_pool_holder_get(EVrpcPoolHolder * holder,
                                                        char *host, int port);

#endif /* _CONN_POLL */
