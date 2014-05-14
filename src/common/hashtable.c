#include "hashtable.h"
#include "dlist.h"

struct _HashNode {
    void *key;
    void *value;
    struct dlist_t dlist;
};

struct _Hashtable {
    int bucket_cnt;
    int size;
    struct _HashNode **buckets;
    HashFunc hash_func;
    EqualFunc key_equal_func;
};

static struct _HashNode *hash_node_new()
{
    struct _HashNode *n = (struct _HashNode *)malloc(sizeof(struct _HashNode));
    memset(n, 0, sizeof(struct _HashNode));
    return n;
}

Hashtable *hashtable_new(HashFunc hash_func, EqualFunc hash_equal_func,
                         int bucket_cnt)
{
    Hashtable *ht = (Hashtable *) malloc(sizeof(Hashtable));
    ht->bucket_cnt = bucket_cnt;
    ht->size = 0;
    ht->buckets = calloc(sizeof(void *), bucket_cnt);
    ht->hash_func = hash_func;
    ht->key_equal_func = hash_equal_func;
    return ht;
}

void hashtable_free(Hashtable * ht)
{
    //FIXME  this is error
    if (ht->buckets) {
        free(ht->buckets);
        ht->buckets = NULL;
    }
    if (ht) {
        free(ht);
    }
}

void hashtable_insert(Hashtable * ht, void *k, void *v)
{
    int pos = ht->hash_func(k) % ht->bucket_cnt;

    if (NULL == ht->buckets[pos]) {
        ht->buckets[pos] = hash_node_new();
        dlist_t *pl = &(ht->buckets[pos]->dlist);
        dlist_init(pl);
    }
    struct _HashNode *bucket = ht->buckets[pos];
    struct _HashNode *n = hash_node_new();
    n->key = k;
    n->value = v;

    dlist_t *head = &(bucket->dlist);
    dlist_t *pl = &(n->dlist);
    dlist_insert_head(head, pl);
    return;
}

void hashtable_replace(Hashtable * ht, void *k, void *v)
{
    assert(0);

}

void hashtable_remove(Hashtable * ht, void *k)
{
    int pos = ht->hash_func(k) % ht->bucket_cnt;
    struct _HashNode *bucket = ht->buckets[pos];

    if (bucket == NULL)
        return;

    struct _HashNode *p;
    dlist_t *head = &(bucket->dlist);
    dlist_t *pl;
    for (pl = head->next; pl != head; pl = pl->next) {
        p = dlist_data(pl, struct _HashNode, dlist);

        if (ht->key_equal_func(k, p->key)) {
            dlist_remove(pl);
            free(p);
        }

    }
    return;

}

void *hashtable_lookup(Hashtable * ht, void *k)
{
    int pos = ht->hash_func(k) % ht->bucket_cnt;
    struct _HashNode *bucket = ht->buckets[pos];

    if (bucket == NULL)
        return NULL;

    struct _HashNode *p;
    dlist_t *head = &(bucket->dlist);
    dlist_t *pl;
    for (pl = head->next; pl != head; pl = pl->next) {
        p = dlist_data(pl, struct _HashNode, dlist);

        if (ht->key_equal_func(k, p->key))
            return p->value;
    }
    return NULL;
}

void hashtable_foreach(Hashtable * ht, ForeachFunc func, void *user_data)
{

    struct _HashNode *p;
    struct _HashNode *bucket;
    int i;
    for (i = 0; i < ht->bucket_cnt; i++) {
        bucket = ht->buckets[i];
        if (bucket == NULL)
            continue;
        dlist_t *head = &(bucket->dlist);
        dlist_t *pl;
        for (pl = head->next; pl != head; pl = pl->next) {
            p = dlist_data(pl, struct _HashNode, dlist);
            func(p->key, p->value, user_data);
        }
    }
}

int hashtable_size(Hashtable * ht)
{
    return ht->size;
}

int hash_int_equal_func(void *v1, void *v2)
{
    return *((const int *)v1) == *((const int *)v2);
}

uint32_t hash_int_hash_func_func(const void *v)
{
    return *(const int *)v;
}

void hash_int_str_foreach(void *key, void *value, void *user_data)
{
    printf("%d => %s\n", *(int *)key, (char *)value);

}

//////////////////////////////////////////////////////////////////////////////////////////////

int equal_int64(void *v1, void *v2)
{
    return *((const int64_t *)v1) == *((const int64_t *)v2);
}

uint32_t hash_int64(const void *v)
{
    uint64_t key = *((const int64_t *)v);
    key = (~key) + (key << 18); // key = (key << 18) - key - 1;
    key = key ^ (key >> 31);
    key = key * 21;             // key = (key + (key << 2)) + (key << 4);
    key = key ^ (key >> 11);
    key = key + (key << 6);
    key = key ^ (key >> 22);
    return (int)key;
}

void foreach_int64_str(void *key, void *value, void *user_data)
{
    printf("%" PRIi64 " => %s\n", *(int64_t *) key, (char *)value);

}

//////////////////////////////////////////////////////////////////////////////////////////////

void hash_str_str_foreach(void *key, void *value, void *user_data)
{
    printf("%s => %s\n", (char *)key, (char *)value);

}

int hash_str_equal_func(void *v1, void *v2)
{
    return 0 == strcmp(((char *)v1), ((char *)v2));
}

uint32_t hash_str_hash_func(const void *v)
{
    char *p = (char *)v;
    int i = 1;
    for (i = strlen(p) - 1; i >= 0; i--) {
        i = i * 7 + 31;
    }
    return i;
}
