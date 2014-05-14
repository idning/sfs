/*
   Copyright 2005-2010 Jakub Kruszona-Zawadzki, Gemius SA.

   This file is part of MooseFS.

   MooseFS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, version 3.

   MooseFS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MooseFS.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _DATAPACK_H_
#define _DATAPACK_H_

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* MFS data pack */

static inline void put64bit(uint8_t ** ptr, uint64_t val)
{
    (*ptr)[0] = ((val) >> 56) & 0xFF;
    (*ptr)[1] = ((val) >> 48) & 0xFF;
    (*ptr)[2] = ((val) >> 40) & 0xFF;
    (*ptr)[3] = ((val) >> 32) & 0xFF;
    (*ptr)[4] = ((val) >> 24) & 0xFF;
    (*ptr)[5] = ((val) >> 16) & 0xFF;
    (*ptr)[6] = ((val) >> 8) & 0xFF;
    (*ptr)[7] = (val) & 0xFF;
    (*ptr) += 8;
}

static inline void put32bit(uint8_t ** ptr, uint32_t val)
{
    printf("put32bit: %p : %d\n", *ptr, val);
    (*ptr) += sprintf((char *)*ptr, "%u,", val);    //由于 msglength 要后来写入，所以这里要定长.
}

static inline void put32bit_width(uint8_t ** ptr, uint32_t val, uint32_t w)
{
    printf("put32bit_width: %p : %d\n", *ptr, val);
    char s[100];
    sprintf(s, "%*u,", w, val); //由于 msglength 要后来写入，所以这里要定长.
    memcpy(*ptr, s, w);
}

//static inline void put16bit(uint8_t **ptr,uint16_t val) {
    //(*ptr)[0]=((val)>>8)&0xFF;
    //(*ptr)[1]=(val)&0xFF;
    //(*ptr)+=2;
//}

//static inline void put8bit(uint8_t **ptr,uint8_t val) {
    //(*ptr)[0]=(val)&0xFF;
    //(*ptr)++;
//}

static inline uint64_t get64bit(const uint8_t ** ptr)
{
    uint64_t t64;
    t64 = ((*ptr)[3] + 256 * ((*ptr)[2] + 256 * ((*ptr)[1] + 256 * (*ptr)[0])));
    t64 <<= 32;
    t64 |=
        (((*ptr)[7] +
          256 * ((*ptr)[6] +
                 256 * ((*ptr)[5] + 256 * (*ptr)[4])))) & 0xffffffffU;
    (*ptr) += 8;
    return t64;
}

static inline uint32_t get32bit(const uint8_t ** ptr)
{
    uint32_t t32;
    sscanf((char *)*ptr, "%u,", &t32);
    fprintf(stderr, "get32bit: %s = %d\n", *ptr, t32);
    (*ptr) = (uint8_t *) strchr((char *)*ptr, ',') + 1;
    return t32;
}

//static inline uint16_t get16bit(const uint8_t **ptr) {
    //uint32_t t16;
    //t16=(*ptr)[1]+256*(*ptr)[0];
    //(*ptr)+=2;
    //return t16;
//}

//static inline uint8_t get8bit(const uint8_t **ptr) {
    //uint32_t t8;
    //t8=(*ptr)[0];
    //(*ptr)++;
    //return t8;
//}

static inline void putstr(uint8_t ** ptr, uint32_t strlen, uint8_t * str)
{
    memcpy(*ptr, str, strlen);
    (*ptr) += strlen;
    (**ptr) = ',';
    (*ptr) += 1;
}

//TODO: ptr should be const uint8_t ** ptr;
static inline uint8_t *getstr(const uint8_t ** ptr, uint32_t strlen)
{
    uint8_t *s = malloc(strlen + 1);
    memcpy(s, *ptr, strlen);
    s[strlen] = '\0';
    (*ptr) += (strlen + 1);
    return s;
}

#ifndef _EVENT_HAVE_STRSEP
/* strsep replacement for platforms that lack it.  Only works if
 * del is one character long. */
static inline char *strsep2(char **s, const char *del)
{
    char *d, *tok;
    if (!s || !*s)
        return NULL;
    tok = *s;
    d = strstr(tok, del);
    if (d) {
        *d = '\0';
        *s = d + 1;
    } else
        *s = NULL;
    return tok;
}
#endif

#endif
