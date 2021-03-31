#ifndef ECS_VECTOR2D_H
#define ECS_VECTOR2D_H

#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef vector vector2d;

vector2d *vector2d_new(u32 column_length, u32 column_init_capacity, float column_capacity_extend_times) {
    vector2d *columns = vector_new(sizeof(vector), column_length, column_init_capacity, column_capacity_extend_times);
    assert(columns != NULL);
    return columns;
}

bool vector2d_row_push(vector2d *vec2d, u32 sizeof_element, u32 length, u32 init_capacity, float capacity_extend_times) {
    vector *row = vector_new(sizeof_element, length, init_capacity, capacity_extend_times);
    return VECTOR_PUSH(vec2d, row);
}

bool vector2d_row_pop(vector2d *vec2d, vector *row) {
    return VECTOR_POP(vec2d, row);
}

#define VECTOR2D_GET_ROW(vec2d, column_index) \
    (vector *)(vec2d->element + column_index * vec2d->sizeof_element);


bool vector2d_row_element_push(vector2d *vec2d, u32 column_index, void *in_row_value) {
    assert(column_index >= 0 && column_index < vec2d->element_length);
    vector *row = VECTOR2D_GET_ROW(vec2d, column_index);
    return VECTOR_PUSH(row, in_row_value);
}

bool vector2d_row_element_pop(vector2d *vec2d, u32 column_index, void *out_row_value) {
    assert(column_index >= 0 && column_index < vec2d->element_length);
    vector *row = VECTOR2D_GET_ROW(vec2d, column_index);
    return VECTOR_POP(row, out_row_value);
}

bool vector2d_set(vector2d *vec2d, u32 row_index, u32 column_index, void *in_value) {
    assert(column_index >= 0 && column_index < vec2d->element_length);
    vector *row = VECTOR2D_GET_ROW(vec2d, column_index);
    return VECTOR_SET(row, row_index, in_value);
}

bool vector2d_get(vector2d *vec2d, u32 row_index, u32 column_index, void *out_value) {
    assert(column_index >= 0 && column_index < vec2d->element_length);
    vector *row = VECTOR2D_GET_ROW(vec2d, column_index);
    return VECTOR_GET(row, row_index, out_value);
}

#define TABLE_CLEAR(tab) { \
    for (u32 i = 0; i < tab->element_length; i++) { \
        vector *v = VECTOR_AT(tab, i); \
        VECTOR_CLEAR(v); \
    } \
}

#define TABLE_FREE(tab) { \
    for(u32 i=0;i<tab->element_length;i++){ \
    vector * v = VECTOR_AT(tab,i);\
    VECTOR_FREE(v);\
    }\
    free(tab);


#endif
