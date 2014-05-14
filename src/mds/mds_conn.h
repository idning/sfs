
inline void replace_pos(fsnode * n, int from, int to);
int migrate_send_request(char * ip, int port, fsnode * root, int from_mds, int to_mds);
void migrate_handler(EVRPC_STRUCT(rpc_migrate) * rpc, void *arg);
