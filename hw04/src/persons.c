#include "persons.h"

#include "errors.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

static void swap(struct person *lhs, struct person *rhs)
{
    char *name = lhs->name;
    char *id = lhs->id;

    lhs->name = rhs->name;
    lhs->id = rhs->id;

    rhs->name = name;
    rhs->id = id;
}

static void ensort(struct persons *persons)
{
    struct person *current = persons->persons + persons->size - 1;
    for (; current != persons->persons; --current) {
        struct person *previous = current - 1;
        if (strcmp(previous->id, current->id) <= 0)
            break;
        swap(previous, current);
    }
}

/*
static int person_cmp(const void *_id, const void *_person)
{
    const char *id = (const char *) _id;
    const struct person *person = (const struct person *) _person;
    return strcmp(id, person->id);
}
*/

struct person *find_person(const struct persons *persons, const char *id)
{
    for (int i = 0; i < persons->size; ++i) {
        if (strcmp(id, persons->persons[i].id) == 0) {
            return &persons->persons[i];
        }
    }
    return NULL;
    //    return (struct person *) bsearch(id, persons->size, persons->persons, sizeof(struct person), person_cmp);
}

static void destroy_persons(void *p)
{
    struct persons *persons = (struct persons *) p;
    for (int i = 0; i < persons->size; i++) {
        free(persons->persons[i].id);
        free(persons->persons[i].name);
    }
    free(persons->persons);
}

void init_persons(struct persons *persons)
{
    object_avoid_duplicit_initialization(persons);
    persons->capacity = 16;
    persons->size = 0;
    persons->persons = NULL;
    object_set_destructor(persons, destroy_persons);
}

void new_person(struct person *new, const char *id, const char *name)
{
    if (id == NULL || name == NULL) {
        OP(NULL, ALLOCATION_FAILED);
    }
    char *new_id = malloc(strlen(id) * sizeof(char) + 1);
    if (name == NULL) {
        perror("Malloc Failure");
        OP(NULL, ALLOCATION_FAILED);
    }
    strcpy(new_id, id);

    char *new_name = malloc(strlen(name) * sizeof(char) + 1);
    if (name == NULL) {
        perror("Malloc Failure");
        OP(NULL, ALLOCATION_FAILED);
    }
    strcpy(new_name, name);

    struct person new_guy = { new_id, new_name, 0 };
    *new = new_guy;
}

void add_person(struct persons *persons, const char *id, const char *name)
{
    OP(!find_person(persons, id), PERSON_ALREADY_PRESENT);

    struct person person_new;
    new_person(&person_new, id, name);

    if (persons->persons == NULL) {
        struct person *people = malloc(sizeof(struct person) * persons->capacity);
        if (people == NULL) {
            OP(NULL, ALLOCATION_FAILED);
        }
        people[persons->size] = person_new;
        persons->size++;
        persons->persons = people;
    } else {
        if (persons->capacity == persons->size) {
            persons->capacity *= 2;
            struct person *tmp;
            OP(tmp = (struct person *) realloc(persons->persons, persons->capacity * sizeof(struct person)), ALLOCATION_FAILED);
            persons->persons = tmp;
        }
        persons->persons[persons->size] = person_new;
        persons->size++;
    }

    ensort(persons);
}
