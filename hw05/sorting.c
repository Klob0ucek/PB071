//
// Created by Jan on 08.05.2023.
//

#include "sorting.h"

#include <string.h>

/***********
 * SORTING *
 **********/

sorting_fn choose_sort_function(struct options *options)
{
    if (options->size_sorted == DEFAULT) {
        return &compare_names;
    }
    if (options->block_size == DEFAULT) {
        return &compare_blocks;
    }
    return &compare_actual;
}

void sort(struct item *item, sorting_fn op)
{
    if (item->type == FOLDER) {
        qsort(item->items,item->items_amount,sizeof(struct item*),op);
        for (int i = 0; i < item->items_amount; ++i) {
            sort(item->items[i], op);
        }
    }
}

void sort_tree(struct item *item, struct options *options)
{
    int (*op)(const void *, const void *) = choose_sort_function(options);
    sort(item, op);
}

int compare_names(const void *a, const void *b)
{
    const struct item *item1 = *(const struct item **) a;
    const struct item *item2 = *(const struct item **) b;

    char *name1 = item1->name;
    char *name2 = item2->name;

    int result;

    result = strcasecmp(name1, name2);

    if (result == 0) {
        if (strcmp(name1, name2) < 0) {
            result = -1;
        } else if (strcmp(name1, name2) > 0) {
            result = 1;
        }
    }

    return result;
}

int compare_actual(const void *a, const void *b)
{
    const struct item *item1 = *(const struct item **) a;
    const struct item *item2 = *(const struct item **) b;

    size_t size1 = item1->real_size;
    size_t size2 = item2->real_size;

    if (size1 < size2)
        return -1;
    if (size1 > size2)
        return 1;
    return 0;
}

int compare_blocks(const void *a, const void *b)
{
    const struct item *item1 = *(const struct item **) a;
    const struct item *item2 = *(const struct item **) b;

    size_t block1 = item1->block_size;
    size_t block2 = item2->block_size;

    if (block1 < block2)
        return -1;
    if (block1 > block2)
        return 1;
    return 0;
}
