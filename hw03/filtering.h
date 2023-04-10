//
// Created by Jan on 10.04.2023.
//

#ifndef HW03_FILTERING_H
#define HW03_FILTERING_H

#endif //HW03_FILTERING_H

#include "containers.h"
#include "stdbool.h"

bool filter_types(const char *filter_str, enum garbage_type **filters);

void parse_interval(char *s, unsigned int *x, unsigned int *y);

bool private_filter(char *input, int *want_private);

bool keep_container(const enum garbage_type *filters, bool use_capacity, unsigned int min, unsigned int max, int want_private, struct container_t *container);

bool filter_containers(const enum garbage_type *filters, bool use_capacity, unsigned int min, unsigned int max, int want_private, struct all_containers *all_conts);
