#include "fs.h"
/*static uint64_t cur_ino = 3;*/
static fsnode *nodehash[NODEHASHSIZE];
static uint64_t version;

void fsnode_tree_insert(fsnode * p, fsnode * n)
{
    if (NULL == p->data.ddata.children) {
        fsnode *new_node = fsnode_new(); 
        dlist_t *pl = &(new_node->tree_dlist);
        dlist_init(pl);
        p->data.ddata.children = new_node;
    }
    fsnode *bucket = p->data.ddata.children;

    dlist_t *head = &(bucket->tree_dlist);
    dlist_t *pl = &(n->tree_dlist);
    dlist_insert_head(head, pl);
}

fsnode *fsnode_tree_find(fsnode * p, uint64_t ino)
{
    fsnode *children_head = p->data.ddata.children;
    dlist_t *head = &(children_head->tree_dlist);
    dlist_t *pl;
    for (pl = head->next; pl != head; pl = pl->next) {
        p = dlist_data(pl, fsnode, tree_dlist);
        if (p->ino == ino)
            return p;
    }
    return NULL;
}

/* useless 
 * */
fsnode *fsnode_tree_lookup(fsnode * p, char *name)
{
    // todo : copy from fsnode_tree_find
    return NULL;
}

//把这个node 从父节点的众儿子中删除，解除兄弟关系 ---??解除父子关系
inline fsnode *fsnode_tree_remove(fsnode * n)
{
    dlist_t *pl = &(n->tree_dlist);
    dlist_remove(pl);
    return n;
}

inline fsnode *fsnode_hash_insert(fsnode * n)
{
    uint32_t pos = NODEHASHPOS(n->ino);
    if (NULL == nodehash[pos]) {
        nodehash[pos] = fsnode_new();
        dlist_t *pl = &(nodehash[pos]->hash_dlist);
        dlist_init(pl);
    }
    fsnode *bucket = nodehash[pos];

    dlist_t *head = &(bucket->hash_dlist);
    dlist_t *pl = &(n->hash_dlist);
    dlist_insert_head(head, pl);
    return n;
}

inline fsnode *fsnode_hash_find(uint64_t ino)
{
    uint32_t pos = NODEHASHPOS(ino);
    fsnode *bucket = nodehash[pos];

    if (bucket == NULL)
        return NULL;

    fsnode *p;
    dlist_t *head = &(bucket->hash_dlist);
    dlist_t *pl;
    for (pl = head->next; pl != head; pl = pl->next) {
        p = dlist_data(pl, fsnode, hash_dlist);
        if (p->ino == ino)
            return p;
    }
    return NULL;
}

inline fsnode *fsnode_hash_remove(fsnode * p)
{
    dlist_t *pl = &(p->hash_dlist);
    dlist_remove(pl);
    return 0;
}

inline fsnode *fsnode_new()
{
    fsnode *p = (fsnode *) malloc(sizeof(fsnode));
    memset(p, 0, sizeof(fsnode));
    p->modifiy_flag = 0;
    return p;
}

int fs_init()
{
    DBG();

    fsnode *vroot;              // for those fsnode whose parent is not in the same mds
    vroot = fsnode_new();
    vroot->ino = FS_VROOT_INO;
    vroot->mode = S_IFDIR;
    vroot->name = "-";
    vroot->nlen = strlen(vroot->name);
    vroot->parent = vroot;
    vroot->data.ddata.children = NULL;

    fsnode_hash_insert(vroot);
    return 0;
}

/*
 * set the modify flag of <which>
 * and it's every ancestor
 * */
void fs_set_modify_flag(fsnode * n, int64_t cnt_inc){
    if (n==NULL)
        return;
    n->modifiy_flag = 1;
    while((n=n->parent) ){
        n->tree_cnt += cnt_inc;
        n->access_counter ++;
        n->modifiy_flag = 1;
        if (n->ino == FS_ROOT_INO || n->ino == FS_VROOT_INO)
            break;
    }
}
void fs_set_visit_flag(fsnode * n){
    if (n==NULL)
        return;
    while((n=n->parent) ){
        n->access_counter ++;
        if (n->ino == FS_ROOT_INO || n->ino == FS_VROOT_INO)
            break;
    }
}


int fs_setattr(uint64_t ino, struct file_stat *st)
{
    logging(LOG_DEUBG, "fs_setattr(%" PRIu64 ")", ino);

    fsnode *n = fsnode_hash_find(ino);

    if (n == NULL) {
        st->ino = 0;
        /*return RST_CODE_NOT_FOUND;*/
        return 0;
    }

    if (S_ISREG(n->mode))
        n->data.fdata.length = st->size; //原来bug在这里,set attr的时候把子树搞坏了，fsnode里面fdata, ddata用的是用一段内存
    fs_set_modify_flag(n, 0);
    version++;
    return 0;
}
//NEW
int fs_stat(uint64_t ino, fsnode ** o_fsnode)
{
    logging(LOG_DEUBG, "fs_stat(%" PRIu64 ")", ino);
    fsnode *n = fsnode_hash_find(ino);
    if (n == NULL) {
        return RST_CODE_NOT_FOUND;
    }
    fs_set_visit_flag(n);
    *o_fsnode = n;
    return 0;
}



//NEW
//返回的是children链表上的一个元素，链表中所有元素为兄弟.
int fs_ls(uint64_t ino, fsnode ** o_fsnode)
{
    logging(LOG_DEUBG, "fs_ls(%" PRIu64 ")", ino);
    fsnode *n = fsnode_hash_find(ino);
    if (n == NULL) {
        return RST_CODE_NOT_FOUND;
    }
    fs_set_visit_flag(n);
    *o_fsnode = n->data.ddata.children;
    return 0;
}

//NEW
int fs_lookup(uint64_t parent_ino, char *name, fsnode ** o_fsnode)
{
    logging(LOG_DEUBG, "fs_lookup (parent_ino = %" PRIu64 " , name = %s)",
            parent_ino, name);

    fsnode *n = fsnode_hash_find(parent_ino);
    if (n == NULL) {
        return RST_CODE_NOT_FOUND;
    }
    fs_set_visit_flag(n);
    n = n->data.ddata.children;
    if (!n){
        *o_fsnode = NULL;
        return 0;
    }


    fsnode *p;

    dlist_t *head = &(n->tree_dlist);
    if (NULL == head){
        *o_fsnode = NULL;
        return 0;
    }
    dlist_t *pl;

    for (pl = head->next; pl != head; pl = pl->next) {
        p = dlist_data(pl, fsnode, tree_dlist);

        if (0 == strcmp(name, p->name)) {
            *o_fsnode = p;
            return 0;
        }
    }
    logging(LOG_DEUBG,
            "fs_lookup (parent_ino = %" PRIu64 " , name = %s) return NULL!!",
            parent_ino, name);
    *o_fsnode = NULL;
    return 0;
}

/*
 *TODO:
  if it's splited to another MDS??
  what will I do ?
  I should not del it at the other mds, gc will do it
 *
 * */
void fs_del_tree_dfs(fsnode * root){
    if(!root)
        return;
    logging(LOG_INFO, "the fs_del_tree_dfs on : %"PRIu64"  %s", root->ino, root->name);
    dlist_t *head ;
    dlist_t *pl;
    fsnode *p;

    if (S_ISDIR(root->mode) && root->data.ddata.children){
        fsnode *children_head = root->data.ddata.children;
        head = &(children_head->tree_dlist);
        for (pl = head->next; pl != head; pl = pl->next) {
            p = dlist_data(pl, fsnode, tree_dlist);
            if (S_ISDIR(p->mode) || S_ISREG(p->mode))
                fs_del_tree_dfs(p);
            /*free(p); FIXME*/
        }
    }
    fsnode_tree_remove(root);
    fsnode_hash_remove(root);
    //free(root); FIXME!!
}

void fs_del_children_dfs(fsnode * root){

    if(!root)
        return;
    logging(LOG_INFO, "the fs_del_children_dfs on : %"PRIu64"  %s", root->ino, root->name);
    dlist_t *head ;
    dlist_t *pl;
    fsnode *p;

    if (S_ISDIR(root->mode) && root->data.ddata.children){
        fsnode *children_head = root->data.ddata.children;
        head = &(children_head->tree_dlist);
        for (pl = head->next; pl != head; pl = pl->next) {
            p = dlist_data(pl, fsnode, tree_dlist);
            if (S_ISDIR(p->mode) || S_ISREG(p->mode))
                fs_del_tree_dfs(p);
            /*free(p); FIXME*/
        }
    }
    root->data.ddata.children = NULL;
    root->tree_cnt = 0;

}

//NEW
int fs_unlink(uint64_t parent_ino, char *name)
{
    logging(LOG_DEUBG, "fs_unlink(parent_ino = %" PRIu64 " , name = %s)",
            parent_ino, name);

    fsnode *n = fsnode_hash_find(parent_ino);

    if (n == NULL) {
        /*return RST_CODE_NOT_FOUND;*/
        return 0; //直接扯蛋了
    }

    fs_set_modify_flag(n, -1);

    fsnode *s ;
    if (RST_CODE_NOT_FOUND == fs_lookup(parent_ino, name, &s) || (s == NULL) ){
        logging(LOG_DEUBG, "fs_unlink(parent_ino = %" PRIu64 " , name = %s) return RST_CODE_NOT_FOUND",
                parent_ino, name);
        return RST_CODE_NOT_FOUND;
    }
    logging(LOG_DEUBG, "parent find :%"PRIu64"",
            s->ino);
    fs_del_tree_dfs(s);
    /*fsnode_tree_remove(s);*/
    /*fsnode_hash_remove(s);*/
    /*free(s);*/
    version++;
    return 0;
}
//NEW
int fs_mknod(uint64_t parent_ino, uint64_t ino, char *name, int type, 
        int mode, fsnode ** o_fsnode)
{
    logging(LOG_DEUBG, "fs_mknod(parent_ino = %" PRIu64 " , name = %s)",
            parent_ino, name);

    //如果已经存在直接忽略，这样客户端如果在两个MDS上的一个创建的成功，另一个创建失败，就不用回滚.
    fsnode *n ;
    int rst_code = fs_lookup(parent_ino, name, &n); //也可用ino在hash表中找.
    if (rst_code == RST_CODE_NOT_FOUND){
        return RST_CODE_NOT_FOUND;
    }
    if (n!=NULL){
        * o_fsnode = n;
        return 0;
    }

    n = fsnode_new();
    n->parent = fsnode_hash_find(parent_ino);
    if (n->parent == NULL) {
        return RST_CODE_NOT_FOUND;
    }
    n->ino = ino;
    n->type = type;
    n->mode = mode;
    n->name = strdup(name);
    n->nlen = strlen(n->name);
    if (n->mode & S_IFREG) {    //is file 
        n->data.fdata.length = 0;
        n->pos_arr[0] = select_osd();
        n->pos_arr[1] = select_osd();
    } else if (n->mode & S_IFDIR) {
        n->pos_arr[0] = n->parent->pos_arr[0];
        n->pos_arr[1] = n->parent->pos_arr[1];
        n->data.ddata.children = NULL;
    }

    fsnode_hash_insert(n);
    fsnode_tree_insert(n->parent, n);
    fs_set_modify_flag(n, 1);
    version++;
    *o_fsnode = n;
    return 0;
}
//NEW
int fs_symlink(uint64_t parent_ino, uint64_t ino, const char *name, const char *path, fsnode ** o_fsnode)
{
    logging(LOG_DEUBG,
            "fs_symlink(parent_ino = %" PRIu64 " , name = %s, path = %s)",
            parent_ino, name, path);
    fsnode *n = fsnode_new();
    n->ino = ino;
    n->type = 255;              //TODO

    n->mode = S_IFLNK;
    n->name = strdup(name);
    n->nlen = strlen(n->name);
    n->data.sdata.path = strdup(path);
    n->data.sdata.pleng = strlen(path);

    n->parent = fsnode_hash_find(parent_ino);
    if (n->parent == NULL) {
        return RST_CODE_NOT_FOUND;
    }

    n->pos_arr[0] = n->parent->pos_arr[0];
    n->pos_arr[1] = n->parent->pos_arr[1];
    fsnode_hash_insert(n);
    fsnode_tree_insert(n->parent, n);
    fs_set_modify_flag(n, 1);
    version++;
    *o_fsnode = n;
    return 0;
}

//NEW
int fs_readlink(uint64_t ino, char ** path)
{
    fsnode *n = fsnode_hash_find(ino);
    if (n && n->mode == S_IFLNK){
        * path = n->data.sdata.path;
        return 0;
    }
    return RST_CODE_NOT_FOUND;
}

/*
 * make root of a whole fs, call by mkfs.sfs
 * */
int fs_mkfs(int mds1, int mds2)
{
    DBG();

    fsnode *root;               // for those fsnode whose parent is not in the same mds
    root = fsnode_new();
    root->ino = 1;
    root->mode = S_IFDIR;
    root->name = "/";
    root->nlen = strlen(root->name);
    root->parent = root;
    root->data.ddata.children = NULL;

    root->pos_arr[0] = mds1;
    root->pos_arr[1] = mds2;

    fsnode_hash_insert(root);

    return 0;
}

void fs_statfs(int *total_space, int *avail_space, int *inode_cnt)
{
    *total_space = 1024;
    *avail_space = 1024;
    *inode_cnt = 8888;
}

//dfs
void fs_dump_dfs(fsnode * root, struct evbuffer * evbuf, int fd){
    struct file_stat * stat = file_stat_new();
    fsnode_to_stat_copy(stat, root);
    log_file_stat("fsdump ", stat);
    /*file_stat_marshal(evbuf, stat);*/
    evtag_marshal_file_stat(evbuf, 1, stat);

    dlist_t *head ;
    dlist_t *pl;
    fsnode *p;

    if (S_ISDIR(root->mode) && root->data.ddata.children){
        fsnode *children_head = root->data.ddata.children;
        head = &(children_head->tree_dlist);
        for (pl = head->next; pl != head; pl = pl->next) {
            p = dlist_data(pl, fsnode, tree_dlist);
            fs_dump_dfs(p, evbuf, fd);
        }
    }
    logging(LOG_INFO, "in fs_dump_dfs evbuffer_get_length(evbuf) : %d", evbuffer_get_length(evbuf));
    if(evbuffer_get_length(evbuf)> 1)
        evbuffer_write(evbuf, fd);
}

int fs_store(char * path){
    struct evbuffer * evbuf = evbuffer_new();
    int fd = open(path, O_WRONLY | O_CREAT, 0755);
    fs_dump_dfs(fsnode_hash_find(1), evbuf, fd);
    close(fd);
    return 0;
}

static size_t get_file_len(char * path){
    size_t rst = 0;
    struct stat st;
    int fd = open(path, O_RDONLY);
    if (fstat(fd, &st) < 0) {
        rst = 0;
    }
    close(fd);
    rst = st.st_size;
    return rst;
}

int fs_load(char * path){
    struct evbuffer * evbuf = evbuffer_new();
    size_t len = get_file_len(path);
    if (!len){
        logging(LOG_INFO, "no mds.data");
        return 0;
    }

    int fd = open(path, O_RDONLY , 0755);
    if (-1 == fd)
        return 0;

    /*
     * Notice:
     * evbuffer_add_file (struct evbuffer *output, int fd, off_t offset, size_t length)
        Move data from a file into the evbuffer for writing to a socket. 
        Just for socket io ,not unmarshal.
        The results of using evbuffer_remove() or evbuffer_pullup() are undefined.    --恰恰就是在这里出了问题!

    evbuffer_add_file(evbuf, fd, 0, len);

     * */

    char buf [64*1024];
    int cnt;
    while((cnt = read(fd, buf, sizeof(buf))) ){ //TODO: optimize
        evbuffer_add(evbuf, buf, cnt);
    }
    cnt = 0;

    while(evbuffer_get_length(evbuf) > 0){
        cnt ++;

        logging(LOG_DEUBG, "ready to unmarshal");
        struct file_stat * stat = file_stat_new();
        if (evtag_unmarshal_file_stat(evbuf, 1, stat) == -1) {
        /*if (file_stat_unmarshal(stat, evbuf) == -1) {*/
            logging(LOG_DEUBG, "unmarshal error");
            break;
        }
        else{
            log_file_stat("evtag_ unmarshal file_stat ", stat);
            fsnode * n = fsnode_new();
            stat_to_fsnode_copy(n, stat);
            fsnode_hash_insert(n);
            if (stat->ino == 1){
                n->parent = n;
            }
            else{
                n->parent = fsnode_hash_find(stat->parent_ino);
                fsnode_tree_insert(n->parent, n);
            }
        }
    }
    logging(LOG_INFO, "%d inode loaded", cnt);
    close(fd);
    return 0;
}

void fsnode_to_stat_copy(struct file_stat *t, fsnode * n)
{
    assert(t);
    if (NULL == n) {
        EVTAG_ASSIGN(t, ino, 0);
        EVTAG_ASSIGN(t, parent_ino, 0);
        EVTAG_ASSIGN(t, size, 0);
        EVTAG_ASSIGN(t, name, "");
        return;
    }

    EVTAG_ASSIGN(t, ino, n->ino);
    EVTAG_ASSIGN(t, parent_ino, n->parent->ino);
    EVTAG_ASSIGN(t, name, n->name);
    EVTAG_ASSIGN(t, mode, n->mode);
    EVTAG_ASSIGN(t, type, n->type);
    EVTAG_ARRAY_ADD_VALUE(t, pos_arr, n->pos_arr[0]);
    EVTAG_ARRAY_ADD_VALUE(t, pos_arr, n->pos_arr[1]);

    EVTAG_ASSIGN(t, size, n->data.fdata.length);
    if (S_ISDIR(n->mode)) {
        EVTAG_ASSIGN(t, size, 4096);
    }
}



void stat_to_fsnode_copy(fsnode * n, struct file_stat *t)
{
    assert(n);
    assert(t);

    n->ino = t->ino;
    /*n->parent = fsnode_hash_find(t->parent_ino);*/
    n->name = strdup(t->name);
    n->mode = t->mode;
    n->type = t->type;
    
    if (n->mode & S_IFREG) {    //is file 
        n->data.fdata.length = t->size;
    } else if (n->mode & S_IFDIR) {
        n->data.ddata.children = NULL;
    }
    n->pos_arr[0] = t->pos_arr[0];
    n->pos_arr[1] = t->pos_arr[1];

}

