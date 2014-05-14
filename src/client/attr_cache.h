
int attr_cache_init();
int attr_cache_add(struct file_stat *st);
int attr_cache_del(uint64_t ino);
struct file_stat *attr_cache_lookup(uint64_t ino);
