#ifndef ECS_VECTOR_H
#define ECS_VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>


typedef uint32_t u32;

typedef struct vector {
    char *element;
    u32 element_length;
    u32 sizeof_element;
    u32 capacity;
    u32 capacity_init;
    float capacity_extend_times;
} vector;

#define VECTOR_AT(vec_ptr, at) ((void *)(vec_ptr->element + at * vec_ptr->sizeof_element))
#define VECTOR_COPY_OUT(vec_ptr, index, out_ptr) memcpy(out_ptr, VECTOR_AT(vec_ptr,index), vec_ptr->sizeof_element)
#define VECTOR_COPY_IN(vec_ptr, index, in_ptr) memcpy(VECTOR_AT(vec_ptr,index), in_ptr, vec_ptr->sizeof_element)
#define VECTOR_FOR_EACH(vec_ptr, index, value_ptr) \
    void * value_ptr = VECTOR_AT(vec_ptr, 0);      \
    for(int index=0;                               \
        index < vec_ptr->element_length;           \
        index++,value_ptr = VECTOR_AT(vec_ptr, index))


extern inline bool vector_reallocate(vector *vec, bool extend) {
    if (extend) {
        if (vec->element_length == vec->capacity
            && vec->capacity >= vec->capacity_init) {
            u32 newCapacity = vec->capacity * vec->capacity_extend_times;

            vec->element = realloc(vec->element, newCapacity * vec->sizeof_element);
            assert(vec->element != NULL);
            vec->capacity = newCapacity;
        }
    } else {
        if (vec->capacity > (u32) (vec->capacity_init * vec->capacity_extend_times)
            && vec->element_length == (u32) (vec->capacity / vec->capacity_extend_times) - 1) {
            u32 newCapacity = vec->element_length;

            vec->element = realloc(vec->element, newCapacity * vec->sizeof_element);
            assert(vec->element != NULL);
            vec->capacity = newCapacity;
        }
    }
    return true;
}

vector *vector_new(u32 sizeof_element, u32 length, u32 init_capacity, float capacity_extend_times) {
    assert(sizeof_element > 0);
    if (init_capacity <= 0) {
        init_capacity = 10;
    }
    if (capacity_extend_times <= 1) {
        capacity_extend_times = 1.5f;
    }
    u32 capacity = length >= init_capacity ? (u32) (length * capacity_extend_times) : init_capacity;
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
#define VECTOR_PUSH_NULL(vec) \
    vector_reallocate(vec, true),\
    (memset(vec->element + (vec->element_length++) * vec->sizeof_element, \
    0, vec->sizeof_element) != NULL)

#define VECTOR_PUSH(vec, push_element) \
    vector_reallocate(vec, true),\
    (memcpy(vec->element + (vec->element_length++) * vec->sizeof_element, \
    push_element, vec->sizeof_element) != NULL)

bool vector_push(vector *vec, void *element) {
    assert(vec != NULL);
    assert(element != NULL);
    return VECTOR_PUSH(vec, element);
}

#define VECTOR_POP(vec, pop_element) \
    vec->element_length--,      \
    ((memcpy(pop_element, vec->element + vec->element_length * vec->sizeof_element, vec->sizeof_element) != NULL)\
     && vector_reallocate(vec, false));

bool vector_pop(vector *vec, void *element) {
    assert(vec != NULL);
    assert(element != NULL);
    assert(vec->element_length >= 1);
    return VECTOR_POP(vec, element);
}

#define VECTOR_SET(vec, index, element) \
    (index < vec->element_length      \
    && memcpy(VECTOR_AT(vec, index),element, vec->sizeof_element)!= NULL);

bool vector_set(vector *vec, u32 index, void *element) {
    assert(vec != NULL);
    assert(element != NULL);
    assert(index >= 0);
    return VECTOR_SET(vec, index, element);
}

#define VECTOR_GET(vec, index, element) \
    (index >= 0 && index < vec->element_length \
    && memcpy(element, VECTOR_AT(vec, index),vec->sizeof_element) != NULL);

bool vector_get(vector *vec, u32 index, void *element) {
    assert(vec != NULL);
    assert(element != NULL);
    return VECTOR_GET(vec, index, element);
}

#define VECTOR_CLEAR(vec) \
    assert(vec != NULL);                \
    vec->element_length = 0;

#define VECTOR_FREE(vec) \
    assert(vec != NULL);                \
    vec->element_length = 0;            \
    free(vec->element);                 \
    free(vec);

typedef int (*sort_function)(void *element_a, void * element_b);

#define SWAP(vec, i, j, temp) \
     (memcpy(temp, VECTOR_AT(vec, j), vec->sizeof_element) != NULL \
            && memcpy(VECTOR_AT(vec, j), VECTOR_AT(vec, i), vec->sizeof_element) != NULL \
            && memcpy(VECTOR_AT(vec, i), temp, vec->sizeof_element) != NULL);

int partition(vector *vec, int low, int high, void *temp, sort_function fun) {
    void *pivot = VECTOR_AT(vec, high); // pivot
    int i =  low ;
    for (int j = low; j <= high ; j++) {
        if (fun(VECTOR_AT(vec, j),pivot) < 0 ) {
            SWAP(vec, i, j, temp);
            i++;
        }
    }
    SWAP(vec, i, high, temp);
    return i;
}

//https://en.wikipedia.org/wiki/Quicksort
void quickSort(vector *vec, int low, int high, void *temp, sort_function fun) {
    if (low < high) {
        int pi = partition(vec, low, high, temp, fun);
        quickSort(vec, low, pi - 1, temp, fun);
        quickSort(vec, pi + 1, high, temp, fun);
    }
}

#define VECTOR_SORT(vec, compare_function) \
    void *temp_for_vector_sort = realloc(NULL, vec->sizeof_element); \
    assert(temp_for_vector_sort != NULL); \
    quickSort(vec, 0, vec->element_length - 1, temp_for_vector_sort, compare_function); \
    free(temp_for_vector_sort);


#endif
