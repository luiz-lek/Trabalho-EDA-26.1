//
// Created by luizao on 01/06/2026.
//

#ifndef TRABALHO_EDA_26_1_HASH_H
#define TRABALHO_EDA_26_1_HASH_H

#define PATH_HASH_MOVIE_TABLE "../hash/movie_table/"
#define PATH_HASH_MOVIE_DATA "../hash/movie_data/"

#define PATH_HASH_PERSON_TABLE "../hash/person/person_table/"
#define PATH_HASH_PERSON_DATA "../hash/person_data/"

#define HASH_NAME_LENGTH 40
#define TAM_HASH 101

#include <stdint.h>

typedef struct {
    char name[HASH_NAME_LENGTH];
    uint32_t id;
    uint32_t next;
    uint8_t valid;
} data;

void hash_initialize(char *hash, char *data);
void hash_insert(char *hash, char *data, char *name, uint32_t id);
void remove_data(char *hash, char *data, uint32_t id);
uint32_t get_id(char *hash, char *data, char *name);

#endif //TRABALHO_EDA_26_1_HASH