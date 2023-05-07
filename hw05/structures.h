//
// Created by Jan on 29.04.2023.
//

#include "stdlib.h"

#include <stdbool.h>

#ifndef HW05_STRUCTURES_H
#define HW05_STRUCTURES_H

#endif //HW05_STRUCTURES_H

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

struct file
{
    size_t size;
    size_t blocks;
    char *name; // on heap
};

struct folder
{
    size_t size;
    size_t blocks;
    bool error_flag;
    char *name;            // on heap
    struct item *children; // on heap
    int amount_of_items;
};

enum item_type
{
    NORM_FILE,
    FOLDER
};

union item_holder
{
    struct file file;
    struct folder folder;
};

struct item
{
    enum item_type item_type;
    union item_holder item_pointer;
};

struct prefix
{
    char *prefix;
    bool error;
    int depth;
};
