//
// Created by bear on 2021-03-30.
//

#ifndef ECS_BITWISE_H
#define ECS_BITWISE_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


typedef uint64_t bits64;

#define BITWISE_COVER(bits,b) ((bits | b) == bits)

#define BITWISE_SET_TRUE_AT(bits,index) (bits | (((bits64)1) << index))

#define BITWISE_SET_FALSE_AT(bits,index) (bits & (~(((bits64)1) << index)))

#define BITWISE_IS_TRUE_AT(bits,index) (bitwise_set_true_at(bits, index) == bits)

#define BITWISE_IS_FALSE_AT(bits,index) (!bitwise_is_true_at(bits,index))

#define BITWISE_VALUE_AT(bits,index) (bitwise_is_true_at(bits,index))


static inline bits64 char_to_bits(const char *s)
{
    bits64 i = 0;
    while (*s) {
        i <<= 1;
        i += *s++ - '0';
    }
    return i;
}

static inline bits64 bits_to_char(bits64 bits)
{
    for (int bit_index = sizeof(bits)-1; bit_index >= 0; --bit_index)
    {
        bool bit = bits >> bit_index & true;
        printf("%d", bit);
    }
}

bits64 bitwise_contain(bits64 bits, bits64 b){
    return (bits | b) == bits;
}

bits64 bitwise_set_true_at(bits64 bits, int index){
    return bits | (((bits64)1) << index);
}

bits64 bitwise_set_false_at(bits64 bits, int index){
    return bits & (~(((bits64)1) << index));
}

bool bitwise_is_true_at(bits64 bits, int index){
     return bitwise_set_true_at(bits, index) == bits;
}

bool bitwise_is_false_at(bits64 bits, int index){
    return !bitwise_is_true_at(bits,index);
}

bool bitwise_value_at(bits64 bits, int index){
    return bitwise_is_true_at(bits,index);
}


#endif //ECS_BITWISE_H
