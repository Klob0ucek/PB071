//
// Created by Jan on 10.04.2023.
//

#ifndef HW03_FILTERING_H
#define HW03_FILTERING_H

#endif //HW03_FILTERING_H

#include "containers.h"
#include "stdbool.h"

bool filter_types(char *filter_str, struct filter *filter);

bool parse_interval(char *s, struct filter *filter);

bool private_filter(char *input, struct filter *filter);

bool keep_container(const struct filter *filter, struct container_t *container);

bool filter_containers(const struct filter *filter, struct all_containers *all_conts);
