#include <stdint-gcc.h>
#include <time.h>

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

enum load_type
{
    CURRENCY,
    RATING
};

#endif
