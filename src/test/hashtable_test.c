#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include "network.h"
#include "hashtable.h"
#include "log.h"

int test1()
{
    Hashtable *ht =
        hashtable_new(hash_int_hash_func_func, hash_int_equal_func, 2);
    int arr[10];
    int k;
    char v[20];
    for (k = 0; k < 10; k++) {
        arr[k] = k;
        sprintf(v, "v%d", k);
        hashtable_insert(ht, arr + k, strdup(v));   //!!!!!!!!!!!!!!! I Should take care of Memory myself 
    }

    hashtable_foreach(ht, hash_int_str_foreach, NULL);

    for (k = 0; k < 10; k++) {
        char *p = hashtable_lookup(ht, &k);
        printf("%s\n", p);
        sprintf(v, "v%d", k);
        assert(0 == strcmp(p, v));
    }
    k = 3;
    hashtable_remove(ht, &k);
    char *p = hashtable_lookup(ht, &k);
    printf("%p\n", p);
    assert(NULL == p);

    return 0;
}

int test2()
{
    Hashtable *ht = hashtable_new(hash_str_hash_func, hash_str_equal_func, 2);
    char v[10];
    int k;
    for (k = 0; k < 10; k++) {
        sprintf(v, "v%d", k);
        hashtable_insert(ht, strdup(v), strdup(v)); //!!!!!!!!!!!!!!! I Should take care of Memory myself 
    }

    hashtable_foreach(ht, hash_str_str_foreach, NULL);

    for (k = 0; k < 10; k++) {
        sprintf(v, "v%d", k);

        char *p = hashtable_lookup(ht, v);
        printf("%s\n", p);
        assert(0 == strcmp(p, v));
    }
    hashtable_remove(ht, "v3");
    char *p = hashtable_lookup(ht, "v3");
    printf("%p\n", p);
    assert(NULL == p);

    return 0;
}

int main()
{
    test1();
    test2();
    return 0;
}
