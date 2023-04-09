//
// Created by Jan on 07.04.2023.
//

#ifndef HW03_PRINTING_TOOLS_H
#define HW03_PRINTING_TOOLS_H

#endif //HW03_PRINTING_TOOLS_H
#include "containers.h"

char *garbage_type_to_string(enum garbage_type garb);

void print_neighbours(struct container_t container);

bool print_container(struct container_t container);

void print_all(const struct all_containers *all_containers);