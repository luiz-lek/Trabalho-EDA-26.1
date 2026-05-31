//
// Created by luizao on 29/05/2026.
//

#ifndef TRABALHO_EDA_26_1_FILE_MANAGER_H
#define TRABALHO_EDA_26_1_FILE_MANAGER_H

#define PATH_METADATA_PERSON_TREE "../tree/metadata_person.dat"
#define PATH_METADATA_MOVIE_TREE "../tree/metadata_movie.dat"

#define PATH_INDEX_PERSON_TREE "../tree/index_person.dat"
#define PATH_INDEX_MOVIE_TREE "tree/index_movie.dat"

#define PATH_DATA_PERSON_TREE "../tree/data_person.dat"
#define PATH_DATA_MOVIE_TREE "../tree/data_movie.dat"

#define PATH_INDEX_PERSON_TREE "../tree/index_person.dat"
#define PATH_INDEX_MOVIE_TREE "../tree/index_movie.dat"

#define PATH_NODES "../requirements/Nodes.txt"
#define PATH_RELATIONSHIPS "../requirements/Relationships.txt"

#define DISK_NULL 0xFFFFFFFF

#include "person.h"
#include "b_plus_tree.h"
#include <stdio.h>

#include "movie.h"

bool tree_node_load(TreeNode *node, uint8_t t, FILE *fp, uint32_t offset);
void tree_node_save(TreeNode *node, uint8_t t, FILE *fp, uint32_t offset);

void person_data_load(Person *p, FILE *fp, uint32_t offset);
void person_save(Person *person, FILE *fp, uint32_t offset);

void movie_load(Movie *m, FILE *fp, uint32_t offset);
void movie_save(Movie *m, FILE *fp, uint32_t offset);

// Carrega structs genérica do arquivo de dados
bool load_data(FILE *fp, uint32_t offset, void *data, size_t size);

uint32_t file_size(FILE *fp);

#endif //TRABALHO_EDA_26_1_FILE_MANAGER_H