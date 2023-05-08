//
// Created by Jan on 08.05.2023.
//

#ifndef HW05_FILES_H
#define HW05_FILES_H

#endif //HW05_FILES_H

#include "stdbool.h"
#include "structures.h"


char *find_name_from_path(char *path);

char *add_name_to_path(char *path, char *name);

char *remove_name_from_path(char *path);

void add_sum(struct item *item, size_t *dir_size, size_t *dir_blocks);

bool load_file(char *path, char *name, struct item *item);

bool load_dir(char *path, char *name, struct item *result_dir);

bool load_item(char *path, struct item *item);

void free_item(struct item *item);
