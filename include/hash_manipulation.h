//
// Created by luizao on 06/06/2026.
//

#ifndef TRABALHO_EDA_26_1_HASH_MANIPULATION_H
#define TRABALHO_EDA_26_1_HASH_MANIPULATION_H

#include <stdint.h>

#include "hash.h"

typedef struct Hash Hash;

typedef enum {
    INT_INT,
    STRING_INT,
    STR_STR,
    TUPLE_INT
} HashType;

struct Hash {
    HashCtx ctx;
    void (*insert)(Hash *self, const void*, const void*);
    void (*update_valor)(Hash *self, const void*, const void*);
    void* (*lookup)(Hash *self, const void*);
    void (*remove)(Hash *self, const void*);
    void (*destroy)(Hash *self);
    void (*print)(Hash *self);
};

void hash_create(Hash *hash, HashType type, int size);

#endif //TRABALHO_EDA_26_1_HASH_MANIPULATION_H