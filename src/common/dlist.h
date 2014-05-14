/*
 * Copyright (C) Igor Sysoev
 * modified by ning @ 2011-01-05
 */
#include <stddef.h>

#ifndef _NGX_QUEUE_H_INCLUDED_
#define _NGX_QUEUE_H_INCLUDED_

typedef struct dlist_t {
    struct dlist_t *prev;
    struct dlist_t *next;
} dlist_t;

#define dlist_init(q)                                                     \
    (q)->prev = q;                                                            \
    (q)->next = q

#define dlist_empty(h)                                                    \
    (h == (h)->prev)

#define dlist_insert_head(h, x)                                           \
    (x)->next = (h)->next;                                                    \
    (x)->next->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->next = x

#define dlist_insert_after   dlist_insert_head

#define dlist_insert_tail(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->next = x;                                                      \
    (x)->next = h;                                                            \
    (h)->prev = x

#define dlist_head(h)                                                     \
    (h)->next

#define dlist_last(h)                                                     \
    (h)->prev

#define dlist_sentinel(h)                                                 \
    (h)

#define dlist_next(q)                                                     \
    (q)->next

#define dlist_prev(q)                                                     \
    (q)->prev

#if (NGX_DEBUG)

#define dlist_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next;                                              \
    (x)->prev = NULL;                                                         \
    (x)->next = NULL

#else

#define dlist_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next

#endif

#define dlist_split(h, q, n)                                              \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->next = n;                                                      \
    (n)->next = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->next = h;                                                      \
    (q)->prev = n;

#define dlist_add(h, n)                                                   \
    (h)->prev->next = (n)->next;                                              \
    (n)->next->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->next = h;

#define dlist_data(q, type, link)                                         \
    (type *) ((unsigned char *) q - offsetof(type, link))

#endif /* _NGX_QUEUE_H_INCLUDED_ */
