//
// Created by bear on 2021-04-02.
//
#ifndef ECS_TEST
#define ECS_TEST

#include "ecs.h"

void print_map(world *w) {
    HASHMAP_FOR_EACH(w->map, kv) {
            printf("[%s,%d]\n\n", (char *) kv->key, kv->value);
        }
    printf("====\n");
}


void length_height_position_function(archetype_column ** columns){
    printf("------length_height_position_function------\n");
    for(int count = (* columns)->element_length,i=0;i<count;i++){
        int * length = (int *)VECTOR_AT((*(columns+0)),i);
        int * height = (int *)VECTOR_AT((*(columns+1)),i);
        A * position = (A *)VECTOR_AT((*(columns+1)),i);
        printf("[%d] length=%d, height=%d, position.x=%d ,position.y=%d\n",
               i, *length,*height,position->x,position->y);
    }
    printf("------------\n");
}

void length_function(archetype_column ** columns){
    printf("------length_function------\n");
    for(int count = (* columns)->element_length,i=0;i<count;i++){
        int * length = (int *)VECTOR_AT((*(columns+0)),i);
        printf("[%d] length=%d, height=%d, position.x=%d ,position.y=%d\n",
               i, *length);
    }
    printf("------------\n");
}

void length_height_function(archetype_column ** columns){
    printf("------length_height_function------\n");
    for(int count = (* columns)->element_length,i=0;i<count;i++){
        int * length = (int *)VECTOR_AT((*(columns+0)),i);
        int * height = (int *)VECTOR_AT((*(columns+1)),i);
        printf("[%d] length=%d, height=%d, position.x=%d ,position.y=%d\n",
               i, *length,*height);
    }
    printf("------------\n");
}



void ecs_test() {
    world *w = ecs_world_new();
    ecs_component_register(w, "length", sizeof(int));
    print_map(w);
    ecs_component_register(w, "length", sizeof(int));
    print_map(w);
    ecs_component_register(w, "height", sizeof(int));
    print_map(w);
    ecs_component_register(w, "position", sizeof(A));
    print_map(w);

    assert(w->component_types->element_length == 3);

    ecs_entity_new(w, (char *[]) {"length", "height", "position"}, 3);
    ecs_entity_new(w, (char *[]) {"length", "height", "position"}, 3);
    ecs_entity_new(w, (char *[]) {"length", "height", "position"}, 3);
    ecs_entity_new(w, (char *[]) {"length", "height", "position"}, 3);
    ecs_entity_new(w, (char *[]) {"length", "height", "position"}, 3);
    ecs_entity_new(w, (char *[]) {"length", "height", "position"}, 3);

    ecs_entity_new(w, (char *[]) {"length", "position"}, 2);
    ecs_entity_new(w, (char *[]) {"length", "position"}, 2);
    ecs_entity_new(w, (char *[]) {"length", "position"}, 2);
    ecs_entity_new(w, (char *[]) {"length", "position"}, 2);
    ecs_entity_new(w, (char *[]) {"length", "position"}, 2);
    ecs_entity_new(w, (char *[]) {"length", "position"}, 2);
    ecs_entity_new(w, (char *[]) {"length", "position"}, 2);

    ecs_entity_new(w, (char *[]) {"length", "height"}, 2);
    ecs_entity_new(w, (char *[]) {"length", "height"}, 2);
    ecs_entity_new(w, (char *[]) {"length", "height"}, 2);
    ecs_entity_new(w, (char *[]) {"length", "height"}, 2);
    ecs_entity_new(w, (char *[]) {"length", "height"}, 2);
    ecs_entity_new(w, (char *[]) {"length", "height"}, 2);
    ecs_entity_new(w, (char *[]) {"length", "height"}, 2);

    ecs_entity_new(w, (char *[]) {"position"}, 1);
    ecs_entity_new(w, (char *[]) {"position"}, 1);
    ecs_entity_new(w, (char *[]) {"position"}, 1);
    ecs_entity_new(w, (char *[]) {"position"}, 1);
    ecs_entity_new(w, (char *[]) {"position"}, 1);
    ecs_entity_new(w, (char *[]) {"position"}, 1);

    assert(w->archetypes->element_length == 4);

    ecs_system_register(w, 0, length_function, (char *[]){"length"}, 1);
    ecs_system_register(w, 1, length_height_function, (char *[]){"length", "height"}, 2);
    ecs_system_register(w, 2, length_height_position_function, (char *[]) {"length", "height", "position"}, 3);

    assert(w->system_types->element_length == 3);

    ecs_run(w);

}

#endif