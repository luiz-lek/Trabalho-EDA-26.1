//
// Created by luizao on 29/05/2026.
//

#ifndef TRABALHO_EDA_26_1_FILE_MANAGER_H
#define TRABALHO_EDA_26_1_FILE_MANAGER_H

#define PATH_METADATA_PERSON_TREE "tree/metadata_person.dat"
#define PATH_METADATA_MOVIE_TREE "tree/metadata_movie.dat"

#define PATH_INDEX_PERSON_TREE "tree/index_person.dat"
#define PATH_INDEX_MOVIE_TREE "tree/index_movie.dat"

#define PATH_DATA_PERSON_TREE "tree/data_person.dat"
#define PATH_DATA_MOVIE_TREE "tree/data_movie.dat"

#define PATH_INDEX_PERSON_TREE "tree/index_person.dat"
#define PATH_INDEX_MOVIE_TREE "tree/index_movie.dat"

#define PATH_NODES "../requirements/Nodes.txt"
#define PATH_RELATIONSHIPS "../requirements/Relationships.txt"

#include "person.h"
#include "b_plus_tree.h"
#include <stdio.h>

TreeNode* tree_node_load(FILE* fp, uint32_t offset);
void tree_node_write(FILE* fp, TreeNode* node, uint32_t offset);

Person* person_data_load(FILE* fp, uint32_t offset);
void person_data_write(FILE* fp, uint32_t offset, Person* person);

uint32_t file_size(FILE* fp);

/*
 * pendente: funções de ler/escrever filme
 *                                                  */

#endif //TRABALHO_EDA_26_1_FILE_MANAGER_H