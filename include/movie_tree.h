//
// Created by luizao on 04/06/2026.
//

#ifndef TRABALHO_EDA_26_1_MOVIE_TREE_H
#define TRABALHO_EDA_26_1_MOVIE_TREE_H

#include "file_manager.h"

void movie_tree_initialize(uint8_t t);
void movie_tree_insert(uint32_t movie_id, uint32_t offset_movie_data);
void movie_tree_remove(uint32_t movie_id);
uint32_t movie_tree_search_by_title(char *title);
uint32_t movie_tree_search_by_id(uint32_t movie_id);
void movie_tree_delete_base();

#endif //TRABALHO_EDA_26_1_MOVIE_TREE_H