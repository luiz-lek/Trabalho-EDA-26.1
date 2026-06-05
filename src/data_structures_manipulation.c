//
// Created by luizao on 04/06/2026.
//

#include "../include/data_structures_manipulation.h"
#include "../include/file_manager.h"
#include "../include/hash.h"

uint32_t hash_string(void *key, int num_buckets) {
    char *str = key;
    uint32_t hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    // Retorna a posiçao já no arquivo
    return hash % num_buckets;
}

uint32_t hash_int(void *key, int num_buckets) {
    return *(uint32_t*)key % num_buckets;
}

uint8_t compare_str(void *a, void *b) {
    return strcmp(a, b) == 0;
}

uint8_t compare_int(void *a, void *b) {
    return *(uint32_t*)a == *(uint32_t*)b;
}

void person_tree_initialize(uint8_t t) {
    tree_initialize(t, PATH_INDEX_PERSON_TREE, PATH_DATA_PERSON_TREE, PATH_METADATA_PERSON_TREE);
}

void person_tree_insert(uint32_t person_id, uint32_t offset_person_data) {
    tree_insert(PATH_INDEX_PERSON_TREE, PATH_METADATA_PERSON_TREE, person_id, offset_person_data);
}

void person_tree_remove(uint32_t movie_id) {
    tree_remove(PATH_INDEX_PERSON_TREE, PATH_METADATA_PERSON_TREE, movie_id);
}

uint32_t person_tree_search(uint32_t movie_id) {
    return tree_search(PATH_INDEX_PERSON_TREE, PATH_METADATA_PERSON_TREE, movie_id);
}

uint32_t person_tree_search_by_name(char *name) {
    uint32_t id = hash_person_id_get_value(name);
    return tree_search(PATH_INDEX_PERSON_TREE, PATH_METADATA_PERSON_TREE, id);
}

void person_tree_delete_base() {
    remove(PATH_INDEX_PERSON_TREE);
    remove(PATH_METADATA_PERSON_TREE);
    remove(PATH_DATA_PERSON_TREE);
}

void movie_tree_initialize(uint8_t t) {
    tree_initialize(t, PATH_INDEX_MOVIE_TREE, PATH_DATA_MOVIE_TREE, PATH_METADATA_MOVIE_TREE);
}

void movie_tree_insert(uint32_t movie_id, uint32_t offset_movie_data) {
    tree_insert(PATH_INDEX_MOVIE_TREE, PATH_METADATA_MOVIE_TREE, movie_id, offset_movie_data);
}

void movie_tree_remove(uint32_t movie_id) {
    tree_remove(PATH_INDEX_MOVIE_TREE, PATH_METADATA_MOVIE_TREE, movie_id);
}

uint32_t movie_tree_search_by_id(uint32_t movie_id) {
    return tree_search(PATH_INDEX_MOVIE_TREE, PATH_METADATA_MOVIE_TREE, movie_id);
}

uint32_t movie_tree_search_by_title(char *title) {
    uint32_t id = hash_movie_id_get_value(title);
    return tree_search(PATH_INDEX_MOVIE_TREE, PATH_METADATA_MOVIE_TREE, id);
}





void movie_tree_delete_base() {
    remove(PATH_INDEX_MOVIE_TREE);
    remove(PATH_METADATA_MOVIE_TREE);
    remove(PATH_DATA_MOVIE_TREE);
}

void hash_movie_id_initialize() {
    hash_initialize(PATH_HASH_MOVIE_TABLE,
        PATH_HASH_MOVIE_DATA,TAM_HASH);
}

void hash_movie_id_insert(const char *movie_name, uint32_t movie_id) {
    hash_insert(PATH_HASH_MOVIE_TABLE,
    PATH_HASH_MOVIE_DATA, TAM_HASH, compare_str, hash_string,
    (void*)movie_name, movie_id, TITLE_LENGTH);
}

void hash_movie_id_remove(const char *movie_name) {
    hash_remove_data(PATH_HASH_MOVIE_TABLE,PATH_HASH_MOVIE_DATA, TAM_HASH,
        compare_str, hash_string, (void*)movie_name);
}

uint32_t hash_movie_id_get_value(const char *movie_name) {
    return hash_get_value(PATH_HASH_MOVIE_TABLE,PATH_HASH_MOVIE_DATA, TAM_HASH,
        compare_str, hash_string, (void*)movie_name);
}

void hash_movie_id_delete_base() {
    hash_delete_hash(PATH_HASH_MOVIE_TABLE, PATH_HASH_MOVIE_DATA);
}





void hash_person_id_initialize() {
    hash_initialize(PATH_HASH_PERSON_TABLE, PATH_HASH_PERSON_DATA, TAM_HASH);
}

void hash_person_id_insert(const char *name, uint32_t person_id) {
    hash_insert(PATH_HASH_PERSON_TABLE, PATH_HASH_PERSON_DATA, TAM_HASH,
        compare_str, hash_string, (void*)name, person_id, NAME_LENGTH);
}

void hash_person_id_remove(const char *name) {
    hash_remove_data(PATH_HASH_PERSON_TABLE, PATH_HASH_PERSON_DATA, TAM_HASH,
        compare_str, hash_string, (void*)name);
}

uint32_t hash_person_id_get_value(const char *name) {
    return hash_get_value(PATH_HASH_PERSON_TABLE, PATH_HASH_PERSON_DATA,TAM_HASH,
    compare_str, hash_string, (void*)name);
}

void hash_person_id_delete_delete_hash() {
    hash_delete_hash(PATH_HASH_PERSON_TABLE, PATH_HASH_PERSON_DATA);
}





void hash_movie_relation_initialize() {
    hash_initialize(PATH_HASH_MOVIE_RELATIONSHIP_TABLE,
        PATH_HASH_MOVIE_RELATIONSHIP_DATA,TAM_HASH);
}

void hash_movie_relation_insert(uint32_t id, uint32_t offset_relation_head) {
    hash_insert(PATH_HASH_MOVIE_RELATIONSHIP_TABLE, PATH_HASH_MOVIE_RELATIONSHIP_DATA,
        TAM_HASH, compare_int, hash_int, &id, offset_relation_head, sizeof(uint32_t));
}

void hash_movie_relation_change_value(uint32_t id, uint32_t new_realation_head) {
    hash_change_value(PATH_HASH_MOVIE_RELATIONSHIP_TABLE,PATH_HASH_MOVIE_RELATIONSHIP_DATA,
        TAM_HASH,compare_int, hash_int, &id, new_realation_head);
}

uint32_t hash_movie_relationship_get_value(uint32_t id) {
    return hash_get_value(PATH_HASH_MOVIE_RELATIONSHIP_TABLE,PATH_HASH_MOVIE_RELATIONSHIP_DATA,
        TAM_HASH, compare_int, hash_int, &id);
}

void hash_movie_relation_remove(uint32_t id) {
    hash_remove_data(PATH_HASH_MOVIE_RELATIONSHIP_TABLE,PATH_HASH_MOVIE_RELATIONSHIP_DATA,
        TAM_HASH, compare_int, hash_int, &id);
}

void hash_movie_relationship_delete_hash() {
    hash_delete_hash(PATH_HASH_MOVIE_RELATIONSHIP_TABLE, PATH_HASH_MOVIE_RELATIONSHIP_DATA);
}





void hash_person_relation_initialize() {
    hash_initialize(PATH_HASH_PERSON_RELATIONSHIP_TABLE,
        PATH_HASH_PERSON_RELATIONSHIP_DATA,TAM_HASH);
}

void hash_person_relation_insert(uint32_t id, uint32_t offset_relatin_head) {
    hash_insert(PATH_HASH_PERSON_RELATIONSHIP_TABLE, PATH_HASH_PERSON_RELATIONSHIP_DATA, TAM_HASH,
        compare_int, hash_int, &id, offset_relatin_head, sizeof(uint32_t));
}

void hash_person_relation_change_value(uint32_t id, uint32_t new_realation_head) {
    hash_change_value(PATH_HASH_PERSON_RELATIONSHIP_TABLE,
        PATH_HASH_PERSON_RELATIONSHIP_DATA, TAM_HASH,
        compare_int, hash_int, &id, new_realation_head);
}

uint32_t hash_person_relationship_get_value(uint32_t id) {
    return hash_get_value(PATH_HASH_PERSON_RELATIONSHIP_TABLE,
        PATH_HASH_PERSON_RELATIONSHIP_DATA, TAM_HASH,
        compare_int, hash_int, &id);
}

void hash_person_relation_remove(uint32_t id) {
    hash_remove_data(PATH_HASH_PERSON_RELATIONSHIP_TABLE,
        PATH_HASH_PERSON_RELATIONSHIP_DATA, TAM_HASH,
        compare_int, hash_int, &id);
}

void hash_person_relationship_delete_hash() {
    hash_delete_hash(PATH_HASH_PERSON_RELATIONSHIP_TABLE, PATH_HASH_PERSON_RELATIONSHIP_DATA);
}