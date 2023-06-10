//
// Created by Jan on 08.05.2023.
//

#ifndef HW05_FILES_H
#define HW05_FILES_H

#endif //HW05_FILES_H

#include "stdbool.h"
#include "structures.h"

char *find_name_from_path(char *path);

void add_sum(struct item *item, size_t *dir_size, size_t *dir_blocks);

struct item *load_file(char *path, char *name);

struct item *load_dir(char *path, char *name);

struct item *load_item(char *path, char *name);

void free_item(struct item *item);
