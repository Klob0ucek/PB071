//
// Created by Jan on 10.04.2023.
//
#include "stdbool.h"

#ifndef HW03_GENERAL_STRUCTS_H
#define HW03_GENERAL_STRUCTS_H

enum garbage_type
{
    Plastic = 1,
    Paper,
    Bio,
    Clear,
    Colored,
    Textile
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

#endif //HW03_GENERAL_STRUCTS_H
