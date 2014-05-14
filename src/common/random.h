#ifndef _RANDOM_H__
#define _RANDOM_H__

#include<stdlib.h>
#include<time.h>

int rand_init();

/**
 * Exponential Distribution
 * The Inverse Function Method:
 * 返回[0, max-1)范围内的一个数.
 */
int rand_exp(int max);

#endif
