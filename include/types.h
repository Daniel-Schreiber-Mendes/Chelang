#ifndef TYPES_H
#define TYPES_H
#include <assert.h>

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef unsigned long long int U64;

typedef U8 C; //char
typedef void V; //void
typedef U8 B; //bool

typedef char I8;
typedef short I16;
typedef int I32;
typedef long long int I64;

#define STATIC_ASSERT(cond, msg) typedef char static_assertion_##msg[(cond) ? 1 : -1];

STATIC_ASSERT(__CHAR_BIT__ == 8, U8_is_8_bits)
STATIC_ASSERT(sizeof(U16) == 2, U16_is_16_bits)
STATIC_ASSERT(sizeof(U32) == 4, U32_is_32_bits)
STATIC_ASSERT(sizeof(U64) == 8, U64_is_64_bits)


#endif