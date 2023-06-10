
#include "sorting.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********
 * MAIN *
 *******/

void close_options(struct options *options)
{
    if (options->block_size == UNSET) {
        options->block_size = DEFAULT;
    }
    if (options->size_sorted == UNSET) {
        options->size_sorted = DEFAULT;
    }
    if (options->depth == -1) {
        options->depth = INT_MAX;
    }
    if (options->percent == UNSET) {
        options->percent = DEFAULT;
    }
}

void init_options(struct options *options)
{
    options->block_size = UNSET;
    options->percent = UNSET;
    options->size_sorted = UNSET;
    options->depth = -1;
}

bool parse_depth(struct options *options, const char *num_str)
{
    int num = -1;
    if (strchr(num_str, '.') != NULL) {
        fprintf(stderr, "Depth number %s is decimal\n", num_str);
        return false;
    }
    sscanf(num_str, "%d", &num);
    if (num < 0) {
        fprintf(stderr, "Depth number %s is negative\n", num_str);
        return false;
    }
    options->depth = num;
    return true;
}

char *max_path(char *original)
{
    char *path = calloc(PATH_MAX_LEN, sizeof(char));
    if (path == NULL) {
        fprintf(stderr, "Malloc failed\n");
        return NULL;
    }
    strcpy(path, original);
    return path;
}

struct options *parse_options(int argc, char **argv)
{
    struct options *options = malloc(sizeof(struct options));
    if (options == NULL){
        fprintf(stderr, "Option malloc failed");
        return NULL;
    }

    init_options(options);

    bool skip_next = false;
    for (int i = 1; i < argc - 1; ++i) {
        if (skip_next) {
            skip_next = false;
            continue;
        }
        if (strcmp(argv[i], "-a") == 0) {
            if (options->block_size == UNSET) {
                options->block_size = OPTION_SET;
                continue;
            } else {
                fprintf(stderr, "Filter %s used twice\n", argv[i]);
                free(options);
                return NULL;
            }
        } else if (strcmp(argv[i], "-s") == 0) {
            if (options->size_sorted == UNSET) {
                options->size_sorted = OPTION_SET;
                continue;
            } else {
                fprintf(stderr, "Filter %s used twice\n", argv[i]);
                free(options);
                return NULL;
            }
        } else if (strcmp(argv[i], "-d") == 0) {
            if (options->depth == -1) {
                skip_next = true;
                if (!parse_depth(options, argv[i + 1])) {
                    free(options);
                    return NULL;
                }
                continue;
            } else {
                fprintf(stderr, "Filter %s used twice\n", argv[i]);
                free(options);
                return NULL;
            }
        } else if (strcmp(argv[i], "-p") == 0) {
            if (options->percent == UNSET) {
                options->percent = OPTION_SET;
            } else {
                fprintf(stderr, "Filter %s used twice\n", argv[i]);
                free(options);
                return NULL;
            }
        } else {
            fprintf(stderr, "Invalid argument\n");
            free(options);
            return NULL;
        }
        skip_next = false;
    }

    close_options(options);
    return options;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Invalid amount of arguments\n");
        return EXIT_FAILURE;
    }

    struct options *options = parse_options(argc, argv);
    if (options == NULL) {
        return EXIT_FAILURE;
    }

    char *root_path = argv[argc - 1];
    char *long_path = max_path(root_path);
    if (long_path == NULL) {
        return EXIT_FAILURE;
    }

    struct item *item;
    if ((item = load_item(long_path, long_path)) == NULL) {
        fprintf(stderr, "Initial item not loaded\n");
        free(long_path);
        return EXIT_SUCCESS;
    }

    sort_tree(item, options);
    print_tree(item, options);

    free(options);
    free_item(item);
    return EXIT_SUCCESS;
}
