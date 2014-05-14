#include "sfs_common.h"

static Hashtable *attr_ht;

int attr_cache_init()
{
    attr_ht = hashtable_new(hash_int64, equal_int64, 1024);
    return 0;
}

struct file_stat *attr_cache_lookup(uint64_t ino)
{
    struct file_stat *st = hashtable_lookup(attr_ht, &ino);
    return st;
}

int attr_cache_add(struct file_stat *st)
{
    assert(st->ino != 0);
    struct file_stat *old = attr_cache_lookup(st->ino);
    if (old){
        /*if (st->parent_ino == FS_VROOT_INO) //split 后的*/
            

        old->size = st->size;
        old->mode = st->mode;
        old->pos_arr[0] = st ->pos_arr[0];
        old->pos_arr[1] = st ->pos_arr[1];
        old->name = strdup(st->name);
        old->version = cluster_get_current_version();
        log_file_stat("attr_cache update instead insert !", old);
    }else{
        assert(st->parent_ino != 0);
        st->version = cluster_get_current_version();
        hashtable_insert(attr_ht, &(st->ino), st);
        log_file_stat("attr_cache_ inserted: ", st);
    }


    return 0;
}

int attr_cache_del(uint64_t ino)
{
    assert(0);
}

