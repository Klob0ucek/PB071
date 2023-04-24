//
// Created by Jan on 10.04.2023.
//
#include <limits.h>
#include "filtering.h"

#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

bool keep_container(const struct filter *filter, struct container_t *container)
{
    bool keep_type = true;
    if (filter->type_filter != NULL) {
        keep_type = false;
        for (int i = 0; i < 6; ++i) {
            if (container->garb_type == filter->type_filter[i]) {
                keep_type = true;
                break;
            }
        }
    }
    bool keep_capacity = true;
    if (filter->use_capacity) {
        keep_capacity = false;
        if (filter->low <= container->capacity && container->capacity <= filter->high) {
            keep_capacity = true;
        }
    }
    bool keep_private = true;
    if (filter->want_private != -1) {
        keep_private = false;
        if ((filter->want_private == 1 && container->public) || (filter->want_private == 0 && !container->public)) {
            keep_private = true;
        }
    }
    return keep_type && keep_capacity && keep_private;
}

bool private_filter(char *input, struct filter *filter)
{
    if (strcmp(input, "Y") == 0) {
        filter->want_private = 1;
    } else if (strcmp(input, "N") == 0) {
        filter->want_private = 0;
    } else {
        return false;
    }
    return true;
}

bool parse_interval(char *s, struct filter *filter) {
    sscanf(s, "%u-%u", &filter->low, &filter->high);
    if (filter->high == UINT_MAX || filter->low == UINT_MAX) {
        fprintf(stderr, "Invalid capacity value\n");
        return false;
    }
    return true;
}

bool filter_types(const char *filter_str, enum garbage_type **filters)
{
    enum garbage_type *filter_array = calloc(6, sizeof(enum garbage_type));
    if (filter_array == NULL) {
        perror("Malloc Failure");
        return false;
    }

    int index = 0;
    while (true) {
        if (filter_str[index] == '\0') {
            break;
        } else if (filter_str[index] == 'A') {
            filter_array[index] = Plastic;
        } else if (filter_str[index] == 'P') {
            filter_array[index] = Paper;
        } else if (filter_str[index] == 'B') {
            filter_array[index] = Bio;
        } else if (filter_str[index] == 'G') {
            filter_array[index] = Clear;
        } else if (filter_str[index] == 'C') {
            filter_array[index] = Colored;
        } else if (filter_str[index] == 'T') {
            filter_array[index] = Textile;
        } else {
            fprintf(stderr, "Invalid filter type\n");
            free(filter_array);
            return false;
        }
        index++;
    }
    *filters = filter_array;

    return true;
}

bool filter_containers(const struct filter *filter, struct all_containers *all_conts)
{
    int cont_size = 10;
    struct container_t *filtered_containers = malloc(sizeof(struct container_t) * cont_size);
    if (filtered_containers == NULL) {
        perror("Malloc Failre");
        return false;
    }

    int index = 0;
    for (int i = 0; i < all_conts->amount; ++i) {
        struct container_t current_container = all_conts->containers[i];
        if (keep_container(filter, &current_container)) {
            if (index == cont_size) {
                cont_size *= 2;
                struct container_t *new_filtered_containers = realloc(filtered_containers, sizeof(struct container_t) * cont_size);
                if (new_filtered_containers == NULL) {
                    perror("Realloc Failure");
                    free(filtered_containers);
                    return false;
                }
                filtered_containers = new_filtered_containers;
            }
            filtered_containers[index] = current_container;
            index++;
        } else {
            free_container(current_container);
        }
    }

    free_struct_all_containers(all_conts);

    struct all_containers all = { filtered_containers, index, NULL, 0 };
    *all_conts = all;
    return true;
}
