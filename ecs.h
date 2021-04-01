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
typedef int64_t s64;

typedef struct component_type {
    char *name;
    u32 component_size;
    u32 index;
} component_type;

typedef struct archetype_row {
    bool used;
    bool deleted;
} archetype_row;

typedef struct archetype_column{
    u32 component_index;
    vector * data;
} archetype_column;

typedef struct archetype {
    u64 bitset_Id;
    u32 index;
    vector *archetype_row;
    vector *archetype_column;
} archetype;

typedef struct entity_Id {
    u32 archetype_Id;
    u32 entity_index;
} entity_Id;

typedef struct run_object {
    void *function;
    void *param_length;
    vector *params;
} run_object;

typedef struct system_type {
    void *function;
    int phrase;
    u64 bitset_Id;
} system_type;

typedef struct world {
    //register archetype
    vector *archetypes;
    //register component_type
    vector *component_types;
    //register systems
    vector *system_types;
    //quick query map
    hashmap *map;
    //system run object
    vector *run_object;
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

u32 ecs_component_register(world *world, char *name, u32 componentSize) {
    component_type *cop = (component_type *) hashmap_get(map, name);
    if (cop != NULL) {
        return cop->index;
    }
    component_type ct = {
            .index = world->component_types->element_length,
            .name = realloc(NULL, strlen(name) + 1),
            .component_size = componentSize,
    }
    ct.name = strcpy(ct.name, name);
    assert(ct.name != NULL);
    VECTOR_PUSH(world->component_types, &ct);
    hashmap_put(map, name, VECTOR_AT(world->component_types, ct.index));
    return world->component_types->element_length - 1;
}

s64 ecs_archetype_find(world *world, char **component_names, u32 size) {
    bits64 bitset = 0;
    for (int i = 0; i < size; i++) {
        component_type *ct = (component_type *) hashmap_get(world->map, *(component_names + i));
        assert(ct != NULL);
        BITWISE_SET_TRUE_AT(bitset, ct->index);
    }
    s64 result = -1;
    VECTOR_FOR_EACH(world->archetypes, index, art_ptr) {
        archetype *current = (archetype *) art_ptr;
        if (current->bitset_Id == bitset) {
            result = current->index;
        }
    }
    return result;
}

u32 ecs_archetype_entity_add(world *world, u32 archetype_index) {
    archetype *art = VECTOR_AT(world->archetypes, archetype_index);
    VECTOR_FOR_EACH(art->archetype_row, index, status_ptr) {
        entity_status *en = (entity_status *) status_ptr;
        if (en->used == false) {
            en->used = true;
            return index;
        }
    }
    VECTOR_PUSH(art->archetype_row, &(entity_status) {
            .used = true,
            .deleted = false,
    });

    VECTOR_FOR_EACH(art->archetype_column, column, component_ptr){
        archetype_column * row = (archetype_column * rom)component_ptr;
        component_type * ct = (component_type *)VECTOR_AT(world->component_types,row->component_index);

        vector_push(row->data,NULL) //todo
    }
    return art->archetype_row->element_length - 1;
}

u32 ecs_archetype_register(world *world, char **component_names, u32 size) {
    archetype art = {
            .index = world->archetypes->element_length,
            .bitset_Id = bitset,
            .archetype_column = vector_new(sizeof(archetype_column), 0, 10, 1.5f),
            .archetype_row = vector_new(sizeof(entity_status), 0, 0, 1.5f),
    }
    VECTOR_PUSH(world->archetypes, &art);
    return world->archetypes->element_length - 1;
}

void ecs_system_register(world *world, int phrase, void *function, char **component_names, u32 size) {
    bits64 bitset = 0;
    for (int i = 0; i < size; i++) {
        component_type *ct = (component_type *) hashmap_get(world->map, *(component_names + i));
        assert(ct != NULL);
        BITWISE_SET_TRUE_AT(bitset, ct->index);
    }
    system_type systemType = {
            .phrase = phrase,
            .bitset_Id = bitset,
            .function = function,
    };
    VECTOR_PUSH(world->system_types, &systemType);
}

void ecs_run_objects_new(world *world, system_type *systemType) {
    //todo
}

void ecs_system_type_run(system_type *systemType) {
    //todo
}

int ecs_system_sort(void *a, void *b) {
    return ((system_type *) a)->phrase - ((system_type *) b)->phrase;
}

void ecs_run(world *world) {
    VECTOR_SORT(world->system_types, ecs_system_sort);
    assert(world->system_types->element_length >= 3);
    //todo
}

entity_Id ecs_entity_new(world *world, char **component_names, u32 size) {
    s64 result = ecs_archetype_find(world,component_names,size);
    u32 art_index = (result > 0) ? (u32) result : ecs_archetype_register();
    entity_Id Id;
    Id.archetype_Id = art_index;
    Id.entity_index = ecs_archetype_entity_add(world,art_index);
    return Id;
}

bool ecs_entity_remove(world *world, entity_Id entityId) {
    archetype *art = VECTOR_AT(world->archetypes, entityId.archetype_Id);
    if (entityId.entity_index < art->archetype_row->element_length && entityId.entity_index >= 0) {
        entity_status *en = (entity_status *) VECTOR_AT(art->archetype_row, entityId.entity_index);
        en->used = false;
        return true;
    }
    return false;
}

void *ecs_set_component(entity_Id Id, char *component_name, void *component) {
    //todo
}


#endif //ECS_ECS_H
