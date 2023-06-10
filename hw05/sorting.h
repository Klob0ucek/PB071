//
// Created by Jan on 08.05.2023.
//

#ifndef HW05_SORTING_H
#define HW05_SORTING_H

#endif //HW05_SORTING_H

#include "printing.h"

typedef int (*sorting_fn)(const void *, const void *);

sorting_fn choose_sort_function(struct options *options);

void sort(struct item *item, sorting_fn op);

void sort_tree(struct item *item, struct options *options);

int compare_names(const void *a, const void *b);

int compare_actual(const void *a, const void *b);

int compare_blocks(const void *a, const void *b);