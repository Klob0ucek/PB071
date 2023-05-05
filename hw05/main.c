#include "files.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>

/***********
 * SORTING *
 **********/



void *choose_sort_function(struct options *options);

void sort(struct item *item, int (*op)(const void*, const void*)){
    if (item->item_type == FOLDER){
        qsort(item->item_pointer.folder.children,
              item->item_pointer.folder.amount_of_items,sizeof(struct item), op);
        for (int i = 0; i < item->item_pointer.folder.amount_of_items; ++i) {
            sort(&item->item_pointer.folder.children[i], op);
        }
    }
}

void sort_tree(struct item *item, struct options *options){
    int (*op)(const void*, const void*) = choose_sort_function(options);
    sort(item, op);
}

int compare_names(const void *a, const void *b) {
    struct item item1 = *(const struct item*) a;
    struct item item2 = *(const struct item*) b;

    char *name1 = (item1.item_type == FOLDER) ? item1.item_pointer.folder.name : item1.item_pointer.file.name;
    char *name2 = (item2.item_type == FOLDER) ? item2.item_pointer.folder.name : item2.item_pointer.file.name;

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

int compare_actual(const void* a, const void* b) {
    struct item item1 = *(const struct item*) a;
    struct item item2 = *(const struct item*) b;

    size_t size1 = (item1.item_type == FOLDER) ? item1.item_pointer.folder.size : item1.item_pointer.file.size;
    size_t size2 = (item2.item_type == FOLDER) ? item2.item_pointer.folder.size : item2.item_pointer.file.size;

    if (size1 < size2) return -1;
    if (size1 > size2) return 1;
    return 0;
}

int compare_blocks(const void* a, const void* b) {
    struct item item1 = *(const struct item*) a;
    struct item item2 = *(const struct item*) b;

    size_t block1 = (item1.item_type == FOLDER) ? item1.item_pointer.folder.blocks : item1.item_pointer.file.blocks;
    size_t block2 = (item2.item_type == FOLDER) ? item2.item_pointer.folder.blocks : item2.item_pointer.file.blocks;

    if (block1 < block2) return -1;
    if (block1 > block2) return 1;
    return 0;
}

void *choose_sort_function(struct options *options) {
    if (options->size_sorted == DEFAULT){
        return &compare_names;
    }
    if (options->block_size == DEFAULT){
        return &compare_blocks;
    }
    return &compare_actual;
};

/************
 * PRINTING *
 ***********/

void print_item(struct item *item, struct prefix *prefix, struct options *options);

const char* print_size(size_t size) {
    static const char* units[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB"};
    int unitIndex = 0;
    double d_size = (double)size;

    while (d_size >= 1024 && unitIndex < 5) {
        d_size /= 1024;
        unitIndex++;
    }
    printf("%6.2f %s ", d_size, units[unitIndex]);
}

void print_file(struct item *file,  struct prefix *prefix, struct options *options){
    if (options->block_size == DEFAULT){
        print_size(file->item_pointer.file.blocks);
    } else {
        print_size(file->item_pointer.file.size);
    }
    printf("%s%s\n", prefix->prefix, file->item_pointer.file.name);
}

void print_dir(struct item *dir,  struct prefix *prefix, struct options *options){
    if (options->block_size == DEFAULT){
        print_size(dir->item_pointer.folder.blocks);
    } else {
        print_size(dir->item_pointer.folder.size);
    }
    printf("%s%s\n", prefix->prefix, dir->item_pointer.folder.name);

    char *dir_prefix = prefix->prefix + (4 * prefix->depth);
    strcpy(dir_prefix, "|-- ");
    if (prefix->depth > 0 && *(dir_prefix - 4) == '\\'){
        memset(dir_prefix - 4, ' ', 4);
    }

    for (int i = 0; i < dir->item_pointer.folder.amount_of_items - 1; i++){
        print_item(&dir->item_pointer.folder.children[i], prefix, options);
    }
    strcpy(dir_prefix, "\\-- ");
    print_item(&dir->item_pointer.folder.children[dir->item_pointer.folder.amount_of_items - 1], prefix, options);
}

void print_item(struct item *item, struct prefix *prefix, struct options *options) {
    prefix->depth++;
    if (item->item_type == FOLDER){
        print_dir(item, prefix, options);
    } else if (item->item_type == NORM_FILE){
        print_file(item, prefix, options);
    }
    prefix->depth--;
}

void print_tree(struct item *item, struct options *options){
    char prefix_str[512];
    memset(prefix_str, '\0', sizeof(char) * 512);
    char *p = (char *) &prefix_str;
    struct prefix prefix = {p, item->item_type == FOLDER ? item->item_pointer.folder.error_flag : false, -1};
    print_item(item, &prefix, options);
}

/*********
 * FILES *
 ********/


struct item load_item(char *path);

char *find_name_from_path(char *path) {
    char *result;
    char *name_start = strrchr(path, '/');
    if (name_start == NULL) {
        result = strdup(path);
    } else {
        name_start++;
        result = strdup(name_start);
    }
    if (result == NULL){
        fprintf(stderr, "Name alloc failed");
    }
    return result;
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

size_t add_sum(struct item *item, size_t *dir_size, size_t *dir_blocks){
    if (item->item_type == FOLDER){
        *dir_blocks += item->item_pointer.folder.blocks;
        *dir_size += item->item_pointer.folder.size;
    } else if (item->item_type == NORM_FILE){
        *dir_blocks += item->item_pointer.file.blocks;
        *dir_size += item->item_pointer.file.size;
    }
}


struct item load_file(char *path) {
    struct stat st;
    stat(path, &st);

    struct file this = {st.st_size, st.st_blksize, find_name_from_path(path)};
    union item_holder holder = {this};
    struct item result = {NORM_FILE, holder};
    return result;
}

struct item load_dir(char *path) {
    struct folder folder = {0, 0, false, find_name_from_path(path), NULL};
    union item_holder holder;
    holder.folder = folder;
    struct item item = {FOLDER, holder};

    size_t dir_size = 0;
    size_t dir_blocks = 0;

    size_t size = 10;
    int index = 0;
    struct item *children = malloc(sizeof(struct item) * size);
    if (children == NULL){
        fprintf(stderr, "Dir malloc failed");
        item.item_pointer.folder.error_flag = true;
        return item;
    }

    struct item loaded;
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
            loaded = load_item(path);
            add_sum(&loaded, &dir_size, &dir_blocks);
            children[index] = loaded;
            ++index;

            remove_name_from_path(path);
        }
        closedir(dir);
    }

    item.item_pointer.folder.size = dir_size;
    item.item_pointer.folder.blocks = dir_blocks;
    item.item_pointer.folder.children = children;
    item.item_pointer.folder.amount_of_items = index;

    return item;
}

struct item load_item(char *path) {
    struct stat file_stats;
    stat(path, &file_stats);

    if (S_ISDIR(file_stats.st_mode)){
        return load_dir(path);
    } else if (S_ISREG(file_stats.st_mode)){
        return load_file(path);
    }
    else if (file_stats.st_mode == DT_UNKNOWN){
        // TODO unknown file finish here
    }
}

void free_item(struct item *item){
    if (item->item_type == NORM_FILE){
        free(item->item_pointer.file.name);
        item->item_pointer.file.name = NULL;
    } else {
        for (int i = 0; i < item->item_pointer.folder.amount_of_items; i++){
            free_item(&item->item_pointer.folder.children[i]);
        }
        free(item->item_pointer.folder.children);
        item->item_pointer.folder.children = NULL;
        free(item->item_pointer.folder.name);
        item->item_pointer.folder.name = NULL;
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

    struct item item = load_item(long_path);
    sort_tree(&item, &options);
    print_tree(&item, &options);

    free_item(&item);
    return 0;
}
