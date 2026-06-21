//
// Created by luizao on 29/05/2026.
//

#ifndef TRABALHO_EDA_26_1_DATABASE_H
#define TRABALHO_EDA_26_1_DATABASE_H

#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t is_leaf;
    uint16_t num_keys;
    uint32_t *key;
    uint32_t *offset; // Se for nó interno, aponta para os dados, caso contrário, é ponteiro pra filho.
    uint32_t next; // Se for folha
} TreeNode;

typedef struct {
    uint32_t root, first_leaf, last_id_generated;
    uint8_t t;
} Header;

typedef struct {
    Header header;
    FILE *fp_index,
         *fp_metadata,
         *fp_data;
} TreeContext;

void tree_initialize(uint8_t t, const char *index, const char *data, const char *metadata);
uint32_t tree_search(const TreeContext *ctx, uint32_t key);
void tree_insert(TreeContext *ctx, uint32_t key, uint32_t offset_data);
void tree_remove(TreeContext *ctx, uint32_t key);
void tree_print(TreeContext *ctx);

#endif //TRABALHO_EDA_26_1_DATABASE_H