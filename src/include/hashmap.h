#ifndef PE_A_HASHMAP_H
#define PE_A_HASHMAP_H

#include <corecrt.h>
#include <malloc.h>
#include "list.h"

typedef struct HASHMAP {
    list_t* buckets;
} hashmap_t;

typedef struct HASHMAP_ENTRY {
    unsigned int key;
    unsigned int hashed_key;
    void* value;
} hashmap_entry_t;

hashmap_t* init_hashmap();

void hashmap_put(hashmap_t* map, unsigned int key, void* value);

void* hashmap_get(hashmap_t* map, unsigned int key);

void hashmap_print(hashmap_t* map);

list_t* hashmap_into_entries(hashmap_t* map);

void free_hashmap(hashmap_t* map);


hashmap_entry_t* init_hashmap_entry(unsigned int key, unsigned int hashed_key, void* value);

void free_hashmap_entry(hashmap_entry_t* entry);

#endif //PE_A_HASHMAP_H
