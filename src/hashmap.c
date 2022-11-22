#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include "include/hashmap.h"

#define DEFAULT_BUCKET_COUNT 16
#define MAX_BUCKET_ENTRIES 4


/**
 * Einfache Pseudo-Hash-Funktion.
 * Diese Operation ist umkehrbar, reicht aber für eine einfache Hashmap aus.
 *
 * @param i Wert, von dem der Hash berechnet werden soll
 * @see https://stackoverflow.com/a/12996028/18931874
 * @return Den Hash von i
 */
unsigned int hash_uint(unsigned int i) {
    i = ((i >> 16) ^ i) * 0x45d9f3b;
    i = ((i >> 16) ^ i) * 0x45d9f3b;
    i = (i >> 16) ^ i;
    return i;
}


hashmap_t* init_hashmap() {
    hashmap_t* map = malloc(sizeof(hashmap_t));

    map->buckets = init_list(sizeof(list_t*));
    map->buckets->element_free_function = (void (*)(void*)) free_list;

    for (size_t i = 0; i < DEFAULT_BUCKET_COUNT; i++) {
        list_t* bucket = init_list(sizeof(hashmap_entry_t*));
        map->buckets->element_free_function = (void (*)(void*)) free_hashmap_entry;
        list_add(map->buckets, bucket);

    }
    return map;
}

void hashmap_put_new_entry(hashmap_t* map, hashmap_entry_t* entry) {
    size_t bucket_index = entry->hashed_key % map->buckets->length;
    list_t* bucket = list_get(map->buckets, bucket_index);
    list_add(bucket, entry);
}

void hashmap_resize(hashmap_t* map) {
    size_t previous_bucket_count = map->buckets->length;

    list_t* entries = hashmap_into_entries(map);

    // Initialize new buckets and double the amount of buckets
    for (size_t b = 0; b < previous_bucket_count; b++) {
        list_t* new_bucket = init_list(sizeof(hashmap_entry_t*));
        map->buckets->element_free_function = (void (*)(void*)) free_hashmap_entry;
        list_add(map->buckets, new_bucket);
    }

    // Reinsert elements
    for (size_t i = 0; i < entries->length; i++) {
        hashmap_entry_t* entry = list_get(entries, i);
        hashmap_put_new_entry(map, entry);
    }
    entries->element_free_function = 0;
    free_list(entries);
}

void hashmap_put(hashmap_t* map, unsigned int key, void* value) {
    unsigned int hashed_key = hash_uint(key);

    size_t bucket_index = hashed_key % map->buckets->length;

    list_t* bucket = list_get(map->buckets, bucket_index);

    for (size_t i = 0; i < bucket->length; i++) {
        hashmap_entry_t* entry = list_get(bucket, i);
        if (hashed_key == entry->hashed_key) {
            entry->value = value;
            return;
        }
    }

    hashmap_entry_t* new_entry = init_hashmap_entry(key, hashed_key, value);
    if (bucket->length >= MAX_BUCKET_ENTRIES) {
        hashmap_resize(map);
        hashmap_put_new_entry(map, new_entry);
    } else {
        list_add(bucket, new_entry);
    }
}

void hashmap_print(hashmap_t* map) {
    for (int b = 0; b < map->buckets->length; b++) {
        list_t* bucket = list_get(map->buckets, b);
        printf("Bucket %i:\n", b);
        for (int i = 0; i < bucket->length; i++) {
            hashmap_entry_t* entry = list_get(bucket, i);
            printf("\tKey: %u  Value: %p\n", entry->key, entry->value);
        }
    }
}

void* hashmap_get(hashmap_t* map, unsigned int key) {
    unsigned int hashed_key = hash_uint(key);

    size_t bucket_index = hashed_key % map->buckets->length;
    list_t* bucket = list_get(map->buckets, bucket_index);

    for (size_t i = 0; i < bucket->length; i++) {
        hashmap_entry_t* entry = list_get(bucket, i);
        if (hashed_key == entry->hashed_key) {
            return entry->value;
        }
    }

    return NULL;
}

/**
 * !! Empties the Hashmap !!
 *
 * @return list of hashmap_entry_t elements
 */
list_t* hashmap_into_entries(hashmap_t* map) {
    list_t* entries = init_list(sizeof(hashmap_entry_t*));
    entries->element_free_function = (void (*)(void*)) free_hashmap_entry;

    for (int b = 0; b < map->buckets->length; b++) {
        list_t* bucket = list_get(map->buckets, b);

        while (bucket->length > 0) {
            list_add(entries, list_remove(bucket, 0));
        }
    }

    return entries;
}

void free_hashmap(hashmap_t* map) {
    free_list(map->buckets); // Automatically frees buckets and entries if free functions of list are still set
    free(map);
}

hashmap_entry_t* init_hashmap_entry(unsigned int key, unsigned int hashed_key, void* value) {
    hashmap_entry_t* entry = malloc(sizeof(hashmap_entry_t));
    entry->key = key;
    entry->hashed_key = hashed_key;
    entry->value = value;

    return entry;
}

void free_hashmap_entry(hashmap_entry_t* entry) {
    free(entry);
}
