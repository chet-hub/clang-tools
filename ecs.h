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

typedef struct archetype_row {
    bool used;
    bool deleted;
} archetype_row;

typedef vector archetype_column;

typedef struct archetype {
    u64 bitset_Id;
    u32 index;
    vector *archetype_rows;
    hashmap *archetype_column_map;
} archetype;

typedef struct entity_Id {
    u32 archetype_Id;
    u32 entity_index;
} entity_Id;

typedef void (*system_function)(archetype_column ** column);

typedef struct run_object {
    int phrase;
    system_function *function;
    int param_length;
    vector *columns;
} run_object;

typedef struct system_type {
    void *function;
    int phrase;
    u64 bitset_Id;
    vector *args_component_index;
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
    vector *run_objects;
} world;

world *ecs_world_new() {
    world *w = malloc(sizeof(world));
    assert(w != NULL);
    w->archetypes = vector_new(sizeof(archetype), 0, 10, 1.5f);
    w->component_types = vector_new(sizeof(component_type), 0, 10, 1.5f);
    w->system_types = vector_new(sizeof(system_type), 0, 10, 1.5f);
    w->map = hashmap_new(6);
    w->run_objects =  vector_new(sizeof(run_object), 0, 10, 1.5f);
    assert(w->archetypes);
    assert(w->component_types);
    assert(w->system_types);
    assert(w->map);
    return w;
}

u32 ecs_component_register(world *world, char *name, u32 componentSize) {
    component_type *cop = (component_type *) hashmap_get(world->map, name, strlen(name)+1);
    if (cop != NULL) {
        return cop->index;
    }
    component_type ct = {
            .index = world->component_types->element_length,
            .name = realloc(NULL, strlen(name) + 1),
            .component_size = componentSize,
    };
    ct.name = strcpy(ct.name, name);
    assert(ct.name != NULL);
    VECTOR_PUSH(world->component_types, &ct);
    hashmap_put(world->map, ct.name, strlen(name)+1, VECTOR_AT(world->component_types, ct.index));
    return world->component_types->element_length - 1;
}

u32 ecs_archetype_entity_add(world *world, u32 archetype_index) {
    archetype *art = VECTOR_AT(world->archetypes, archetype_index);
    VECTOR_FOR_EACH(art->archetype_rows, index, status_ptr) {
        archetype_row *en = (archetype_row *) status_ptr;
        if (en->used == false) {
            en->used = true;
            return index;
        }
    }
    archetype_row art_row = {
            .used = true,
            .deleted = false,
    };
    VECTOR_PUSH(art->archetype_rows, &art_row);
    HASHMAP_FOR_EACH(art->archetype_column_map, kv) {
            vector *component_data_array = (vector *) kv->value;
            VECTOR_PUSH_NULL(component_data_array);
        }
    return art->archetype_rows->element_length - 1;
}

u32 ecs_archetype_register(world *world, char **component_names, u32 size) {
    bits64 bitset = 0;
    for (int i = 0; i < size; i++) {
        char * name = *(component_names + i);
        component_type *ct = (component_type *) hashmap_get(
                world->map,name,
                strlen(name)+1);
        assert(ct != NULL);
        bitset = BITWISE_SET_TRUE_AT(bitset, ct->index);
    }

    VECTOR_FOR_EACH(world->archetypes, index, art_ptr) {
        archetype *current = (archetype *) art_ptr;
        if (current->bitset_Id == bitset) {
            return current->index;
        }
    };

    archetype art = {
            .index = world->archetypes->element_length,
            .bitset_Id = bitset,
            .archetype_column_map = hashmap_new(5),
            .archetype_rows = vector_new(sizeof(archetype_row), 0, 10, 1.5f),
    };

    for (int i = 0; i < sizeof(bits64); i++) {
        if (BITWISE_IS_TRUE_AT(bitset, i)) {
            component_type *ct = VECTOR_AT(world->component_types, i);
            hashmap_put(art.archetype_column_map, &(ct->index), sizeof(u32), vector_new(ct->component_size, 0, 10, 1.5));
        }
    }
    VECTOR_PUSH(world->archetypes, &art);
    return world->archetypes->element_length - 1;
}

void ecs_system_register(world *world, int phrase, void *function, char **component_names, u32 size) {
    bits64 bitset = 0;
    vector *component_index_list = vector_new(sizeof(u32), 0, size, 1.5f);
    for (int i = 0; i < size; i++) {
        char * name = *(component_names + i);
        component_type *ct = (component_type *) hashmap_get(world->map, name,
                                                            strlen(name)+1);
        assert(ct != NULL);
        bitset = BITWISE_SET_TRUE_AT(bitset, ct->index);
        VECTOR_PUSH(component_index_list, &(ct->index));
    }
    system_type systemType = {
            .phrase = phrase,
            .bitset_Id = bitset,
            .function = function,
            .args_component_index = component_index_list,
            .function = function,
    };
    VECTOR_PUSH(world->system_types, &systemType);
}

entity_Id ecs_entity_new(world *world, char **component_names, u32 size) {
    u32 art_index = ecs_archetype_register(world, component_names, size);
    entity_Id Id = {
            .archetype_Id = art_index,
            .entity_index = ecs_archetype_entity_add(world, art_index),
    };
    return Id;
}

bool ecs_entity_remove(world *world, entity_Id entityId) {
    archetype *art = VECTOR_AT(world->archetypes, entityId.archetype_Id);
    if (entityId.entity_index < art->archetype_rows->element_length && entityId.entity_index >= 0) {
        archetype_row *row = (archetype_row *) VECTOR_AT(art->archetype_rows, entityId.entity_index);
        row->used = false;
        return true;
    }
    return false;
}

bool *ecs_set_component(world *world, entity_Id Id, char *component_name, void *component) {
    archetype *art = VECTOR_AT(world->archetypes, Id.archetype_Id);
    component_type *ct = (component_type *) hashmap_get(world->component_types, component_name, strlen(component_name) + 1);
    archetype_column *p = (archetype_column *) hashmap_get(art->archetype_column_map, &(ct->index), sizeof(u32));
    VECTOR_SET(p, Id.entity_index, component);
}

int ecs_run_objects_sort(void *a, void *b) {
    return ((run_object *) a)->phrase - ((run_object *) b)->phrase;
}

void ecs_run_objects_renew(world *world) {
    VECTOR_CLEAR(world->run_objects);
    VECTOR_FOR_EACH(world->system_types, index, system_type_ptr) {
        system_type *st = (system_type *) system_type_ptr;
        run_object ro = {
                .phrase = st->phrase,
                .function = st->function,
                .param_length = st->args_component_index->element_length,
                .columns = vector_new(sizeof(archetype_column *), 0, 6, 1.5f),
        };
        VECTOR_PUSH(world->run_objects, &ro);
        VECTOR_FOR_EACH(world->archetypes, i, archetype_ptr) {
            archetype *art = (archetype *) archetype_ptr;
            if (BITWISE_IS_COVER(art->bitset_Id, st->bitset_Id)) {
                VECTOR_FOR_EACH(st->args_component_index, j, ct_index_ptr) {
                    u32 *ind = (u32 *) ct_index_ptr;
                    archetype_column *ac = hashmap_get(art->archetype_column_map, ind, sizeof(u32));
                    assert(ac != NULL);
                    VECTOR_PUSH(ro.columns, &ac);
                }
            }
        }
    }
    VECTOR_SORT(world->run_objects, ecs_run_objects_sort);
}

void ecs_run(world *world) {
    ecs_run_objects_renew(world);
    VECTOR_FOR_EACH(world->run_objects, index, run_object_ptr) {
        run_object *ro = (run_object *) run_object_ptr;
        for(int i=0;i<ro->columns->element_length;i+=ro->param_length){
            system_function fn = ro->function;
            archetype_column ** columns_start = VECTOR_AT(ro->columns,i);
            fn(columns_start);
        }
    }
}


#endif //ECS_ECS_H
