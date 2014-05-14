/*
the source code is mainly from mfs/mfsmaster/filesystem.c
*/
#include "sfs_common.h"
#define FS_ROOT_INO 1
#define FS_VROOT_INO 0
//Vroot是那些split节点，它们的parent不在自己所在的MDS上，所以parent指向vroot

typedef struct _fsnode {
    uint64_t ino;               //4
    uint32_t ctime, mtime, atime;   //16
    uint8_t type;               //
    uint8_t goal;
    uint16_t mode;              //20
    uint32_t uid;
    uint32_t gid;
    struct _fsnode *parent;

    uint16_t nlen;
    uint8_t *name;

    uint8_t modifiy_flag; 
    int32_t access_counter;  //how many descendant
    int64_t tree_cnt;  //how many descendant
    int64_t tree_size; //how large descendant 

    uint32_t pos_arr[3];        //每一份的存储位置，对于dir，是mds的mid,  /对于file，是osd的mid, 现在最多存3份.

    struct dlist_t tree_dlist;  //在parent->children构成的链表 实际上是同父的众节点之间的兄弟关系，用于ls操作. 
    struct dlist_t hash_dlist;  //在hash[]构成的链表中，用于根据id 找文件. lookup操作

    union _data {
        struct _ddata {         // type==TYPE_DIRECTORY
            struct _fsnode *children;
            uint32_t nlink;
            uint32_t elements;
        } ddata;
        //////////////////
        struct _sdata {         // type==TYPE_SYMLINK
            uint32_t pleng;
            uint8_t *path;
        } sdata;
        //////////////////
        uint32_t rdev;          // type==TYPE_BLOCKDEV ; type==TYPE_CHARDEV
        /////////////////
        struct _fdata {         // type==TYPE_FILE
            int xxxxx;
            uint64_t length;
            //uint64_t *chunktab;
            //uint32_t chunks;
        } fdata;
        /////////////////
    } data;                     //52
} fsnode;

/* type for readdir command */
//#define TYPE_FILE             'f'
//#define TYPE_SYMLINK          'l'
//#define TYPE_DIRECTORY        'd'
//#define TYPE_FIFO             'q'
//#define TYPE_BLOCKDEV         'b'
//#define TYPE_CHARDEV          'c'
//#define TYPE_SOCKET           's'
//#define TYPE_UNKNOWN          '?'

#define NODEHASHBITS (22)
#define NODEHASHSIZE (1<<NODEHASHBITS)
#define NODEHASHPOS(nodeid) ((nodeid)&(NODEHASHSIZE-1))

fsnode *fsnode_new();

inline fsnode *fsnode_hash_find(uint64_t ino);
inline fsnode *fsnode_hash_insert(fsnode * n);

void fsnode_tree_insert(fsnode * p, fsnode * n);


/*************fs write op****************/

int fs_mknod(uint64_t parent_ino, uint64_t ino, char *name, int type, int mode, fsnode ** o_fsnode);

int fs_symlink(uint64_t parent_ino, uint64_t ino, const char *name, const char *path, fsnode ** o_fsnode);

int fs_unlink(uint64_t parent_ino, char *name);

int fs_setattr(uint64_t ino, struct file_stat *st);

/*************fs read op****************/
int fs_stat(uint64_t ino, fsnode ** o_fsnode);
int fs_ls(uint64_t ino, fsnode ** o_fsnode);
int fs_lookup(uint64_t parent_ino, char *name, fsnode ** o_fsnode);

int fs_readlink(uint64_t ino, char ** path);


void fs_statfs(int *total_space, int *avail_space, int *inode_cnt);
int fs_mkfs();


void fsnode_to_stat_copy(struct file_stat *t, fsnode * n);
void stat_to_fsnode_copy(fsnode * n, struct file_stat *t);

int fs_rename();
int fs_get_goal();
int fs_set_goal();
int fs_init();

int fs_load(char * path);
int fs_store(char * path);
void fs_del_children_dfs(fsnode * root);
