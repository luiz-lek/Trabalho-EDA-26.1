#ifndef TRABALHO_EDA_26_1_PERSON_TREE_H
#define TRABALHO_EDA_26_1_PERSON_TREE_H

#include "relationship.h"
#include <stdint.h>

// Árvore de pessoas
void person_tree_insert(uint32_t person_id, uint32_t offset_person_data);
void person_tree_initialize(uint8_t t);
void person_tree_remove(uint32_t movie_id);
uint32_t person_tree_search(uint32_t movie_id);
uint32_t person_tree_search_by_name(char *name);
void person_tree_delete_base();

// Árvore de filmes
void movie_tree_initialize(uint8_t t);
void movie_tree_insert(uint32_t movie_id, uint32_t offset_movie_data);
void movie_tree_remove(uint32_t movie_id);
uint32_t movie_tree_search_by_title(char *title);
uint32_t movie_tree_search_by_id(uint32_t movie_id);
void movie_tree_delete_base();

// Hash que relaciona o nome do filme ao seu id
void hash_movie_to_id_initialize();
void hash_movie_to_id_insert(const char *movie_name, uint32_t movie_id);
void hash_movie_to_id_remove(const char *movie_name);
uint32_t hash_movie_to_id_get_value(const char *movie_name);
void hash_movie_to_id_delete_table();

// Hash que relaciona o nome da pessoa a seu id
void hash_person_to_id_initialize();
void hash_person_to_id_insert(const char *name, uint32_t person_id);
void hash_person_to_id_remove(const char *name);
uint32_t hash_person_to_id_get_value(const char *movie_name);
void hash_person_to_id_delete_table();

// Hash que relaciona um filme a sua lista de pessoas
void hash_movie_to_person_initialize();
void hash_movie_to_person_insert(uint32_t id, uint32_t offset_relation_head);
void hash_movie_to_person_change_value(uint32_t id, uint32_t new_realation_head);
uint32_t hash_movie_to_person_get_value(uint32_t id);
void hash_movie_to_person_remove(uint32_t id);
void hash_movie_to_person_delete_table();

// Hash que relaciona uma pessoa a sua lista de filmes
void hash_person_to_movie_initialize();
void hash_person_to_movie_insert(uint32_t id, uint32_t offset_relatin_head);
void hash_person_to_movie_change_value(uint32_t id, uint32_t new_realation_head);
uint32_t hash_person_to_movie_get_value(uint32_t id);
void hash_person_to_movie_remove(uint32_t id);
void hash_person_to_movie_delete_table();

void insert_relation(Relationship *r);

#endif