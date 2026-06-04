//
// Created by luizao on 04/06/2026.
//

#include "../include/movie_tree.h"

#include "../include/hash.h"

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
    uint32_t id = get_id(PATH_HASH_MOVIE_TABLE, PATH_HASH_MOVIE_DATA, TAM_HASH,  title);
    return tree_search(PATH_INDEX_MOVIE_TREE, PATH_METADATA_MOVIE_TREE, id);
}

void movie_tree_delete_base() {
    remove(PATH_INDEX_MOVIE_TREE);
    remove(PATH_METADATA_MOVIE_TREE);
    remove(PATH_DATA_MOVIE_TREE);
}