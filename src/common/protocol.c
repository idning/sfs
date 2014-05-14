#include<protocol.h>
#include<protocol.gen.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "log.h"

EVRPC_GENERATE(rpc_ping, ping, pong);
    EVRPC_GENERATE(rpc_stat, stat_request, stat_response);
    EVRPC_GENERATE(rpc_ls, ls_request, ls_response);
    EVRPC_GENERATE(rpc_mknod, mknod_request, mknod_response);
    EVRPC_GENERATE(rpc_unlink, unlink_request, unlink_response);
    EVRPC_GENERATE(rpc_statfs, statfs_request, statfs_response);
    EVRPC_GENERATE(rpc_lookup, lookup_request, lookup_response);
    EVRPC_GENERATE(rpc_setattr, setattr_request, setattr_response);
    EVRPC_GENERATE(rpc_mkfs, mkfs_request, mkfs_response);
    EVRPC_GENERATE(rpc_symlink, symlink_request, symlink_response);
    EVRPC_GENERATE(rpc_readlink, readlink_request, readlink_response);
    EVRPC_GENERATE(rpc_uuid, uuid_request, uuid_response);
    EVRPC_GENERATE(rpc_migrate, migrate_request, migrate_response);
/*EVRPC_GENERATE(rpc_, _request, _response)*/
/*EVRPC_GENERATE(rpc_, _request, _response)*/
/**
 * this should be in rpc_gen.py 
 * but I have no time to do this
 */
void file_stat_init(struct file_stat *stat)
{
    struct file_stat *null_stat = file_stat_new();
    memcpy(stat, null_stat, sizeof(struct file_stat));
    file_stat_free(null_stat);

    /*stat->base = &__file_stat_base; */
}

void log_file_stat(char *hint, struct file_stat *t)
{

    logging(LOG_DEUBG, "%d", 1);
    if (t->pos_arr) {
        logging(LOG_DEUBG,
                "%s  ::::  {ino: %" PRIu64 ", parent: %" PRIu64 ", size: %"
                PRIu64 ", type : %d, mode : %04o, pos [%d, %d], name : %s}", hint, t->ino,
                t->parent_ino, t->size, t->type, t->mode, t->pos_arr[0],
                t->pos_arr[1], t->name);
    } else {
        logging(LOG_DEUBG,
                "%s  ::::  {ino: %" PRIu64 ", size: %" PRIu64
                ", type : %d, mode : %04o }", hint, t->ino, t->size,
                t->type, t->mode);
    }

    /*logging(LOG_DEUBG, */
    /*"%s {ino: %" PRIu64 ", name: %s, size: %" PRIu64 */
    /*", mode: %04o}", hint, t->ino, t->name, t->size, t->mode); */
}





