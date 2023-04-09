//
// Created by Jan on 07.04.2023.
//

#ifndef HW03_CONTAINERS_H
#define HW03_CONTAINERS_H

#endif //HW03_CONTAINERS_H
#include "stdbool.h"

enum garbage_type {
    Plastic = 1, Paper, Bio, Clear, Colored, Textile
};

struct container_t {
    unsigned int id;

    double coordinates_x;
    double coordinates_y;
    int group;

    enum garbage_type garb_type;
    unsigned int capacity;
    char *name;
    char *street;
    unsigned int house_number;
    bool public;

    unsigned int *neighbours; // will contain sorted ids of neighbours - still can contain same ids
    int neighbour_count;
};

struct all_containers{
    // !!! ON HEAP !!!
    struct container_t *containers;
    int amount;

    struct group *groups;
    int group_amount;
};

struct group{
    int id;
    double coordinates_x;
    double coordinates_y;

    unsigned int *containers;
    int container_count;
    int alloc_size;
    enum garbage_type *garbage_types;
};


int try_neighbour(int index, unsigned int wanted_id, unsigned int *neighbour);

int compare_ints(const void* a, const void* b);

bool fill_neighbours(struct container_t *container);

int load_container(int line_index, struct container_t *container);

bool parse_input(struct all_containers *all_containers);

bool deep_free_all_containers(struct all_containers *all_conts);

void free_struct_all_containers(struct all_containers *all_conts);


bool make_new_group(struct group *pointer_group, int id, double x, double y, unsigned int first_id, enum garbage_type garbage);

bool add_to_group(struct group *group, unsigned int id, enum garbage_type garbage);

void free_groups(struct all_containers *all_conts);

bool groupify(struct all_containers *all_conts);
