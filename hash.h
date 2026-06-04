//
// Created by luizao on 01/06/2026.
//

#ifndef TRABALHO_EDA_26_1_HASH_H
#define TRABALHO_EDA_26_1_HASH_H

#define PATH_HASH_MOVIE_TABLE "../hash/movie/table.dat"
#define PATH_HASH_MOVIE_DATA "../hash/movie/data.dat"

#define PATH_HASH_PERSON_TABLE "../hash/person/table.dat"
#define PATH_HASH_PERSON_DATA "../hash/person/data.dat"

# define TAM_HASH 101

#define HASH_NAME_LENGTH 40

#include <stdint.h>

typedef struct {
    char name[HASH_NAME_LENGTH];
    uint32_t id;
    uint32_t next;
    uint8_t valid;
} HashData;

void hash_initialize(char *table, char *data, uint32_t num_buckets);
void hash_insert(char *table, char *data, uint32_t num_buckets, const char *string, uint32_t id);
void remove_data(char *table, char *data, uint32_t num_buckets, const char *string);
uint32_t get_id(char *table, char *data, uint32_t num_buckets, const char *string);

#endif //TRABALHO_EDA_26_1_HASH