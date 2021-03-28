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


//XXH64_hash_t XXH3_64bits(const void* data, size_t len);

typedef struct hashmap{
    vector * key_values;
    vector * key_values_for_collision;
} hashmap;

hashmap * hashmap_new(int sizeof_element, int length, int init_capacity, float capacity_extend_times){
    hashmap * map = realloc(NULL,sizeof(hashmap));
    map
}



#endif //ECS_HASHMAP_H
