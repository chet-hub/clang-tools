#include <stdio.h>
#include "vector.h"
#include "vector2d.h"
#include "hashmap.h"
#include "bitwise.h"


typedef struct A {
    int x;
    int y;
} A;

int sort(void * a, void * b){
    A * a_ptr = (A *)a;
    A * b_ptr = (A *)b;
    return a_ptr->x - b_ptr->x;
}

int main() {

    /**
     * test vector
     */

    vector * v = vector_new(sizeof(A), 0, 20, 1.5f);

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

    VECTOR_FOR_EACH(v, index, value_ptr){
        printf("v[%d].x = %d, v[%d].y = %d\n",index,((A *)value_ptr)->x,index,((A *)value_ptr)->y);
    }

    printf("\n====after sort====\n\n");
    VECTOR_SORT(v,sort);

    VECTOR_FOR_EACH(v, index, value_ptr1){
        printf("v[%d].x = %d, v[%d].y = %d\n",index,((A *)value_ptr1)->x,index,((A *)value_ptr1)->y);
    }


    VECTOR_FREE(v);


    printf("\n====test hashmap======\n\n");


    /**
     * test hashmap
     */
    hashmap *map = hashmap_new(3);
    int a = 1,*temp;


    char **s = (char**)malloc(10 * sizeof(char*));

    s[0] = "cc0";
    s[1] = "cc1";
    s[2] = "cc2";
    s[3] = "cc2";
    s[4] = "cc4";
    s[5] = "cc5";
    s[6] = "cc6";
    s[7] = "cc7";
    s[8] = "cc8";
    s[9] = "cc9";

    for(int i=0;i<10;i++){
        hashmap_put(map, s[i], &a);
    }

    for(int i=0;i<10;i++){
        temp = hashmap_get(map, s[i]);
        assert(temp != NULL && *temp == 1);
    }
    assert(hashmap_remove(map, "not exit key") != true);
    for(int i=0;i<10;i++){
        if(i==3){//remove the same key twice
            assert(!hashmap_remove(map, s[i]));
        }else{
            assert(hashmap_remove(map, s[i]));
        }
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


    /**
     * test bitwise
     */
    bits64 bits = 0b0000000000000000000000000000000000000000000000000000000000010100;
    bits64 a000 = 0b0000000000000000000000000000000000000000000000000000000000000000;
    bits64 a001 = 0b0000000000000000000000000000000000000000000000000000000000000100;
    bits64 a002 = 0b0000000000000000000000000000000000000000000000000000000000010000;
    bits64 a003 = 0b0000000000000000000000000000000000000000000000000000000000010100;
    bits64 a004 = 0b0000000000000000000000000000000000000000000000000000000000010101;
    bits64 a005 = 0b0000000000000000000000000000000000000000000000000000000000010100;
    assert(bitwise_contain(bits,a001));
    assert(bitwise_contain(bits,a002));
    assert(bitwise_contain(bits,a003));
    assert(!bitwise_contain(bits,a004));

    assert(bitwise_set_true_at(a005,0) == a004);
    assert(bitwise_set_true_at(a001,4) == bits);

    assert(bitwise_set_false_at(a004,0) == a005);
    assert(bitwise_set_false_at(bits,4) == a001);

    assert(bitwise_is_false_at(bits,0) == true);
    assert(bitwise_is_true_at(bits,2) == true);

    assert(bitwise_value_at(bits,0) == false);
    assert(bitwise_value_at(bits,1) == false);
    assert(bitwise_value_at(bits,2) == true);
    assert(bitwise_value_at(bits,3) == false);
    assert(bitwise_value_at(bits,4) == true);

    return 0;
}
