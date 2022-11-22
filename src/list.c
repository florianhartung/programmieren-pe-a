#include <malloc.h>
#include "include/list.h"

#define DEFAULT_CAPACITY 2

list_t* init_list(size_t element_size) {
    list_t* list = malloc(sizeof(list_t));
    list->element_size = element_size;
    list->length = 0;

    list->capacity = DEFAULT_CAPACITY;
    list->data = calloc(list->capacity, element_size);
    list->element_free_function = 0;

    return list;
}


void list_add(list_t* list, void* element) {
    list->length += 1;

    if (list->length >= list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * list->element_size);
    }

    list->data[list->length - 1] = element;
}

void* list_remove(list_t* list, size_t index) {
    void* deleted = list_get(list, index);
    if (index + 1 < list->length) {
        for (size_t i = index + 1; i < list->length; i++) {
            list->data[i - 1] = list->data[i];
        }
    }

    list->length -= 1;

    return deleted;
}

void* list_get(list_t* list, size_t index) {
    return list->data[index];
}

void free_list(list_t* list) {
    if (list->element_free_function) {
        for (int i = 0; i < list->length; i++) {
            void* element = list_get(list, i);
            list->element_free_function(element);
        }
    }

    free(list->data);
    free(list);
}