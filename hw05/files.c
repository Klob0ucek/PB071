//
// Created by Jan on 08.05.2023.
//

#include "files.h"

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

char *make_path(char *path, char *name)
{
    char *new = malloc((strlen(path) + strlen(name) + 2) * sizeof(char));
    if (new == NULL) {
        fprintf(stderr, "Name malloc failure\n");
        return NULL;
    }
    sprintf(new, "%s/%s", path, name);
    return new;
}

char *copy_name(char dir_name[256])
{
    char *new = calloc(256, sizeof(char));
    if (new == NULL) {
        fprintf(stderr, "Malloc failed in name_copy\n");
        return NULL;
    }
    strcpy(new, dir_name);
    return new;
}

char *find_name_from_path(char *path)
{
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

void add_sum(struct item *item, size_t *dir_size, size_t *dir_blocks)
{
    if (item->item_type == FOLDER) {
        *dir_blocks += item->item_pointer.folder.blocks;
        *dir_size += item->item_pointer.folder.size;
    } else if (item->item_type == NORM_FILE) {
        *dir_blocks += item->item_pointer.file.blocks;
        *dir_size += item->item_pointer.file.size;
    }
}

bool load_file(char *path, char *name, struct item *item)
{
    struct file this = { 0, 0, name};
    union item_holder holder = { this };
    struct item result = { NORM_FILE, holder };

    struct stat st;
    if (stat(path, &st)) {
        fprintf(stderr, "Unable to load: %s\n", path);
        *item = result;
        return false;
    }

    result.item_pointer.file.size = st.st_size;
    result.item_pointer.file.blocks = st.st_blocks * 512;
    *item = result;
    return true;
}

bool load_dir(char *path, char *name, struct item *result_dir)
{
    struct folder folder = { 0, 0, false, name, NULL, 0 };
    union item_holder holder;
    holder.folder = folder;
    struct item item = { FOLDER, holder };

    struct stat st;
    if (stat(path, &st)) {
        fprintf(stderr, "Unable to load: %s\n", path);
        *result_dir = item;
        return false;
    }

    size_t dir_size = st.st_size;
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
            if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0) {
                continue;
            }
            char *new_path = make_path(path, dir_entry->d_name);
            char *new_name = copy_name(dir_entry->d_name);

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
            if (dir_entry->d_type == DT_REG) {
                if (!load_file(new_path, new_name, &loaded)) {
                    item.item_pointer.folder.error_flag = true;
                    free(new_path);
//                    continue;
                }
            } else if (dir_entry->d_type == DT_DIR) {
                if (!load_dir(new_path, new_name, &loaded)) {
                    item.item_pointer.folder.error_flag = true;
                    free(new_path);
//                    continue;
                }
                if (loaded.item_pointer.folder.error_flag) {
                    item.item_pointer.folder.error_flag = true;
                }
            } else {
                if (!load_item(new_path, &loaded)) {
                    item.item_pointer.folder.error_flag = true;
                    free(new_path);
                    continue;
                }
            }

            free(new_path);

            add_sum(&loaded, &dir_size, &dir_blocks);
            children[index] = loaded;
            ++index;
        }
        closedir(dir);
    } else {
        free(children);
        fprintf(stderr, "Unable to load: %s\n", path);
        item.item_pointer.folder.error_flag = true;
        *result_dir = item;
        return true;
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
    if (stat(path, &file_stats)) {
        fprintf(stderr, "Unable to load: %s\n", path);
        return false;
    };

    char *name = find_name_from_path(path);
    if (S_ISDIR(file_stats.st_mode)) {
        return load_dir(path, name, item);
    } else if (S_ISREG(file_stats.st_mode)) {
        return load_file(path, name, item);
    } else {
        free(name);
        return false;
    }
}

void free_item(struct item *item)
{
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
