//
// Created by Jan on 07.04.2023.
//

#ifndef HW03_PRINTING_TOOLS_H
#define HW03_PRINTING_TOOLS_H

#endif //HW03_PRINTING_TOOLS_H
#include "filtering.h"

void print_garb_type(enum garbage_type garb);

void print_neighbours(struct container_t container);

void print_container(struct container_t container);

void print_all(const struct all_containers *all_containers);

void print_group_type(const int *types);

void print_group(struct group current_group, int *group_neighbours, int size);

void print_groups(struct all_containers *all_containers);

void print_group_neighbours(const int *groups_neighbours, int size);
