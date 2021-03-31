//
// Created by bear on 2021-03-18.
//

#ifndef ECS_ECS_H
#define ECS_ECS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "vector.h"
#include "vector2d.h"
#include "hashmap.h"
#include "bitwise.h"


typedef uint32_t u32;
typedef uint64_t u64;

typedef struct component_type {
    char *name;
    u32 component_size;
    u32 index;
} component_type;

typedef struct archetype {
    char *name;
    u64 bitset_Id;
    u32 index;
    u32 length;
    vector2d *components
} archetype;

typedef struct entity_Id {
    u32 archetype_index;
    u32 component_index;
    u32 entity_index;
} entity_Id;

typedef struct input_component {
    void *components;
    u32 component_index;
    u32 archetype_index;
} input_component;

typedef struct system_type {
    void *function;
    char *phrase;
    u64 bitset_Id;
    vector2d *input_components;
} system_type;

typedef struct world {
    //register archetype
    vector *archetypes;
    //register component_type
    vector *component_types;
    //register systems
    vector *system_types;
    //quick query
    hashmap *map;
} world;

world *ecs_world_new() {
    world *w = malloc(sizeof(world));
    assert(w != NULL);
    w->archetypes = vector_new(sizeof(archetype), 0, 10, 1.5f);
    w->component_types = vector_new(sizeof(component_type), 0, 10, 1.5f);
    w->system_types = vector_new(sizeof(system_type), 0, 10, 1.5f);
    w->map = hashmap_new(6);
    assert(w->archetypes);
    assert(w->component_types);
    assert(w->system_types);
    assert(w->map);
    return w;
}

void ecs_component_register(world *world, char *name, u32 componentSize) {
    component_type ct = {
            .index = world->component_types->element_length,
            .name = realloc(NULL, strlen(name) + 1),
            .component_size = componentSize,
    }
    ct.name = strcpy(ct.name, name);
    assert(ct.name != NULL);
    VECTOR_PUSH(world->component_types, &ct);
    hashmap_put(map, name, VECTOR_AT(world->component_types, ct.index));
}

void ecs_archetype_register(world *world, char *archetype_name, char **component_names, u32 size) {
    bits64 bitset = 0;
    for (int i = 0; i < size; i++) {
        component_type *ct = (component_type *) hashmap_get(world->map, *(component_names + i));
        assert(ct != NULL);
        BITWISE_SET_TRUE_AT(bitset, ct->index);
    }
    archetype art = {
            .index = world->archetypes->element_length,
            .name = realloc(NULL, strlen(archetype_name) + 1),
            .bitset_Id = bitset,
            .length = 0,
            .components = vector2d_new(size, 0, 1.5f);
    }
    art.name = strcpy(ct.name, archetype_name);
    assert(art.name != NULL);
    VECTOR_PUSH(world->archetypes, &art);
    hashmap_put(map, archetype_name, VECTOR_AT(world->archetypes, art.index));
}

void SYSTEM_INPUT_NEW(world *world, vector2d *input_components, bits64 input_bit, u32 input_size) {
    VECTOR_FOR_EACH(world->archetypes, index, art, {
        archetype *art_ptr = (archetype *) art;
        if (BITWISE_COVER(art_ptr->bitset_Id, input_bit)) {
            vector2d_row_push(input_components, sizeof(input_component), 0, input_size, 1.5f);
            for (int i = 0; i < sizeof(bits64) && input_size > 0; i++) {
                if(BITWISE_IS_TRUE_AT(input_bit,i)){
                    input_size --;
                    input_component inputComponent = {
                            .component_index = i,
                            .archetype_index = art_ptr->index,
                    };
                    vector2d_row_element_push(input_components,input_components->element_length,&inputComponent);
                }
            }
        }
    })

    VECTOR_PUSH(systemInput, sys_input);
}

void ecs_system_register(world *world, char *phrase, void *function, char **component_names, u32 size) {
    bits64 bitset = 0;
    for (int i = 0; i < size; i++) {
        component_type *ct = (component_type *) hashmap_get(world->map, *(component_names + i));
        assert(ct != NULL);
        BITWISE_SET_TRUE_AT(bitset, ct->index);
    }
    system_type systemType = {
            .phrase = realloc(NULL, strlen(phrase) + 1),
            .bitset_Id = bitset,
            .function = function,
            .input_components = vector2d_new(0, 5, 1.5f),
    };
    systemType.phrase = strcpy(systemType.phrase, phrase);
    assert(systemType.phrase != NULL);
    VECTOR_PUSH(world->system_types, &systemType);
    hashmap_put(world->map,phrase,VECTOR_AT(world->system_types, world->system_types->element_length - 1));
    SYSTEM_INPUT_NEW(world, systemType.input_components, bitset, size);
}


void ecs_run(world *world,) {
    //init
    system_type * init_functions = hashmap_get(world->map,"init");
    //update
    system_type * update_functions = hashmap_get(world->map,"update");
    //finish
    system_type * finish_functions = hashmap_get(world->map,"finish");



}


void *ecs_entity_new(char *archetype_name) {
    //todo
}

void *ecs_entity_remove(char *archetype_name) {
    //todo
}

void *ecs_set_component(entity_Id Id, char *component_name, void *component) {
    //todo
}


#endif //ECS_ECS_H
