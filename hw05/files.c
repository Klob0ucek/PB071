//
// Created by Jan on 08.05.2023.
//

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
#include "files.h"

char *find_name_from_path(char *path) {
    char *result;
    char *name_start = strrchr(path, '/');
    if (name_start == NULL) {
        result = strdup(path);
    } else {
        name_start++;
        result = strdup(name_start);
    }
    if (result == NULL) {
        fprintf(stderr, "Name alloc failed");
    }
    return result;
}

char *add_name_to_path(char *path, char *name) {
    strcat(path, "/");
    strcat(path, name);
    return path;
}

char *remove_name_from_path(char *path) {
    char *forward = strrchr(path, '/');
    *forward = '\0';
    return path;
}

void add_sum(struct item *item, size_t *dir_size, size_t *dir_blocks) {
    if (item->item_type == FOLDER) {
        *dir_blocks += item->item_pointer.folder.blocks;
        *dir_size += item->item_pointer.folder.size;
    } else if (item->item_type == NORM_FILE) {
        *dir_blocks += item->item_pointer.file.blocks;
        *dir_size += item->item_pointer.file.size;
    }
}

bool load_file(char *path, struct item *item) {
    struct stat st;
    if (stat(path, &st)) {
        return false;
    }

    struct file this = { st.st_size, st.st_blksize, find_name_from_path(path) };
    union item_holder holder = { this };
    struct item result = { NORM_FILE, holder };
    *item = result;
    return true;
}

bool load_dir(char *path, struct item *result_dir) {
    struct folder folder = { 0, 0, false, find_name_from_path(path), NULL, 0 };
    union item_holder holder;
    holder.folder = folder;
    struct item item = { FOLDER, holder };

    size_t dir_size = 0;
    size_t dir_blocks = 0;

    int size = 10;
    int index = 0;
    struct item *children = malloc(sizeof(struct item) * size);
    if (children == NULL) {
        fprintf(stderr, "Dir malloc failed\n");
        return false;
    }

    DIR *dir = NULL;
    if ((dir = opendir(path)) != NULL) {
        struct dirent *dir_entry = NULL;
        while ((dir_entry = readdir(dir)) != NULL) {
            if (*dir_entry->d_name == '.') {
                continue;
            }
            add_name_to_path(path, dir_entry->d_name);
            if (index == size) {
                size *= 2;
                struct item *new = realloc(children, sizeof(struct item) * size);
                if (new == NULL) {
                    perror("Realloc failed\n");
                    free(children);
                    return false;
                }
                children = new;
            }
            struct item loaded;
            if (!load_item(path, &loaded)) {
                item.item_pointer.folder.error_flag = true;
                remove_name_from_path(path);
                continue;
            }
            add_sum(&loaded, &dir_size, &dir_blocks);
            children[index] = loaded;
            ++index;

            remove_name_from_path(path);
        }
        closedir(dir);
    }

    item.item_pointer.folder.size = dir_size;
    item.item_pointer.folder.blocks = dir_blocks;
    item.item_pointer.folder.children = children;
    item.item_pointer.folder.amount_of_items = index;
    *result_dir = item;
    return true;
}

bool load_item(char *path, struct item *item)
{
    struct stat file_stats;
    stat(path, &file_stats);

    if (S_ISDIR(file_stats.st_mode)) {
        return load_dir(path, item);
    } else if (S_ISREG(file_stats.st_mode)) {
        return load_file(path, item);
    } else {
        return false;
    }
}


void free_item(struct item *item) {
    if (item->item_type == NORM_FILE) {
        free(item->item_pointer.file.name);
        item->item_pointer.file.name = NULL;
    } else {
        for (int i = 0; i < item->item_pointer.folder.amount_of_items; i++) {
            free_item(&item->item_pointer.folder.children[i]);
        }
        free(item->item_pointer.folder.children);
        item->item_pointer.folder.children = NULL;
        free(item->item_pointer.folder.name);
        item->item_pointer.folder.name = NULL;
    }
}


