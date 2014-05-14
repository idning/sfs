#include "protocol.gen.h"
#include "protocol.h"

#ifndef _CLUSTER_H__
#define _CLUSTER_H__

#define MACHINE_CMGR    1
#define MACHINE_MDS     2
#define MACHINE_OSD     3
#define MACHINE_CLIENT  4

void ping_handler(EVRPC_STRUCT(rpc_ping) * rpc, void *arg);
int select_osd();

int ping_send_request();
int ping_send_request_force_update();
void rpc_client_setup(char *self_host, int self_port, int self_type);
struct machine *cluster_get_machine_by_mid(int mid);

void cluster_get_mds_arr(int **o_arr, int *o_cnt);

void cluster_get_osd_arr(int **o_arr, int *o_cnt);
int cluster_get_osd_cnt();

void set_self_machine_load(int load);
int get_self_machine_load();

int cluster_get_current_version();
struct machine *cluster_get_mds_with_lowest_load();
struct machine *cluster_get_mds_with_max_load();
struct machine * get_self_machine();
#endif
