#ifndef TRABALHO_EDA_26_1_PERSON_TREE_H
#define TRABALHO_EDA_26_1_PERSON_TREE_H

#include "relationship.h"
#include "hash_disk.h"
#include <stdint.h>

#include "b_plus_tree.h"

typedef struct {
    TreeContext tree_movie_context,
                tree_person_context;

    HashContext hash_person_lookup_context,
                hash_movie_lookup_context,
                hash_person_relations_context,
                hash_movie_relations_context;

    FILE *fp_relations_data;
} DB;

void db_inicialize(int t);
void db_destroy();
void db_load_context(DB *db);
void db_close_context(DB *db);

// Árvore de pessoas
void person_tree_initialize(uint8_t t);
uint32_t person_tree_search_by_name(TreeContext *tree_ctx, HashContext *hash_context, char *name);
void tree_person_get_context(TreeContext *ctx);
void person_tree_delete_base();

// Árvore de filmes
void movie_tree_initialize(uint8_t t);
uint32_t movie_tree_search_by_title(TreeContext *tree_ctx, HashContext *hash_ctx, char *title);
void tree_movie_get_context(TreeContext *ctx);
void movie_tree_delete_base();

void close_tree_context(TreeContext *ctx);

// Hash que relaciona o nome do filme ao seu id
void open_hash_movie_to_id_context(HashContext *ctx);
void hash_movie_to_id_initialize();
void hash_movie_to_id_delete_table();

// Hash que relaciona o nome da pessoa a seu id
void open_hash_person_to_id_context(HashContext *ctx);
void hash_person_to_id_initialize();
void hash_person_to_id_delete_table();

// Hash que relaciona um filme a sua lista de pessoas
void open_hash_movie_to_person_context(HashContext *ctx);
void hash_movie_to_person_initialize();
void hash_movie_to_person_delete_table();

// Hash que relaciona uma pessoa a sua lista de filmes
void open_hash_person_to_movie_context(HashContext *ctx);
void hash_person_to_movie_initialize();
void hash_person_to_movie_delete_table();

void close_hash_context(HashContext *ctx);

void insert_relation(FILE *fp, Relationship *r,
                     HashContext *hash_person_to_movie_ctx,
                     HashContext *hash_movie_to_person_ctx);

#endif