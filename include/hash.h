//
// Created by luizao on 06/06/2026.
//

#ifndef TRABALHO_EDA_26_1_HASH_H
#define TRABALHO_EDA_26_1_HASH_H

#include <stddef.h>

#include "utils.h"

typedef int (*HashFunction)(const void *key);
typedef void (*PrintFunction)(const void *key, const void *valor);

typedef struct hash {
    void *key;
    void *value;
    struct hash *next;
} HashNode;

typedef struct {
    HashNode** table;
    int table_size;
    HashFunction generate_number;
    CompareFunction equal;
} HashCtx;

int hash(int num, int table_size);
void hash_inicialize(HashCtx *ctx, int table_size, HashFunction generate_number, CompareFunction equal);
void hash_insert(HashCtx *ctx, const void *key, size_t key_size, const void *value, size_t value_size);
void hash_update_valor(HashCtx *ctx, const void *key, const void *value, size_t value_size);
void* hash_lookup(HashCtx *ctx, const void *key);
void hash_remove(HashCtx *ctx, const void *key);
void hash_destroy(HashCtx *ctx);
void hash_print(HashCtx *ctx, PrintFunction print);

#endif //TRABALHO_EDA_26_1_HASH_H