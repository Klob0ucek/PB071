
#include "sorting.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#

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
    char *path = calloc(4096, sizeof(char));
    if (strcmp(original, "./") == 0) {
        getcwd(path, sizeof(char) * 4096);
    } else {
        strcpy(path, original);
    }
    return path;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Invalid amount of arguments\n");
        return 1;
    }

    struct options options = { UNSET, UNSET, -1, UNSET };

    bool skip_next = false;
    for (int i = 1; i < argc - 1; ++i) {
        if (skip_next) {
            skip_next = false;
            continue;
        }
        if (strcmp(argv[i], "-a") == 0) {
            if (options.block_size == UNSET) {
                options.block_size = OPTION_SET;
                continue;
            } else {
                fprintf(stderr, "Filter %s used twice\n", argv[i]);
                return EXIT_FAILURE;
            }
        } else if (strcmp(argv[i], "-s") == 0) {
            if (options.size_sorted == UNSET) {
                options.size_sorted = OPTION_SET;
                continue;
            } else {
                fprintf(stderr, "Filter %s used twice\n", argv[i]);
                return EXIT_FAILURE;
            }
        } else if (strcmp(argv[i], "-d") == 0) {
            if (options.depth == -1) {
                skip_next = true;
                if (!parse_depth(&options, argv[i + 1])) {
                    return EXIT_FAILURE;
                }
                continue;
            } else {
                fprintf(stderr, "Filter %s used twice\n", argv[i]);
                return EXIT_FAILURE;
            }
        } else if (strcmp(argv[i], "-p") == 0) {
            if (options.percent == UNSET) {
                options.percent = OPTION_SET;
            } else {
                fprintf(stderr, "Filter %s used twice\n", argv[i]);
                return EXIT_FAILURE;
            }
        } else {
            fprintf(stderr, "Invalid argument\n");
            return EXIT_FAILURE;
        }
        skip_next = false;
    }
    close_options(&options);

    char *root_path = argv[argc - 1];
    char *long_path = max_path(root_path);

    struct item *item;
    if ((item = load_item(long_path)) == NULL) {
        fprintf(stderr, "Initial item not loaded\n");
        free(long_path);
        return EXIT_FAILURE;
    }
    free(long_path);

    sort_tree(item, &options);
    print_tree(item, &options);

    free_item(item);
    return 0;
}
