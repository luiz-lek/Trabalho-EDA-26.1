//
// Created by luizao on 04/06/2026.
//

#include "../include/data_base_manipulation.h"
#include "../include/file_manager.h"
#include "../include/b_plus_tree.h"
#include "../include/hash_disk.h"
#include "../include/person.h"
#include "../include/movie.h"
#include "utils.h"

void db_inicialize(int t) {
    movie_tree_initialize(t);
    person_tree_initialize(t);

    hash_person_to_movie_initialize();
    hash_movie_to_person_initialize();

    hash_person_to_id_initialize();
    hash_movie_to_id_initialize();

    create_file(PATH_TREE_PERSON_DATA);
    create_file(PATH_TREE_MOVIE_DATA);
    create_file(PATH_RELATIONS_DATA);
}

void db_destroy() {
    const char *files_to_delete[] = {
        PATH_TREE_MOVIE_INDEX, PATH_TREE_MOVIE_DATA, PATH_TREE_MOVIE_METADATA,
        PATH_TREE_PERSON_INDEX, PATH_TREE_PERSON_DATA, PATH_TREE_PERSON_METADATA,
        PATH_HASH_MOVIE_LOOKUP_TABLE, PATH_HASH_MOVIE_LOOKUP_DATA,
        PATH_HASH_PERSON_LOOKUP_TABLE, PATH_HASH_PERSON_LOOKUP_DATA,
        PATH_HASH_MOVIE_RELS_TABLE, PATH_HASH_MOVIE_RELS_DATA,
        PATH_HASH_PERSON_RELS_TABLE, PATH_HASH_PERSON_RELS_DATA,
        PATH_RELATIONS_DATA
    };

    int num_files = sizeof(files_to_delete) / sizeof(files_to_delete[0]);

    for (int i = 0; i < num_files; i++) {
        remove(files_to_delete[i]);
    }
}

void db_load_context(DB *db) {
    tree_movie_get_context(&db->tree_movie_context);
    tree_person_get_context(&db->tree_person_context);

    open_hash_person_to_id_context(&db->hash_person_lookup_context);
    open_hash_movie_to_id_context(&db->hash_movie_lookup_context);
    open_hash_person_to_movie_context(&db->hash_person_relations_context);
    open_hash_movie_to_person_context(&db->hash_movie_relations_context);
    db->fp_relations_data = open_file(PATH_RELATIONS_DATA, "rb+");
}

void db_close_context(DB *db) {
    close_tree_context(&db->tree_movie_context);
    close_tree_context(&db->tree_person_context);

    close_hash_context(&db->hash_person_lookup_context);
    close_hash_context(&db->hash_movie_lookup_context);
    close_hash_context(&db->hash_person_relations_context);
    close_hash_context(&db->hash_movie_relations_context);
    fclose(db->fp_relations_data);
}

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
    tree_initialize(t, PATH_TREE_PERSON_INDEX, PATH_TREE_PERSON_DATA, PATH_TREE_PERSON_METADATA);
}

void tree_person_get_context(TreeContext *ctx) {
    ctx->fp_index = open_file(PATH_TREE_PERSON_INDEX, "rb+");
    ctx->fp_metadata = open_file(PATH_TREE_PERSON_METADATA, "rb+");
    ctx->fp_data = open_file(PATH_TREE_PERSON_DATA, "rb+");

    read_data(ctx->fp_metadata, 0, &ctx->header, sizeof(Header));
}

uint32_t person_tree_search_by_name(TreeContext *tree_ctx, HashContext *hash_context, char *name) {
    uint32_t id = hash_disk_lookup(hash_context, name);

    if(id == DISK_NULL) {
        printf("[tree] pessoa não encontrada.\n");
        return DISK_NULL;
    }

    return tree_search(tree_ctx, id);
}

void person_tree_delete_base() {
    remove(PATH_TREE_PERSON_INDEX);
    remove(PATH_TREE_PERSON_METADATA);
    remove(PATH_TREE_PERSON_DATA);
}





void movie_tree_initialize(uint8_t t) {
    tree_initialize(t, PATH_TREE_MOVIE_INDEX, PATH_TREE_MOVIE_DATA, PATH_TREE_MOVIE_METADATA);
}

uint32_t movie_tree_search_by_title(TreeContext *tree_ctx, HashContext *hash_ctx, char *title) {
    uint32_t id = hash_disk_lookup(hash_ctx, title);

    if(id == DISK_NULL) {
        printf("[tree] filme não encontrada.\n");
        return DISK_NULL;
    }

    return tree_search(tree_ctx, id);
}

void tree_movie_get_context(TreeContext *ctx) {
    ctx->fp_index = open_file(PATH_TREE_MOVIE_INDEX, "rb+");
    ctx->fp_metadata = open_file(PATH_TREE_MOVIE_METADATA, "rb+");
    ctx->fp_data = open_file(PATH_TREE_MOVIE_DATA, "rb+");

    read_data(ctx->fp_metadata, 0, &ctx->header, sizeof(Header));
}

void movie_tree_delete_base() {
    remove(PATH_TREE_MOVIE_INDEX);
    remove(PATH_TREE_MOVIE_METADATA);
    remove(PATH_TREE_MOVIE_DATA);
}

void close_tree_context(TreeContext *ctx) {
    fclose(ctx->fp_index);
    fclose(ctx->fp_metadata);
    fclose(ctx->fp_data);
}




void open_hash_movie_to_id_context(HashContext *ctx) {
    ctx->fph = open_file(PATH_HASH_MOVIE_LOOKUP_TABLE, "rb+");
    ctx->fpd = open_file(PATH_HASH_MOVIE_LOOKUP_DATA, "rb+");
    ctx->comparator = str_compare;
    ctx->generate_brute_number = hash_string;
    ctx->key_size = NAME_LENGTH;
    ctx->num_buckets = TAM_HASH;
    strcpy(ctx->hash_file_name, PATH_HASH_MOVIE_LOOKUP_TABLE);
    strcpy(ctx->data_file_name, PATH_HASH_MOVIE_LOOKUP_DATA);
}

void hash_movie_to_id_initialize() {
    hash_disk_initialize(PATH_HASH_MOVIE_LOOKUP_TABLE,
                         PATH_HASH_MOVIE_LOOKUP_DATA,
                       TAM_HASH);
}

void hash_movie_to_id_delete_table() {
    hash_disk_delete_table(PATH_HASH_MOVIE_LOOKUP_TABLE, PATH_HASH_MOVIE_LOOKUP_DATA);
}





void open_hash_person_to_id_context(HashContext *ctx) {
    ctx->fph = open_file(PATH_HASH_PERSON_LOOKUP_TABLE, "rb+");
    ctx->fpd = open_file(PATH_HASH_PERSON_LOOKUP_DATA, "rb+");
    ctx->comparator = str_compare;
    ctx->generate_brute_number = hash_string;
    ctx->key_size = NAME_LENGTH;
    ctx->num_buckets = TAM_HASH;
    strcpy(ctx->hash_file_name, PATH_HASH_PERSON_LOOKUP_TABLE);
    strcpy(ctx->data_file_name, PATH_HASH_PERSON_LOOKUP_DATA);
}

void hash_person_to_id_initialize() {
    hash_disk_initialize(PATH_HASH_PERSON_LOOKUP_TABLE,
                         PATH_HASH_PERSON_LOOKUP_DATA,
                       TAM_HASH);
}

void hash_person_to_id_delete_table() {
    hash_disk_delete_table(PATH_HASH_PERSON_LOOKUP_TABLE, PATH_HASH_PERSON_LOOKUP_DATA);
}





void open_hash_movie_to_person_context(HashContext *ctx) {
    ctx->fph = open_file(PATH_HASH_MOVIE_RELS_TABLE, "rb+");
    ctx->fpd = open_file(PATH_HASH_MOVIE_RELS_DATA, "rb+");
    ctx->comparator = int_compare;
    ctx->generate_brute_number = hash_int;
    ctx->key_size = sizeof(uint32_t);
    ctx->num_buckets = TAM_HASH;
    strcpy(ctx->hash_file_name, PATH_HASH_MOVIE_RELS_TABLE);
    strcpy(ctx->data_file_name, PATH_HASH_MOVIE_RELS_DATA);
}

void hash_movie_to_person_initialize() {
    hash_disk_initialize(PATH_HASH_MOVIE_RELS_TABLE,
                         PATH_HASH_MOVIE_RELS_DATA,
                       TAM_HASH);
}


void hash_movie_to_person_delete_table() {
    hash_disk_delete_table(PATH_HASH_MOVIE_RELS_TABLE, PATH_HASH_MOVIE_RELS_DATA);
}





void open_hash_person_to_movie_context(HashContext *ctx) {
    ctx->fph = open_file(PATH_HASH_PERSON_RELS_TABLE, "rb+");
    ctx->fpd = open_file(PATH_HASH_PERSON_RELS_DATA, "rb+");
    ctx->comparator = int_compare;
    ctx->generate_brute_number = hash_int;
    ctx->key_size = sizeof(uint32_t);
    ctx->num_buckets = TAM_HASH;
    strcpy(ctx->hash_file_name, PATH_HASH_PERSON_RELS_TABLE);
    strcpy(ctx->data_file_name, PATH_HASH_PERSON_RELS_DATA);
}

void hash_person_to_movie_initialize() {
    hash_disk_initialize(PATH_HASH_PERSON_RELS_TABLE,
                         PATH_HASH_PERSON_RELS_DATA,
                       TAM_HASH);
}

void hash_person_to_movie_delete_table() {
    hash_disk_delete_table(PATH_HASH_PERSON_RELS_TABLE, PATH_HASH_PERSON_RELS_DATA);
}





void close_hash_context(HashContext *ctx) {
    if(ctx->fph != NULL) {
        fclose(ctx->fph);
        ctx->fph = NULL;
    }
    if(ctx->fpd != NULL) {
        fclose(ctx->fpd);
        ctx->fpd = NULL;
    }
}


void insert_relation(FILE *fp,
                     Relationship *r,
                     HashContext *hash_person_to_movie_ctx,
                     HashContext *hash_movie_to_person_ctx)
{
    // pendente: Adiocionar função para reaproveitar espaços vazio no meio do arquivo (remoção de relacionamento)
    uint32_t offset_new_relation = file_size(fp);

    r->offset_next_person = hash_disk_lookup(hash_movie_to_person_ctx, &r->movie_id);
    if(r->offset_next_person == DISK_NULL) hash_disk_insert(hash_movie_to_person_ctx, &r->movie_id, offset_new_relation);
    else hash_disk_change_value(hash_movie_to_person_ctx, &r->movie_id, offset_new_relation);

    r->offset_next_movie = hash_disk_lookup(hash_person_to_movie_ctx, &r->person_id);
    if(r->offset_next_movie == DISK_NULL) hash_disk_insert(hash_person_to_movie_ctx, &r->person_id, offset_new_relation);
    else hash_disk_change_value(hash_person_to_movie_ctx, &r->person_id, offset_new_relation);

    write_data(fp, offset_new_relation, r, sizeof(Relationship));
}