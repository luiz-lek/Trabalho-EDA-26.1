//
// Created by luizao on 06/06/2026.
//

#ifndef TRABALHO_EDA_26_1_HASH_H
#define TRABALHO_EDA_26_1_HASH_H

#include <stdbool.h>
#include <stddef.h>

typedef int (*HashFunction)(const void *key);
typedef bool (*CompareFunction)(const void *a, const void *b);
typedef void (*PrintFunction)(const void *key, const void *valor);

typedef struct hash {
    void *key;
    void *value;
    struct hash *next;
} HashNode;

int hash(int num, int table_size);
HashNode** hash_inicialize(HashNode** table, int table_size);

void hash_insert(HashNode** table, int table_size, HashFunction generate_number, CompareFunction equal,
    const void *key, size_t key_size, const void *value, size_t value_size);

void hash_update_valor(HashNode** table, int table_size, HashFunction generate_number, CompareFunction equal, const void *key, const void *value, size_t value_size);

void hash_remove(HashNode** table, int table_size, HashFunction generate_number, CompareFunction equal, const void *key);
void hash_destroy(HashNode** table, int table_size);

void hash_print(HashNode** table, int table_size, PrintFunction print);

#endif //TRABALHO_EDA_26_1_HASH_H