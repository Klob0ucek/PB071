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
};

struct all_containers{
    struct container_t **containers;
    int amount;
};

bool print_container(const struct container_t *container) {
    printf("ID: %o, ", container->id);
    printf("Type: %d, ", container->garb_type);
    printf("Capacity: %o, ", container->capacity);
    printf("Address: %s %o, ", container->street, container->house_number);
    printf("Neighbours: Fill neighbours\n");
    return true;
}

bool print_all(const struct all_containers *all_containers){
    for (int i = 0; i < all_containers->amount; ++i) {
        print_container(all_containers->containers[i]);
    }
    return true;
}

bool load_container(int line_index, struct container_t **container) {
    struct container_t *new_container = malloc(sizeof(struct container_t));
    if (new_container == NULL) {
        free(new_container);
        perror("Malloc Failure");
        return false;
    }

    // loading container id - !!! doesnt check if id is unique !!!
    unsigned int id = atoi(get_container_id(line_index));
    if (id <= 0 || id > UINT_MAX) {
        return false;
    }
    new_container->id = id;

    // loading X coordinates
    double x_coords = atof(get_container_x(line_index));
    if (x_coords == 0.0) {
        return false;
    }
    new_container->coordinates_x = x_coords;

    //loading Y coordinates
    double y_coords = atof(get_container_y(line_index));
    if (y_coords == 0.0) {
        return false;
    }
    new_container->coordinates_y = y_coords;

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
        return false;
    }
    new_container->garb_type = garb;

    // loading capacity
    unsigned int capacity = atoi(get_container_capacity(line_index));
    if (capacity <= 0 || capacity > UINT_MAX) {
        return false;
    }
    new_container->capacity = capacity;

    //loading container name
    const char *original_str = get_container_name(line_index);
    char *name = malloc(strlen(original_str) * sizeof(char) + 1);
    strcpy(name, original_str);
    new_container->name = name;

    //loading container street
    original_str = get_container_street(line_index);
    char *street = malloc(strlen(original_str) * sizeof(char) + 1);
    strcpy(street, original_str);
    new_container->street = street;

    // loading container house number
    const char *house_number = get_container_number(line_index);
    unsigned int house_num;
    if (strcmp(house_number, "") == 0) {
        house_num = 0;
    } else {
        house_num = atoi(house_number);
        if (house_num <= 0 || house_num > UINT_MAX) {
            return false;
        }
    }
    new_container->house_number = house_num;

    //loading if container is public or private
    bool public;
    const char *public_garb = get_container_public(line_index);
    if (strcmp(public_garb, "Y") == 0) {
        public = true;
    } else if (strcmp(public_garb, "N") == 0) {
        public = false;
    } else {
        return false;
    }
    new_container->public = public;

    *container = new_container;
    return true;
}

bool parse_input(const char *containers_path, const char *paths_path, struct all_containers *all_conts) {
    int cont_size = 10;
    struct container_t **containers;
    containers = malloc(sizeof(void *) * cont_size);
    if (containers == NULL) {
        perror("Malloc Failre");
        return false;
    }

    int index = 0;
    struct container_t *container;
    while (true) {
        if (get_container_id(index) == NULL) {
            break;
        }
        bool cont_load_success = load_container(index, &container);
        if (!cont_load_success) {
            fprintf(stderr, "Invalid input file with containers");
            free(containers);
            return false;
        }
        if (index == cont_size) {
            struct container_t *new_containers = realloc(containers, cont_size);
            if (new_containers == NULL) {
                perror("Realloc Failure");
                free(containers);
                return false;
            }
            cont_size *= 2;
            containers = &new_containers;
        }
        containers[index] = container;
        index++;
    }
    struct all_containers all = {containers, index};
    *all_conts = all;
    return true;
}

bool free_all_containers(struct all_containers *all_conts){
    for (int i = 0; i < all_conts->amount; ++i) {
        free(all_conts->containers[i]);
        all_conts->containers[i] = NULL;
    }
    free(all_conts->containers);
    all_conts->containers = NULL;
    free(all_conts);
    all_conts = NULL;
}

bool filter_types(const char *filter_str, enum garbage_type **filters) {
    enum garbage_type *filter_array = calloc(sizeof(enum garbage_type) * 6, sizeof(enum garbage_type));
    if (filter_array == NULL) {
        perror("Malloc Failure");
        return false;
    }
    memset(filter_array, 0, sizeof(enum garbage_type));

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
        *filters = filter_array;
    }
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

int main(int argc, char *argv[]) {


    const char* cont_path_test = "C:/Files/MUNI/PB071/C/hw03/data/Brno-BosonohyContainers.csv";
    const char* road_path_test = "C:/Files/MUNI/PB071/C/hw03/data/Brno-BosonohyPaths.csv";
    init_data_source(cont_path_test, road_path_test);
    struct all_containers all_containers;
    parse_input(cont_path_test,road_path_test, &all_containers);
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
        printf("We will print out container groups");
    } else {
        printf("We will be filtering data and printing out");
        for (int i = 1; i < (argc - 2); i += 2) {
            if (strcmp(argv[i], "-t") == 0) {
                enum garbage_type *type_filter;
                if (!filter_types(argv[i + 1], &type_filter)){
                    return EXIT_FAILURE;
                }

                // filter data

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
    free_all_containers(&all_containers);

    return EXIT_SUCCESS; // May your program be as successful as this macro. Good luck!
}
