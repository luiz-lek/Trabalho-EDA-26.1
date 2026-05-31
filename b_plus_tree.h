//
// Created by luizao on 29/05/2026.
//

#ifndef TRABALHO_EDA_26_1_DATABASE_H
#define TRABALHO_EDA_26_1_DATABASE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t is_leaf;
    uint16_t num_keys;
    uint32_t *key;
    uint32_t *offset; // Se for nó interno, aponta para os dados, caso contrário, é ponteiro pra filho.
    uint32_t next; // Se for folha
} TreeNode;

void tree_initialize(uint8_t t, char *index, char *data, char *metadata);
uint32_t tree_search(FILE *index, uint8_t t, uint32_t offset, uint32_t key);
void tree_insert(char *index, char *metadata, uint32_t key, uint32_t offset_data);
void tree_print(char *index, char *metadata);

#endif //TRABALHO_EDA_26_1_DATABASE_H