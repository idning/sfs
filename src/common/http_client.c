#include "http_client.h"
#include "connection_pool.h"
#include "log.h"

#include <event.h>
#include <evhttp.h>
#include <evutil.h>
#include <event2/buffer.h>
#include <unistd.h>
#include <assert.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/queue.h>

static ConnectionPool *conn_pool = NULL;

static http_response *http_response_new(int status_code,
                                        struct evbuffer *headers,
                                        struct evbuffer *body);
void http_response_free(http_response * r);
struct http_response *http_get(const char *url, struct evkeyvalq *headers);
struct http_response *http_post(const char *url, struct evkeyvalq *headers,
                                struct evbuffer *postdata);

typedef void (*callback_func) (struct http_response * response);


struct request_context {
    struct evhttp_uri *uri;
    struct evhttp_connection *conn;
    struct evhttp_request *req;

    struct evbuffer *buffer;

    int method;
    struct evkeyvalq *req_headers;
    struct evbuffer *postdata_buffer;
    int ok;
};

static void http_client_callback(struct evhttp_request *req, void *arg);

static int client_renew_request(struct request_context *ctx);

static void http_client_callback(struct evhttp_request *req, void *arg)
{
    struct request_context *ctx = (struct request_context *)arg;
    struct evhttp_uri *new_uri = NULL;
    const char *new_location = NULL;

    switch (req->response_code) {
    case HTTP_OK:
        event_loopexit(0);
        break;

    case HTTP_MOVEPERM:
    case HTTP_MOVETEMP:
        new_location = evhttp_find_header(req->input_headers, "Location");
        if (!new_location)
            return;

        new_uri = evhttp_uri_parse(new_location);
        if (!new_uri)
            return;

        evhttp_uri_free(ctx->uri);
        ctx->uri = new_uri;

        client_renew_request(ctx);
        return;

    default:
        event_loopexit(0);
        return;
    }

    evbuffer_add_buffer(ctx->buffer, req->input_buffer);

    ctx->ok = 1;
}

struct request_context *context_new(const char *url, int verb,
                                    struct evkeyvalq *headers,
                                    struct evbuffer *data)
{
    struct request_context *ctx = 0;
    ctx = calloc(1, sizeof(*ctx));
    if (!ctx)
        return 0;

    ctx->uri = evhttp_uri_parse(url);
    ctx->method = verb;

    if (NULL != data) {
        ctx->postdata_buffer = evbuffer_new();
        evbuffer_add_buffer(ctx->postdata_buffer, data);
    }
    ctx->req_headers = headers;

    if (!ctx->uri)
        return 0;

    ctx->buffer = evbuffer_new();

    client_renew_request(ctx);

    return ctx;
}

void context_free(struct request_context *ctx)
{

    connection_pool_insert(conn_pool, ctx->uri->host, ctx->uri->port,
                           ctx->conn);
    /*evhttp_connection_free(ctx->conn); */

    if (ctx->buffer)
        evbuffer_free(ctx->buffer);
    if (ctx->postdata_buffer)
        evbuffer_free(ctx->postdata_buffer);

    evhttp_uri_free(ctx->uri);
    evhttp_request_free(ctx->req);
    free(ctx);
}

static int client_renew_request(struct request_context *ctx)
{
    /* free connections & request */
    if (ctx->conn)
        evhttp_connection_free(ctx->conn);
    struct evhttp_connection *conn =
        connection_pool_get_free_conn(conn_pool, ctx->uri->host,
                                      ctx->uri->port);
    if (conn != NULL) {
        logging(LOG_DEUBG, "get a conn from pool");
        ctx->conn = conn;
        //fprintf(stderr, " conn->state: %d\n", conn->state);

        /*conn */
        /*evhttp_connection_reset(ctx->conn); no !!! this will send FIN . what should I do */
    } else {
        logging(LOG_DEUBG, "no conn in pool, new one");
        ctx->conn =
            evhttp_connection_new(ctx->uri->host,
                                  ctx->uri->port > 0 ? ctx->uri->port : 80);
        //fprintf(stderr, " conn->state: %d\n", conn->state);
    }

    ctx->req = evhttp_request_new(http_client_callback, ctx);
    evhttp_request_own(ctx->req);   // means that I will free it myself

    struct evkeyval *header;

    if (ctx->req_headers != NULL) {
        TAILQ_FOREACH(header, ctx->req_headers, next) {
            evhttp_add_header(ctx->req->output_headers, header->key,
                              header->value);
        }
    }

    evhttp_add_header(ctx->req->output_headers, "Host", ctx->uri->host);

    if (ctx->method == EVHTTP_REQ_POST) {
        evbuffer_add_buffer(ctx->req->output_buffer, ctx->postdata_buffer);
        evhttp_make_request(ctx->conn, ctx->req, EVHTTP_REQ_POST,
                            ctx->uri->path ? ctx->uri->path : "/");

    } else if (ctx->method == EVHTTP_REQ_GET) {
        evhttp_make_request(ctx->conn, ctx->req, EVHTTP_REQ_GET,
                            ctx->uri->path ? ctx->uri->path : "/");
    } else {
        assert(0);
    }

    return 0;
}

static struct http_response *http_request(const char *url, int verb,
                                          struct evkeyvalq *headers,
                                          struct evbuffer *data)
{
    /*fprintf(stderr, "http_request: %s, %d\n", url, verb);*/
    struct request_context *ctx = context_new(url, verb, headers, data);

    event_dispatch();

    struct evbuffer *body = 0;
    int response_code = ctx->req->response_code;

    if (ctx->ok) {
        body = ctx->buffer;
        ctx->buffer = 0;
        context_free(ctx);
        struct evbuffer *header = evbuffer_new();
        return http_response_new(200, header, body);
    } else {
        logging(LOG_INFO, "http_request error %d : %s ", response_code, url);
        return NULL;
    }
}

struct http_response *http_get(const char *url, struct evkeyvalq *headers)
{
    return http_request(url, EVHTTP_REQ_GET, headers, NULL);
}

struct http_response *http_post(const char *url, struct evkeyvalq *headers,
                                struct evbuffer *postdata)
{
    return http_request(url, EVHTTP_REQ_POST, headers, postdata);
}

http_response *http_response_new(int status_code, struct evbuffer * headers,
                                 struct evbuffer * body)
{
    http_response *r = (http_response *) malloc(sizeof(http_response));
    r->status_code = status_code;
    r->headers = headers;
    r->body = body;
    return r;
};

void http_response_free(http_response * r)
{
    if (r->headers)
        evbuffer_free(r->headers);
    if (r->body)
        evbuffer_free(r->body);
    free(r);
}

void http_client_init()
{
    conn_pool = connection_pool_new();
}
