#ifndef ECS_VECTOR2D_H_TEST
#define ECS_VECTOR2D_H_TEST
#include "vector2d.h"

void vector2d_test(){
    vector2d *v2d = vector2d_new(0, 10, 1.5f);
    assert(v2d->element_length == 0);

    for (int i = 0; i < 10; i++) {
        vector2d_row_push(v2d, sizeof(int), 0, 10, 1.5f);
        for (int j = 0; j < 10; j++) {
            vector2d_row_element_push(v2d, i, &j);
            printf("%d,", j);
        }
        printf("\n");
        assert(v2d->element_length == i + 1);
    }
    assert(v2d->element_length == 10);
    printf("\n");

    int temp_value = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            vector2d_get(v2d, j, i, &temp_value);
            printf("%d,", temp_value);
        }
        printf("\n");
    }

    printf("\n");

    temp_value = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            vector2d_get(v2d, j, i, &temp_value);
            temp_value++;
            vector2d_set(v2d, j, i, &temp_value);
            printf("%d,", temp_value);
        }
        printf("\n");
    }

    printf("\n");

    for (int i = 0; i < 10; i++) {
        vector temp_row;
        vector2d_row_pop(v2d, &temp_row);
        for (int j = 0; j < 10; j++) {
            vector2d_row_pop(&temp_row, &temp_value);
            printf("%d,", temp_value);
        }
        assert(temp_row.element_length == 0);
        assert(v2d->element_length == 9 - i);
        printf("\n");
    }
}


#endif