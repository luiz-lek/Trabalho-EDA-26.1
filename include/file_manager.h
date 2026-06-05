//
// Created by luizao on 29/05/2026.
//

#ifndef TRABALHO_EDA_26_1_FILE_MANAGER_H
#define TRABALHO_EDA_26_1_FILE_MANAGER_H

#define PATH_METADATA_PERSON_TREE "../data_base/tree/person/metadata.dat"
#define PATH_METADATA_MOVIE_TREE "../data_base/tree/movie/metadata.dat"

#define PATH_INDEX_PERSON_TREE "../data_base/tree/person/index.dat"
#define PATH_INDEX_MOVIE_TREE "../data_base/tree/movie/index.dat"

#define PATH_DATA_PERSON_TREE "../data_base/tree/person/data_base.dat"
#define PATH_DATA_MOVIE_TREE "../data_base/tree/movie/data_base.dat"

#define PATH_INDEX_PERSON_TREE "../data_base/tree/person/index.dat"
#define PATH_INDEX_MOVIE_TREE "../data_base/tree/movie/index.dat"

#define PATH_NODES "../requirements/Nodes.txt"
#define PATH_RELATIONSHIPS "../requirements/Relationships.txt"

#define PATH_HASH_MOVIE_TABLE "../data_base/hash/movie/table.dat"
#define PATH_HASH_MOVIE_DATA "../data_base/hash/movie/data.dat"

#define PATH_HASH_PERSON_TABLE "../data_base/hash/person/table.dat"
#define PATH_HASH_PERSON_DATA "../data_base/hash/person/data.dat"

#define PATH_RELATIONSHIPS_DATA "../data_base/tree/relationships.dat"

#define PATH_HASH_MOVIE_RELATIONSHIP_TABLE "../data_base/hash/movie_relationship/table.dat"
#define PATH_HASH_MOVIE_RELATIONSHIP_DATA "../data_base/hash/movie_relationship/data.dat"

#define PATH_HASH_PERSON_RELATIONSHIP_TABLE "../data_base/hash/person_relationship/table.dat"
#define PATH_HASH_PERSON_RELATIONSHIP_DATA "../data_base/hash/person_relationship/data.dat"

#define DISK_NULL 0xFFFFFFFF

#include "person.h"
#include "b_plus_tree.h"

#include <stdio.h>
#include <stdbool.h>

#include "movie.h"

bool tree_node_read(TreeNode *node, uint8_t t, FILE *fp, uint32_t offset);
void tree_node_write(TreeNode *node, uint8_t t, FILE *fp, uint32_t offset);

void person_write(Person *p, FILE *fp, uint32_t offset);
void person_read(Person *person, FILE *fp, uint32_t offset);

void movie_read(Movie *m, FILE *fp, uint32_t offset);
void movie_write(Movie *m, FILE *fp, uint32_t offset);

// Carrega structs genérica do arquivo de dados
bool read_data(FILE *fp, uint32_t offset, void *data, size_t size);
bool write_data(FILE *fp, uint32_t offset, void *data, size_t size);

FILE* open_file(const char *filename, const char *mode);

uint32_t file_size(FILE *fp);

#endif //TRABALHO_EDA_26_1_FILE_MANAGER_H