#ifndef ECS_VECTOR_H_TEST
#define ECS_VECTOR_H_TEST
#include "vector.h"

typedef struct A {
    int x;
    int y;
} A;

int sort(void *a, void *b) {
    A *a_ptr = (A *) a;
    A *b_ptr = (A *) b;
    return a_ptr->x - b_ptr->x;
}


void vector_test(){
    vector *v = vector_new(sizeof(A), 0, 20, 1.5f);

    A a1 = (A) {.x = 1, .y=1};
    for (int i = 0; i < 1000; i++) {
        vector_push(v, &a1);
        //printf(",%d",v->capacity);
    }
    printf("\n");
    for (int i = 0; i < 1000; i++) {
        vector_pop(v, &a1);
        //printf(",%d",v->capacity);
    }

    A a2 = (A) {.x = 2, .y=2};
    A a3 = (A) {.x = 3, .y=3};

    assert(vector_push(v, &a1));
    A a1_temp;
    assert(vector_pop(v, &a1_temp));
    assert(a1.x == a1_temp.x && a1.y == a1_temp.y);


    assert(vector_push(v, &a1));
    int a1_index = v->element_length - 1;

    assert(vector_push(v, &a2));
    int a2_index = v->element_length - 1;

    assert(vector_push(v, &a3));
    int a3_index = v->element_length - 1;

    A a2_v;
    vector_get(v, a2_index, &a2_v);
    assert(a2_v.x == a2.x && a2_v.y == a2.y);

    A a3_v;
    vector_get(v, a3_index, &a3_v);
    assert(a3_v.x == a3.x && a3_v.y == a3.y);

    A a1_v;
    vector_get(v, a1_index, &a1_v);
    assert(a1_v.x == a1.x && a1_v.y == a1.y);

    vector_set(v, a2_index, &a1);
    A a21_v;
    vector_get(v, a2_index, &a21_v);
    assert(a21_v.x == a1.x && a21_v.y == a1.y);


    VECTOR_CLEAR(v);

    A a0 = (A) {.x = 0, .y=3};
    a1 = (A) {.x = 1, .y=3};
    a2 = (A) {.x = 2, .y=3};
    a3 = (A) {.x = 3, .y=3};
    A a4 = (A) {.x = 4, .y=3};
    A a5 = (A) {.x = 5, .y=3};
    A a6 = (A) {.x = 6, .y=3};
    A a7 = (A) {.x = 7, .y=3};

    vector_push(v, &a3);
    vector_push(v, &a2);
    vector_push(v, &a1);
    vector_push(v, &a7);
    vector_push(v, &a6);
    vector_push(v, &a4);
    vector_push(v, &a5);
    vector_push(v, &a0);

    VECTOR_FOR_EACH(v, index, value_ptr) {
        printf("v[%d].x = %d, v[%d].y = %d\n", index, ((A *) value_ptr)->x, index, ((A *) value_ptr)->y);
    }

    printf("\n====after sort====\n\n");
    VECTOR_SORT(v, sort);

    VECTOR_FOR_EACH(v, index, value_ptr1) {
        printf("v[%d].x = %d, v[%d].y = %d\n", index, ((A *) value_ptr1)->x, index, ((A *) value_ptr1)->y);
    }


    VECTOR_FREE(v);


    printf("\n====test hashmap======\n\n");
}

#endif