#include "printing_tools.h"
#include "stdbool.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int UNSIGNED_INT_MAX = 4294967295;

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Not Enough Arguments\n");
        return EXIT_FAILURE;
    }

    const char *cont_path_test = argv[argc - 2];
    const char *road_path_test = argv[argc - 1];

    struct all_containers all_containers;
    if (!parse_input(&all_containers, cont_path_test, road_path_test)) {
        return EXIT_FAILURE;
    }

    if (argc == 3) {
        print_all(&all_containers);
        deep_free_all_containers(&all_containers);
        return EXIT_SUCCESS;
    }
    if (argc == 4 && strcmp(argv[1], "-s") == 0) {
        if (!groupify(&all_containers)) {
            fprintf(stderr, "Grouping containers failed\n");
            deep_free_all_containers(&all_containers);
            return EXIT_FAILURE;
        }
        print_groups(&all_containers);
        deep_free_all_containers(&all_containers);
        return EXIT_SUCCESS;

    } else {
        enum garbage_type *type_filter = NULL;
        unsigned int low = UNSIGNED_INT_MAX;
        unsigned int high = UNSIGNED_INT_MAX;
        bool use_capacity = false;
        int want_private = -1;

        for (int i = 1; i < (argc - 2); i += 2) {
            if (strcmp(argv[i], "-t") == 0) {
                if (!filter_types(argv[i + 1], &type_filter)) {
                    deep_free_all_containers(&all_containers);
                    return EXIT_FAILURE;
                }
            } else if (strcmp(argv[i], "-c") == 0) {
                if (!parse_interval(argv[i + 1], &low, &high)){
                    return EXIT_FAILURE;
                }
                use_capacity = true;
            } else if (strcmp(argv[i], "-p") == 0) {
                if (!(private_filter(argv[i + 1], &want_private))) {
                    fprintf(stderr, "Invalid filter option\n");
                    deep_free_all_containers(&all_containers);
                    return EXIT_FAILURE;
                }
            } else {
                fprintf(stderr, "Unknown argument: Use -t, -c or -p\n");
                deep_free_all_containers(&all_containers);
                return EXIT_FAILURE;
            }
        }

        if (!filter_containers(type_filter, use_capacity, low, high, want_private, &all_containers)) {
            fprintf(stderr, "Data filtering failed\n");
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
