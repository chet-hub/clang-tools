#include <stdio.h>
#include "vector.h"
#include "vector2d.h"
#include "hashmap.h"
#include "bitwise.h"
#include "ecs.h"

#include "vector.test.h"
#include "vector2d.test.h"
#include "hashmap.test.h"
#include "bitwise.test.h"
#include "ecs.test.h"


int main() {
    vector_test();
    vector2d_test();
    hashmap_test();
    bitwise_test();
    ecs_test();

//    hashmap *map = hashmap_new(3);
    char ** words = (char **) malloc(10 * sizeof(char *));
    words[0] = "length";
    words[1] = "height";
    words[2] = "position";
//
//    int l = strlen(words[0] ) + 1;
//    hashmap_put(map,"length",l,&l);
//    int *ct = hashmap_get(map,words[0],l);
//    printf("%d",ct);

    char ** tst = (char *[]) {"length", "height", "position"};
    char * test = "length";
    printf("%d\n\n",memcmp("height",*(tst+1),strlen(words[0]) + 1)) ;

    //char str1[15];
    char * str1[] = {"abcdef"};
    //char str2[15];
    char * str2[] = {"abcdef"};
    int ret = memcmp(str1, str2, 5);
    if(ret > 0) {
        printf("str2 is less than str1");
    } else if(ret < 0) {
        printf("str1 is less than str2");
    } else {
        printf("str1 is equal to str2");
    }


    return 0;
}
