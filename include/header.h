//
// Created by luizao on 02/06/2026.
//

#ifndef TRABALHO_EDA_26_1_HEADER_H
#define TRABALHO_EDA_26_1_HEADER_H

#define HEADER_SIZE sizeof(Header)

#include <stdint.h>

typedef struct {
    uint8_t t;
    uint32_t root;
    uint32_t first_leaf;
    uint32_t last_id_generated;
} Header;

void load_header(char *metadata_file, Header *h);
void save_header(char *metadata_file, Header *h);

#endif //TRABALHO_EDA_26_1_HEADER_H