//
// Created by luizao on 01/06/2026.
//

#ifndef TRABALHO_EDA_26_1_HASH_H
#define TRABALHO_EDA_26_1_HASH_H

#define TAM_HASH 101

#define KEY_STR_LENGTH 50

#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef uint8_t (*CompareFunction)(const void *a, const void *b);
typedef uint32_t (*HashFunction)(const void *key);

typedef struct {
    union {
        char as_string[KEY_STR_LENGTH];
        uint32_t as_id;
    } key;

    uint32_t valor;
    uint32_t next;
    uint8_t valid;
} HashDiskNode;

typedef struct {
    FILE *fph;
    FILE *fpd;
    CompareFunction comparator;
    HashFunction generate_brute_number;
    int num_buckets;
    size_t key_size;
    char data_file_name[70];
    char hash_file_name[70];
} HashContext;

void hash_disk_initialize(const char *path_table, const char *path_data, int num_buckets);
void hash_disk_insert(HashContext *ctx, const void *key, uint32_t valor);
void hash_disk_remove_data(HashContext *ctx, int num_buckets, const void *key);
uint32_t hash_disk_get_value(HashContext *ctx, const void *key);
void hash_disk_change_value(HashContext *ctx, const void *key, uint32_t new_value);
void hash_disk_delete_table(const char *path_table, const char *path_data);

#endif //TRABALHO_EDA_26_1_HASH