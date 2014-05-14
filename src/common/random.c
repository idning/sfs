#include <random.h>
int rand_init()
{
    srandom(time(NULL));
    return 0;
}

/**
 * Exponential Distribution
 * The Inverse Function Method:
 * 返回[0, max-1)范围内的一个数.
 */
int rand_exp(int max)
{
    //TODO
    return 0;
}
