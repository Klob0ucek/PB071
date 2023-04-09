#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stdbool.h"


// TODO: [Optional] Think about an appropriate division of the solution into files.
//                  e.g. Move data loading and validating to another .c file.

#include "printing_tools.h"

bool filter_types(const char *filter_str, enum garbage_type **filters) {
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
            fprintf(stderr, "Invalid filter type");
            return false;
        }
        index++;
    }
    *filters = filter_array;

    return true;
}

void parse_interval(char *s, unsigned int *x, unsigned int *y) {
    sscanf(s, "%u-%u", x, y);
}

bool private_filter(char *input, int *want_private) {
    if (strcmp(input, "Y") == 0) {
        *want_private = 1;
    } else if (strcmp(input, "N") == 0) {
        *want_private = 0;
    } else {
        return false;
    }
    return true;
}

bool keep_container(const enum garbage_type *filters, bool use_capacity, const unsigned int min, const unsigned int max,
        const int want_private, struct container_t *container) {
    bool keep_type = true;
    if (filters != NULL) {
        keep_type = false;
        for (int i = 0; i < 6; ++i) {
            if (container->garb_type == filters[i]) {
                keep_type = true;
                break;
            }
        }
    }
    bool keep_capacity = true;
    if (use_capacity) {
        keep_capacity = false;
        if (min <= container->capacity && container->capacity <= max) {
            keep_capacity = true;
        }
    }
    bool keep_private = true;
    if (want_private != -1) {
        keep_private = false;
        if ((want_private == 1 && container->public) || (want_private == 0 && !container->public)){
            keep_private = true;
        }
    }
    return keep_type && keep_capacity && keep_private;
}

bool filter_containers(const enum garbage_type *filters, bool use_capacity, unsigned int min, unsigned int max,
        int want_private, struct all_containers *all_conts){
    int cont_size = 10;
    struct container_t *filtered_containers = malloc(sizeof(struct container_t) * cont_size);
    if (filtered_containers == NULL) {
        perror("Malloc Failre");
        return false;
    }

    int index = 0;
    for (int i = 0; i < all_conts->amount; ++i){
        struct container_t current_container = all_conts->containers[i];
        if (keep_container(filters, use_capacity, min, max, want_private, &current_container)){
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

    struct all_containers all = {filtered_containers, index, NULL, 0};
    *all_conts = all;
    return true;
}

int main(int argc, char *argv[]) {

    const char* cont_path_test = argv[argc - 2];
    const char* road_path_test = argv[argc - 1];


    struct all_containers all_containers;
    parse_input(&all_containers, cont_path_test, road_path_test);

    if (argc < 3){
        fprintf(stderr, "Not Enough Arguments");
        return EXIT_FAILURE;
    }
    if (argc == 3) {
        print_all(&all_containers);
        deep_free_all_containers(&all_containers);
        return EXIT_SUCCESS;
    }
    if (argc == 4 && strcmp(argv[1], "-s") == 0) {
        if (!groupify(&all_containers)){
            fprintf(stderr, "Grouping containers failed");
            return false;
        }
        deep_free_all_containers(&all_containers);
        return EXIT_SUCCESS;
        // TODO we need to print groups then

    } else {
        enum garbage_type *type_filter = NULL;
        unsigned int low = 0;
        unsigned int high = 0;
        bool use_capacity = false;
        int want_private = -1;

        for (int i = 1; i < (argc - 2); i += 2) {
            if (strcmp(argv[i], "-t") == 0) {
                if (!filter_types(argv[i + 1], &type_filter)) {
                    deep_free_all_containers(&all_containers);
                    return EXIT_FAILURE;
                }
            }
            else if (strcmp(argv[i], "-c") == 0) {
                parse_interval(argv[i + 1], &low, &high);
                use_capacity = true;
            }
            else if (strcmp(argv[i], "-p") == 0) {
                if (!(private_filter(argv[i + 1], &want_private))){
                    fprintf(stderr, "Invalid filter option");
                    deep_free_all_containers(&all_containers);
                    return EXIT_FAILURE;
                }
            } else {
                fprintf(stderr, "Unknown argument: Use -t, -c or -p with appropriate values");
                deep_free_all_containers(&all_containers);
                return EXIT_FAILURE;
            }
        }

        if (!filter_containers(type_filter, use_capacity, low, high, want_private, &all_containers)){
            fprintf(stderr, "Data filtering failed");
            deep_free_all_containers(&all_containers);
            return EXIT_FAILURE;
        }

        print_all(&all_containers);
        free(type_filter);
        type_filter = NULL;
    }
    deep_free_all_containers(&all_containers);

    return EXIT_SUCCESS; // May your program be as successful as this macro. Good luck!
}
