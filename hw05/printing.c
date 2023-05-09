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

    while (d_size >= 1024 && unit_index < 5) {
        d_size /= 1024;
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

void print_file(struct item *file, struct prefix *prefix, struct options *options, size_t max)
{
    if (prefix->error) { // {ERROR}
        printf("  ");
    }

    if (options->percent == DEFAULT) { // {SIZE}
        if (options->block_size == DEFAULT) {
            print_size(file->item_pointer.file.blocks);
        } else {
            print_size(file->item_pointer.file.size);
        }
    } else {
        if (options->block_size == DEFAULT) {
            print_percentage(file->item_pointer.file.blocks, max);
        } else {
            print_percentage(file->item_pointer.file.size, max);
        }
    }
    // {PREFIX + NAME}
    printf("%s%s\n", prefix->prefix, file->item_pointer.file.name);
}

void print_dir(struct item *dir, struct prefix *prefix, struct options *options, size_t max)
{
    if (prefix->error) { // {ERROR}
        if (dir->item_pointer.folder.error_flag) {
            printf("? ");
        } else {
            printf("  ");
        }
    }

    if (options->percent == DEFAULT) { // {SIZE}
        if (options->block_size == DEFAULT) {
            print_size(dir->item_pointer.folder.blocks);
        } else {
            print_size(dir->item_pointer.folder.size);
        }
    } else {
        if (options->block_size == DEFAULT) {
            print_percentage(dir->item_pointer.folder.blocks, max);
        } else {
            print_percentage(dir->item_pointer.folder.size, max);
        }
    }
    printf("%s%s\n", prefix->prefix, dir->item_pointer.folder.name);

    // {PREFIX + NAME}
    char *dir_prefix = prefix->prefix + (4 * prefix->depth);
    if (prefix->depth > 0 && *(dir_prefix - 4) != '\\') {
        memset(dir_prefix - 4, '|', 1);
        memset(dir_prefix - 3, ' ', 3);
    }
    if (prefix->depth > 0 && *(dir_prefix - 4) == '\\') {
        memset(dir_prefix - 4, ' ', 4);
    }

    if (dir->item_pointer.folder.error_flag && dir->item_pointer.folder.children == NULL) {
        return;
    }

    if (dir->item_pointer.folder.amount_of_items > 0) {
        for (int i = 0; i < dir->item_pointer.folder.amount_of_items - 1; i++) {
            strcpy(dir_prefix, "|-- ");
            print_item(&dir->item_pointer.folder.children[i], prefix, options, max);
        }
        strcpy(dir_prefix, "\\-- ");
        print_item(&dir->item_pointer.folder.children[dir->item_pointer.folder.amount_of_items - 1], prefix, options, max);
    }
}

size_t get_max(struct item *item, struct options *options)
{
    if (options->block_size == DEFAULT) {
        return item->item_type == FOLDER ? item->item_pointer.folder.blocks : item->item_pointer.file.blocks;
    }
    return item->item_type == FOLDER ? item->item_pointer.folder.size : item->item_pointer.file.size;
}

void print_tree(struct item *item, struct options *options)
{
    char prefix_str[512];
    memset(prefix_str, '\0', sizeof(char) * 512);
    char *p = (char *) &prefix_str;
    struct prefix prefix = { p, item->item_type == FOLDER ? item->item_pointer.folder.error_flag : false, -1 };
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
    if (item->item_type == FOLDER) {
        print_dir(item, prefix, options, max);
    } else if (item->item_type == NORM_FILE) {
        print_file(item, prefix, options, max);
    }
    prefix->depth--;
}
