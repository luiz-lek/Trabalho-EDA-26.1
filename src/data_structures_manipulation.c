//
// Created by luizao on 04/06/2026.
//

#include "../include/data_structures_manipulation.h"
#include "../include/file_manager.h"
#include "../include/b_plus_tree.h"
#include "../include/hash_disk.h"
#include "../include/person.h"
#include "../include/movie.h"

uint32_t hash_string(const void *key) {
    const unsigned char *str = key;
    uint32_t hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    // Retorna o número bruto
    return hash;
}

uint32_t hash_int(const void *key) {
    return *(uint32_t*)key;
}

uint8_t compare_str(const void *a, const void *b) {
    return strcmp(a, b) == 0;
}

uint8_t compare_int(const void *a, const void *b) {
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
    uint32_t id = hash_person_to_id_get_value(name);
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
    uint32_t id = hash_movie_to_id_get_value(title);
    return tree_search(PATH_INDEX_MOVIE_TREE, PATH_METADATA_MOVIE_TREE, id);
}

void movie_tree_delete_base() {
    remove(PATH_INDEX_MOVIE_TREE);
    remove(PATH_METADATA_MOVIE_TREE);
    remove(PATH_DATA_MOVIE_TREE);
}





void hash_movie_to_id_initialize() {
    hash_disk_initialize(PATH_HASH_MOVIE_TO_ID_TABLE,
        PATH_HASH_MOVIE_TO_ID_DATA,TAM_HASH);
}

void hash_movie_to_id_insert(const char *movie_name, uint32_t movie_id) {
    hash_disk_insert(PATH_HASH_MOVIE_TO_ID_TABLE,
    PATH_HASH_MOVIE_TO_ID_DATA, TAM_HASH, compare_str, hash_string,
    (void*)movie_name, movie_id, NAME_LENGTH);
}

void hash_movie_to_id_remove(const char *movie_name) {
    hash_disk_remove_data(PATH_HASH_MOVIE_TO_ID_TABLE,PATH_HASH_MOVIE_TO_ID_DATA, TAM_HASH,
        compare_str, hash_string, (void*)movie_name);
}

uint32_t hash_movie_to_id_get_value(const char *movie_name) {
    return hash_disk_get_value(PATH_HASH_MOVIE_TO_ID_TABLE,PATH_HASH_MOVIE_TO_ID_DATA, TAM_HASH,
        compare_str, hash_string, (void*)movie_name);
}

void hash_movie_to_id_delete_table() {
    hash_disk_delete_table(PATH_HASH_MOVIE_TO_ID_TABLE, PATH_HASH_MOVIE_TO_ID_DATA);
}





void hash_person_to_id_initialize() {
    hash_disk_initialize(PATH_HASH_PERSON_TO_ID_TABLE, PATH_HASH_PERSON_TO_ID_DATA, TAM_HASH);
}

void hash_person_to_id_insert(const char *name, uint32_t person_id) {
    hash_disk_insert(PATH_HASH_PERSON_TO_ID_TABLE, PATH_HASH_PERSON_TO_ID_DATA, TAM_HASH,
        compare_str, hash_string, (void*)name, person_id, NAME_LENGTH);
}

void hash_person_to_id_remove(const char *name) {
    hash_disk_remove_data(PATH_HASH_PERSON_TO_ID_TABLE, PATH_HASH_PERSON_TO_ID_DATA, TAM_HASH,
        compare_str, hash_string, (void*)name);
}

uint32_t hash_person_to_id_get_value(const char *name) {
    return hash_disk_get_value(PATH_HASH_PERSON_TO_ID_TABLE, PATH_HASH_PERSON_TO_ID_DATA,TAM_HASH,
    compare_str, hash_string, (void*)name);
}

void hash_person_to_id_delete_table() {
    hash_disk_delete_table(PATH_HASH_PERSON_TO_ID_TABLE, PATH_HASH_PERSON_TO_ID_DATA);
}





void hash_movie_to_person_initialize() {
    hash_disk_initialize(PATH_HASH_MOVIE_TO_PERSON_TABLE,
        PATH_HASH_MOVIE_TO_PERSON_DATA,TAM_HASH);
}

void hash_movie_to_person_insert(uint32_t id, uint32_t offset_relation_head) {
    hash_disk_insert(PATH_HASH_MOVIE_TO_PERSON_TABLE, PATH_HASH_MOVIE_TO_PERSON_DATA,
        TAM_HASH, compare_int, hash_int, &id, offset_relation_head, sizeof(uint32_t));
}

void hash_movie_to_person_change_value(uint32_t id, uint32_t new_realation_head) {
    hash_disk_change_value(PATH_HASH_MOVIE_TO_PERSON_TABLE,PATH_HASH_MOVIE_TO_PERSON_DATA,
        TAM_HASH,compare_int, hash_int, &id, new_realation_head);
}

uint32_t hash_movie_to_person_get_value(uint32_t id) {
    return hash_disk_get_value(PATH_HASH_MOVIE_TO_PERSON_TABLE,PATH_HASH_MOVIE_TO_PERSON_DATA,
        TAM_HASH, compare_int, hash_int, &id);
}

void hash_movie_to_person_remove(uint32_t id) {
    hash_disk_remove_data(PATH_HASH_MOVIE_TO_PERSON_TABLE,PATH_HASH_MOVIE_TO_PERSON_DATA,
        TAM_HASH, compare_int, hash_int, &id);
}

void hash_movie_to_person_delete_table() {
    hash_disk_delete_table(PATH_HASH_MOVIE_TO_PERSON_TABLE, PATH_HASH_MOVIE_TO_PERSON_DATA);
}





void hash_person_to_movie_initialize() {
    hash_disk_initialize(PATH_HASH_PERSON_TO_MOVIE_TABLE,
        PATH_HASH_PERSON_TO_MOVIE_DATA,TAM_HASH);
}

void hash_person_to_movie_insert(uint32_t id, uint32_t offset_relatin_head) {
    hash_disk_insert(PATH_HASH_PERSON_TO_MOVIE_TABLE, PATH_HASH_PERSON_TO_MOVIE_DATA, TAM_HASH,
        compare_int, hash_int, &id, offset_relatin_head, sizeof(uint32_t));
}

void hash_person_to_movie_change_value(uint32_t id, uint32_t new_realation_head) {
    hash_disk_change_value(PATH_HASH_PERSON_TO_MOVIE_TABLE,
        PATH_HASH_PERSON_TO_MOVIE_DATA, TAM_HASH,
        compare_int, hash_int, &id, new_realation_head);
}

uint32_t hash_person_to_movie_get_value(uint32_t id) {
    return hash_disk_get_value(PATH_HASH_PERSON_TO_MOVIE_TABLE,
        PATH_HASH_PERSON_TO_MOVIE_DATA, TAM_HASH,
        compare_int, hash_int, &id);
}

void hash_person_to_movie_remove(uint32_t id) {
    hash_disk_remove_data(PATH_HASH_PERSON_TO_MOVIE_TABLE,
        PATH_HASH_PERSON_TO_MOVIE_DATA, TAM_HASH,
        compare_int, hash_int, &id);
}

void hash_person_to_movie_delete_table() {
    hash_disk_delete_table(PATH_HASH_PERSON_TO_MOVIE_TABLE, PATH_HASH_PERSON_TO_MOVIE_DATA);
}




void insert_relation(Relationship *r) {
    // pendente: Adiocionar função para reaproveitar espaços vazio no meio do arquivo (remoção de relacionamento)
    FILE *fp = open_file(PATH_RELATIONS_DATA, "rb+");
    uint32_t offset_new_relation = file_size(fp);

    r->offset_next_person = hash_movie_to_person_get_value(r->movie_id);
    r->offset_next_movie = hash_person_to_movie_get_value(r->person_id);

    write_data(fp, offset_new_relation, r, sizeof(Relationship));

    if(r->offset_next_person == DISK_NULL) hash_movie_to_person_insert(r->movie_id, offset_new_relation);
    else hash_movie_to_person_change_value(r->movie_id, offset_new_relation);

    if(r->offset_next_movie == DISK_NULL) hash_person_to_movie_insert(r->person_id, offset_new_relation);
    else hash_person_to_movie_change_value(r->person_id, offset_new_relation);

    fclose(fp);
}