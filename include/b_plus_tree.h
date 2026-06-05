//
// Created by luizao on 29/05/2026.
//

#ifndef TRABALHO_EDA_26_1_DATABASE_H
#define TRABALHO_EDA_26_1_DATABASE_H

#include <stdint.h>

typedef struct {
    uint8_t is_leaf;
    uint16_t num_keys;
    uint32_t *key;
    uint32_t *offset; // Se for nó interno, aponta para os dados, caso contrário, é ponteiro pra filho.
    uint32_t next; // Se for folha
} TreeNode;

void tree_initialize(uint8_t t, char *index, char *data, char *metadata);
uint32_t tree_search(char *index, char *metadata, uint32_t key);
void tree_insert(char *index, char *metadata, uint32_t key, uint32_t offset_data);
void tree_remove(char *index, char *metadata, uint32_t key);
void tree_print(char *index, char *metadata);

typedef struct {
    uint8_t t;
    uint32_t root;
    uint32_t first_leaf;
    uint32_t last_id_generated;
} Header;

void load_header(const char *metadata_file, Header *h);
void save_header(const char *metadata_file, Header *h);

#endif //TRABALHO_EDA_26_1_DATABASE_H