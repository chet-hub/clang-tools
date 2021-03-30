#include <stdio.h>
#include "vector.h"
#include "table.h"
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

    int a1_index = vector_push(v, &a1);
    assert(a1_index == 0);

    int a2_index = vector_push(v, &a2);
    assert(a2_index == 1);

    int a3_index = vector_push(v, &a3);
    assert(a3_index == 2);

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

    vector_free(v);


    /**
     * test hashmap
     */
    hashmap *map = hashmap_new(3);
    int a = 1,temp = 0;

    char s[10][3] = {"cc0","cc1","cc2","cc2","cc4","cc5","cc6","cc7","cc8","cc9"};
    for(int i=0;i<10;i++){
        hashmap_put(map, s[i], &a, sizeof(a));
    }

    for(int i=0;i<10;i++){
        assert(hashmap_get(map, s[i], &temp) && temp == 1);
    }

    assert(hashmap_remove(map, "not exit key") != true);

    for(int i=0;i<10;i++){
        assert(hashmap_remove(map, s[i]));
    }

    assert(hashmap_get(map, s[0], &temp) == false);



    hashmap_free(map);

    return 0;
}
