//
// Created by bear on 2021-03-26.
//

#ifndef ECS_HASHMAP_H
#define ECS_HASHMAP_H

#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define XXH_INLINE_ALL

#include "xxhash.h"

#define KEY_LENGTH 50

typedef struct key_value key_value;

typedef struct key_value {
    char key[KEY_LENGTH];
    void *value;
    int size;
    key_value *next;
} key_value;

key_value * key_value_new(char *key, void * value, int size){
    key_value * kv = realloc(NULL,sizeof(key_value));
    assert(kv!=NULL);
    kv->size = size;
    strcpy(kv->key,key);
    kv->value = value;
    kv->next = NULL;
    return kv;
}

typedef struct hashmap {
    vector *key_values;
    unsigned int bit_shift_value;
} hashmap;

hashmap *hashmap_new(int bit_shift_value) {
    assert(bit_shift_value >= 1 && bit_shift_value < 64 );
    hashmap *map = realloc(NULL, sizeof(hashmap));
    assert(map != NULL);
    int init_capacity = ((int) 2) << bit_shift_value;
    map->bit_shift_value = bit_shift_value;
    map->key_values = vector_new(sizeof(key_value), init_capacity, init_capacity, 1.5f);
    assert(map->key_values != NULL);
    key_value kv = {
            .next = NULL,
            .key = "",
            .value = NULL,
            .size = 0
    };
    for (int i = 0; i < init_capacity; i++) {
        VECTOR_COPY_IN(map->key_values, i, &kv);
    }
    return map;
}

#define KEY_TO_INDEX(index, map, key) \
    uint64_t hash_value = XXH3_64bits(key, strlen(key)); \
    int rest_bit = 64 - map->bit_shift_value; \
    int index =  ( hash_value << rest_bit ) >> rest_bit;

#define GET_KV(map, index) \
    (key_value *) (map->key_values->element + index * map->key_values->sizeof_element);


void hashmap_put(hashmap *map, char *key, void *value, int size) {
    KEY_TO_INDEX(index, map, key);
    key_value *kv = GET_KV(map, index);
    while ((strcmp(key, kv->key) != 0)) {
        if (kv->next == NULL) {
            kv->next = key_value_new(key,value,size);
        }
        kv = kv->next;
    }
    kv->value = value;
    kv->size = size;
}

bool hashmap_contain(hashmap *map, char *key) {
    KEY_TO_INDEX(index, map, key);
    key_value *kv = GET_KV(map, index);
    while ((strcmp(key, kv->key) != 0)) {
        if (kv->next == NULL) {
            return false;
        }
        kv = kv->next;
    }
    return true;
}

bool hashmap_get(hashmap *map, char *key, void *value) {
    KEY_TO_INDEX(index, map, key);
    key_value *kv = GET_KV(map, index);
    while ((strcmp(key, kv->key) != 0)) {
        if (kv->next == NULL) {
            return false;
        }
        kv = kv->next;
    }
    return (memcpy(value,kv->value,kv->size) != NULL);
}

bool hashmap_remove(hashmap *map, char *key) {
    KEY_TO_INDEX(index, map, key);
    key_value *kv = GET_KV(map, index);
    //find key in header
    if (strcmp(key, kv->key) == 0) {
        key_value *new_kv = kv->next;
        free(kv);
        strcpy(new_kv->key, key);
        kv->value = new_kv->value;
        kv->size = new_kv->size;
        kv->next = new_kv->next;
        return true;
    } else {//find key in the next chains
        key_value * parent = kv;
        kv = kv->next;
        while (kv!=NULL && (strcmp(key, kv->key) != 0)) {
            if (kv->next == NULL) {
                return false;
            }
            parent = kv;
            kv = kv->next;
        }
        parent->next = kv->next;
        free(kv);
        return true;
    }
}

void _hashmap_key_value_free(key_value * kv) {
    if (kv == NULL) {
        return;
    } else {
        if (kv->next != NULL) {
            _hashmap_key_value_free(kv->next);
            free(kv);
        } else {
            free(kv);
        }
    }
}

void hashmap_free(hashmap *map) {
    for (int i = 0; i < map->key_values->element_length; i++) {
        key_value *kv = GET_KV(map, i);
        _hashmap_key_value_free(kv->next);
    }
    vector_free(map->key_values);
    free(map);
}


#endif //ECS_HASHMAP_H
