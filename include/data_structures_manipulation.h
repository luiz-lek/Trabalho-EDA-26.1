//
// Created by luizao on 04/06/2026.
//

#ifndef TRABALHO_EDA_26_1_PERSON_TREE_H
#define TRABALHO_EDA_26_1_PERSON_TREE_H

#include <stdint.h>

// Person tree
void person_tree_insert(uint32_t person_id, uint32_t offset_person_data);
void person_tree_initialize(uint8_t t);
void person_tree_remove(uint32_t movie_id);
uint32_t person_tree_search(uint32_t movie_id);
uint32_t person_tree_search_by_name(char *name);
void person_tree_delete_base();

// Movie tree
void movie_tree_initialize(uint8_t t);
void movie_tree_insert(uint32_t movie_id, uint32_t offset_movie_data);
void movie_tree_remove(uint32_t movie_id);
uint32_t movie_tree_search_by_title(char *title);
uint32_t movie_tree_search_by_id(uint32_t movie_id);
void movie_tree_delete_base();

// Hash que relaciona o nome do filme ao seu id
void hash_movie_id_initialize();
void hash_movie_id_insert(const char *movie_name, uint32_t movie_id);
void hash_movie_id_remove(const char *movie_name);
uint32_t hash_movie_id_get_value(const char *movie_name);
void hash_movie_id_delete_base();

// Hash que relaciona o nome da pessoa a seu id
void hash_person_id_initialize();
void hash_person_id_insert(const char *name, uint32_t person_id);
void hash_person_id_remove(const char *name);
uint32_t hash_person_id_get_value(const char *movie_name);
void hash_person_id_delete_delete_hash();

// Hash que relaciona um filme a sua lista de pessoas
void hash_movie_relation_initialize();
void hash_movie_relation_insert(uint32_t id, uint32_t offset_relation_head);
void hash_movie_relation_change_value(uint32_t id, uint32_t new_realation_head);
uint32_t hash_movie_relationship_get_value(uint32_t id);
void hash_movie_relation_remove(uint32_t id);
void hash_movie_relationship_delete_hash();

// Hash que relaciona uma pessoa a sua lista de filmes
void hash_person_relation_initialize();
void hash_person_relation_insert(uint32_t id, uint32_t offset_relatin_head);
void hash_person_relation_change_value(uint32_t id, uint32_t new_realation_head);
uint32_t hash_person_relationship_get_value(uint32_t id);
void hash_person_relation_remove(uint32_t id);
void hash_person_relationship_delete_hash();

#endif