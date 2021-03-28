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


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float f32;
typedef double f64;


#define NAME_LENGTH 100
#define COMPONENT_LENGTH 64
#define ARCHETYPE_LENGTH 10
#define INIT_LENGTH 10
#define EXTEND_LENGTH_TIMES 1.5


typedef struct component_type {
    char name[NAME_LENGTH];
    u32 component_size;
    u32 index;
} component_type;

typedef struct archetype {
    char name[NAME_LENGTH];
    u64 bitset_Id;
    u32 index;
    u32 entity_length;
    u32 entity_capacity;
} archetype;

typedef struct entity_Id {
    u32 component_index;
    u32 archetype_index;
    u32 entity_index;
} entity_Id;

typedef void * component_list;

typedef struct component_header {
    u32 *length;
    u32 component_index;
    u32 archetype_index;
} component_header;

typedef struct system_type {
    void *function;
    u64 bitset_Id;
    component_header * componentHeader;
    u32 component_header_length;
    u32 params_size;
} system_type;

typedef enum phrase{
    init,update,finish,_lastOne
} phrase;


typedef struct world {
    //register archetype
    u32 archetype_capacity;
    u32 archetype_length;
    archetype *archetypes;

    //register component_type
    u32 component_capacity;
    u32 component_length;
    component_type *component_type;

    //componentList table
    component_list *** component_List;

    //register systems table
    system_type *** system_type;
    u32 system_type_length[_lastOne];
    u32 system_type_capacity[_lastOne];

} world;

world * ecs_world_new() {

    world *w = malloc(sizeof(world));

    w->archetype_capacity = COMPONENT_LENGTH;
    w->archetype_length = 0;
    w->archetypes = malloc(sizeof(archetype) * ARCHETYPE_LENGTH);

    w->component_capacity = ARCHETYPE_LENGTH;
    w->component_length = 0;
    w->component_type = malloc(sizeof(component_type) * COMPONENT_LENGTH);

    w->component_List = malloc(sizeof(component_list *) * COMPONENT_LENGTH * ARCHETYPE_LENGTH);

//    w->systems_register = (systems_register) {
//            .capacity = {},
//            .length = {},
//    };
//    for(int i =0;i<_lastOne;i++) {
//        w->systems_register.length[i] = 0;
//        w->systems_register.capacity[i] = 0;
//    }
    if (w->archetypes == NULL || w->component_type == NULL || w->component_List == NULL) {
        printf("Ecs_new: malloc memory failed!\n");
        return NULL;
    }

    return w;
}

world * p_ecs_archetype_capacity_allocate(world * world) {
    u32 newCapacity = world->component_capacity * EXTEND_LENGTH_TIMES;
    archetype * newArchetype = malloc(sizeof(archetype) * newCapacity);
    if(newArchetype == NULL){
        printf("_ecs_extend_archetype_capacity failed!");
        return NULL;
    }else{
        memcpy(newArchetype,world->archetypes,world->component_capacity);
        free(world->archetypes);
        world->component_capacity = newCapacity;
        return world;
    }
}

bool ecs_component_register(world *world, char *name, u32 componentSize) {
    //if the component is full
    if(world->component_length == world->component_capacity){
        printf("Ecs_component_register: The component_type is full and can't register!\n");
        return false;
    }
    //if the name is available
    if (strlen(name) == 0) {
        printf("Ecs_component_register: Name is empty!\n");
        return false;
    } else if (strlen(name) >= NAME_LENGTH) {
        printf("Ecs_component_register: Length of [%s] is longer than COMPONENT_NAME_LENGTH in component_type!\n",
               name);
        return false;
    } else {
        for (int i = 0; i < world->component_length; i++) {
            char *component_name = (world->component_type + i)->name;
            int ret = strcmp(name, component_name);
            if (ret == 0) {
                printf("Ecs_component_register: The component name [%s] exist!\n", name);
                return false;
            }
        }
    }
    //register component
    u32 index = world->component_length++;
    component_type *componentType = world->component_type + index;
    strcpy(componentType->name, name);
    componentType->index = index;
    componentType->component_size = componentSize;
    return true;
}

component_list * p_ecs_componentList_allocate(u32 * capacityPointer, component_type * componentType, component_list * oldList){
    u32 newCapacity = oldList == NULL ? INIT_LENGTH : (int)((* capacityPointer) * EXTEND_LENGTH_TIMES);
    u32 size = componentType->component_size;
    void * newComponents = realloc(oldList,size * newCapacity);
    if(newComponents == NULL){
        printf("_ecs_componentList_allocate: realloc failed!\n");
        return NULL;
    }else{
        *capacityPointer = newCapacity;
        return newComponents;
    }
}

bool p_ecs_componentLists_allocate_for_archetype(world *world, archetype * archetype){
    for(int i=0;i<sizeof(u64) && i<world->component_length;i++){
        if((((u64)1 << i) & archetype->bitset_Id) !=0){
            if(p_ecs_componentList_allocate(&archetype->entity_capacity,
                                            world->component_type + i,
                                            world->component_List[archetype->index][i]) == NULL){
                printf("_ecs_archetype_register: allocate all of the components fails!\n");
                return false;
            }
        }
    }
    return true;
}

archetype * p_ecs_archetype_new(world *world, u64 bitset_Id) {
    for(int i=0;i<world->archetype_length;i++){
        if(bitset_Id == world->archetypes->bitset_Id){
            printf("_ecs_archetype_register: The bitset_Id exist!\n");
            return NULL;
        }
    }

    if(world->archetype_length == world->component_capacity){
        world = p_ecs_archetype_capacity_allocate(world);
        if(world == NULL){
            return NULL;
        }
    }
    int index = world->archetype_length++;
    archetype * current = (archetype *)(world->archetypes + world->archetype_length);
    current->index = index;
    current->bitset_Id = bitset_Id;
    current->entity_length = 0;
    current->entity_capacity = 0;
    //allocate all of the components of this archetype.
    if(p_ecs_componentLists_allocate_for_archetype(world, current)){
        return current;
    }else{
        return NULL;
    }
}

u64 utils_to_bitset_Id(world *world, char ** component_names, u32 size){
    u64 bitset = 0;
    for(int i=0;i<size;i++){
        bool hasName = false;
        for(int j=0;j<world->component_length;j++){
            if(strcmp((world->component_type+j)->name,component_names[i])==0){
                u32 componentIndex = (u32)j;
                bitset ^= (u64)1 << componentIndex;
                hasName = true;
            }
        }
        if(!hasName){
            printf("utils_to_bitset_Id: can't find component named %s!",component_names[i]);
        }
    }
    return bitset;
}

bool ecs_archetype_register(world *world, char * archetype_name, char ** component_names, u32 size) {
    //if the archetype_name is available
    if (strlen(archetype_name) == 0) {
        printf("ecs_archetype_register: archetype_name is empty!\n");
        return false;
    } else if (strlen(archetype_name) >= NAME_LENGTH) {
        printf("ecs_archetype_register: Length of [%s] is longer than NAME_LENGTH in component_type!\n",
               archetype_name);
        return false;
    } else {
        u64 bitset_id = utils_to_bitset_Id(world,component_names,size);
        for (int i = 0; i < world->archetype_length; i++) {
            char *name = (world->archetypes + i)->name;
            int ret = strcmp(name, archetype_name);
            if (ret == 0) {
                if(world->archetypes[i].bitset_Id == bitset_id){
                    //printf("Ecs_component_register: The archetype [%s] exist!\n", name);
                    return true;
                }else{
                    printf("Ecs_component_register: The archetype [%s] exist, but has different bitset_id! "
                           "please change the name or component names\n", archetype_name);
                    return false;
                }
            }else{
                if(world->archetypes[i].bitset_Id == bitset_id){
                    printf("Ecs_component_register: The archetype name [%s] doesn't exist, but with a same bitset_id, "
                           "please change the component names, or use exist name [%s]\n", archetype_name,name);
                    return false;
                }else{
                    //printf("Ecs_component_register: The archetype [%s] doesn't exist, will create one\n", name);
                    archetype * archetype_current = p_ecs_archetype_new(world, bitset_id);
                    strcpy(archetype_current->name,archetype_name);
                    return archetype_current == NULL ? false : true;
                }
            }
        }
    }
}

bool p_ecs_systems_register_phrase_allocate(world * world,phrase phrase_index){
//        int newCapacity = world->systems_register.capacity[phrase_index] == 0 ? INIT_LENGTH
//                : (int) (world->systems_register.capacity[phrase_index] * EXTEND_LENGTH_TIMES);
//        system_type * newSystemType = (system_type *)realloc(world->systems_register.system_type[phrase_index],
//                                                             sizeof(system_type *) * newCapacity);
//        if(newSystemType == NULL){
//            printf("p_ecs_systems_register_allocate: malloc (system_type *) failed!\n");
//            return false;
//        }
//        world->systems_register.capacity[phrase_index] = newCapacity;
//        world->systems_register.system_type[phrase_index] = newSystemType;
    return true;
}

void * utils_memory_reallocate(void * ptr, int * capacity, int length){
    if(*capacity == length){
        int newCapacity = (ptr == NULL)? INIT_LENGTH : (int)(length * EXTEND_LENGTH_TIMES);
        void * newPtr = realloc(ptr,newCapacity);
        if(newPtr != NULL){
            *capacity = newCapacity;
            return newPtr;
        }
    }
    return NULL;
}

void ecs_system_register(world * world, phrase phrase_index, void * system, char ** component_names, u32 size) {
//    if(world->systems_register.capacity[phrase_index] == world->systems_register.length[phrase_index]){
//        p_ecs_systems_register_phrase_allocate(world, phrase_index);
//    }
//    u64 bitset = utils_to_bitset_Id(world,component_names,size);
//    system_type * systemType = realloc(NULL,sizeof(system_type *));
//    int capacity = 0;
//    systemType->componentHeader = (component_header *) utils_memory_reallocate(NULL, &capacity, 0);
//    if(systemType->componentHeader == NULL){
//        //todo
//    }
//    systemType->component_header_length = 0;
//    systemType->bitset_Id = bitset;
//    for(int i=0;i<world->archetype_length;i++){
//        if(utils_bitset_contains((world->archetypes + i)->bitset_Id, bitset)){
//            for(int j=0;j<world->component_length;j++){
//                if(utils_bitset_contains(bitset,((u64)1) << j)){
//                    component_header * currentHeader =  systemType->componentHeader + (systemType->component_header_length++);
//                    currentHeader->length = &((world->archetypes + i)->entity_length);
//                    currentHeader->archetype_index = i;
//                    currentHeader->component_index = j;
//                }
//            }
//        }
//    }
}


void ecs_run(char *phrases) {
    //todo
}


void *ecs_entity_new(char * archetype_name) {
    //todo
}

void *ecs_entity_remove(char * archetype_name) {
    //todo
}

void *ecs_set_component(entity_Id Id, char * component_name, void * component) {
    //todo
}



#endif //ECS_ECS_H
