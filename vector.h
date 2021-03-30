#ifndef ECS_VECTOR_H
#define ECS_VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef struct vector {
    char *element;
    int element_length;
    int sizeof_element;
    int capacity;
    int capacity_init;
    float capacity_extend_times;
} vector;

#define VECTOR_AT(vec_ptr, index) ((void *)(vec_ptr->element + index * vec_ptr->sizeof_element))
#define VECTOR_COPY_OUT(vec_ptr, index, out_ptr) memcpy(out_ptr, VECTOR_AT(vec_ptr,index), vec_ptr->sizeof_element)
#define VECTOR_COPY_IN(vec_ptr, index, in_ptr) memcpy(VECTOR_AT(vec_ptr,index), in_ptr, vec_ptr->sizeof_element)

extern inline bool vector_reallocate(vector * vec, bool extend){
    if (extend) {
        if (vec->element_length == vec->capacity
            && vec->capacity >= vec->capacity_init) {
            int newCapacity = vec->capacity * vec->capacity_extend_times;

            vec->element = realloc(vec->element, newCapacity * vec->sizeof_element);
            assert(vec->element != NULL);
            vec->capacity = newCapacity;
        }
    } else {
        if (vec->capacity > (int) (vec->capacity_init * vec->capacity_extend_times)
            && vec->element_length == (int) (vec->capacity / vec->capacity_extend_times) - 1) {
            int newCapacity = vec->element_length;

            vec->element = realloc(vec->element, newCapacity * vec->sizeof_element);
            assert(vec->element != NULL);
            vec->capacity = newCapacity;
        }
    }
    return true;
}

vector *vector_new(int sizeof_element, int length, int init_capacity, float capacity_extend_times) {
    assert(sizeof_element > 0);
    if (init_capacity <= 0) {
        init_capacity = 10;
    }
    if (capacity_extend_times <= 1) {
        capacity_extend_times = 1.5f;
    }
    int capacity = length >= init_capacity ? (int) (length * capacity_extend_times) : init_capacity;
    vector *v = (vector *) realloc(NULL, sizeof(vector));
    void *e = realloc(NULL, capacity * sizeof_element);
    if (v == NULL || e == NULL) {
        printf("vector_new: memory allocate failed");
        return NULL;
    } else {
        v->element_length = length;
        v->element = e;
        v->sizeof_element = sizeof_element;
        v->capacity = capacity;
        v->capacity_init = init_capacity;
        v->capacity_extend_times = capacity_extend_times;
        return v;
    }
}

#define VECTOR_PUSH(vec,push_element) \
    vector_reallocate(vec, true),\
    (memcpy(vec->element + (vec->element_length++) * vec->sizeof_element, \
    push_element, vec->sizeof_element) != NULL);

bool vector_push(vector *vec, void *element) {
    assert(vec != NULL);
    assert(element != NULL);
    return VECTOR_PUSH(vec,element);
}

#define VECTOR_POP(vec,pop_element) \
    vec->element_length--,      \
    ((memcpy(pop_element, vec->element + vec->element_length * vec->sizeof_element, vec->sizeof_element) != NULL)\
     && vector_reallocate(vec, false));

bool vector_pop(vector *vec, void *element) {
    assert(vec != NULL);
    assert(element != NULL);
    assert(vec->element_length >= 1);
    return VECTOR_POP(vec,element);
}

#define VECTOR_SET(vec,index,element) \
    (index < vec->element_length      \
    && memcpy(VECTOR_AT(vec, index),element, vec->sizeof_element)!= NULL);

bool vector_set(vector *vec, int index, void *element) {
    assert(vec != NULL);
    assert(element != NULL);
    assert(index >= 0);
    return VECTOR_SET(vec,index,element);
}

#define VECTOR_GET(vec,index,element) \
    (index >= 0 && index < vec->element_length \
    && memcpy(element, VECTOR_AT(vec, index),vec->sizeof_element) != NULL);

bool vector_get(vector *vec, int index, void *element) {
    assert(vec != NULL);
    assert(element != NULL);
    return VECTOR_GET(vec,index,element);
}

#define VECTOR_CLEAR(vec) \
    assert(vec != NULL);                \
    vec->element_length = 0;


#define VECTOR_FREE(vec) \
    assert(vec != NULL);                \
    vec->element_length = 0;            \
    free(vec->element);                 \
    free(vec);


#endif
