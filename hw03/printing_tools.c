//
// Created by Jan on 07.04.2023.
//
#include "printing_tools.h"

#include "stdio.h"
#include "stdbool.h"
#include "string.h"

char *garbage_type_to_string(enum garbage_type garb) {
    switch (garb) {
        case Plastic:
            return "Plastics and Aluminium";
        case Paper:
            return "Paper";
        case Bio:
            return "Biodegradable waste";
        case Clear:
            return "Clear glass";
        case Colored:
            return "Colored glass";
        case Textile:
            return "Textile";
        default:
            return "NaN";
    }
}

void print_neighbours(const struct container_t container) {
    unsigned int printed = 0;
    for (int i = 0; i < container.neighbour_count; ++i) {
        if (container.neighbours[i] == printed) {
            continue;
        }
        printf(" %u", container.neighbours[i]);
        printed = container.neighbours[i];
    }
}

bool print_container(const struct container_t container) {
    printf("ID: %u, ", container.id);
    printf("Type: %s, ", garbage_type_to_string(container.garb_type));
    printf("Capacity: %u, ", container.capacity);
    if ((strcmp(container.street, "") != 0) && container.house_number != 0){
        printf("Address: %s %u, ", container.street, container.house_number);
    }
    if (container.neighbour_count != 0) {
        printf("Neighbours:");
        print_neighbours(container);
    }
    printf("\n");
    return true;
}

void print_all(const struct all_containers *all_containers) {
    struct container_t *boxes = all_containers->containers;
    for (int i = 0; i < (*all_containers).amount; ++i) {
        print_container(boxes[i]);
    }
}
