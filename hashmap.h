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

typedef struct key_value key_value;

typedef struct key_value {
    void *key;
    int key_size;
    void *value;
    key_value *next;
} key_value;

key_value * key_value_new(void *key,int key_size, void * value){
    key_value * kv = realloc(NULL,sizeof(key_value));
    assert(kv!=NULL);
    kv->key = realloc(NULL,key_size);
    assert(kv->key!=NULL);
    kv->key = memcpy(kv->key,key,key_size);
    assert(kv->key!=NULL);
    kv->value = value;
    kv->next = NULL;
    return kv;
}

typedef struct hashmap {
    vector *key_values;
    u32 bit_shift_value;
} hashmap;

hashmap *hashmap_new(u32 bit_shift_value) {
    assert(bit_shift_value >= 1 && bit_shift_value < 64 );
    hashmap *map = realloc(NULL, sizeof(hashmap));
    assert(map != NULL);
    u32 init_capacity = ((u32) 2) << bit_shift_value;
    map->bit_shift_value = bit_shift_value;
    map->key_values = vector_new(sizeof(key_value), init_capacity, init_capacity, 1.5f);
    assert(map->key_values != NULL);
    key_value kv = {
            .next = NULL,
            .key = NULL,
            .value = NULL,
    };
    for (u32 i = 0; i < init_capacity; i++) {
        VECTOR_COPY_IN(map->key_values, i, &kv);
    }
    return map;
}

#define KEY_TO_INDEX(index, map, key, key_size) \
    uint64_t hash_value = XXH3_64bits(key, key_size); \
    u32 rest_bit = 64 - map->bit_shift_value; \
    u32 index =  ( hash_value << rest_bit ) >> rest_bit;

#define GET_KV(map, index) \
    (key_value *) (map->key_values->element + index * map->key_values->sizeof_element);

#define FREE_KV(kv) \
    free(kv->key);  \
    free(kv);

void hashmap_put(hashmap *map, void *key,int key_size, void *value) {
    KEY_TO_INDEX(index, map, key,key_size);
    key_value *kv = GET_KV(map, index);
    if(kv->key == NULL){
        kv->key = realloc(NULL,key_size);
        kv->key = memcpy(kv->key,key,key_size);
        assert(kv->key!=NULL);
        kv->value = value;
        return;
    }
    while ((memcmp(key, kv->key,key_size) != 0)) {
        if (kv->next == NULL) {
            kv->next = key_value_new(key,key_size,value);
            return;
        }
        kv = kv->next;
    }
    kv->value = value;
}

bool hashmap_contain(hashmap *map, void *key,int key_size) {
    KEY_TO_INDEX(index, map, key,key_size);
    key_value *kv = GET_KV(map, index);
    if(kv->key == NULL){
        return false;
    }
    while ((memcmp(key, kv->key,key_size) != 0)) {
        if (kv->next == NULL) {
            return false;
        }
        kv = kv->next;
    }
    return true;
}

void * hashmap_get(hashmap *map, void *key,int key_size) {
    KEY_TO_INDEX(index, map, key,key_size);
    key_value *kv = GET_KV(map, index);
    if(kv->key == NULL){
        return NULL;
    }
    while ((memcmp(key, kv->key,key_size) != 0)) {
        if (kv->next == NULL) {
            return NULL;
        }
        kv = kv->next;
    }
    return kv->value;
}

bool hashmap_remove(hashmap *map, void *key,int key_size) {
    KEY_TO_INDEX(index, map, key,key_size);
    key_value *kv = GET_KV(map, index);
    //find key in header
    if(kv->key == NULL){
        return false;
    }
    if (memcmp(key, kv->key,key_size) == 0) {
        key_value *new_kv = kv->next;
        if(new_kv == NULL){
            kv->key = NULL;
            kv->value = NULL;
            kv->next = NULL;
            return true;
        }
        kv->key = new_kv->key;
        kv->value = new_kv->value;
        kv->next = new_kv->next;
        return true;
    } else {//to find key in the next chains
        key_value * parent = kv;
        kv = kv->next;
        while (kv!=NULL && (memcmp(key, kv->key,key_size) != 0)) {
            if (kv->next == NULL) {
                return false;
            }
            parent = kv;
            kv = kv->next;
        }
        if(kv == NULL){
            return false;
        }else{
            key_value * to_free_value = parent->next;
            parent->next = kv->next;
            FREE_KV(to_free_value)
            return true;
        }
    }
}

void _hashmap_key_value_free(key_value * kv) {
    if (kv == NULL) {
        return;
    } else {
        if (kv->next != NULL) {
            _hashmap_key_value_free(kv->next);
            FREE_KV(kv);
        } else {
            FREE_KV(kv);
        }
    }
}

void hashmap_free(hashmap *map) {
    for (u32 i = 0; i < map->key_values->element_length; i++) {
        key_value *kv = GET_KV(map, i);
        _hashmap_key_value_free(kv->next);
    }
    VECTOR_FREE(map->key_values);
    free(map);
}


#endif //ECS_HASHMAP_H
