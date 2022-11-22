#ifndef PE_A3_LIST_H
#define PE_A3_LIST_H

#include <corecrt.h>

typedef struct LIST {
    void** data;

    size_t element_size;
    size_t length;
    size_t capacity;


    void (*element_free_function)(void*);
} list_t;

list_t* init_list(size_t element_size);

void list_add(list_t* list, void* element);

void* list_remove(list_t* list, size_t index);

void* list_get(list_t* list, size_t index);

void free_list(list_t* list);

#endif //PE_A3_LIST_H
