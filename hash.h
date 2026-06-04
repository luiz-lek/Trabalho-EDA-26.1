//
// Created by luizao on 01/06/2026.
//

#ifndef TRABALHO_EDA_26_1_HASH_H
#define TRABALHO_EDA_26_1_HASH_H

#define PATH_HASH_ID_TABLE "../hash/id_table/"
#define PATH_HASH_ID_DATA "../hash/id_data/"

#define PATH_HASH_PERSON "../hash/person_hash/"
#define PATH_HASH_PERSON_DATA "../hash/person_data/"

#define HASH_NAME_LENGTH 40

#include <stdint.h>

typedef struct {
    char name[HASH_NAME_LENGTH];
    uint32_t id;
    uint32_t next;
    uint8_t valid;
} data;

uint32_t get_id(char *hash, char *data, char *name);
void hash_insert(char *hash, char *data, char *name, uint32_t id);
void remove_data(char *hash, char *data, uint32_t id);

#endif //TRABALHO_EDA_26_1_HASH