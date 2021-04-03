//
// Created by bear on 2021-04-02.
//
#ifndef ECS_BITWISE_H_TEST
#define ECS_BITWISE_H_TEST

#include "bitwise.h"
#include <assert.h>
void bitwise_test(){

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
    assert(bitwise_contain(bits, a001));
    assert(bitwise_contain(bits, a002));
    assert(bitwise_contain(bits, a003));
    assert(!bitwise_contain(bits, a004));

    assert(bitwise_set_true_at(a005, 0) == a004);
    assert(bitwise_set_true_at(a001, 4) == bits);

    assert(bitwise_set_false_at(a004, 0) == a005);
    assert(bitwise_set_false_at(bits, 4) == a001);

    assert(bitwise_is_false_at(bits, 0) == true);
    assert(bitwise_is_true_at(bits, 2) == true);

    assert(bitwise_value_at(bits, 0) == false);
    assert(bitwise_value_at(bits, 1) == false);
    assert(bitwise_value_at(bits, 2) == true);
    assert(bitwise_value_at(bits, 3) == false);
    assert(bitwise_value_at(bits, 4) == true);
}


#endif