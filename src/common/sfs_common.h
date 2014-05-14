#ifndef _SFS_COMMON_H_
#define _SFS_COMMON_H_

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <signal.h>

#include <event.h>
#include <evhttp.h>
#include <evutil.h>

#include "app.h"
#include "cfg.h"
#include "cluster.h"
#include "dlist.h"
#include "hashtable.h"
#include "http_client.h"
#include "log.h"
#include "network.h"
#include "protocol.gen.h"
#include "protocol.h"
#include "random.h"
#include "connection_pool.h"

// load_new should > migrate_threshold  
#define migrate_threshold 1000
#define migrate_precent 0.65
#define migrate_count 50000

//#define migrate_threshold 50
//#define migrate_precent 0
//#define migrate_count 500




#endif /* _SFS_COMMON_H_ */
