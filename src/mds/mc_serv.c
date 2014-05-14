#include "sfs_common.h"
#include "fs.h"
#include "mds_conn.h"
int mds_op_counter = 0;
void fsnode_to_stat_copy(struct file_stat *t, fsnode * n);

static void setattr_handler(EVRPC_STRUCT(rpc_setattr) * rpc, void *arg)
{
    DBG();
    mds_op_counter++;
    struct setattr_request *request = rpc->request;
    struct setattr_response *response = rpc->reply;

    struct file_stat *stat;
    EVTAG_ARRAY_GET(request, stat_arr, 0, &stat);

    logging(LOG_DEUBG,
            "setattr(%" PRIu64 ", name=%s, size=%" PRIu64 ", mode=%04o)",
            stat->ino, stat->name, stat->size, stat->mode);
    int rst_code = fs_setattr(stat->ino, stat);
    EVTAG_ASSIGN(response, rst_code, rst_code);

    if(rst_code != 0 ){
        logging(LOG_INFO,
                "setattr(%" PRIu64 ", name=%s, size=%" PRIu64 ", mode=%04o) return rst_code != 0",
                stat->ino, stat->name, stat->size, stat->mode);

        goto done;
    }

    struct file_stat *t = EVTAG_ARRAY_ADD(response, stat_arr);
    EVTAG_ASSIGN(t, ino, stat->ino);
    EVTAG_ASSIGN(t, size, stat->size);
done:

    EVRPC_REQUEST_DONE(rpc);
}

static void stat_handler(EVRPC_STRUCT(rpc_stat) * rpc, void *arg)
{
    DBG();
    mds_op_counter++;
    struct stat_request *request = rpc->request;
    struct stat_response *response = rpc->reply;
    uint64_t ino;
    EVTAG_ARRAY_GET(request, ino_arr, 0, &ino);
    logging(LOG_DEUBG, "stat(%" PRIu64 ")", ino);


    fsnode *n ;
    int rst_code = fs_stat(ino, &n);
    EVTAG_ASSIGN(response, rst_code, rst_code);
    if(rst_code != 0 ){
        logging(LOG_WARN, "stat_handler(%"PRIu64") return rst_code != 0", ino);
        goto done;
    }

    struct file_stat *t = EVTAG_ARRAY_ADD(response, stat_arr);
    fsnode_to_stat_copy(t, n);
    log_file_stat("stat return ", t);
done:

    EVRPC_REQUEST_DONE(rpc);
}

static void ls_handler(EVRPC_STRUCT(rpc_stat) * rpc, void *arg)
{
    DBG();
    mds_op_counter++;
    struct stat_request *request = rpc->request;
    struct stat_response *response = rpc->reply;

    uint64_t ino;
    EVTAG_ARRAY_GET(request, ino_arr, 0, &ino);
    logging(LOG_DEUBG, "ls(%" PRIu64 ")", ino);

    fsnode *p = fsnode_hash_find(ino);
    if(p == NULL ){
        EVTAG_ASSIGN(response, rst_code, RST_CODE_NOT_FOUND);
        logging(LOG_WARN, "ls_handler return rst_code != 0");
        goto done;
    }
    



    struct file_stat *t = EVTAG_ARRAY_ADD(response, stat_arr);

    EVTAG_ASSIGN(t, ino, p->ino);
    EVTAG_ASSIGN(t, size, 4096);
    EVTAG_ASSIGN(t, name, ".");
    EVTAG_ASSIGN(t, type, p->type);
    EVTAG_ASSIGN(t, mode, p->mode);

    p = p->parent;

    t = EVTAG_ARRAY_ADD(response, stat_arr);
    EVTAG_ASSIGN(t, ino, p->ino);
    EVTAG_ASSIGN(t, size, 4096);
    EVTAG_ASSIGN(t, name, "..");
    EVTAG_ASSIGN(t, type, p->type);
    EVTAG_ASSIGN(t, mode, p->mode);

    fsnode *n ;
    int rst_code = fs_ls(ino, &n);
    EVTAG_ASSIGN(response, rst_code, rst_code);
    if((rst_code != 0 )||(n==NULL) ){
        logging(LOG_WARN, "ls_handler return rst_code != 0");
        goto done;
    }
    dlist_t *head = &(n->tree_dlist);
    dlist_t *pl;
    for (pl = head->next; pl != head; pl = pl->next) {

        p = dlist_data(pl, fsnode, tree_dlist);
        t = EVTAG_ARRAY_ADD(response, stat_arr);
        fsnode_to_stat_copy(t, p);
        log_file_stat("ls return : ", t);
    }
done:
    EVRPC_REQUEST_DONE(rpc);
}

static void statfs_handler(EVRPC_STRUCT(rpc_statfs) * rpc, void *arg)
{
    DBG();
    mds_op_counter++;

    struct statfs_response *response = rpc->reply;
    int avail_space;
    int total_space;
    int inode_cnt;

    fs_statfs(&total_space, &avail_space, &inode_cnt);

    EVTAG_ASSIGN(response, total_space, total_space);
    EVTAG_ASSIGN(response, avail_space, avail_space);
    EVTAG_ASSIGN(response, inode_cnt, inode_cnt);

    EVRPC_REQUEST_DONE(rpc);
}


static void mknod_handler(EVRPC_STRUCT(rpc_mknod) * rpc, void *arg)
{
    DBG();
    mds_op_counter++;
    while(cluster_get_osd_cnt() < 2 ){
        logging(LOG_WARN, "do not have more then 2 osds");
        sleep(5);
    }

    struct mknod_request *request = rpc->request;
    struct mknod_response *response = rpc->reply;
    logging(LOG_INFO, "mknod(parent=%" PRIu64 ", ino = % "PRIu64", name=%s, type=%d, mode=%04o)",
            request->parent_ino, request->ino, request->name, request->type, request->mode);

    fsnode *n ;
    int rst_code = fs_mknod(request->parent_ino, request->ino, request->name, request->type,
                         request->mode, &n);
    EVTAG_ASSIGN(response, rst_code, rst_code);

    if(rst_code != 0 ){
        logging(LOG_WARN, "mknod_handler return rst_code != 0");
        goto done;
    }


    struct file_stat *t = EVTAG_ARRAY_ADD(response, stat_arr);
    fsnode_to_stat_copy(t, n);
    log_file_stat("mknod return ", t);


    //FIXME: this is just for debug. 
    /*struct machine * self = get_self_machine();*/
    /*if (self->mid == 10000){*/
        /*struct machine * m = cluster_get_machine_by_mid(10002);*/
        /*migrate_send_request(m->ip, m->port, n, self->mid, m->mid);*/
    /*}*/
    //end

done:
    EVRPC_REQUEST_DONE(rpc);
}

static void symlink_handler(EVRPC_STRUCT(rpc_symlink) * rpc, void *arg)
{
    DBG();
    mds_op_counter++;

    struct symlink_request *request = rpc->request;
    struct symlink_response *response = rpc->reply;

    logging(LOG_DEUBG, "symlink(parent=%" PRIu64 ", ino = %"PRIu64", name=%s, path =%s)",
            request->parent_ino, request->ino, request->name, request->path);

    fsnode *n ;
    int rst_code = fs_symlink(request->parent_ino, request->ino, request->name, request->path, &n);
    EVTAG_ASSIGN(response, rst_code, rst_code);

    if(rst_code != 0 ){
        logging(LOG_WARN, "stat_handler return rst_code != 0");
        goto done;
    }

    struct file_stat *t;
    EVTAG_GET(response, stat, &t);


    fsnode_to_stat_copy(t, n);
    log_file_stat("symlink return ", t);
done:

    EVRPC_REQUEST_DONE(rpc);
}

static void readlink_handler(EVRPC_STRUCT(rpc_readlink) * rpc, void *arg)
{
    DBG();
    mds_op_counter++;

    struct readlink_request *request = rpc->request;
    struct readlink_response *response = rpc->reply;

    char *p ; 
    int rst_code = fs_readlink(request->ino, &p);
    EVTAG_ASSIGN(response, rst_code, rst_code);

    if(rst_code != 0 ){
        logging(LOG_WARN, "readlink_handler return rst_code != 0");
        goto done;
    }

    logging(LOG_DEUBG, "readlink(ino =%" PRIu64 ")", request->ino);
    logging(LOG_DEUBG, "readlink return : %s", p);

    EVTAG_ASSIGN(response, path, p);

done:
    EVRPC_REQUEST_DONE(rpc);
}

static void lookup_handler(EVRPC_STRUCT(rpc_lookup) * rpc, void *arg)
{
    DBG();
    mds_op_counter++;

    struct lookup_request *request = rpc->request;
    struct lookup_response *response = rpc->reply;

    logging(LOG_DEUBG, "lookup(parent=%" PRIu64 ", name=%s)",
            request->parent_ino, request->name);

    fsnode *n ;
    int rst_code = fs_lookup(request->parent_ino, request->name, &n);
    EVTAG_ASSIGN(response, rst_code, rst_code);
    if(rst_code != 0 ){
        logging(LOG_WARN, "lookup_handler return rst_code != 0");
        goto done;
    }

    struct file_stat *t = EVTAG_ARRAY_ADD(response, stat_arr);
    fsnode_to_stat_copy(t, n);
    log_file_stat("lookup return ", t);
done:

    EVRPC_REQUEST_DONE(rpc);
}

static void unlink_handler(EVRPC_STRUCT(rpc_unlink) * rpc, void *arg)
{
    DBG();
    mds_op_counter++;

    struct unlink_request *request = rpc->request;
    struct unlink_response * response = rpc->reply;

    logging(LOG_DEUBG, "unlink (parent=%" PRIu64 ", name=%s)",
            request->parent_ino, request->name);

    int rst_code = fs_unlink(request->parent_ino, request->name);
    EVTAG_ASSIGN(response, rst_code, rst_code);
    if(rst_code != 0 ){
        logging(LOG_WARN, "unlink_handler return rst_code != 0");
        goto done;
    }
done:

    EVRPC_REQUEST_DONE(rpc);
}

static void mkfs_handler(EVRPC_STRUCT(rpc_mkfs) * rpc, void *arg)
{
    DBG();
    mds_op_counter++;

    struct mkfs_request *request = rpc->request;
    /*struct mkfs_response * response = rpc->reply; */

    int mds1, mds2;
    EVTAG_ARRAY_GET(request, pos_arr, 0, &mds1);
    EVTAG_ARRAY_GET(request, pos_arr, 1, &mds2);

    logging(LOG_INFO, "mkfs (mds1=%d, mds2=%d)", mds1, mds2);
    fs_mkfs(mds1, mds2);
    EVRPC_REQUEST_DONE(rpc);
}

static void rpc_setup()
{
    struct evhttp *http = NULL;
    struct evrpc_base *base = NULL;

    char *listen_host = cfg_getstr("MDS2CLIENT_LISTEN_HOST", "*");
    int port = cfg_getint32("MDS2CLIENT_LISTEN_PORT", 9527);

    http = evhttp_start(listen_host, port);
    /*http = evhttp_start("192.168.1.102", port); */
    if (!http) {
        perror("can't start server!");
        exit(-1);
    }
    printf("Start mds at %s:%d\n", listen_host, port);

    base = evrpc_init(http);

    EVRPC_REGISTER(base, rpc_ping, ping, pong, ping_handler, NULL);
    EVRPC_REGISTER(base, rpc_migrate , migrate_request, migrate_response, 
            migrate_handler, NULL);

    EVRPC_REGISTER(base, rpc_stat, stat_request, stat_response, stat_handler,
                   NULL);
    EVRPC_REGISTER(base, rpc_ls, ls_request, ls_response, ls_handler, NULL);
    EVRPC_REGISTER(base, rpc_mknod, mknod_request, mknod_response,
                   mknod_handler, NULL);
    EVRPC_REGISTER(base, rpc_lookup, lookup_request, lookup_response,
                   lookup_handler, NULL);
    EVRPC_REGISTER(base, rpc_setattr, setattr_request, setattr_response,
                   setattr_handler, NULL);
    EVRPC_REGISTER(base, rpc_unlink, unlink_request, unlink_response,
                   unlink_handler, NULL);

    EVRPC_REGISTER(base, rpc_statfs, statfs_request, statfs_response,
                   statfs_handler, NULL);
    EVRPC_REGISTER(base, rpc_mkfs, mkfs_request, mkfs_response,
                   mkfs_handler, NULL);
    EVRPC_REGISTER(base, rpc_symlink, symlink_request, symlink_response,
                   symlink_handler, NULL);
    EVRPC_REGISTER(base, rpc_readlink, readlink_request, readlink_response,
                   readlink_handler, NULL);

}

void usage(const char *appname)
{

}

char * mds_data_file(){
    static char tmp [1024];
    char * cfgfile = cfg_getstr("CFG_FILE", "");
    char *bname = basename(cfgfile);
    sprintf(tmp, "data/%s.data", bname);
    return tmp;
}

void onexit()
{
    fs_store(mds_data_file());
}

double cpu_usage(){
    static long double a[4], b[4],loadavg;
    FILE *fp;

    fp = fopen("/proc/stat","r");
    fscanf(fp,"%*s %Lf %Lf %Lf %Lf",&b[0],&b[1],&b[2],&b[3]);

    loadavg = ((b[0]+b[1]+b[2]) - (a[0]+a[1]+a[2])) / ((b[0]+b[1]+b[2]+b[3]) - (a[0]+a[1]+a[2]+a[3]));
    a[0] = b[0];
    a[1] = b[1];
    a[2] = b[2];
    a[3] = b[3];
    fclose(fp);
    return loadavg;

}

static void update_clustermap_from_cmgr_on_timer_cb(evutil_socket_t fd, short what, void *arg)
{ 
    static int load_max = 0; //max for now
    int load_current = mds_op_counter;
    if (load_current > load_max)
        load_max = load_current;
    
    mds_op_counter = 0;

    int load_old = get_self_machine_load();
    int load_new = 0;
    if(load_current > load_new){
        load_new = (int) (load_old * 0.9 + load_current * 0.1);
    }else{
        load_new = load_old*0.6 + load_current*0.4;
    }

    /*logging(LOG_INFO, "load_old: %d, load_current: %d, load_new: %d, load_max: %d ",*/
            /*load_old,load_current, load_new, load_max);*/
    logging(LOG_WARN, "load_old: %d, load_current: %d, load_new: %d, load_max: %d, cpu:%lf",
            load_old,load_current, load_new, load_max, cpu_usage());

    set_self_machine_load(load_new);

    ping_send_request();


    if( (load_new > migrate_threshold) && (  1.0 * load_new/ load_max  > migrate_precent)){ // is about 0.9 max load for a long time
        logging(LOG_WARN, "it' overload ,will I migrate? ");

        ping_send_request_force_update();
        struct machine * self = get_self_machine();
        struct machine * max = cluster_get_mds_with_max_load();

        if (self->mid == max ->mid ){
            /*xxxx */
            logging(LOG_WARN, "YES, I'm the boss , do it ! migrate !");
            do_migrate();
            set_self_machine_load(load_new/2);
        }else{
            logging(LOG_WARN, "I'm not the biggest, no migrate");
            
        }
    }
}


fsnode * locate_hot_sub_tree(fsnode * root){
    DBG();

    fsnode *n = root->data.ddata.children;
    if (n == 0x1000) //奇怪的bug FIXME
        logging(LOG_INFO, "n == 0x1000  at locate_hot_sub_tree : %"PRIu64"  %s", root ->ino, root->name);
    fsnode *rst = n;
    int max_access_count = 0;
    fsnode *p;
    if (n != NULL) {
        dlist_t *head = &(n->tree_dlist);
        dlist_t *pl;
        for (pl = head->next; pl != head; pl = pl->next) {
            p = dlist_data(pl, fsnode, tree_dlist);
            if (p->access_counter > max_access_count){
                rst = p;
                max_access_count = p->access_counter;
            }
            p->access_counter = 0; 
        }
    }
    return rst;
}

void do_migrate2(fsnode * r){
    DBG();
    struct machine * m = cluster_get_mds_with_lowest_load();
    struct machine * self = get_self_machine();
    logging(LOG_WARN, "migrate on %"PRIu64". %s, tree_cnt : %"PRIu64" ----- from %d to %d", r->ino, r->name, r->tree_cnt, self->mid, m->mid);
    migrate_send_request(m->ip, m->port, r, self->mid, m->mid);
}
/*
 * 从根开始，找下一层目录中access最多(最热)，，设为x，
 * 然后在x的儿子中找，也是找最热的一个，直到找到一个，它的大小小于1w个inode，就迁移它
 * 每次找的过程中，所有访问过的node, access_cnt都重置
 * */
void do_migrate(){
    DBG();

    fsnode * n = fsnode_hash_find(FS_ROOT_INO);
    if (n==NULL) //对于没有root的MDS，目前没实现,FIXME
        return ;
    logging(LOG_INFO, "the finding root @ do_migrate is : %"PRIu64"  %s", n->ino, n->name);
    while(1){
        n = locate_hot_sub_tree(n);
        if (n == NULL)
            return ;
        logging(LOG_INFO, "the finding hot @ do_migrate is : %"PRIu64"  %s", n->ino, n->name);
        if (!S_ISDIR(n->mode))
            return;
        if (n->tree_cnt == 0)
            return;
        if (n->tree_cnt < migrate_count && n->tree_cnt > 10) {// 1w
            if (n ->ino == FS_VROOT_INO || n->ino == FS_ROOT_INO)
                return ;
            do_migrate2(n);
            return;
        }
    }

}

int main(int argc, char **argv)
{
    init_app(argc, argv, "mds");
    event_init();

    fs_init();


    fs_load(mds_data_file());
    rpc_setup();
    char *self_host = cfg_getstr("MDS2CLIENT_LISTEN_HOST", "*");
    int self_port = cfg_getint32("MDS2CLIENT_LISTEN_PORT", 9527);
    rpc_client_setup(self_host, self_port, MACHINE_MDS);


    struct timeval five_seconds = {2,0};
    struct event *update_clustermap_event= event_new(NULL, -1, EV_PERSIST, update_clustermap_from_cmgr_on_timer_cb, NULL);
    event_add(update_clustermap_event, &five_seconds);


    event_dispatch();
    return 0;
}
