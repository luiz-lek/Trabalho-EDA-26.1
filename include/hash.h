//
// Created by luizao on 06/06/2026.
//

#ifndef TRABALHO_EDA_26_1_HASH_H
#define TRABALHO_EDA_26_1_HASH_H

#include <stdbool.h>
#include <stddef.h>

typedef int (HashFunction)(const void *key);
typedef bool (*CompareFunction)(const void *a, const void *b);

#define TAM_HASH 101

typedef struct hash {
    void *key;
    void *value;
    struct hash *next;
} HashList;

typedef HashList* TH[TAM_HASH];

int hash(int num);
HashList* hash_list_create();
void hash_inicilize(TH table);

void hash_insert(TH table, HashFunction generate_number, CompareFunction equal,
    const void *key, size_t key_size, const void *value, size_t value_size);

void hash_update_valor(TH table, HashFunction generate_number, CompareFunction equal, const void *key, const void *value, size_t value_size);

void hash_remove(TH table, HashFunction generate_number, CompareFunction equal, const void *key);
void hash_destroy(TH table);

#endif //TRABALHO_EDA_26_1_HASH_H