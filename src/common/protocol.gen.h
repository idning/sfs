/*
 * Automatically generated from common/protocol.rpc
 */

#ifndef _COMMON_PROTOCOL_RPC_
#define _COMMON_PROTOCOL_RPC_

#include <event2/util.h> /* for ev_uint*_t */
#include <event2/rpc.h>

struct ping;
struct machine;
struct pong;
struct file_stat;
struct stat_request;
struct stat_response;
struct setattr_request;
struct setattr_response;
struct ls_request;
struct ls_response;
struct mknod_request;
struct mknod_response;
struct symlink_request;
struct symlink_response;
struct readlink_request;
struct readlink_response;
struct lookup_request;
struct lookup_response;
struct unlink_request;
struct unlink_response;
struct statfs_request;
struct statfs_response;
struct mkfs_request;
struct mkfs_response;
struct uuid_request;
struct uuid_response;
struct migrate_request;
struct migrate_response;

/* Tag definition for ping */
enum ping_ {
  PING_VERSION=1,
  PING_SELF_IP=2,
  PING_SELF_PORT=3,
  PING_SELF_TYPE=4,
  PING_MID=5,
  PING_LOAD=6,
  PING_MAX_TAGS
};

/* Structure declaration for ping */
struct ping_access_ {
  int (*version_assign)(struct ping *, const ev_uint32_t);
  int (*version_get)(struct ping *, ev_uint32_t *);
  int (*self_ip_assign)(struct ping *, const char *);
  int (*self_ip_get)(struct ping *, char * *);
  int (*self_port_assign)(struct ping *, const ev_uint32_t);
  int (*self_port_get)(struct ping *, ev_uint32_t *);
  int (*self_type_assign)(struct ping *, const ev_uint32_t);
  int (*self_type_get)(struct ping *, ev_uint32_t *);
  int (*mid_assign)(struct ping *, const ev_uint32_t);
  int (*mid_get)(struct ping *, ev_uint32_t *);
  int (*load_assign)(struct ping *, const ev_uint32_t);
  int (*load_get)(struct ping *, ev_uint32_t *);
};

struct ping {
  struct ping_access_ *base;

  ev_uint32_t version;
  char *self_ip;
  ev_uint32_t self_port;
  ev_uint32_t self_type;
  ev_uint32_t mid;
  ev_uint32_t load;

  ev_uint8_t version_set;
  ev_uint8_t self_ip_set;
  ev_uint8_t self_port_set;
  ev_uint8_t self_type_set;
  ev_uint8_t mid_set;
  ev_uint8_t load_set;
};

struct ping *ping_new(void);
struct ping *ping_new_with_arg(void *);
void ping_free(struct ping *);
void ping_clear(struct ping *);
void ping_marshal(struct evbuffer *, const struct ping *);
int ping_unmarshal(struct ping *, struct evbuffer *);
int ping_complete(struct ping *);
void evtag_marshal_ping(struct evbuffer *, ev_uint32_t,
    const struct ping *);
int evtag_unmarshal_ping(struct evbuffer *, ev_uint32_t,
    struct ping *);
int ping_version_assign(struct ping *, const ev_uint32_t);
int ping_version_get(struct ping *, ev_uint32_t *);
int ping_self_ip_assign(struct ping *, const char *);
int ping_self_ip_get(struct ping *, char * *);
int ping_self_port_assign(struct ping *, const ev_uint32_t);
int ping_self_port_get(struct ping *, ev_uint32_t *);
int ping_self_type_assign(struct ping *, const ev_uint32_t);
int ping_self_type_get(struct ping *, ev_uint32_t *);
int ping_mid_assign(struct ping *, const ev_uint32_t);
int ping_mid_get(struct ping *, ev_uint32_t *);
int ping_load_assign(struct ping *, const ev_uint32_t);
int ping_load_get(struct ping *, ev_uint32_t *);
/* --- ping done --- */

/* Tag definition for machine */
enum machine_ {
  MACHINE_MID=1,
  MACHINE_IP=2,
  MACHINE_PORT=3,
  MACHINE_TYPE=4,
  MACHINE_LOAD=6,
  MACHINE_MAX_TAGS
};

/* Structure declaration for machine */
struct machine_access_ {
  int (*mid_assign)(struct machine *, const ev_uint32_t);
  int (*mid_get)(struct machine *, ev_uint32_t *);
  int (*ip_assign)(struct machine *, const char *);
  int (*ip_get)(struct machine *, char * *);
  int (*port_assign)(struct machine *, const ev_uint32_t);
  int (*port_get)(struct machine *, ev_uint32_t *);
  int (*type_assign)(struct machine *, const ev_uint32_t);
  int (*type_get)(struct machine *, ev_uint32_t *);
  int (*load_assign)(struct machine *, const ev_uint32_t);
  int (*load_get)(struct machine *, ev_uint32_t *);
};

struct machine {
  struct machine_access_ *base;

  ev_uint32_t mid;
  char *ip;
  ev_uint32_t port;
  ev_uint32_t type;
  ev_uint32_t load;

  ev_uint8_t mid_set;
  ev_uint8_t ip_set;
  ev_uint8_t port_set;
  ev_uint8_t type_set;
  ev_uint8_t load_set;
};

struct machine *machine_new(void);
struct machine *machine_new_with_arg(void *);
void machine_free(struct machine *);
void machine_clear(struct machine *);
void machine_marshal(struct evbuffer *, const struct machine *);
int machine_unmarshal(struct machine *, struct evbuffer *);
int machine_complete(struct machine *);
void evtag_marshal_machine(struct evbuffer *, ev_uint32_t,
    const struct machine *);
int evtag_unmarshal_machine(struct evbuffer *, ev_uint32_t,
    struct machine *);
int machine_mid_assign(struct machine *, const ev_uint32_t);
int machine_mid_get(struct machine *, ev_uint32_t *);
int machine_ip_assign(struct machine *, const char *);
int machine_ip_get(struct machine *, char * *);
int machine_port_assign(struct machine *, const ev_uint32_t);
int machine_port_get(struct machine *, ev_uint32_t *);
int machine_type_assign(struct machine *, const ev_uint32_t);
int machine_type_get(struct machine *, ev_uint32_t *);
int machine_load_assign(struct machine *, const ev_uint32_t);
int machine_load_get(struct machine *, ev_uint32_t *);
/* --- machine done --- */

/* Tag definition for pong */
enum pong_ {
  PONG_VERSION=1,
  PONG_MID=2,
  PONG_MACHINES=3,
  PONG_MAX_TAGS
};

/* Structure declaration for pong */
struct pong_access_ {
  int (*version_assign)(struct pong *, const ev_uint32_t);
  int (*version_get)(struct pong *, ev_uint32_t *);
  int (*mid_assign)(struct pong *, const ev_uint32_t);
  int (*mid_get)(struct pong *, ev_uint32_t *);
  int (*machines_assign)(struct pong *, int, const struct machine*);
  int (*machines_get)(struct pong *, int, struct machine* *);
  struct machine*  (*machines_add)(struct pong *msg);
};

struct pong {
  struct pong_access_ *base;

  ev_uint32_t version;
  ev_uint32_t mid;
  struct machine* *machines;
  int machines_length;
  int machines_num_allocated;

  ev_uint8_t version_set;
  ev_uint8_t mid_set;
  ev_uint8_t machines_set;
};

struct pong *pong_new(void);
struct pong *pong_new_with_arg(void *);
void pong_free(struct pong *);
void pong_clear(struct pong *);
void pong_marshal(struct evbuffer *, const struct pong *);
int pong_unmarshal(struct pong *, struct evbuffer *);
int pong_complete(struct pong *);
void evtag_marshal_pong(struct evbuffer *, ev_uint32_t,
    const struct pong *);
int evtag_unmarshal_pong(struct evbuffer *, ev_uint32_t,
    struct pong *);
int pong_version_assign(struct pong *, const ev_uint32_t);
int pong_version_get(struct pong *, ev_uint32_t *);
int pong_mid_assign(struct pong *, const ev_uint32_t);
int pong_mid_get(struct pong *, ev_uint32_t *);
int pong_machines_assign(struct pong *, int, const struct machine*);
int pong_machines_get(struct pong *, int, struct machine* *);
struct machine*  pong_machines_add(struct pong *msg);
/* --- pong done --- */

/* Tag definition for file_stat */
enum file_stat_ {
  FILE_STAT_INO=1,
  FILE_STAT_SIZE=2,
  FILE_STAT_TYPE=3,
  FILE_STAT_NAME=4,
  FILE_STAT_MODE=5,
  FILE_STAT_NLINK=6,
  FILE_STAT_ATIME=7,
  FILE_STAT_MTIME=8,
  FILE_STAT_CTIME=9,
  FILE_STAT_POS_ARR=10,
  FILE_STAT_PARENT_INO=11,
  FILE_STAT_VERSION=12,
  FILE_STAT_MAX_TAGS
};

/* Structure declaration for file_stat */
struct file_stat_access_ {
  int (*ino_assign)(struct file_stat *, const ev_uint64_t);
  int (*ino_get)(struct file_stat *, ev_uint64_t *);
  int (*size_assign)(struct file_stat *, const ev_uint64_t);
  int (*size_get)(struct file_stat *, ev_uint64_t *);
  int (*type_assign)(struct file_stat *, const ev_uint32_t);
  int (*type_get)(struct file_stat *, ev_uint32_t *);
  int (*name_assign)(struct file_stat *, const char *);
  int (*name_get)(struct file_stat *, char * *);
  int (*mode_assign)(struct file_stat *, const ev_uint32_t);
  int (*mode_get)(struct file_stat *, ev_uint32_t *);
  int (*nlink_assign)(struct file_stat *, const ev_uint32_t);
  int (*nlink_get)(struct file_stat *, ev_uint32_t *);
  int (*atime_assign)(struct file_stat *, const ev_uint32_t);
  int (*atime_get)(struct file_stat *, ev_uint32_t *);
  int (*mtime_assign)(struct file_stat *, const ev_uint32_t);
  int (*mtime_get)(struct file_stat *, ev_uint32_t *);
  int (*ctime_assign)(struct file_stat *, const ev_uint32_t);
  int (*ctime_get)(struct file_stat *, ev_uint32_t *);
  int (*pos_arr_assign)(struct file_stat *, int, const ev_uint32_t);
  int (*pos_arr_get)(struct file_stat *, int, ev_uint32_t *);
  ev_uint32_t * (*pos_arr_add)(struct file_stat *msg, const ev_uint32_t value);
  int (*parent_ino_assign)(struct file_stat *, const ev_uint64_t);
  int (*parent_ino_get)(struct file_stat *, ev_uint64_t *);
  int (*version_assign)(struct file_stat *, const ev_uint32_t);
  int (*version_get)(struct file_stat *, ev_uint32_t *);
};

struct file_stat {
  struct file_stat_access_ *base;

  ev_uint64_t ino;
  ev_uint64_t size;
  ev_uint32_t type;
  char *name;
  ev_uint32_t mode;
  ev_uint32_t nlink;
  ev_uint32_t atime;
  ev_uint32_t mtime;
  ev_uint32_t ctime;
  ev_uint32_t *pos_arr;
  int pos_arr_length;
  int pos_arr_num_allocated;
  ev_uint64_t parent_ino;
  ev_uint32_t version;

  ev_uint8_t ino_set;
  ev_uint8_t size_set;
  ev_uint8_t type_set;
  ev_uint8_t name_set;
  ev_uint8_t mode_set;
  ev_uint8_t nlink_set;
  ev_uint8_t atime_set;
  ev_uint8_t mtime_set;
  ev_uint8_t ctime_set;
  ev_uint8_t pos_arr_set;
  ev_uint8_t parent_ino_set;
  ev_uint8_t version_set;
};

struct file_stat *file_stat_new(void);
struct file_stat *file_stat_new_with_arg(void *);
void file_stat_free(struct file_stat *);
void file_stat_clear(struct file_stat *);
void file_stat_marshal(struct evbuffer *, const struct file_stat *);
int file_stat_unmarshal(struct file_stat *, struct evbuffer *);
int file_stat_complete(struct file_stat *);
void evtag_marshal_file_stat(struct evbuffer *, ev_uint32_t,
    const struct file_stat *);
int evtag_unmarshal_file_stat(struct evbuffer *, ev_uint32_t,
    struct file_stat *);
int file_stat_ino_assign(struct file_stat *, const ev_uint64_t);
int file_stat_ino_get(struct file_stat *, ev_uint64_t *);
int file_stat_size_assign(struct file_stat *, const ev_uint64_t);
int file_stat_size_get(struct file_stat *, ev_uint64_t *);
int file_stat_type_assign(struct file_stat *, const ev_uint32_t);
int file_stat_type_get(struct file_stat *, ev_uint32_t *);
int file_stat_name_assign(struct file_stat *, const char *);
int file_stat_name_get(struct file_stat *, char * *);
int file_stat_mode_assign(struct file_stat *, const ev_uint32_t);
int file_stat_mode_get(struct file_stat *, ev_uint32_t *);
int file_stat_nlink_assign(struct file_stat *, const ev_uint32_t);
int file_stat_nlink_get(struct file_stat *, ev_uint32_t *);
int file_stat_atime_assign(struct file_stat *, const ev_uint32_t);
int file_stat_atime_get(struct file_stat *, ev_uint32_t *);
int file_stat_mtime_assign(struct file_stat *, const ev_uint32_t);
int file_stat_mtime_get(struct file_stat *, ev_uint32_t *);
int file_stat_ctime_assign(struct file_stat *, const ev_uint32_t);
int file_stat_ctime_get(struct file_stat *, ev_uint32_t *);
int file_stat_pos_arr_assign(struct file_stat *, int, const ev_uint32_t);
int file_stat_pos_arr_get(struct file_stat *, int, ev_uint32_t *);
ev_uint32_t * file_stat_pos_arr_add(struct file_stat *msg, const ev_uint32_t value);
int file_stat_parent_ino_assign(struct file_stat *, const ev_uint64_t);
int file_stat_parent_ino_get(struct file_stat *, ev_uint64_t *);
int file_stat_version_assign(struct file_stat *, const ev_uint32_t);
int file_stat_version_get(struct file_stat *, ev_uint32_t *);
/* --- file_stat done --- */

/* Tag definition for stat_request */
enum stat_request_ {
  STAT_REQUEST_INO_ARR=1,
  STAT_REQUEST_MAX_TAGS
};

/* Structure declaration for stat_request */
struct stat_request_access_ {
  int (*ino_arr_assign)(struct stat_request *, int, const ev_uint64_t);
  int (*ino_arr_get)(struct stat_request *, int, ev_uint64_t *);
  ev_uint64_t * (*ino_arr_add)(struct stat_request *msg, const ev_uint64_t value);
};

struct stat_request {
  struct stat_request_access_ *base;

  ev_uint64_t *ino_arr;
  int ino_arr_length;
  int ino_arr_num_allocated;

  ev_uint8_t ino_arr_set;
};

struct stat_request *stat_request_new(void);
struct stat_request *stat_request_new_with_arg(void *);
void stat_request_free(struct stat_request *);
void stat_request_clear(struct stat_request *);
void stat_request_marshal(struct evbuffer *, const struct stat_request *);
int stat_request_unmarshal(struct stat_request *, struct evbuffer *);
int stat_request_complete(struct stat_request *);
void evtag_marshal_stat_request(struct evbuffer *, ev_uint32_t,
    const struct stat_request *);
int evtag_unmarshal_stat_request(struct evbuffer *, ev_uint32_t,
    struct stat_request *);
int stat_request_ino_arr_assign(struct stat_request *, int, const ev_uint64_t);
int stat_request_ino_arr_get(struct stat_request *, int, ev_uint64_t *);
ev_uint64_t * stat_request_ino_arr_add(struct stat_request *msg, const ev_uint64_t value);
/* --- stat_request done --- */

/* Tag definition for stat_response */
enum stat_response_ {
  STAT_RESPONSE_RST_CODE=1,
  STAT_RESPONSE_STAT_ARR=2,
  STAT_RESPONSE_MAX_TAGS
};

/* Structure declaration for stat_response */
struct stat_response_access_ {
  int (*rst_code_assign)(struct stat_response *, const ev_uint32_t);
  int (*rst_code_get)(struct stat_response *, ev_uint32_t *);
  int (*stat_arr_assign)(struct stat_response *, int, const struct file_stat*);
  int (*stat_arr_get)(struct stat_response *, int, struct file_stat* *);
  struct file_stat*  (*stat_arr_add)(struct stat_response *msg);
};

struct stat_response {
  struct stat_response_access_ *base;

  ev_uint32_t rst_code;
  struct file_stat* *stat_arr;
  int stat_arr_length;
  int stat_arr_num_allocated;

  ev_uint8_t rst_code_set;
  ev_uint8_t stat_arr_set;
};

struct stat_response *stat_response_new(void);
struct stat_response *stat_response_new_with_arg(void *);
void stat_response_free(struct stat_response *);
void stat_response_clear(struct stat_response *);
void stat_response_marshal(struct evbuffer *, const struct stat_response *);
int stat_response_unmarshal(struct stat_response *, struct evbuffer *);
int stat_response_complete(struct stat_response *);
void evtag_marshal_stat_response(struct evbuffer *, ev_uint32_t,
    const struct stat_response *);
int evtag_unmarshal_stat_response(struct evbuffer *, ev_uint32_t,
    struct stat_response *);
int stat_response_rst_code_assign(struct stat_response *, const ev_uint32_t);
int stat_response_rst_code_get(struct stat_response *, ev_uint32_t *);
int stat_response_stat_arr_assign(struct stat_response *, int, const struct file_stat*);
int stat_response_stat_arr_get(struct stat_response *, int, struct file_stat* *);
struct file_stat*  stat_response_stat_arr_add(struct stat_response *msg);
/* --- stat_response done --- */

/* Tag definition for setattr_request */
enum setattr_request_ {
  SETATTR_REQUEST_STAT_ARR=1,
  SETATTR_REQUEST_MAX_TAGS
};

/* Structure declaration for setattr_request */
struct setattr_request_access_ {
  int (*stat_arr_assign)(struct setattr_request *, int, const struct file_stat*);
  int (*stat_arr_get)(struct setattr_request *, int, struct file_stat* *);
  struct file_stat*  (*stat_arr_add)(struct setattr_request *msg);
};

struct setattr_request {
  struct setattr_request_access_ *base;

  struct file_stat* *stat_arr;
  int stat_arr_length;
  int stat_arr_num_allocated;

  ev_uint8_t stat_arr_set;
};

struct setattr_request *setattr_request_new(void);
struct setattr_request *setattr_request_new_with_arg(void *);
void setattr_request_free(struct setattr_request *);
void setattr_request_clear(struct setattr_request *);
void setattr_request_marshal(struct evbuffer *, const struct setattr_request *);
int setattr_request_unmarshal(struct setattr_request *, struct evbuffer *);
int setattr_request_complete(struct setattr_request *);
void evtag_marshal_setattr_request(struct evbuffer *, ev_uint32_t,
    const struct setattr_request *);
int evtag_unmarshal_setattr_request(struct evbuffer *, ev_uint32_t,
    struct setattr_request *);
int setattr_request_stat_arr_assign(struct setattr_request *, int, const struct file_stat*);
int setattr_request_stat_arr_get(struct setattr_request *, int, struct file_stat* *);
struct file_stat*  setattr_request_stat_arr_add(struct setattr_request *msg);
/* --- setattr_request done --- */

/* Tag definition for setattr_response */
enum setattr_response_ {
  SETATTR_RESPONSE_RST_CODE=1,
  SETATTR_RESPONSE_STAT_ARR=2,
  SETATTR_RESPONSE_MAX_TAGS
};

/* Structure declaration for setattr_response */
struct setattr_response_access_ {
  int (*rst_code_assign)(struct setattr_response *, const ev_uint32_t);
  int (*rst_code_get)(struct setattr_response *, ev_uint32_t *);
  int (*stat_arr_assign)(struct setattr_response *, int, const struct file_stat*);
  int (*stat_arr_get)(struct setattr_response *, int, struct file_stat* *);
  struct file_stat*  (*stat_arr_add)(struct setattr_response *msg);
};

struct setattr_response {
  struct setattr_response_access_ *base;

  ev_uint32_t rst_code;
  struct file_stat* *stat_arr;
  int stat_arr_length;
  int stat_arr_num_allocated;

  ev_uint8_t rst_code_set;
  ev_uint8_t stat_arr_set;
};

struct setattr_response *setattr_response_new(void);
struct setattr_response *setattr_response_new_with_arg(void *);
void setattr_response_free(struct setattr_response *);
void setattr_response_clear(struct setattr_response *);
void setattr_response_marshal(struct evbuffer *, const struct setattr_response *);
int setattr_response_unmarshal(struct setattr_response *, struct evbuffer *);
int setattr_response_complete(struct setattr_response *);
void evtag_marshal_setattr_response(struct evbuffer *, ev_uint32_t,
    const struct setattr_response *);
int evtag_unmarshal_setattr_response(struct evbuffer *, ev_uint32_t,
    struct setattr_response *);
int setattr_response_rst_code_assign(struct setattr_response *, const ev_uint32_t);
int setattr_response_rst_code_get(struct setattr_response *, ev_uint32_t *);
int setattr_response_stat_arr_assign(struct setattr_response *, int, const struct file_stat*);
int setattr_response_stat_arr_get(struct setattr_response *, int, struct file_stat* *);
struct file_stat*  setattr_response_stat_arr_add(struct setattr_response *msg);
/* --- setattr_response done --- */

/* Tag definition for ls_request */
enum ls_request_ {
  LS_REQUEST_INO_ARR=1,
  LS_REQUEST_MAX_TAGS
};

/* Structure declaration for ls_request */
struct ls_request_access_ {
  int (*ino_arr_assign)(struct ls_request *, int, const ev_uint64_t);
  int (*ino_arr_get)(struct ls_request *, int, ev_uint64_t *);
  ev_uint64_t * (*ino_arr_add)(struct ls_request *msg, const ev_uint64_t value);
};

struct ls_request {
  struct ls_request_access_ *base;

  ev_uint64_t *ino_arr;
  int ino_arr_length;
  int ino_arr_num_allocated;

  ev_uint8_t ino_arr_set;
};

struct ls_request *ls_request_new(void);
struct ls_request *ls_request_new_with_arg(void *);
void ls_request_free(struct ls_request *);
void ls_request_clear(struct ls_request *);
void ls_request_marshal(struct evbuffer *, const struct ls_request *);
int ls_request_unmarshal(struct ls_request *, struct evbuffer *);
int ls_request_complete(struct ls_request *);
void evtag_marshal_ls_request(struct evbuffer *, ev_uint32_t,
    const struct ls_request *);
int evtag_unmarshal_ls_request(struct evbuffer *, ev_uint32_t,
    struct ls_request *);
int ls_request_ino_arr_assign(struct ls_request *, int, const ev_uint64_t);
int ls_request_ino_arr_get(struct ls_request *, int, ev_uint64_t *);
ev_uint64_t * ls_request_ino_arr_add(struct ls_request *msg, const ev_uint64_t value);
/* --- ls_request done --- */

/* Tag definition for ls_response */
enum ls_response_ {
  LS_RESPONSE_RST_CODE=1,
  LS_RESPONSE_STAT_ARR=2,
  LS_RESPONSE_MAX_TAGS
};

/* Structure declaration for ls_response */
struct ls_response_access_ {
  int (*rst_code_assign)(struct ls_response *, const ev_uint32_t);
  int (*rst_code_get)(struct ls_response *, ev_uint32_t *);
  int (*stat_arr_assign)(struct ls_response *, int, const struct file_stat*);
  int (*stat_arr_get)(struct ls_response *, int, struct file_stat* *);
  struct file_stat*  (*stat_arr_add)(struct ls_response *msg);
};

struct ls_response {
  struct ls_response_access_ *base;

  ev_uint32_t rst_code;
  struct file_stat* *stat_arr;
  int stat_arr_length;
  int stat_arr_num_allocated;

  ev_uint8_t rst_code_set;
  ev_uint8_t stat_arr_set;
};

struct ls_response *ls_response_new(void);
struct ls_response *ls_response_new_with_arg(void *);
void ls_response_free(struct ls_response *);
void ls_response_clear(struct ls_response *);
void ls_response_marshal(struct evbuffer *, const struct ls_response *);
int ls_response_unmarshal(struct ls_response *, struct evbuffer *);
int ls_response_complete(struct ls_response *);
void evtag_marshal_ls_response(struct evbuffer *, ev_uint32_t,
    const struct ls_response *);
int evtag_unmarshal_ls_response(struct evbuffer *, ev_uint32_t,
    struct ls_response *);
int ls_response_rst_code_assign(struct ls_response *, const ev_uint32_t);
int ls_response_rst_code_get(struct ls_response *, ev_uint32_t *);
int ls_response_stat_arr_assign(struct ls_response *, int, const struct file_stat*);
int ls_response_stat_arr_get(struct ls_response *, int, struct file_stat* *);
struct file_stat*  ls_response_stat_arr_add(struct ls_response *msg);
/* --- ls_response done --- */

/* Tag definition for mknod_request */
enum mknod_request_ {
  MKNOD_REQUEST_PARENT_INO=1,
  MKNOD_REQUEST_INO=2,
  MKNOD_REQUEST_NAME=3,
  MKNOD_REQUEST_TYPE=4,
  MKNOD_REQUEST_MODE=5,
  MKNOD_REQUEST_MAX_TAGS
};

/* Structure declaration for mknod_request */
struct mknod_request_access_ {
  int (*parent_ino_assign)(struct mknod_request *, const ev_uint64_t);
  int (*parent_ino_get)(struct mknod_request *, ev_uint64_t *);
  int (*ino_assign)(struct mknod_request *, const ev_uint64_t);
  int (*ino_get)(struct mknod_request *, ev_uint64_t *);
  int (*name_assign)(struct mknod_request *, const char *);
  int (*name_get)(struct mknod_request *, char * *);
  int (*type_assign)(struct mknod_request *, const ev_uint32_t);
  int (*type_get)(struct mknod_request *, ev_uint32_t *);
  int (*mode_assign)(struct mknod_request *, const ev_uint32_t);
  int (*mode_get)(struct mknod_request *, ev_uint32_t *);
};

struct mknod_request {
  struct mknod_request_access_ *base;

  ev_uint64_t parent_ino;
  ev_uint64_t ino;
  char *name;
  ev_uint32_t type;
  ev_uint32_t mode;

  ev_uint8_t parent_ino_set;
  ev_uint8_t ino_set;
  ev_uint8_t name_set;
  ev_uint8_t type_set;
  ev_uint8_t mode_set;
};

struct mknod_request *mknod_request_new(void);
struct mknod_request *mknod_request_new_with_arg(void *);
void mknod_request_free(struct mknod_request *);
void mknod_request_clear(struct mknod_request *);
void mknod_request_marshal(struct evbuffer *, const struct mknod_request *);
int mknod_request_unmarshal(struct mknod_request *, struct evbuffer *);
int mknod_request_complete(struct mknod_request *);
void evtag_marshal_mknod_request(struct evbuffer *, ev_uint32_t,
    const struct mknod_request *);
int evtag_unmarshal_mknod_request(struct evbuffer *, ev_uint32_t,
    struct mknod_request *);
int mknod_request_parent_ino_assign(struct mknod_request *, const ev_uint64_t);
int mknod_request_parent_ino_get(struct mknod_request *, ev_uint64_t *);
int mknod_request_ino_assign(struct mknod_request *, const ev_uint64_t);
int mknod_request_ino_get(struct mknod_request *, ev_uint64_t *);
int mknod_request_name_assign(struct mknod_request *, const char *);
int mknod_request_name_get(struct mknod_request *, char * *);
int mknod_request_type_assign(struct mknod_request *, const ev_uint32_t);
int mknod_request_type_get(struct mknod_request *, ev_uint32_t *);
int mknod_request_mode_assign(struct mknod_request *, const ev_uint32_t);
int mknod_request_mode_get(struct mknod_request *, ev_uint32_t *);
/* --- mknod_request done --- */

/* Tag definition for mknod_response */
enum mknod_response_ {
  MKNOD_RESPONSE_RST_CODE=2,
  MKNOD_RESPONSE_STAT_ARR=1,
  MKNOD_RESPONSE_MAX_TAGS
};

/* Structure declaration for mknod_response */
struct mknod_response_access_ {
  int (*rst_code_assign)(struct mknod_response *, const ev_uint32_t);
  int (*rst_code_get)(struct mknod_response *, ev_uint32_t *);
  int (*stat_arr_assign)(struct mknod_response *, int, const struct file_stat*);
  int (*stat_arr_get)(struct mknod_response *, int, struct file_stat* *);
  struct file_stat*  (*stat_arr_add)(struct mknod_response *msg);
};

struct mknod_response {
  struct mknod_response_access_ *base;

  ev_uint32_t rst_code;
  struct file_stat* *stat_arr;
  int stat_arr_length;
  int stat_arr_num_allocated;

  ev_uint8_t rst_code_set;
  ev_uint8_t stat_arr_set;
};

struct mknod_response *mknod_response_new(void);
struct mknod_response *mknod_response_new_with_arg(void *);
void mknod_response_free(struct mknod_response *);
void mknod_response_clear(struct mknod_response *);
void mknod_response_marshal(struct evbuffer *, const struct mknod_response *);
int mknod_response_unmarshal(struct mknod_response *, struct evbuffer *);
int mknod_response_complete(struct mknod_response *);
void evtag_marshal_mknod_response(struct evbuffer *, ev_uint32_t,
    const struct mknod_response *);
int evtag_unmarshal_mknod_response(struct evbuffer *, ev_uint32_t,
    struct mknod_response *);
int mknod_response_rst_code_assign(struct mknod_response *, const ev_uint32_t);
int mknod_response_rst_code_get(struct mknod_response *, ev_uint32_t *);
int mknod_response_stat_arr_assign(struct mknod_response *, int, const struct file_stat*);
int mknod_response_stat_arr_get(struct mknod_response *, int, struct file_stat* *);
struct file_stat*  mknod_response_stat_arr_add(struct mknod_response *msg);
/* --- mknod_response done --- */

/* Tag definition for symlink_request */
enum symlink_request_ {
  SYMLINK_REQUEST_PARENT_INO=1,
  SYMLINK_REQUEST_INO=2,
  SYMLINK_REQUEST_NAME=3,
  SYMLINK_REQUEST_PATH=4,
  SYMLINK_REQUEST_MAX_TAGS
};

/* Structure declaration for symlink_request */
struct symlink_request_access_ {
  int (*parent_ino_assign)(struct symlink_request *, const ev_uint64_t);
  int (*parent_ino_get)(struct symlink_request *, ev_uint64_t *);
  int (*ino_assign)(struct symlink_request *, const ev_uint64_t);
  int (*ino_get)(struct symlink_request *, ev_uint64_t *);
  int (*name_assign)(struct symlink_request *, const char *);
  int (*name_get)(struct symlink_request *, char * *);
  int (*path_assign)(struct symlink_request *, const char *);
  int (*path_get)(struct symlink_request *, char * *);
};

struct symlink_request {
  struct symlink_request_access_ *base;

  ev_uint64_t parent_ino;
  ev_uint64_t ino;
  char *name;
  char *path;

  ev_uint8_t parent_ino_set;
  ev_uint8_t ino_set;
  ev_uint8_t name_set;
  ev_uint8_t path_set;
};

struct symlink_request *symlink_request_new(void);
struct symlink_request *symlink_request_new_with_arg(void *);
void symlink_request_free(struct symlink_request *);
void symlink_request_clear(struct symlink_request *);
void symlink_request_marshal(struct evbuffer *, const struct symlink_request *);
int symlink_request_unmarshal(struct symlink_request *, struct evbuffer *);
int symlink_request_complete(struct symlink_request *);
void evtag_marshal_symlink_request(struct evbuffer *, ev_uint32_t,
    const struct symlink_request *);
int evtag_unmarshal_symlink_request(struct evbuffer *, ev_uint32_t,
    struct symlink_request *);
int symlink_request_parent_ino_assign(struct symlink_request *, const ev_uint64_t);
int symlink_request_parent_ino_get(struct symlink_request *, ev_uint64_t *);
int symlink_request_ino_assign(struct symlink_request *, const ev_uint64_t);
int symlink_request_ino_get(struct symlink_request *, ev_uint64_t *);
int symlink_request_name_assign(struct symlink_request *, const char *);
int symlink_request_name_get(struct symlink_request *, char * *);
int symlink_request_path_assign(struct symlink_request *, const char *);
int symlink_request_path_get(struct symlink_request *, char * *);
/* --- symlink_request done --- */

/* Tag definition for symlink_response */
enum symlink_response_ {
  SYMLINK_RESPONSE_RST_CODE=1,
  SYMLINK_RESPONSE_STAT=2,
  SYMLINK_RESPONSE_MAX_TAGS
};

/* Structure declaration for symlink_response */
struct symlink_response_access_ {
  int (*rst_code_assign)(struct symlink_response *, const ev_uint32_t);
  int (*rst_code_get)(struct symlink_response *, ev_uint32_t *);
  int (*stat_assign)(struct symlink_response *, const struct file_stat*);
  int (*stat_get)(struct symlink_response *, struct file_stat* *);
};

struct symlink_response {
  struct symlink_response_access_ *base;

  ev_uint32_t rst_code;
  struct file_stat* stat;

  ev_uint8_t rst_code_set;
  ev_uint8_t stat_set;
};

struct symlink_response *symlink_response_new(void);
struct symlink_response *symlink_response_new_with_arg(void *);
void symlink_response_free(struct symlink_response *);
void symlink_response_clear(struct symlink_response *);
void symlink_response_marshal(struct evbuffer *, const struct symlink_response *);
int symlink_response_unmarshal(struct symlink_response *, struct evbuffer *);
int symlink_response_complete(struct symlink_response *);
void evtag_marshal_symlink_response(struct evbuffer *, ev_uint32_t,
    const struct symlink_response *);
int evtag_unmarshal_symlink_response(struct evbuffer *, ev_uint32_t,
    struct symlink_response *);
int symlink_response_rst_code_assign(struct symlink_response *, const ev_uint32_t);
int symlink_response_rst_code_get(struct symlink_response *, ev_uint32_t *);
int symlink_response_stat_assign(struct symlink_response *, const struct file_stat*);
int symlink_response_stat_get(struct symlink_response *, struct file_stat* *);
/* --- symlink_response done --- */

/* Tag definition for readlink_request */
enum readlink_request_ {
  READLINK_REQUEST_INO=1,
  READLINK_REQUEST_MAX_TAGS
};

/* Structure declaration for readlink_request */
struct readlink_request_access_ {
  int (*ino_assign)(struct readlink_request *, const ev_uint64_t);
  int (*ino_get)(struct readlink_request *, ev_uint64_t *);
};

struct readlink_request {
  struct readlink_request_access_ *base;

  ev_uint64_t ino;

  ev_uint8_t ino_set;
};

struct readlink_request *readlink_request_new(void);
struct readlink_request *readlink_request_new_with_arg(void *);
void readlink_request_free(struct readlink_request *);
void readlink_request_clear(struct readlink_request *);
void readlink_request_marshal(struct evbuffer *, const struct readlink_request *);
int readlink_request_unmarshal(struct readlink_request *, struct evbuffer *);
int readlink_request_complete(struct readlink_request *);
void evtag_marshal_readlink_request(struct evbuffer *, ev_uint32_t,
    const struct readlink_request *);
int evtag_unmarshal_readlink_request(struct evbuffer *, ev_uint32_t,
    struct readlink_request *);
int readlink_request_ino_assign(struct readlink_request *, const ev_uint64_t);
int readlink_request_ino_get(struct readlink_request *, ev_uint64_t *);
/* --- readlink_request done --- */

/* Tag definition for readlink_response */
enum readlink_response_ {
  READLINK_RESPONSE_RST_CODE=1,
  READLINK_RESPONSE_PATH=2,
  READLINK_RESPONSE_MAX_TAGS
};

/* Structure declaration for readlink_response */
struct readlink_response_access_ {
  int (*rst_code_assign)(struct readlink_response *, const ev_uint32_t);
  int (*rst_code_get)(struct readlink_response *, ev_uint32_t *);
  int (*path_assign)(struct readlink_response *, const char *);
  int (*path_get)(struct readlink_response *, char * *);
};

struct readlink_response {
  struct readlink_response_access_ *base;

  ev_uint32_t rst_code;
  char *path;

  ev_uint8_t rst_code_set;
  ev_uint8_t path_set;
};

struct readlink_response *readlink_response_new(void);
struct readlink_response *readlink_response_new_with_arg(void *);
void readlink_response_free(struct readlink_response *);
void readlink_response_clear(struct readlink_response *);
void readlink_response_marshal(struct evbuffer *, const struct readlink_response *);
int readlink_response_unmarshal(struct readlink_response *, struct evbuffer *);
int readlink_response_complete(struct readlink_response *);
void evtag_marshal_readlink_response(struct evbuffer *, ev_uint32_t,
    const struct readlink_response *);
int evtag_unmarshal_readlink_response(struct evbuffer *, ev_uint32_t,
    struct readlink_response *);
int readlink_response_rst_code_assign(struct readlink_response *, const ev_uint32_t);
int readlink_response_rst_code_get(struct readlink_response *, ev_uint32_t *);
int readlink_response_path_assign(struct readlink_response *, const char *);
int readlink_response_path_get(struct readlink_response *, char * *);
/* --- readlink_response done --- */

/* Tag definition for lookup_request */
enum lookup_request_ {
  LOOKUP_REQUEST_PARENT_INO=1,
  LOOKUP_REQUEST_NAME=2,
  LOOKUP_REQUEST_MAX_TAGS
};

/* Structure declaration for lookup_request */
struct lookup_request_access_ {
  int (*parent_ino_assign)(struct lookup_request *, const ev_uint64_t);
  int (*parent_ino_get)(struct lookup_request *, ev_uint64_t *);
  int (*name_assign)(struct lookup_request *, const char *);
  int (*name_get)(struct lookup_request *, char * *);
};

struct lookup_request {
  struct lookup_request_access_ *base;

  ev_uint64_t parent_ino;
  char *name;

  ev_uint8_t parent_ino_set;
  ev_uint8_t name_set;
};

struct lookup_request *lookup_request_new(void);
struct lookup_request *lookup_request_new_with_arg(void *);
void lookup_request_free(struct lookup_request *);
void lookup_request_clear(struct lookup_request *);
void lookup_request_marshal(struct evbuffer *, const struct lookup_request *);
int lookup_request_unmarshal(struct lookup_request *, struct evbuffer *);
int lookup_request_complete(struct lookup_request *);
void evtag_marshal_lookup_request(struct evbuffer *, ev_uint32_t,
    const struct lookup_request *);
int evtag_unmarshal_lookup_request(struct evbuffer *, ev_uint32_t,
    struct lookup_request *);
int lookup_request_parent_ino_assign(struct lookup_request *, const ev_uint64_t);
int lookup_request_parent_ino_get(struct lookup_request *, ev_uint64_t *);
int lookup_request_name_assign(struct lookup_request *, const char *);
int lookup_request_name_get(struct lookup_request *, char * *);
/* --- lookup_request done --- */

/* Tag definition for lookup_response */
enum lookup_response_ {
  LOOKUP_RESPONSE_RST_CODE=1,
  LOOKUP_RESPONSE_STAT_ARR=2,
  LOOKUP_RESPONSE_MAX_TAGS
};

/* Structure declaration for lookup_response */
struct lookup_response_access_ {
  int (*rst_code_assign)(struct lookup_response *, const ev_uint32_t);
  int (*rst_code_get)(struct lookup_response *, ev_uint32_t *);
  int (*stat_arr_assign)(struct lookup_response *, int, const struct file_stat*);
  int (*stat_arr_get)(struct lookup_response *, int, struct file_stat* *);
  struct file_stat*  (*stat_arr_add)(struct lookup_response *msg);
};

struct lookup_response {
  struct lookup_response_access_ *base;

  ev_uint32_t rst_code;
  struct file_stat* *stat_arr;
  int stat_arr_length;
  int stat_arr_num_allocated;

  ev_uint8_t rst_code_set;
  ev_uint8_t stat_arr_set;
};

struct lookup_response *lookup_response_new(void);
struct lookup_response *lookup_response_new_with_arg(void *);
void lookup_response_free(struct lookup_response *);
void lookup_response_clear(struct lookup_response *);
void lookup_response_marshal(struct evbuffer *, const struct lookup_response *);
int lookup_response_unmarshal(struct lookup_response *, struct evbuffer *);
int lookup_response_complete(struct lookup_response *);
void evtag_marshal_lookup_response(struct evbuffer *, ev_uint32_t,
    const struct lookup_response *);
int evtag_unmarshal_lookup_response(struct evbuffer *, ev_uint32_t,
    struct lookup_response *);
int lookup_response_rst_code_assign(struct lookup_response *, const ev_uint32_t);
int lookup_response_rst_code_get(struct lookup_response *, ev_uint32_t *);
int lookup_response_stat_arr_assign(struct lookup_response *, int, const struct file_stat*);
int lookup_response_stat_arr_get(struct lookup_response *, int, struct file_stat* *);
struct file_stat*  lookup_response_stat_arr_add(struct lookup_response *msg);
/* --- lookup_response done --- */

/* Tag definition for unlink_request */
enum unlink_request_ {
  UNLINK_REQUEST_PARENT_INO=1,
  UNLINK_REQUEST_NAME=2,
  UNLINK_REQUEST_MAX_TAGS
};

/* Structure declaration for unlink_request */
struct unlink_request_access_ {
  int (*parent_ino_assign)(struct unlink_request *, const ev_uint64_t);
  int (*parent_ino_get)(struct unlink_request *, ev_uint64_t *);
  int (*name_assign)(struct unlink_request *, const char *);
  int (*name_get)(struct unlink_request *, char * *);
};

struct unlink_request {
  struct unlink_request_access_ *base;

  ev_uint64_t parent_ino;
  char *name;

  ev_uint8_t parent_ino_set;
  ev_uint8_t name_set;
};

struct unlink_request *unlink_request_new(void);
struct unlink_request *unlink_request_new_with_arg(void *);
void unlink_request_free(struct unlink_request *);
void unlink_request_clear(struct unlink_request *);
void unlink_request_marshal(struct evbuffer *, const struct unlink_request *);
int unlink_request_unmarshal(struct unlink_request *, struct evbuffer *);
int unlink_request_complete(struct unlink_request *);
void evtag_marshal_unlink_request(struct evbuffer *, ev_uint32_t,
    const struct unlink_request *);
int evtag_unmarshal_unlink_request(struct evbuffer *, ev_uint32_t,
    struct unlink_request *);
int unlink_request_parent_ino_assign(struct unlink_request *, const ev_uint64_t);
int unlink_request_parent_ino_get(struct unlink_request *, ev_uint64_t *);
int unlink_request_name_assign(struct unlink_request *, const char *);
int unlink_request_name_get(struct unlink_request *, char * *);
/* --- unlink_request done --- */

/* Tag definition for unlink_response */
enum unlink_response_ {
  UNLINK_RESPONSE_RST_CODE=1,
  UNLINK_RESPONSE_INO=2,
  UNLINK_RESPONSE_MAX_TAGS
};

/* Structure declaration for unlink_response */
struct unlink_response_access_ {
  int (*rst_code_assign)(struct unlink_response *, const ev_uint32_t);
  int (*rst_code_get)(struct unlink_response *, ev_uint32_t *);
  int (*ino_assign)(struct unlink_response *, const ev_uint64_t);
  int (*ino_get)(struct unlink_response *, ev_uint64_t *);
};

struct unlink_response {
  struct unlink_response_access_ *base;

  ev_uint32_t rst_code;
  ev_uint64_t ino;

  ev_uint8_t rst_code_set;
  ev_uint8_t ino_set;
};

struct unlink_response *unlink_response_new(void);
struct unlink_response *unlink_response_new_with_arg(void *);
void unlink_response_free(struct unlink_response *);
void unlink_response_clear(struct unlink_response *);
void unlink_response_marshal(struct evbuffer *, const struct unlink_response *);
int unlink_response_unmarshal(struct unlink_response *, struct evbuffer *);
int unlink_response_complete(struct unlink_response *);
void evtag_marshal_unlink_response(struct evbuffer *, ev_uint32_t,
    const struct unlink_response *);
int evtag_unmarshal_unlink_response(struct evbuffer *, ev_uint32_t,
    struct unlink_response *);
int unlink_response_rst_code_assign(struct unlink_response *, const ev_uint32_t);
int unlink_response_rst_code_get(struct unlink_response *, ev_uint32_t *);
int unlink_response_ino_assign(struct unlink_response *, const ev_uint64_t);
int unlink_response_ino_get(struct unlink_response *, ev_uint64_t *);
/* --- unlink_response done --- */

/* Tag definition for statfs_request */
enum statfs_request_ {
  STATFS_REQUEST_NOTHING=1,
  STATFS_REQUEST_MAX_TAGS
};

/* Structure declaration for statfs_request */
struct statfs_request_access_ {
  int (*nothing_assign)(struct statfs_request *, const ev_uint32_t);
  int (*nothing_get)(struct statfs_request *, ev_uint32_t *);
};

struct statfs_request {
  struct statfs_request_access_ *base;

  ev_uint32_t nothing;

  ev_uint8_t nothing_set;
};

struct statfs_request *statfs_request_new(void);
struct statfs_request *statfs_request_new_with_arg(void *);
void statfs_request_free(struct statfs_request *);
void statfs_request_clear(struct statfs_request *);
void statfs_request_marshal(struct evbuffer *, const struct statfs_request *);
int statfs_request_unmarshal(struct statfs_request *, struct evbuffer *);
int statfs_request_complete(struct statfs_request *);
void evtag_marshal_statfs_request(struct evbuffer *, ev_uint32_t,
    const struct statfs_request *);
int evtag_unmarshal_statfs_request(struct evbuffer *, ev_uint32_t,
    struct statfs_request *);
int statfs_request_nothing_assign(struct statfs_request *, const ev_uint32_t);
int statfs_request_nothing_get(struct statfs_request *, ev_uint32_t *);
/* --- statfs_request done --- */

/* Tag definition for statfs_response */
enum statfs_response_ {
  STATFS_RESPONSE_TOTAL_SPACE=1,
  STATFS_RESPONSE_AVAIL_SPACE=2,
  STATFS_RESPONSE_INODE_CNT=3,
  STATFS_RESPONSE_MAX_TAGS
};

/* Structure declaration for statfs_response */
struct statfs_response_access_ {
  int (*total_space_assign)(struct statfs_response *, const ev_uint32_t);
  int (*total_space_get)(struct statfs_response *, ev_uint32_t *);
  int (*avail_space_assign)(struct statfs_response *, const ev_uint32_t);
  int (*avail_space_get)(struct statfs_response *, ev_uint32_t *);
  int (*inode_cnt_assign)(struct statfs_response *, const ev_uint32_t);
  int (*inode_cnt_get)(struct statfs_response *, ev_uint32_t *);
};

struct statfs_response {
  struct statfs_response_access_ *base;

  ev_uint32_t total_space;
  ev_uint32_t avail_space;
  ev_uint32_t inode_cnt;

  ev_uint8_t total_space_set;
  ev_uint8_t avail_space_set;
  ev_uint8_t inode_cnt_set;
};

struct statfs_response *statfs_response_new(void);
struct statfs_response *statfs_response_new_with_arg(void *);
void statfs_response_free(struct statfs_response *);
void statfs_response_clear(struct statfs_response *);
void statfs_response_marshal(struct evbuffer *, const struct statfs_response *);
int statfs_response_unmarshal(struct statfs_response *, struct evbuffer *);
int statfs_response_complete(struct statfs_response *);
void evtag_marshal_statfs_response(struct evbuffer *, ev_uint32_t,
    const struct statfs_response *);
int evtag_unmarshal_statfs_response(struct evbuffer *, ev_uint32_t,
    struct statfs_response *);
int statfs_response_total_space_assign(struct statfs_response *, const ev_uint32_t);
int statfs_response_total_space_get(struct statfs_response *, ev_uint32_t *);
int statfs_response_avail_space_assign(struct statfs_response *, const ev_uint32_t);
int statfs_response_avail_space_get(struct statfs_response *, ev_uint32_t *);
int statfs_response_inode_cnt_assign(struct statfs_response *, const ev_uint32_t);
int statfs_response_inode_cnt_get(struct statfs_response *, ev_uint32_t *);
/* --- statfs_response done --- */

/* Tag definition for mkfs_request */
enum mkfs_request_ {
  MKFS_REQUEST_POS_ARR=1,
  MKFS_REQUEST_MAX_TAGS
};

/* Structure declaration for mkfs_request */
struct mkfs_request_access_ {
  int (*pos_arr_assign)(struct mkfs_request *, int, const ev_uint32_t);
  int (*pos_arr_get)(struct mkfs_request *, int, ev_uint32_t *);
  ev_uint32_t * (*pos_arr_add)(struct mkfs_request *msg, const ev_uint32_t value);
};

struct mkfs_request {
  struct mkfs_request_access_ *base;

  ev_uint32_t *pos_arr;
  int pos_arr_length;
  int pos_arr_num_allocated;

  ev_uint8_t pos_arr_set;
};

struct mkfs_request *mkfs_request_new(void);
struct mkfs_request *mkfs_request_new_with_arg(void *);
void mkfs_request_free(struct mkfs_request *);
void mkfs_request_clear(struct mkfs_request *);
void mkfs_request_marshal(struct evbuffer *, const struct mkfs_request *);
int mkfs_request_unmarshal(struct mkfs_request *, struct evbuffer *);
int mkfs_request_complete(struct mkfs_request *);
void evtag_marshal_mkfs_request(struct evbuffer *, ev_uint32_t,
    const struct mkfs_request *);
int evtag_unmarshal_mkfs_request(struct evbuffer *, ev_uint32_t,
    struct mkfs_request *);
int mkfs_request_pos_arr_assign(struct mkfs_request *, int, const ev_uint32_t);
int mkfs_request_pos_arr_get(struct mkfs_request *, int, ev_uint32_t *);
ev_uint32_t * mkfs_request_pos_arr_add(struct mkfs_request *msg, const ev_uint32_t value);
/* --- mkfs_request done --- */

/* Tag definition for mkfs_response */
enum mkfs_response_ {
  MKFS_RESPONSE_RST=1,
  MKFS_RESPONSE_MAX_TAGS
};

/* Structure declaration for mkfs_response */
struct mkfs_response_access_ {
  int (*rst_assign)(struct mkfs_response *, const ev_uint32_t);
  int (*rst_get)(struct mkfs_response *, ev_uint32_t *);
};

struct mkfs_response {
  struct mkfs_response_access_ *base;

  ev_uint32_t rst;

  ev_uint8_t rst_set;
};

struct mkfs_response *mkfs_response_new(void);
struct mkfs_response *mkfs_response_new_with_arg(void *);
void mkfs_response_free(struct mkfs_response *);
void mkfs_response_clear(struct mkfs_response *);
void mkfs_response_marshal(struct evbuffer *, const struct mkfs_response *);
int mkfs_response_unmarshal(struct mkfs_response *, struct evbuffer *);
int mkfs_response_complete(struct mkfs_response *);
void evtag_marshal_mkfs_response(struct evbuffer *, ev_uint32_t,
    const struct mkfs_response *);
int evtag_unmarshal_mkfs_response(struct evbuffer *, ev_uint32_t,
    struct mkfs_response *);
int mkfs_response_rst_assign(struct mkfs_response *, const ev_uint32_t);
int mkfs_response_rst_get(struct mkfs_response *, ev_uint32_t *);
/* --- mkfs_response done --- */

/* Tag definition for uuid_request */
enum uuid_request_ {
  UUID_REQUEST_COUNT=1,
  UUID_REQUEST_MAX_TAGS
};

/* Structure declaration for uuid_request */
struct uuid_request_access_ {
  int (*count_assign)(struct uuid_request *, const ev_uint32_t);
  int (*count_get)(struct uuid_request *, ev_uint32_t *);
};

struct uuid_request {
  struct uuid_request_access_ *base;

  ev_uint32_t count;

  ev_uint8_t count_set;
};

struct uuid_request *uuid_request_new(void);
struct uuid_request *uuid_request_new_with_arg(void *);
void uuid_request_free(struct uuid_request *);
void uuid_request_clear(struct uuid_request *);
void uuid_request_marshal(struct evbuffer *, const struct uuid_request *);
int uuid_request_unmarshal(struct uuid_request *, struct evbuffer *);
int uuid_request_complete(struct uuid_request *);
void evtag_marshal_uuid_request(struct evbuffer *, ev_uint32_t,
    const struct uuid_request *);
int evtag_unmarshal_uuid_request(struct evbuffer *, ev_uint32_t,
    struct uuid_request *);
int uuid_request_count_assign(struct uuid_request *, const ev_uint32_t);
int uuid_request_count_get(struct uuid_request *, ev_uint32_t *);
/* --- uuid_request done --- */

/* Tag definition for uuid_response */
enum uuid_response_ {
  UUID_RESPONSE_RANGE_MIN=1,
  UUID_RESPONSE_RANGE_MAX=2,
  UUID_RESPONSE_MAX_TAGS
};

/* Structure declaration for uuid_response */
struct uuid_response_access_ {
  int (*range_min_assign)(struct uuid_response *, const ev_uint64_t);
  int (*range_min_get)(struct uuid_response *, ev_uint64_t *);
  int (*range_max_assign)(struct uuid_response *, const ev_uint64_t);
  int (*range_max_get)(struct uuid_response *, ev_uint64_t *);
};

struct uuid_response {
  struct uuid_response_access_ *base;

  ev_uint64_t range_min;
  ev_uint64_t range_max;

  ev_uint8_t range_min_set;
  ev_uint8_t range_max_set;
};

struct uuid_response *uuid_response_new(void);
struct uuid_response *uuid_response_new_with_arg(void *);
void uuid_response_free(struct uuid_response *);
void uuid_response_clear(struct uuid_response *);
void uuid_response_marshal(struct evbuffer *, const struct uuid_response *);
int uuid_response_unmarshal(struct uuid_response *, struct evbuffer *);
int uuid_response_complete(struct uuid_response *);
void evtag_marshal_uuid_response(struct evbuffer *, ev_uint32_t,
    const struct uuid_response *);
int evtag_unmarshal_uuid_response(struct evbuffer *, ev_uint32_t,
    struct uuid_response *);
int uuid_response_range_min_assign(struct uuid_response *, const ev_uint64_t);
int uuid_response_range_min_get(struct uuid_response *, ev_uint64_t *);
int uuid_response_range_max_assign(struct uuid_response *, const ev_uint64_t);
int uuid_response_range_max_get(struct uuid_response *, ev_uint64_t *);
/* --- uuid_response done --- */

/* Tag definition for migrate_request */
enum migrate_request_ {
  MIGRATE_REQUEST_FROM_MDS=1,
  MIGRATE_REQUEST_TO_MDS=2,
  MIGRATE_REQUEST_STAT_ARR=3,
  MIGRATE_REQUEST_OP=4,
  MIGRATE_REQUEST_MAX_TAGS
};

/* Structure declaration for migrate_request */
struct migrate_request_access_ {
  int (*from_mds_assign)(struct migrate_request *, const ev_uint32_t);
  int (*from_mds_get)(struct migrate_request *, ev_uint32_t *);
  int (*to_mds_assign)(struct migrate_request *, const ev_uint32_t);
  int (*to_mds_get)(struct migrate_request *, ev_uint32_t *);
  int (*stat_arr_assign)(struct migrate_request *, int, const struct file_stat*);
  int (*stat_arr_get)(struct migrate_request *, int, struct file_stat* *);
  struct file_stat*  (*stat_arr_add)(struct migrate_request *msg);
  int (*op_assign)(struct migrate_request *, const ev_uint32_t);
  int (*op_get)(struct migrate_request *, ev_uint32_t *);
};

struct migrate_request {
  struct migrate_request_access_ *base;

  ev_uint32_t from_mds;
  ev_uint32_t to_mds;
  struct file_stat* *stat_arr;
  int stat_arr_length;
  int stat_arr_num_allocated;
  ev_uint32_t op;

  ev_uint8_t from_mds_set;
  ev_uint8_t to_mds_set;
  ev_uint8_t stat_arr_set;
  ev_uint8_t op_set;
};

struct migrate_request *migrate_request_new(void);
struct migrate_request *migrate_request_new_with_arg(void *);
void migrate_request_free(struct migrate_request *);
void migrate_request_clear(struct migrate_request *);
void migrate_request_marshal(struct evbuffer *, const struct migrate_request *);
int migrate_request_unmarshal(struct migrate_request *, struct evbuffer *);
int migrate_request_complete(struct migrate_request *);
void evtag_marshal_migrate_request(struct evbuffer *, ev_uint32_t,
    const struct migrate_request *);
int evtag_unmarshal_migrate_request(struct evbuffer *, ev_uint32_t,
    struct migrate_request *);
int migrate_request_from_mds_assign(struct migrate_request *, const ev_uint32_t);
int migrate_request_from_mds_get(struct migrate_request *, ev_uint32_t *);
int migrate_request_to_mds_assign(struct migrate_request *, const ev_uint32_t);
int migrate_request_to_mds_get(struct migrate_request *, ev_uint32_t *);
int migrate_request_stat_arr_assign(struct migrate_request *, int, const struct file_stat*);
int migrate_request_stat_arr_get(struct migrate_request *, int, struct file_stat* *);
struct file_stat*  migrate_request_stat_arr_add(struct migrate_request *msg);
int migrate_request_op_assign(struct migrate_request *, const ev_uint32_t);
int migrate_request_op_get(struct migrate_request *, ev_uint32_t *);
/* --- migrate_request done --- */

/* Tag definition for migrate_response */
enum migrate_response_ {
  MIGRATE_RESPONSE_RST=1,
  MIGRATE_RESPONSE_MAX_TAGS
};

/* Structure declaration for migrate_response */
struct migrate_response_access_ {
  int (*rst_assign)(struct migrate_response *, const ev_uint32_t);
  int (*rst_get)(struct migrate_response *, ev_uint32_t *);
};

struct migrate_response {
  struct migrate_response_access_ *base;

  ev_uint32_t rst;

  ev_uint8_t rst_set;
};

struct migrate_response *migrate_response_new(void);
struct migrate_response *migrate_response_new_with_arg(void *);
void migrate_response_free(struct migrate_response *);
void migrate_response_clear(struct migrate_response *);
void migrate_response_marshal(struct evbuffer *, const struct migrate_response *);
int migrate_response_unmarshal(struct migrate_response *, struct evbuffer *);
int migrate_response_complete(struct migrate_response *);
void evtag_marshal_migrate_response(struct evbuffer *, ev_uint32_t,
    const struct migrate_response *);
int evtag_unmarshal_migrate_response(struct evbuffer *, ev_uint32_t,
    struct migrate_response *);
int migrate_response_rst_assign(struct migrate_response *, const ev_uint32_t);
int migrate_response_rst_get(struct migrate_response *, ev_uint32_t *);
/* --- migrate_response done --- */

#endif  /* _COMMON_PROTOCOL_RPC_ */
