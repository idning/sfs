#include <stdint.h>

#define MAX_HDD_PER_MACHINE 100

typedef struct hdd_space {
    char *path;
    uint64_t totalspace;
    uint64_t availspace;
    uint64_t leavefree;
    /*uint64_t chunkcount; */
} hdd_space;

typedef struct hdd_chunk {
    char *path;                 // the path this chunk store at 
    uint64_t chunkid;
    size_t size;
    struct hdd_chunk *next;
} hdd_chunk;

//void hdd_get_space(hdd_space * space);
void hdd_refresh_usage(hdd_space * hdd);

void hdd_init(char *config_file);

int select_hdd();

hdd_chunk *chunk_hashtable_get(uint64_t chunkid);
hdd_chunk *hdd_create_chunk(uint64_t chunkid, size_t size);
hdd_chunk *hdd_md5_chunk(uint64_t chunkid);
void hdd_chunk_printf(hdd_chunk * chunk);
