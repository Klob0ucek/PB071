//
// Created by Jan on 29.04.2023.
//

#include "stdlib.h"

#include <stdbool.h>

#ifndef HW05_STRUCTURES_H
#define HW05_STRUCTURES_H

#endif //HW05_STRUCTURES_H

#ifndef STAT_FAILED
#define STAT_FAILED (-1)
#endif

#ifndef NAME_MAX_LEN
#define NAME_MAX_LEN 256
#endif

#ifndef SIZE_OF_BLOCK
#define SIZE_OF_BLOCK 512
#endif

#ifndef PREFIX_SIZE
#define PREFIX_SIZE 512
#endif

#ifndef UNIT_RANGE
#define UNIT_RANGE 1024
#endif

#ifndef PATH_MAX_LEN
#define PATH_MAX_LEN 4096
#endif

enum option_values
{
    UNSET,
    DEFAULT,
    OPTION_SET
};

struct options
{
    enum option_values block_size;
    enum option_values size_sorted;
    int depth; // -1 = unset, ... set depth
    enum option_values percent;
};

enum item_type
{
    NORM_FILE,
    FOLDER
};

struct item
{
    char *name;
    size_t block_size;
    size_t real_size;
    enum item_type type;
    struct item **items;
    int items_amount;
    bool error;
};

struct prefix
{
    char *prefix;
    bool error;
    int depth;
};
