//
// Created by Jan on 10.04.2023.
//
#include "stdbool.h"

#ifndef HW03_GENERAL_STRUCTS_H
#define HW03_GENERAL_STRUCTS_H

enum garbage_type
{
    Plastic = 'A',
    Paper = 'P',
    Bio = 'B',
    Clear = 'G',
    Colored = 'C',
    Textile = 'T'
};

struct container_t
{
    unsigned int id;

    double coordinates_x;
    double coordinates_y;
    int group;

    enum garbage_type garb_type;
    unsigned int capacity;
    // on heap
    char *name;
    // on heap
    char *street;
    unsigned int house_number;
    bool public;

    // on heap
    unsigned int *neighbours; // will contain sorted ids of neighbours - still can contain same ids
    int neighbour_count;
};

struct all_containers
{
    // on heap
    struct container_t *containers;
    int amount;

    // on heap
    struct group *groups;
    int group_amount;
};

struct group
{
    int id;
    double coordinates_x;
    double coordinates_y;

    // on heap
    unsigned int *containers;
    int container_count;
    int alloc_size;
    // on heap
    int *garbage_types;
};

struct filter
{
    enum garbage_type *type_filter;

    unsigned int low;
    unsigned int high;
    bool use_capacity;

    int want_private;
};

#endif //HW03_GENERAL_STRUCTS_H
