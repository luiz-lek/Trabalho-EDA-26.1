#ifndef TRABALHO_EDA_26_1_FILE_MANAGER_H
#define TRABALHO_EDA_26_1_FILE_MANAGER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define PATH_NODES "../requirements/Nodes.txt"
#define PATH_RELATIONSHIPS "../requirements/Relationships.txt"

// Arquivos árvore de pessoas
#define PATH_INDEX_PERSON_TREE "../data_base/tree/person/index.dat"
#define PATH_DATA_PERSON_TREE "../data_base/tree/person/data.dat"
#define PATH_METADATA_PERSON_TREE "../data_base/tree/person/metadata.dat"

// Arquivos árvore de filmes
#define PATH_INDEX_MOVIE_TREE "../data_base/tree/movie/index.dat"
#define PATH_DATA_MOVIE_TREE "../data_base/tree/movie/data.dat"
#define PATH_METADATA_MOVIE_TREE "../data_base/tree/movie/metadata.dat"





//Arquivo de relacionamentos
#define PATH_RELATIONS_DATA "../data_base/tree/relations.dat"





// Arquivos da hash que relaciona o nome do filme com seu id
#define PATH_HASH_MOVIE_TO_ID_TABLE "../data_base/hash/movie_to_id/table.dat"
#define PATH_HASH_MOVIE_TO_ID_DATA "../data_base/hash/movie_to_id/data.dat"

// Arquivos da hash que relaciona o nome da pessoa com seu id
#define PATH_HASH_PERSON_TO_ID_TABLE "../data_base/hash/person_to_id/table.dat"
#define PATH_HASH_PERSON_TO_ID_DATA "../data_base/hash/person_to_id/data.dat"

// Arquivos da hash que relaciona o id do filme com a lista de pessoas participantes
#define PATH_HASH_MOVIE_TO_PERSON_TABLE "../data_base/hash/movie_to_person/table.dat"
#define PATH_HASH_MOVIE_TO_PERSON_DATA "../data_base/hash/movie_to_person/data.dat"

// Arquivo que relaciona o id da pessoa com a lista de filmes que ela participou
#define PATH_HASH_PERSON_TO_MOVIE_TABLE "../data_base/hash/person_to_movie/table.dat"
#define PATH_HASH_PERSON_TO_MOVIE_DATA "../data_base/hash/person_to_movie/data.dat"



#define DISK_NULL 0xFFFFFFFF

// Carrega structs genérica do arquivo de dados
bool read_data(FILE *fp, uint32_t offset,  void *data, size_t size);
bool write_data(FILE *fp, uint32_t offset, const void *data, size_t size);

FILE* open_file(const char *file_name, const char *mode);

uint32_t file_size(FILE *fp);
void create_file(const char *file_name);

#endif //TRABALHO_EDA_26_1_FILE_MANAGER_H