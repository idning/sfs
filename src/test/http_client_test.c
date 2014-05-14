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

char *test_get()
{
    char *p;
    //http://www.baidu.com/search/error.html
    http_response *response = http_get("http://127.0.0.1:6006/get/999999", NULL);   // xiaonei.com/home
    printf("%d\n", response->status_code);

    while ((p = evbuffer_readline(response->headers))) {
        printf("%s\n", p);
        free(p);
    }
    p = malloc(1024000);
    evbuffer_copyout(response->body, p, evbuffer_get_length(response->body));
    printf("evbuffer_get_length after evbuffer_copyout:  %d\n",
           evbuffer_get_length(response->body));

    return p;

}

void test_post(char *str)
{
    char *p;
    struct evbuffer *buffer = evbuffer_new();
    evbuffer_add_printf(buffer, "%s", str);
    http_response *response = http_post("http://127.0.0.1:6006/put/999999", NULL, buffer);  // xiaonei.com/home
    printf("%d\n", response->status_code);

    while ((p = evbuffer_readline(response->headers))) {
        printf("%s\n", p);
        free(p);
    }

    while ((p = evbuffer_readline(response->body))) {
        printf("%s\n", p);
        free(p);
    }

    printf(" here is the request body len : %d ", evbuffer_get_length(buffer));

    while ((p = evbuffer_readline(buffer))) {
        printf(" g");

        printf("%s\n", p);
        free(p);
    }
    http_response_free(response);
}

int main()
{
    event_init();
    http_client_init();
    char *s = "abc";
    test_post(s);
    char *t = test_get();
    printf("get: %s", t);
    assert(0 == strcmp(s, t));
    test_post(s);
    return 0;
}
