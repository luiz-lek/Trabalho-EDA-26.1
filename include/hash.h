//
// Created by luizao on 01/06/2026.
//

#ifndef TRABALHO_EDA_26_1_HASH_H
#define TRABALHO_EDA_26_1_HASH_H

#define TAM_HASH 101

#define HASH_NAME_LENGTH 40

#include <stdint.h>
#include <string.h>

typedef uint8_t (*Comparator)(void *a, void *b);
typedef uint32_t (*HashFunction)(void *key, int num_buckets);

typedef struct {
    union {
        char as_string[HASH_NAME_LENGTH];
        uint32_t as_id;
    } key;

    uint32_t valor ;
    uint32_t next;
    uint8_t valid;
} HashData;

void hash_initialize(char *path_table, char *path_data, int num_buckets);

void hash_insert(char *path_table, char *path_data, int num_buckets,
    Comparator comparator, HashFunction hash, void *key, uint32_t value, size_t key_size);

void hash_remove_data(char *path_table, char *path_data, int num_buckets,
    Comparator comparator, HashFunction hash, void *key);

uint32_t hash_get_value(char *path_table, char *path_data, int num_buckets,
    Comparator comparator, HashFunction hash, void *key);

void hash_change_value(char *path_table, char *path_data, int num_buckets,
    Comparator comparator, HashFunction hash, void *key, uint32_t new_value);

void hash_delete_hash(char *path_table, char *path_data);

#endif //TRABALHO_EDA_26_1_HASH