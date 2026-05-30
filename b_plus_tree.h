//
// Created by luizao on 29/05/2026.
//

#ifndef TRABALHO_EDA_26_1_DATABASE_H
#define TRABALHO_EDA_26_1_DATABASE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool is_leaf;
    uint16_t num_keys;
    uint32_t* keys;
    uint32_t* offsets; // Se for nó interno, aponta para os dados, caso contrário, é ponteiro pra filho.
    uint32_t next; // Se for folha
} TreeNode;

#endif //TRABALHO_EDA_26_1_DATABASE_H