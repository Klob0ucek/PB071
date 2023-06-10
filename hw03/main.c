#include "printing_tools.h"
#include "stdbool.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool check_parameter(const char *arg1, const char *arg2, const char *cont_path_test, const char *road_path_test)
{
    if (strcmp(arg1, "-t") != 0 &&
            strcmp(arg1, "-p") != 0 &&
            strcmp(arg1, "-c") != 0) {
        fprintf(stderr, "Invalid argument\n");
        return false;
    }
    if (strcmp(arg2, cont_path_test) == 0 ||
            strcmp(arg2, road_path_test) == 0 ||
            strcmp(arg2, "-s") == 0 ||
            strcmp(arg2, "-t") == 0 ||
            strcmp(arg2, "-p") == 0 ||
            strcmp(arg2, "-c") == 0) {
        fprintf(stderr, "Missing argument parameter\n");
        return false;
    }
    return true;
}

void free_structure(struct all_containers *all_containers, enum garbage_type *type_filter)
{
    deep_free_all_containers(all_containers);
    if (type_filter != NULL) {
        free(type_filter);
    }
}

bool process_data(const char *cont_path_test, const char *road_path_test, struct filter *filter, bool want_groups)
{
    struct all_containers all_containers;
    if (!parse_input(&all_containers, cont_path_test, road_path_test)) {
        return EXIT_FAILURE;
    }
    if (filter == NULL && !want_groups) {
        print_all(&all_containers);
        free_structure(&all_containers, NULL);
        return EXIT_SUCCESS;

    } else if (want_groups) {
        if (!groupify(&all_containers)) {
            fprintf(stderr, "Grouping containers failed\n");
            free_structure(&all_containers, NULL);
            return EXIT_FAILURE;
        }
        print_groups(&all_containers);
        free_structure(&all_containers, NULL);
        return EXIT_SUCCESS;

    } else if (filter != NULL) {
        if (!filter_containers(filter, &all_containers)) {
            fprintf(stderr, "Data filtering failed\n");
            free_structure(&all_containers, NULL);
            return EXIT_FAILURE;
        }

        print_all(&all_containers);
        free_structure(&all_containers, filter->type_filter);
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "Invalid inputs");
        free_structure(&all_containers, filter->type_filter);
        return EXIT_FAILURE;
    }
}

bool filtering_options(char *filter_str, struct filter *filter, bool operation(char *, struct filter *))
{
    if (!operation(filter_str, filter)) {
        if (filter->type_filter != NULL) {
            free(filter->type_filter);
            filter->type_filter = NULL;
        }
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Not Enough Arguments\n");
        return EXIT_FAILURE;
    }

    const char *cont_path_test = argv[argc - 2];
    const char *road_path_test = argv[argc - 1];

    if (argc == 3) {
        return process_data(cont_path_test, road_path_test, NULL, false);
    }

    if (argc == 4 && strcmp(argv[1], "-s") == 0) {
        return process_data(cont_path_test, road_path_test, NULL, true);
    } else {
        struct filter filter = { NULL, UINT_MAX, UINT_MAX, false, -1 };

        for (int i = 1; i < (argc - 2); i += 2) {
            if (!check_parameter(argv[i], argv[i + 1], cont_path_test, road_path_test)) {
                if (filter.type_filter != NULL) {
                    free(filter.type_filter);
                    filter.type_filter = NULL;
                };
                return EXIT_FAILURE;
            }

            if (strcmp(argv[i], "-t") == 0) {
                if (!filtering_options(argv[i + 1], &filter, filter_types)) {
                    return EXIT_FAILURE;
                }

            } else if (strcmp(argv[i], "-c") == 0) {
                if (!filtering_options(argv[i + 1], &filter, parse_interval)) {
                    return EXIT_FAILURE;
                }

            } else if (strcmp(argv[i], "-p") == 0) {
                if (!filtering_options(argv[i + 1], &filter, private_filter)) {
                    fprintf(stderr, "Invalid filter option\n");
                    return EXIT_FAILURE;
                }

            } else {
                fprintf(stderr, "Unknown argument: Use -t, -c or -p\n");
                if (filter.type_filter != NULL) {
                    free(filter.type_filter);
                    filter.type_filter = NULL;
                }
                return EXIT_FAILURE;
            }
        }
        process_data(cont_path_test, road_path_test, &filter, false);
    }

    return EXIT_SUCCESS; // May your program be as successful as this macro. Good luck!
}
