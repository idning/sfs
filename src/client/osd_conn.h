
#include "sfs_common.h"

struct write_buf *write_buf_hash_find(uint64_t ino);

void buffered_write(struct file_stat *stat, uint64_t offset, uint64_t size,
                    const uint8_t * buff);

int buffered_write_flush(struct file_stat *stat);
