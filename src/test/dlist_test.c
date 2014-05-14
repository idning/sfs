#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include "network.h"
#include "dlist.h"
#include "log.h"

typedef struct dlist_entry {
    char *str;
    struct dlist_entry *next;
    struct dlist_entry *prev;
} dlist_entry;

dlist_entry head_v;
dlist_entry *head = &head_v;

int main()
{
    int i;
    dlist_entry *p;
    dlist_entry entries[10];

    dlist_init(head);
    printf("head: %p; head->next->%p ; head->next->next: %p", head, head->next,
           head->next->next);

    for (i = 0; i < 10; i++) {
        entries[i].str = (char *)malloc(100);
        sprintf(entries[i].str, "entry %d", i);
        dlist_insert_tail(head, entries + i);
    }

    for (i = 0, p = head->next; p != head; p = p->next, i++) {
        assert(p == entries + i);
        /*printf("print : %s\n", p->str); */
    }
    return 0;
}
