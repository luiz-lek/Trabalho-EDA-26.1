//
// Created by luizao on 04/06/2026.
//

#ifndef TRABALHO_EDA_26_1_PERSON_TREE_H
#define TRABALHO_EDA_26_1_PERSON_TREE_H

#include <stdint.h>

void person_tree_insert(uint32_t person_id, uint32_t offset_person_data);
void person_tree_initialize(uint8_t t);
void person_tree_remove(uint32_t movie_id);
uint32_t person_tree_search(uint32_t movie_id);
uint32_t person_tree_search_by_name(char *name);
void person_tree_delete_base();

#endif //TRABALHO_EDA_26_1_PERSON_TREE_H