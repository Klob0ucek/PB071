#include "printing_tools.h"
#include "stdbool.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int UNSIGNED_INT_MAX = 4294967294;

bool check_parameter(const char* arg1, const char* arg2, const char *cont_path_test, const char *road_path_test){
    if (strcmp(arg1, "-t") != 0 &&
        strcmp(arg1, "-p") != 0 &&
        strcmp(arg1, "-c") != 0){
        fprintf(stderr, "Invalid argument\n");
        return false;
    }
    if (strcmp(arg2, cont_path_test) == 0 ||
        strcmp(arg2, road_path_test) == 0 ||
        strcmp(arg2, "-s") == 0 ||
        strcmp(arg2, "-t") == 0 ||
        strcmp(arg2, "-p") == 0 ||
        strcmp(arg2, "-c") == 0){
        fprintf(stderr, "Missing argument parameter\n");
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
            if (!check_parameter(argv[i], argv[i + 1], cont_path_test, road_path_test)){
                deep_free_all_containers(&all_containers);
                if (type_filter != NULL){
                    free(type_filter);
                }
                return EXIT_FAILURE;
            }
            if (strcmp(argv[i], "-t") == 0) {
                if(type_filter != NULL){
                    fprintf(stderr, "Filter already used\n");
                    deep_free_all_containers(&all_containers);
                    free(type_filter);
                    return EXIT_FAILURE;
                }
                if (!filter_types(argv[i + 1], &type_filter)) {
                    deep_free_all_containers(&all_containers);
                    if (type_filter != NULL){
                        free(type_filter);
                    }
                    return EXIT_FAILURE;
                }
            } else if (strcmp(argv[i], "-c") == 0) {
                if(low != UNSIGNED_INT_MAX || high != UNSIGNED_INT_MAX){
                    fprintf(stderr, "Filter already used\n");
                    deep_free_all_containers(&all_containers);
                    if (type_filter != NULL){
                        free(type_filter);
                    }
                    return EXIT_FAILURE;
                }
                if (!parse_interval(argv[i + 1], &low, &high)){
                    deep_free_all_containers(&all_containers);
                    if (type_filter != NULL){
                        free(type_filter);
                    }
                    return EXIT_FAILURE;
                }
                use_capacity = true;
            } else if (strcmp(argv[i], "-p") == 0) {
                if(want_private != -1){
                    fprintf(stderr, "Filter already used\n");
                    deep_free_all_containers(&all_containers);
                    if (type_filter != NULL){
                        free(type_filter);
                    }
                    return EXIT_FAILURE;
                }
                if (!(private_filter(argv[i + 1], &want_private))) {
                    fprintf(stderr, "Invalid filter option\n");
                    deep_free_all_containers(&all_containers);
                    if (type_filter != NULL){
                        free(type_filter);
                    }
                    return EXIT_FAILURE;
                }
            } else {
                fprintf(stderr, "Unknown argument: Use -t, -c or -p\n");
                deep_free_all_containers(&all_containers);
                if (type_filter != NULL){
                    free(type_filter);
                }
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
