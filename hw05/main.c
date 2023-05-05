#include "files.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>

/************
 * PRINTING *
 ***********/

void print_item(struct item *item);

void print_file(struct item *file){
    printf("%s - %zu (blocks: %zu)\n", file->item_pointer.file.name, file->item_pointer.file.size, file->item_pointer.file.blocks);
}

void print_dir(struct item *dir){
    printf("/*** FOLDER %s ***/\n", dir->item_pointer.folder.name);
    for (int i = 0; i < dir->item_pointer.folder.amount_of_items; i++){
        print_item(&dir->item_pointer.folder.children[i]);
    }
    printf("/*****/\n");
}

void print_item(struct item *item) {
    if (item->item_type == FOLDER){
        print_dir(item);
    } else if (item->item_type == NORM_FILE){
        print_file(item);
    }
}

void print_structure(struct tree_structure *ts){
    print_item(&ts->root);
}

/*********
 * FILES *
 ********/

void posix_print_files(const char* path) {
    DIR *dir = NULL;
    if ((dir = opendir(path)) != NULL) {                // connect to directory
        struct dirent *dir_entry = NULL;
        while ((dir_entry = readdir(dir)) != NULL) {    // obtain next item
            printf("File %s\n", dir_entry->d_name);     // get d_name
        }
        closedir(dir);                                  // finish work with directory
    }
}

char *find_name_from_path(char *path) {
    char *name_start = strrchr(path, '/');
    if (name_start == NULL) {
        return strdup(path);
    } else {
        name_start++;
        return strdup(name_start);
    }
}

char *add_name_to_path(char *path, char *name) {
    strcat(path, "/");
    strcat(path, name);
    return path;
}

char *remove_name_from_path(char *path) {
    char *forward = strrchr(path, '/');
    *forward = '\0';
    return path;
}

struct item load_file(char *path) {
    struct stat st;
    stat(path, &st);

    struct file this = {st.st_size, st.st_blocks, find_name_from_path(path)};
    union item_holder holder = {this};
    struct item result = {NORM_FILE, holder};
    return result;
}

struct item load_dir(char *path) {
    struct folder folder = {0, false, find_name_from_path(path), NULL};
    union item_holder holder;
    holder.folder = folder;
    struct item item = {FOLDER, holder};


    size_t size = 10;
    int index = 0;
    struct item *children = malloc(sizeof(struct item) * size);
    if (children == NULL){
        fprintf(stderr, "Dir malloc failed");
        item.item_pointer.folder.error_flag = true;
        return item;
    }

    DIR *dir = NULL;
    if ((dir = opendir(path)) != NULL) {
        struct dirent *dir_entry = NULL;
        while ((dir_entry = readdir(dir)) != NULL) {
            if (*dir_entry->d_name == '.') {
                continue;
            }
            add_name_to_path(path, dir_entry->d_name);
            if (index == size){
                size *= 2;
                struct item *new = realloc(children, sizeof(struct item) * size);
                if (new == NULL){
                    perror("Realloc failed");
                    free(children);
                    item.item_pointer.folder.error_flag = true;
                    return item;
                }
            }
            children[index] = load_item(path);
            ++index;
            remove_name_from_path(path);
        }
        closedir(dir);
    }

    item.item_pointer.folder.children = children;
    item.item_pointer.folder.amount_of_items = index;
    // fill size of folder

    return item;
}

struct item load_item(char *path) {
    struct stat file_stats;
    stat(path, &file_stats);

    if (S_ISDIR(file_stats.st_mode)){
        printf("It is dir - ");
        printf("%ld\n", file_stats.st_size);
        return load_dir(path);
    } else if (S_ISREG(file_stats.st_mode)){
        printf("It is norm_file - ");
        printf("%ld\n", file_stats.st_size);
        return load_file(path);
    }
    else if (file_stats.st_mode == DT_UNKNOWN){  // TODO unknown file finish here
    }

    else {
        printf("Unknown file.\n");
    }
}


/********
 * MAIN *
 *******/

void close_options(struct options *options) {
    if (options->block_size == UNSET){
        options->block_size = DEFAULT;
    }
    if (options->size_sorted == UNSET){
        options->size_sorted = DEFAULT;
    }
    if (options->depth == -1){
        options->depth = INT_MAX;
    }
    if (options->percent == UNSET){
        options->percent = DEFAULT;
    }
};

bool parse_depth(struct options *options, const char *num_str){
    int num = -1;
    if (strchr(num_str, '.') != NULL){
        fprintf(stderr, "Depth number %s is decimal", num_str);
        return false;
    }
    sscanf(num_str, "%d", &num);
    if (num < 0) {
        fprintf(stderr, "Depth number %s is negative", num_str);
        return false;
    }
    options->depth = num;
    return true;
}

char *max_path(char *original){
    char path[4096];
    memset(path, '0', sizeof(char) * 4096);
    strcpy(path, original);
    char *p = (char *) &path;
    return p;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Invalid amount of arguments\n");
        return 1;
    }

    struct options options = {UNSET, UNSET, -1, UNSET};


    bool skip_next = false;
    for (int i = 1; i < argc - 1; ++i) {
        if (skip_next){
            skip_next = false;
            continue;
        }
        if (strcmp(argv[i], "-a") == 0 ){
            if (options.block_size == UNSET){
                options.block_size = OPTION_SET;
                continue;
            } else {
                fprintf(stderr, "Filter %s used twice", argv[i]);
                return EXIT_FAILURE;
            }
        }
        else if (strcmp(argv[i], "-s") == 0 ){
            if (options.size_sorted == UNSET){
                options.size_sorted = OPTION_SET;
                continue;
            } else {
                fprintf(stderr, "Filter %s used twice", argv[i]);
                return EXIT_FAILURE;
            }
        }
        else if (strcmp(argv[i], "-d") == 0 ){
            if (options.depth == -1){
                skip_next = true;
                if (!parse_depth(&options, argv[i + 1])){
                    return EXIT_FAILURE;
                }
                continue;
            } else {
                fprintf(stderr, "Filter %s used twice", argv[i]);
                return EXIT_FAILURE;
            }
        }
        else if (strcmp(argv[i], "-p") == 0 ){
            if (options.percent == UNSET){
                options.percent = OPTION_SET;
            } else {
                fprintf(stderr, "Filter %s used twice", argv[i]);
                return EXIT_FAILURE;
            }
        }
        else {
            fprintf(stderr, "Invalid argument");
            return EXIT_FAILURE;
        }
        skip_next = false;
    }
    close_options(&options);

    char *root_path = argv[argc-1];
    char *long_path = max_path(root_path);
    
//    printf("Options:\n File size: %d\n Sorting: %d\n Size: %d\n Percentage %d\n", options.block_size, options.size_sorted, options.depth, options.percent);
//    printf("Path: %s\n", root_path);

    struct item item = load_item(long_path);
    print_item(&item);



    return 0;
}
