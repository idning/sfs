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

void test_post(char *str)
{
    struct evbuffer *b1 = evbuffer_new();
    struct evbuffer *b2 = evbuffer_new();
    printf("len(b1): %d \n", evbuffer_get_length(b1));
    printf("len(b2): %d \n", evbuffer_get_length(b2));

    evbuffer_add_printf(b1, "%s", str);
    evbuffer_add_buffer(b2, b1);
    printf("len(b1): %d \n", evbuffer_get_length(b1));
    printf("len(b2): %d \n", evbuffer_get_length(b2));
    evbuffer_free(b1);
    printf("len(b1): %d \n", evbuffer_get_length(b1));
    printf("len(b2): %d \n", evbuffer_get_length(b2));

}

int main()
{
    event_init();
    http_client_init();
    char *s = "abc";
    test_post(s);

    return 0;
}
