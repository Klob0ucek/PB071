
#include "containers.h"

#include "stdbool.h"
#include "stdio.h"
#include "data_source.h"
#include "string.h"
#include <limits.h>

//
// Created by Jan on 07.04.2023.
//

int try_neighbour(int index, unsigned int wanted_id, unsigned int *neighbour) {
    if (get_path_a_id(index) == NULL){
        return -1;
    }
    unsigned int a;
    unsigned int b;
    sscanf(get_path_a_id(index), "%u", &a);
    sscanf(get_path_b_id(index), "%u", &b);

    if (a == wanted_id) {
        *neighbour = b;
        return 1;
    }
    if (b == wanted_id){
        *neighbour = a;
        return 1;
    }
    return 0;
}

int compare_ints(const void *a, const void *b) {
    // copied from cppreference
    unsigned int arg1 = *(const unsigned int*)a;
    unsigned int arg2 = *(const unsigned int*)b;

    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

bool fill_neighbours(struct container_t *container) {
    int size = 10;
    unsigned int *neighbours = malloc(sizeof(unsigned int) * size);
    if (neighbours == NULL) {
        perror("Malloc Failre");
        return false;
    }

    int index = 0;
    unsigned int new_neighbour;
    int neighbour_count = 0;
    int state;
    while (true){
        state = try_neighbour(index, container->id, &new_neighbour);
        if (state == -1){
            break;
        }
        if (state == 1) {
            if (size == neighbour_count) {
                size *= 2;
                unsigned int *neighbours_new = realloc(neighbours, sizeof(unsigned int) * size);
                if (neighbours_new == NULL) {
                    perror("Realloc Failure");
                    free(neighbours);
                    return false;
                }
                neighbours = neighbours_new;
            }
            neighbours[neighbour_count] = new_neighbour;
            neighbour_count++;
        }
        index++;
    }
    // remalloc to correct size
    unsigned int *neighbours_new = realloc(neighbours, sizeof(unsigned int) * neighbour_count);
    if (neighbours_new == NULL) {
        perror("Realloc Failure");
        free(neighbours);
        return false;
    }
    neighbours = neighbours_new;

    // qsort - list of elements, amount of elements, size of element, function that compares two elements
    qsort(neighbours, neighbour_count, sizeof(unsigned int), compare_ints);

    container->neighbours = neighbours;
    container->neighbour_count = neighbour_count;
    return true;
}

int load_container(int line_index, struct container_t *container) {
    const char *id_str = get_container_id(line_index);
    if (id_str == NULL) {
        return -1;
    }

    // loading container id - !!! doesnt check if id is unique !!!
    unsigned int id;
    sscanf(id_str, "%u", &id);
    if (id <= 0 || id > UINT_MAX) {
        return 0;
    }

    // loading X coordinates
    double x_coords;
    sscanf(get_container_x(line_index), "%lf", &x_coords);
    if (x_coords == 0.0) {
        return 0;
    }

    //loading Y coordinates
    double y_coords;
    sscanf(get_container_x(line_index), "%lf", &y_coords);
    if (y_coords == 0.0) {
        return 0;
    }

    //loading garbage type
    enum garbage_type garb;
    const char *garb_str = get_container_waste_type(line_index);
    if (strcmp(garb_str, "Plastics and Aluminium") == 0) {
        garb = Plastic;
    } else if (strcmp(garb_str, "Paper") == 0) {
        garb = Paper;
    } else if (strcmp(garb_str, "Biodegradable waste") == 0) {
        garb = Bio;
    } else if (strcmp(garb_str, "Clear glass") == 0) {
        garb = Clear;
    } else if (strcmp(garb_str, "Colored glass") == 0) {
        garb = Colored;
    } else if (strcmp(garb_str, "Textile") == 0) {
        garb = Textile;
    } else {
        return 0;
    }

    // loading capacity
    unsigned int capacity;
    sscanf(get_container_capacity(line_index), "%u", &capacity);
    if (capacity <= 0 || capacity > UINT_MAX) {
        return 0;
    }

    //loading container name
    const char *original_str = get_container_name(line_index);
    char *name = malloc(strlen(original_str) * sizeof(char) + 1);
    strcpy(name, original_str);

    //loading container street
    original_str = get_container_street(line_index);
    char *street = malloc(strlen(original_str) * sizeof(char) + 1);
    strcpy(street, original_str);

    // loading container house number
    const char *house_number = get_container_number(line_index);
    unsigned int house_num;
    if (strcmp(house_number, "") == 0) {
        house_num = 0;
    } else {
        sscanf(house_number, "%u", &house_num);
        if (house_num <= 0 || house_num > UINT_MAX) {
            return 0;
        }
    }

    //loading if container is public or private
    bool public;
    const char *public_garb = get_container_public(line_index);
    if (strcmp(public_garb, "Y") == 0) {
        public = true;
    } else if (strcmp(public_garb, "N") == 0) {
        public = false;
    } else {
        return 0;
    }

    struct container_t new_container = {id, x_coords,y_coords, 0, garb,
                                        capacity, name, street, house_num, public, NULL, 0};

    if (!fill_neighbours(&new_container)){
        return 0;
    }
    *container = new_container;
    return 1;
}

bool parse_input(struct all_containers *all_containers, const char *cont_path_test, const char *road_path_test) {


    init_data_source(cont_path_test, road_path_test);

    int cont_size = 10;
    struct container_t *containers;
    containers = malloc(sizeof(struct container_t) * cont_size);
    if (containers == NULL) {
        perror("Malloc Failre");
        return false;
    }

    int index = 0;
    struct container_t container;
    while (true) {
        int cont_load_success = load_container(index, &container);
        if (cont_load_success == -1) {
            break;
        }
        if (cont_load_success == 0) {
            fprintf(stderr, "Invalid input file with containers");
            free(containers);
            return false;
        }

        if (index == cont_size) {
            cont_size *= 2;
            struct container_t *new_containers = realloc(containers, sizeof(struct container_t) * cont_size);
            if (new_containers == NULL) {
                perror("Realloc Failure");
                free(containers);
                return false;
            }
            containers = new_containers;
        }
        containers[index] = container;
        index++;
    }
    struct all_containers all = {containers, index, NULL, 0};
    *all_containers = all;
    destroy_data_source();
    return true;
}

bool free_container(struct container_t container) {
    if (container.neighbours != NULL) {
        free(container.neighbours);
        container.neighbours = NULL;
    }
    free(container.name);
    container.name = NULL;
    free(container.street);
    container.street = NULL;
    return true;
}

bool free_group(struct group current_group){
    free(current_group.containers);
    current_group.containers = NULL;
    return true;
}

bool deep_free_all_containers(struct all_containers *all_conts) {
    for (int i = 0; i < all_conts->amount; i++){
        free_container(all_conts->containers[i]);
    }
    for (int i = 0; i < all_conts->group_amount; i++){
        free_group(all_conts->groups[i]);
    }

    free(all_conts->containers);
    all_conts->containers = NULL;
    free(all_conts->groups);
    all_conts->groups = NULL;

    return true;
}

void free_struct_all_containers(struct all_containers *all_conts) {
    free(all_conts->containers);
    all_conts->containers = NULL;
    all_conts = NULL;
}

void free_groups(struct all_containers *all_conts) {
    for (int i = 0; i < all_conts->group_amount; i++){
        free_group(all_conts->groups[i]);
    }
    free(all_conts->groups);
    all_conts->groups = NULL;
    all_conts->group_amount = 0;
}

bool make_new_group(struct group *pointer_group, int id, double x, double y, unsigned int first_id,
                    enum garbage_type garbage) {
    int groups_size = 5;
    unsigned int *group_ids = malloc(sizeof(unsigned int) * groups_size);
    if (group_ids == NULL){
        perror("Malloc failure");
        return false;
    }

    group_ids[0] = first_id;
    enum garbage_type new_garbage_types[6] = {0};
    new_garbage_types[garbage - 1] = 1;

    struct group new_group = {id, x, y, group_ids,
                              1, 5, new_garbage_types};
    *pointer_group = new_group;
    return true;
}

bool add_to_group(struct group *group, unsigned int id, enum garbage_type garbage) {
    if (group->container_count == group->alloc_size){
        group->alloc_size *= 2;
        unsigned int *new_group_ids = realloc(group->containers, sizeof(unsigned int) * group->alloc_size);
        if (new_group_ids == NULL) {
            perror("Realloc Failure");
            // free all groups if failed
            return false;
        }
        group->containers = new_group_ids;
    }

    group->containers[group->container_count] = id;
    group->container_count +=1;
    group->garbage_types[garbage - 1] = 1;
    return true;
}

bool groupify(struct all_containers *all_conts) {
    int cont_size = 10;
    struct group *groups;
    groups = malloc(sizeof(struct group) * cont_size);
    if (groups == NULL) {
        perror("Malloc Failre");
        return false;
    }

    int group_index = 0;
    for (int i = 0; i < all_conts->amount; i++){
        struct container_t current_container = all_conts->containers[i];

        if (group_index == cont_size) {
            cont_size *= 2;
            struct group *new_groups = realloc(groups, sizeof(struct group) * cont_size);
            if (new_groups == NULL) {
                perror("Realloc Failure");
                free(groups);
                return false;
            }
            groups = new_groups;
        }

        for (int j = 0; j < group_index; j++) {
            if (groups[j].coordinates_x == current_container.coordinates_x && groups[j].coordinates_y == current_container.coordinates_y){
                if (!add_to_group(&groups[j], current_container.id, current_container.garb_type)){
                    free_groups(all_conts);
                    return false;
                }
                current_container.group = groups[j].id;
                break;
            }
        }
        if (current_container.group == 0){
            struct group new_group;
            if (!make_new_group(&new_group, group_index + 1, current_container.coordinates_x,
                                current_container.coordinates_y, current_container.id, current_container.garb_type)){
                free_groups(all_conts);
                return false;
            }
            groups[group_index] = new_group;
            group_index++;
        }
    }
    all_conts->groups = groups;
    all_conts->group_amount = group_index;
    for (int i = 0; i < all_conts->group_amount; i++) {
        printf("GROUPID: %u - ", all_conts->groups[i].id);
        for (int j = 0; j < all_conts->groups[i].container_count; j++){
            printf("%u ", all_conts->groups[i].containers[j]);
        }
        printf("\n");
    }
    return true;
}
