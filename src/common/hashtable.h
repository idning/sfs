#ifndef _HASH_H
#define _HASH_H

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef int (*EqualFunc) (void *a, void *b);
typedef uint32_t(*HashFunc) (const void *key);
typedef void (*ForeachFunc) (void *key, void *value, void *user_data);

typedef struct _Hashtable Hashtable;

Hashtable *hashtable_new(HashFunc hash_func, EqualFunc hash_equal_func,
                         int bucket_cnt);
void hashtable_free(Hashtable * ht);

void hashtable_insert(Hashtable * ht, void *k, void *v);
void hashtable_replace(Hashtable * ht, void *k, void *v);
void hashtable_remove(Hashtable * ht, void *k);
void *hashtable_lookup(Hashtable * ht, void *k);
void hashtable_foreach(Hashtable * ht, ForeachFunc func, void *user_data);
int hashtable_size(Hashtable * ht);

int hash_int_equal_func(void *v1, void *v2);
uint32_t hash_int_hash_func_func(const void *v);
void hash_int_str_foreach(void *key, void *value, void *user_data);
///////////////////////////////////////////////////////////

int equal_int64(void *v1, void *v2);
uint32_t hash_int64(const void *v);
void foreach_int64_str(void *key, void *value, void *user_data);

/////////////////////////////////////////////////////////
int hash_str_equal_func(void *v1, void *v2);
uint32_t hash_str_hash_func(const void *v);
void hash_str_str_foreach(void *key, void *value, void *user_data);

#endif /* _HASH_H */
