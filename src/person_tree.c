//
// Created by luizao on 04/06/2026.
//

#include "../include/person_tree.h"
#include "../include/file_manager.h"
#include "../include/hash.h"

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
    return tree_search(PATH_INDEX_MOVIE_TREE, PATH_METADATA_MOVIE_TREE, movie_id);
}

uint32_t person_tree_search_by_name(char *name) {
    uint32_t id = get_id(PATH_HASH_PERSON_TABLE, PATH_HASH_PERSON_DATA, TAM_HASH,  name);
    return tree_search(PATH_INDEX_PERSON_TREE, PATH_METADATA_PERSON_TREE, id);
}

void person_tree_delete_base() {
    remove(PATH_INDEX_PERSON_TREE);
    remove(PATH_METADATA_PERSON_TREE);
    remove(PATH_DATA_PERSON_TREE);
}