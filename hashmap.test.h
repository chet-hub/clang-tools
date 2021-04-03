//
// Created by bear on 2021-04-02.
//
#ifndef ECS_HASHMAP_H_TEST
#define ECS_HASHMAP_H_TEST

#include "hashmap.h"

void hashmap_test() {
    hashmap *map = hashmap_new(3);
    int a = 1, *temp;

    //char * words[] = {"length", "height", "position"};
    //(char *[]){"length", "height", "position"}
    char **s = (char **) malloc(10 * sizeof(char *));

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

    for (int i = 0; i < 10; i++) {
        hashmap_put(map, s[i], sizeof(int), &a);
    }

    for (int i = 0; i < 10; i++) {
        temp = hashmap_get(map, s[i], sizeof(int));
        assert(temp != NULL && *temp == 1);
    }

    HASHMAP_FOR_EACH(map, kv) {
        printf("[%s,%d]\n", (char *) kv->key, *((int *) (kv->value)));
    }

    assert(hashmap_remove(map, "not exit key", sizeof("not exit key")) != true);
    for (int i = 0; i < 10; i++) {
        if (i == 3) {//remove the same key twice
            assert(!hashmap_remove(map, s[i], sizeof(int)));
        } else {
            assert(hashmap_remove(map, s[i], sizeof(int)));
        }
    }
    assert(hashmap_get(map, s[0], sizeof(int)) == NULL);

    char * name = "cdsdfsdfdsf";
    a = 0;
    hashmap_put(map,name,strlen(name),&a);
    assert(hashmap_get(map,name,strlen(name)) == &a);

    hashmap_free(map);
}


#endif