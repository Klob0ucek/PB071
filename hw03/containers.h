//
// Created by Jan on 07.04.2023.
//

#ifndef HW03_CONTAINERS_H
#define HW03_CONTAINERS_H



#endif //HW03_CONTAINERS_H
#include "stdbool.h"
#include "general_structs.h"

int try_neighbour(int index, unsigned int wanted_id, unsigned int *neighbour);

int compare_unsigned_ints(const void *a, const void *b);

int compare_ints(const void* a, const void* b);

bool fill_neighbours(struct container_t *container);

int load_container(int line_index, struct container_t *container);

bool parse_input(struct all_containers *all_containers, const char* cont_path_test, const char* road_path_test);

bool free_container(struct container_t container);

bool free_group(struct group current_group);

bool deep_free_all_containers(struct all_containers *all_conts);

void free_struct_all_containers(struct all_containers *all_conts);

void free_groups(struct all_containers *all_conts);

int garb_to_int(enum garbage_type type);

bool make_new_group(struct group *pointer_group, int id, double x, double y, unsigned int first_id, enum garbage_type garbage);

bool add_to_group(struct group *group, unsigned int id, enum garbage_type garbage);

bool groupify(struct all_containers *all_conts);

bool find_container_by_id(unsigned int wanted_id, const struct all_containers *all_conts, struct container_t *container);

bool id_in_group(int group, const int *pInt, int index);

bool fill_neighbouring_groups(const struct all_containers *all_conts, struct group current_group, int **result, int *size);
