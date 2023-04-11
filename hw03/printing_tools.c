//
// Created by Jan on 07.04.2023.
//
#include "printing_tools.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void print_garb_type(enum garbage_type garb)
{
    switch (garb) {
    case Plastic:
        printf("Plastics and Aluminium");
        break;
    case Paper:
        printf("Paper");
        break;
    case Bio:
        printf("Biodegradable waste");
        break;
    case Clear:
        printf("Clear glass");
        break;
    case Colored:
        printf("Colored glass");
        break;
    case Textile:
        printf("Textile");
        break;
    default:
        printf("NaN");
        break;
    }
}

void print_neighbours(const struct container_t container)
{
    unsigned int printed = 0;
    for (int i = 0; i < container.neighbour_count; ++i) {
        if (container.neighbours[i] == printed) {
            continue;
        }
        printf(" %u", container.neighbours[i]);
        printed = container.neighbours[i];
    }
}

void print_container(const struct container_t container)
{
    printf("ID: %u, ", container.id);
    printf("Type: ");
    print_garb_type(container.garb_type);
    printf(", ");
    printf("Capacity: %u, ", container.capacity);
    printf("Address: ");
    printf("%s", container.street);
    if (strcmp(container.street, "") != 0){
        printf(" ");
    }
    if (container.house_number != 0){
        printf("%d, ", container.house_number);
    } else {
        printf(", ");
    }
    printf("Neighbors:");
    print_neighbours(container);
    printf("\n");
}

void print_all(const struct all_containers *all_containers)
{
    struct container_t *boxes = all_containers->containers;
    for (int i = 0; i < (*all_containers).amount; ++i) {
        print_container(boxes[i]);
    }
}

void print_int_to_type(const int type)
{
    switch (type) {
    case 0:
        printf("A");
        break;
    case 1:
        printf("P");
        break;
    case 2:
        printf("B");
        break;
    case 3:
        printf("G");
        break;
    case 4:
        printf("C");
        break;
    case 5:
        printf("T");
        break;
    default:
        fprintf(stderr, "Incorrect value");
        return;
    }
}

void print_group_type(const int *types)
{
    for (int i = 0; i < 6; i++) {
        if (types[i] == 1) {
            print_int_to_type(i);
        }
    }
}

void print_group(struct group current_group, int *group_neighbours, int size)
{
    printf("%d;", current_group.id);
    print_group_type(current_group.garbage_types);
    printf(";");
    print_group_neighbours(group_neighbours, size);
    printf("\n");
}

void print_groups(struct all_containers *all_containers)
{
    int *groups_neighbours = NULL;
    int group_size = 0;
    for (int i = 0; i < all_containers->group_amount; i++) {
        if (!fill_neighbouring_groups(all_containers, all_containers->groups[i], &groups_neighbours, &group_size)) {
            fprintf(stderr, "Group printing stopped");
            return;
        }
        print_group(all_containers->groups[i], groups_neighbours, group_size);
        free(groups_neighbours);
    }
}

void print_group_neighbours(const int *groups_neighbours, int size)
{
    for (int i = 0; i < size; i++) {
        printf("%d", groups_neighbours[i]);
        if (i != size - 1) {
            printf(",");
        }
    }
}

