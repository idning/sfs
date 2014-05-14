#include <event.h>
#include <evhttp.h>
#include "protocol.gen.h"

#ifndef _PROTOCOL_H__
#define _PROTOCOL_H__

void file_stat_init(struct file_stat *stat);
void log_file_stat(char *hint, struct file_stat *t);

EVRPC_HEADER(rpc_ping, ping, pong);
    EVRPC_HEADER(rpc_stat, stat_request, stat_response);
    EVRPC_HEADER(rpc_ls, ls_request, ls_response);
    EVRPC_HEADER(rpc_mknod, mknod_request, mknod_response);
    EVRPC_HEADER(rpc_unlink, unlink_request, unlink_response);
    EVRPC_HEADER(rpc_statfs, statfs_request, statfs_response);
    EVRPC_HEADER(rpc_lookup, lookup_request, lookup_response);
    EVRPC_HEADER(rpc_setattr, setattr_request, setattr_response);
    EVRPC_HEADER(rpc_mkfs, mkfs_request, mkfs_response);

    EVRPC_HEADER(rpc_symlink, symlink_request, symlink_response)
    EVRPC_HEADER(rpc_readlink, readlink_request, readlink_response);
    EVRPC_HEADER(rpc_uuid, uuid_request, uuid_response);
    EVRPC_HEADER(rpc_migrate, migrate_request, migrate_response);




void file_stat_marshal(struct evbuffer *evbuf, const struct file_stat *tmp);

int file_stat_unmarshal(struct file_stat *tmp,  struct evbuffer *evbuf);


int evtag_unmarshal_file_stat(struct evbuffer *evbuf, ev_uint32_t need_tag, struct file_stat *msg);

void evtag_marshal_file_stat(struct evbuffer *evbuf, ev_uint32_t tag, const struct file_stat *msg);

#define RST_CODE_NOT_FOUND 1

#endif
