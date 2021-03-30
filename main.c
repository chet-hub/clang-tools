#include <stdio.h>
#include "vector.h"
#include "vector2d.h"
#include "hashmap.h"


int main() {


    /**
     * test vector
     */

    typedef struct A {
        int x;
        int y;
    } A;

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
    int a1_index = v->element_length -1;

    assert(vector_push(v, &a2));
    int a2_index = v->element_length -1;

    assert(vector_push(v, &a3));
    int a3_index = v->element_length -1;

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
    VECTOR_FREE(v);

    /**
     * test hashmap
     */
    hashmap *map = hashmap_new(3);
    int a = 1,*temp;

    char s[10][3] = {"cc0","cc1","cc2","cc2","cc4","cc5","cc6","cc7","cc8","cc9"};
    for(int i=0;i<10;i++){
        hashmap_put(map, s[i], &a);
    }

    for(int i=0;i<10;i++){
        temp = hashmap_get(map, s[i]);
        assert(temp != NULL && *temp == 1);
    }
    assert(hashmap_remove(map, "not exit key") != true);
    for(int i=0;i<10;i++){
        assert(hashmap_remove(map, s[i]));
    }
    assert(hashmap_get(map, s[0]) == NULL);

    hashmap_free(map);

    /**
     * test table
     */
    vector2d * v2d = vector2d_new(0,10,1.5f);
    assert(v2d->element_length == 0);

    for(int i=0;i<10;i++){
        vector2d_row_push(v2d,sizeof(int),0,10,1.5f);
        for(int j=0;j<10;j++){
            vector2d_row_element_push(v2d,i,&j);
            printf("%d,",j);
        }
        printf("\n");
        assert(v2d->element_length == i+1);
    }
    assert(v2d->element_length == 10);
    printf("\n");

    int temp_value=0;
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            vector2d_get(v2d,j,i,&temp_value);
            printf("%d,",temp_value);
        }
        printf("\n");
    }

    printf("\n");

    temp_value=0;
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            vector2d_get(v2d,j,i,&temp_value);
            temp_value++;
            vector2d_set(v2d,j,i,&temp_value);
            printf("%d,",temp_value);
        }
        printf("\n");
    }

    printf("\n");

    for(int i=0;i<10;i++){
        vector temp_row;
        vector2d_row_pop(v2d,&temp_row);
        for(int j=0;j<10;j++){
            vector2d_row_pop(&temp_row,&temp_value);
            printf("%d,",temp_value);
        }
        assert(temp_row.element_length == 0);
        assert(v2d->element_length == 9-i);
        printf("\n");
    }

    return 0;
}
