//
// Created by Jan on 08.05.2023.
//

#ifndef HW05_PRINTING_H
#define HW05_PRINTING_H

#endif //HW05_PRINTING_H

#include "files.h"

void print_item(struct item *item, struct prefix *prefix, struct options *options, size_t max);

void print_size(size_t size);

void print_percentage(size_t num, size_t max);

void print_file(struct item *file, struct prefix *prefix, struct options *options, size_t max);

void print_dir(struct item *dir, struct prefix *prefix, struct options *options, size_t max);

void print_tree(struct item *item, struct options *options);