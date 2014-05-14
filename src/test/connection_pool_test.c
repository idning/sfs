#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include<event.h>
#include<event2/buffer.h>
#include <evutil.h>
#include <evhttp.h>

#include "http_client.h"
#include "protocol.gen.h"
#include "log.h"
#include "connection_pool.h"

int main()
{
    ConnectionPool *pool = connection_pool_new();
    char s1[] = "s1";
    connection_pool_insert(pool, "1", 1, (struct evhttp_connection *)s1);

    hashtable_foreach(pool->ht, hash_str_str_foreach, NULL);

    char *p1 = (char *)connection_pool_get_free_conn(pool, "1", 1);

    printf("%s\n", p1);

    return 0;
}
