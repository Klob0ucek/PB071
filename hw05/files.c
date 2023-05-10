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


void add_sum(struct item *item, size_t *dir_size, size_t *dir_blocks)
{
    *dir_blocks += item->block_size;
    *dir_size += item->real_size;
}

struct item *load_file(char *path, char *name)
{
    struct item *file = malloc(sizeof(struct item));
    if (!file) {
        fprintf(stderr, "Malloc has failed!\n");
        return NULL;
    }

    struct stat st;
    if (stat(path, &st)) {
        fprintf(stderr, "Unable to load: %s\n", path);
        return NULL;
    }

    file->name = name;
    file->block_size = st.st_blocks * 512;
    file->real_size = st.st_size;
    file->type = NORM_FILE;

    file->items_amount = 0;
    file->items = NULL;
    file->error = false;

    return file;
}

struct item *load_dir(char *path, char *name)
{
    struct item *dir = malloc(sizeof(struct item));
    if (!dir) {
        fprintf(stderr, "Malloc has failed!\n");
        return NULL;
    }

    dir->error = false;
    dir->name = name;
    dir->type = FOLDER;
    dir->real_size = 0;
    dir->block_size = 0;

    dir->items = NULL;
    dir->items_amount = 0;

    struct stat st;
    if (stat(path, &st)) {
        fprintf(stderr, "Unable to load: %s\n", path);
        dir->error = true;
        return dir;
    }

    size_t dir_size = st.st_size;
    size_t dir_blocks = st.st_blocks * 512;

    dir->block_size = dir_blocks;
    dir->real_size = dir_size;

    int size = 10;
    int index = 0;
    struct item **children = malloc(sizeof(struct item *) * size);
    if (children == NULL) {
        fprintf(stderr, "Dir malloc failed\n");
        dir->error = true;
        return dir;
    }

    struct item *loaded;
    DIR *open_dir = NULL;
    if ((open_dir = opendir(path)) != NULL) {
        struct dirent *dir_entry = NULL;
        while ((dir_entry = readdir(open_dir)) != NULL) {
            if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0) {
                continue;
            }

            char *new_path = make_path(path, dir_entry->d_name);
            char *new_name = copy_name(dir_entry->d_name);

            if (index == size) {
                size *= 2;
                struct item **new = realloc(children, sizeof(struct item *) * size);
                if (new == NULL) {
                    fprintf(stderr, "Realloc failed\n");
                    free(children);
                    dir->error = true;
                    return dir;
                }
                children = new;
            }

            if ((loaded = load_item(new_path, new_name)) == NULL) {
                dir->error = true;
                free(new_path);
                continue;
            }
            if (loaded->error) {
                dir->error = true;
            }

            free(new_path);

            add_sum(loaded, &dir_size, &dir_blocks);
            children[index] = loaded;
            ++index;
        }
        closedir(open_dir);
    } else {
        free(children);
        fprintf(stderr, "Unable to open: %s\n", path);
        dir->error = true;
        return dir;
    }

    dir->real_size = dir_size;
    dir->block_size = dir_blocks;
    dir->items = children;
    dir->items_amount = index;
    return dir;
}

struct item *load_item(char *path, char *name)
{
    struct stat file_stats;
    if (stat(path, &file_stats)) {
        fprintf(stderr, "Unable to load: %s\n", path);
        return NULL;
    };

    if (S_ISDIR(file_stats.st_mode)) {
        return load_dir(path, name);
    } else if (S_ISREG(file_stats.st_mode)) {
        return load_file(path, name);
    } else {
        free(name);
        return NULL;
    }
}

void free_item(struct item *item)
{
    if (item->type == NORM_FILE) {
        free(item->name);
        item->name = NULL;
    } else {
        if (item->items != NULL) {
            for (int i = 0; i < item->items_amount; i++) {
                free_item(item->items[i]);
            }
        }
        free(item->items);
        item->items = NULL;
        free(item->name);
        item->name = NULL;
    }
    free(item);
}
