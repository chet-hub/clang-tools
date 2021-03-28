//#include <stdio.h>
//#include "ecs.h"
#include "vector.h"
#define XXH_INLINE_ALL
#include "xxhash.h"

int main() {

    typedef struct Position {
        int x;
        int y;
    } Position;

    typedef struct A {
        int x;
        int y;
    } A;

//
//    world *w = ecs_world_new();
//
//    //test ecs_component_register
//    if (ecs_component_register(w, "Position", sizeof(Position)) == 0
//        && ecs_component_register(w, "A", sizeof(A)) == 1
//        && ecs_component_register(w, "A", sizeof(Position)) == -1
//        && ecs_component_register(w, "", sizeof(Position)) == -1) {
//        printf("====ecs_component_register=== ok\n");
//    }

    vector * v = vector_new(sizeof(A),0,20,1.5f);

    A a1 = (A){.x = 1, .y=2};
    for(int i=0;i<1000;i++){
        vector_push(v, &a1);
        printf(",%d",v->capacity);
    }
    printf("\n");
    for(int i=0;i<1000;i++){
        vector_pop(v,&a1);
        printf(",%d",v->capacity);
    }
    A a2 = (A){.x = 2, .y=3};
    A a3 = (A){.x = 3, .y=4};

    int a1_index = vector_push(v, &a1);
    int a2_index = vector_push(v, &a2);
    int a3_index = vector_push(v, &a3);
    A * a2_v;
    vector_get(v, a2_index,&a2_v);
    A * a3_v;
    vector_get(v, a3_index,&a2_v);
    A * a1_v;
    vector_get(v, a1_index,&a2_v);

    //vector_set(v,a2_index,&a1);


//    char * a = malloc(1000);
//    memcpy(a,&a1,sizeof(A));
//    memcpy(a + sizeof(A),&a2,sizeof(A));
//    memcpy(a + 2 * sizeof(A),&a3,sizeof(A));
//    A * test1 = (A*)a ;
//    A * test2 = (A*)a + 1;
//    A * test3 = (A*)a + 2;


    char * data = "13123213";
    size_t len = strlen(data);
    uint64_t h = XXH3_64bits(data, len);

    printf("\n%jd",h);

    vector * table = vector_new(sizeof(vector),0,20,1.5f);
    vector_push(table,v);
    vector v2;
    vector_pop(table,&v2);
    return 0;
}
