//
// Created by Jan on 08.05.2023.
//

#include "printing.h"

#include "math.h"

#include <stdio.h>
#include <string.h>

/************
 * PRINTING *
 ***********/

void print_size(size_t size)
{
    static const char *units[] = { "B  ", "KiB", "MiB", "GiB", "TiB", "PiB" };
    int unit_index = 0;
    double d_size = (double) size;

    while (d_size >= UNIT_RANGE && unit_index < 5) {
        d_size /= UNIT_RANGE;
        unit_index++;
    }
    double rounded = floor(d_size * 10) / 10.0;
    printf("%6.1f %s ", rounded, units[unit_index]);
}

void print_percentage(size_t num, size_t max)
{
    double percentage = (num / (double) max) * 100;
    printf("%5.1f%% ", percentage);
}

void print_size_from_options(struct item *item, struct options *options, size_t max)
{
    if (options->percent == DEFAULT) {
        if (options->block_size == DEFAULT) {
            print_size(item->block_size);
        } else {
            print_size(item->real_size);
        }
    } else {
        if (options->block_size == DEFAULT) {
            print_percentage(item->block_size, max);
        } else {
            print_percentage(item->real_size, max);
        }
    }
}

void print_file(struct item *file, struct prefix *prefix, struct options *options, size_t max)
{
    if (prefix->error) { // {ERROR}
        printf("  ");
    }

    print_size_from_options(file, options, max); // {SIZE}

    // {PREFIX + NAME}
    printf("%s%s\n", prefix->prefix, file->name);
}

void print_dir(struct item *dir, struct prefix *prefix, struct options *options, size_t max)
{
    if (prefix->error) { // {ERROR}
        if (dir->error) {
            printf("? ");
        } else {
            printf("  ");
        }
    }

    print_size_from_options(dir, options, max); // {SIZE}

    printf("%s%s\n", prefix->prefix, dir->name);

    // {PREFIX + NAME}
    char *dir_prefix = prefix->prefix + (4 * prefix->depth);
    if (prefix->depth > 0 && *(dir_prefix - 4) != '\\') {
        memset(dir_prefix - 4, '|', 1);
        memset(dir_prefix - 3, ' ', 3);
    }
    if (prefix->depth > 0 && *(dir_prefix - 4) == '\\') {
        memset(dir_prefix - 4, ' ', 4);
    }

    if (dir->items == NULL) {
        return;
    }

    if (dir->items_amount > 0) {
        for (int i = 0; i < dir->items_amount - 1; i++) {
            strcpy(dir_prefix, "|-- ");
            print_item(dir->items[i], prefix, options, max);
        }
        strcpy(dir_prefix, "\\-- ");
        print_item(dir->items[dir->items_amount - 1], prefix, options, max);
    }
}

size_t get_max(struct item *item, struct options *options)
{
    if (options->block_size == DEFAULT) {
        return item->block_size;
    }
    return item->real_size;
}

void print_tree(struct item *item, struct options *options)
{
    char prefix_str[PREFIX_SIZE];
    memset(prefix_str, '\0', sizeof(char) * PREFIX_SIZE);
    char *p = (char *) &prefix_str;
    struct prefix prefix = { p, item->error, -1 };
    size_t max = get_max(item, options);
    print_item(item, &prefix, options, max);
}

void print_item(struct item *item, struct prefix *prefix, struct options *options, size_t max)
{
    prefix->depth++;
    if (prefix->depth - 1 >= options->depth) {
        prefix->depth--;
        return;
    }
    if (item->type == FOLDER) {
        print_dir(item, prefix, options, max);
    } else if (item->type == NORM_FILE) {
        print_file(item, prefix, options, max);
    }
    prefix->depth--;
}
