#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

// TODO: [Optional] Think about an appropriate division of the solution into files.
//                  e.g. Move data loading and validating to another .c file.

#include "data_source.h" // TODO: Read the content of this file.

enum garbage_type {
    Plastic = 1, Paper, Bio, Clear, Colored, Textile
};

struct container_t {
    unsigned int id;
    double coordinates_x;
    double coordinates_y;
    enum garbage_type garb_type;
    unsigned int capacity;
    char *name;
    char *street;
    unsigned int house_number;
    bool public;
    // !!! ON HEAP !!!
    unsigned int *neighbours; // will contain sorted ids of neighbours - still can contain same ids
    int neighbour_count;
};

struct all_containers{
    // !!! ON HEAP !!!
    struct container_t *containers;
    int amount;
};

char *garbage_type_to_string(enum garbage_type garb){
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
    printf("ID: %o, ", container.id);
    printf("Type: %s, ", garbage_type_to_string(container.garb_type));
    printf("Capacity: %o, ", container.capacity);
    printf("Address: %s %o, ", container.street, container.house_number);
    printf("Neighbours:");
    print_neighbours(container);
    printf("\n");
    return true;
}

void print_all(const struct all_containers *all_containers){
    struct container_t *boxes = all_containers->containers;
    for (int i = 0; i < (*all_containers).amount; ++i) {
        print_container(boxes[i]);
    }
}

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

int compare_ints(const void* a, const void* b)
{
    // copied from cppreference
    unsigned int arg1 = *(const unsigned int*)a;
    unsigned int arg2 = *(const unsigned int*)b;

    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}


bool fill_neighbours(struct container_t *container) {
    int size = 10;
    unsigned int *neighbours =  malloc(sizeof(unsigned int) * size);
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
    unsigned int id = atoi(id_str);
    if (id <= 0 || id > UINT_MAX) {
        return 0;
    }

    // loading X coordinates
    double x_coords = atof(get_container_x(line_index));
    if (x_coords == 0.0) {
        return 0;
    }

    //loading Y coordinates
    double y_coords = atof(get_container_y(line_index));
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
    unsigned int capacity = atoi(get_container_capacity(line_index));
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
        house_num = atoi(house_number);
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

    struct container_t new_container = {id, x_coords,y_coords,garb,
            capacity, name, street, house_num, public, NULL, 0};

    if (!fill_neighbours(&new_container)){
        return 0;
    }
    *container = new_container;
    return 1;
}

bool parse_input(struct all_containers *all_containers) {
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
    struct all_containers all = {containers, index};
    *all_containers = all;
    return true;
}

bool deep_free_all_containers(struct all_containers *all_conts){
    for (int i = 0; i < all_conts->amount; i++){
        free(all_conts->containers[i].neighbours);
        all_conts->containers[i].neighbours = NULL;
    }
    free(all_conts->containers);
    return true;
}

bool free_struct_all_containers(struct all_containers *all_conts){
    free(all_conts->containers);
    all_conts->containers = NULL;
    all_conts = NULL;
}

bool filter_types(const char *filter_str, enum garbage_type **filters) {
    enum garbage_type *filter_array = calloc(6, sizeof(enum garbage_type));
    if (filter_array == NULL) {
        perror("Malloc Failure");
        return false;
    }

    int index = 0;
    while (true) {
        if (filter_str[index] == '\0') {
            break;
        } else if (filter_str[index] == 'A') {
            filter_array[index] = Plastic;
        } else if (filter_str[index] == 'P') {
            filter_array[index] = Paper;
        } else if (filter_str[index] == 'B') {
            filter_array[index] = Bio;
        } else if (filter_str[index] == 'G') {
            filter_array[index] = Clear;
        } else if (filter_str[index] == 'C') {
            filter_array[index] = Colored;
        } else if (filter_str[index] == 'T') {
            filter_array[index] = Textile;
        } else {
            fprintf(stderr, "Invalid filter type");
            return false;
        }
        index++;
    }
    *filters = filter_array;

    return true;
}

void parse_interval(char *s, int *x, int *y) {
    sscanf(s, "%d-%d", x, y);
}

bool private_filter(char *input, bool *want_private) {
    if (strcmp(input, "Y") == 0) {
        *want_private = true;
    } else if (strcmp(input, "N") == 0) {
        *want_private = false;
    } else {
        return false;
    }
    return true;
}

bool filter_garb_type(const enum garbage_type *filters, struct all_containers *all_conts){
    int cont_size = 10;
    struct container_t *filtered_containers = malloc(sizeof(struct container_t) * cont_size);
    if (filtered_containers == NULL) {
        perror("Malloc Failre");
        return false;
    }

    int index = 0;
    for (int i = 0; i < all_conts->amount; ++i){
        struct container_t current_container = all_conts->containers[i];
        for (int j = 0; j < 6; ++j) {
            if (filters[j] == current_container.garb_type){
                if (index == cont_size) {
                    cont_size *= 2;
                    struct container_t *new_filtered_containers = realloc(filtered_containers, sizeof(struct container_t) * cont_size);
                    if (new_filtered_containers == NULL) {
                        perror("Realloc Failure");
                        free(filtered_containers);
                        return false;
                    }
                    filtered_containers = new_filtered_containers;
                }
                filtered_containers[index] = current_container;
                index++;
            }
        }
    }

    free_struct_all_containers(all_conts);

    struct all_containers all = {filtered_containers, index};
    *all_conts = all;
    return true;
}

int main(int argc, char *argv[]) {
    
    const char* cont_path_test = "C:/Files/MUNI/PB071/C/hw03/data/Brno-BosonohyContainers.csv";
    const char* road_path_test = "C:/Files/MUNI/PB071/C/hw03/data/Brno-BosonohyPaths.csv";

    init_data_source(cont_path_test, road_path_test);
    struct all_containers all_containers;
    parse_input(&all_containers);
    printf("Containers Loaded: %d\n", all_containers.amount);
    destroy_data_source();


    if (argc < 3){
        fprintf(stderr, "Not Enough Arguments");
        return EXIT_FAILURE;
    }
    if (argc == 3) {
        print_all(&all_containers);
    }
    if (argc == 4 && strcmp(argv[1], "-s") == 0) {
        printf("%s", argv[1]);
        printf("We will print out container groups\n");
    } else {
        printf("We will be filtering data and printing out\n");
        for (int i = 1; i < (argc - 2); i += 2) {
            if (strcmp(argv[i], "-t") == 0) {
                enum garbage_type *type_filter;
                if (!filter_types(argv[i + 1], &type_filter)){
                    return EXIT_FAILURE;
                }
                if (!filter_garb_type(type_filter, &all_containers)){
                    fprintf(stderr, "Data filtering failed");
                    return EXIT_FAILURE;
                }
                print_all(&all_containers);
                free(type_filter);
                type_filter = NULL;

            }
            if (strcmp(argv[i], "-c") == 0) {
                int low = 0;
                int high = 0;
                parse_interval(argv[i + 1], &low, &high);

                //filter data

            }
            if (strcmp(argv[i], "-p") == 0) {
                bool want_private;
                if (!(private_filter(argv[i + 1], &want_private))){
                    fprintf(stderr, "Invalid filter option");
                    return false;
                }

                //filter data

            }
        }
    }
    deep_free_all_containers(&all_containers);

    return EXIT_SUCCESS; // May your program be as successful as this macro. Good luck!
}
