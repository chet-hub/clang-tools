
#ifndef ECS_LINKNODE_H
#define ECS_LINKNODE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef struct node {
    char * data;
    int size;
    struct node * next;
} node;

node * node_new(void * data, int size){
    node * n = (node *)realloc(NULL, sizeof(node));
    assert(n != NULL);
    n->data = (char *)realloc(NULL, sizeof(char) * size);
    assert(n->data != NULL);
    n->size = size;
    assert(size > 0);
    void * re = memcpy(n->data,data,sizeof(char) * size);
    assert(re != NULL);
    n->next = NULL;
    return n;
}

void node_next(node * first, node * next){
    assert(first->next == NULL);
    assert(next != NULL);
    first->next = next;
}

void node_value(node * n, void * out_value){
    assert(first != NULL);
    assert(out_value != NULL);
    void * re = memcpy(out_value, n->data, sizeof(char) * n->size);
    assert(re != NULL);
}

void node_equal(node * first, node * second){
    assert(first != NULL);
    assert(second != NULL);
    int result = strcmp(first->data,second->data);
    return result == 0;
}

#endif
