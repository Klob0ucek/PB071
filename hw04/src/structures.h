#include <time.h>
#include <stdint-gcc.h>

#ifndef STRUCTURES_H
#define STRUCTURES_H

struct transaction
{
    struct person *source;
    struct person *target;
    int64_t amount;
    char *currency;
};

struct transactions
{
    struct transaction *transactions;
    int size;
    int capacity;
};

#endif
