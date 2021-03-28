#ifndef ECS_TABLE_H
#define ECS_TABLE_H

#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef vector table;

table *table_new(int column_length, int column_init_capacity, float column_capacity_extend_times) {
    table *columns = vector_new(sizeof(vector), column_length, column_init_capacity, column_capacity_extend_times);
    assert(columns != NULL);
    return columns;
}

bool _table_reallocate(table *tab, bool extend) {
    return _vector_reallocate(tab, extend);
}

int table_column_push(table *tab, int sizeof_element, int length, int init_capacity, float capacity_extend_times) {
    vector *row = vector_new(sizeof_element, length, init_capacity, capacity_extend_times);
    _table_reallocate(tab,true);
    return vector_push(tab, row);
}

int table_column_pop(table *tab, vector *row) {
    return vector_pop(tab, row);
}

int table_row_value_push(table *tab, int column_index, void *in_row_value) {
    assert(column_index >= 0 && column_index < tab->element_length)
    vector *row = (vector *) (tab->element + column_index * tab->sizeof_element);
    return vector_push(row, in_row_value);
}

int table_row_value_pop(table *tab,int column_index, void *out_row_value) {
    assert(column_index >= 0 && column_index < tab->element_length)
    vector *row = (vector *) (tab->element + column_index * tab->sizeof_element);
    return vector_pop(row, out_row_value);
}

bool table_set(table *tab, int row_index, int column_index, void *in_value) {
    assert(column_index >= 0 && column_index < tab->element_length)
    vector *row = (vector *) (tab->element + column_index * tab->sizeof_element);
    return vector_set(row,row_index,in_value)
}

bool table_get(table *tab, int row_index, int column_index, void *out_value) {
    assert(column_index >= 0 && column_index < tab->element_length)
    vector *row = (vector *) (tab->element + column_index * tab->sizeof_element);
    return vector_set(row,row_index,out_value)
}

bool table_clear(table *tab) {
    for(int i=0;i<tab->element_length;i++){
        vector * v = tab->elements + i * tab->sizeof_element;
        vector_clear(v);
    }
}

bool vector_free(table *tab) {
    for(int i=0;i<tab->element_length;i++){
        vector * v = tab->elements + i * tab->sizeof_element;
        vector_free(v);
    }
    free(tab);
}

#endif
