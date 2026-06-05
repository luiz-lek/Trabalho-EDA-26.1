//
// Created by luizao on 01/06/2026.
//

#ifndef TRABALHO_EDA_26_1_HASH_H
#define TRABALHO_EDA_26_1_HASH_H

#define TAM_HASH 101

#define KEY_STR_LENGTH 50

#include <stdint.h>
#include <string.h>

typedef uint8_t (*Comparator)(const void *a, const void *b);
typedef uint32_t (*HashFunction)(const void *key);

typedef struct {
    union {
        char as_string[KEY_STR_LENGTH];
        uint32_t as_id;
    } key;

    uint32_t valor ;
    uint32_t next;
    uint8_t valid;
} HashData;

void hash_initialize(const char *path_table, const char *path_data, int num_buckets);

void hash_insert(const char *path_table, const char *path_data, int num_buckets,
    Comparator comparator, HashFunction generate_brute_number, const void *key, uint32_t value, size_t key_size);

void hash_remove_data(const char *path_table, const char *path_data, int num_buckets,
    Comparator comparator, HashFunction generate_brute_number, const void *key);

uint32_t hash_get_value(const char *path_table, const char *path_data, int num_buckets,
    Comparator comparator, HashFunction generate_brute_number, const void *key);

void hash_change_value(const char *path_table, const char *path_data, int num_buckets,
    Comparator comparator, HashFunction generate_brute_number, const void *key, uint32_t new_value);

void hash_delete_hash(const char *path_table, const char *path_data);

#endif //TRABALHO_EDA_26_1_HASH