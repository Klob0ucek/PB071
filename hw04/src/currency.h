#include "errors.h"
#include "utils.h"

#include <stdint-gcc.h>

#ifndef CURRENCY_H
#define CURRENCY_H

struct currency
{
    char *name;
    int64_t rating;
};

struct currency_table
{
    struct object _object;
    struct currency *currencies;
    char *main_currency;
    int size;
    int capacity;
};

void init_currency_table(struct currency_table *table);
void add_currency(struct currency_table *table, const char *name, int64_t rating);
int64_t convert_currency(struct currency_table *table, int64_t amount, const char *currency);


#endif
