#include "../include/hash_manipulation.h"

#include <stdio.h>
#include <string.h>

/*
 * As funções de remoção pode ser reutilizadas por novas hashs que tenham o mesmo tipo de chave.
 * Ex: Uma nova hash que mapeia INT para FLOAT pode usar a função "hash_int_remove"
*/

// ======================
// Funções compartilhadas
// ======================

static void hash_destroy_table(Hash *self) {
    hash_destroy(&self->ctx);
}

static void hash_remove_valor(Hash *self, const void *key) {
    hash_remove(&self->ctx, key);
}

static void* lookup(Hash *self, const void *key) {
    return hash_lookup(&self->ctx, key);
}

// ==========================================================
// Funções da hash que mapeia chave inteira pra valor inteiro
// ==========================================================

static int int_generator(const void *key) {
    return *(int*)key;
}

static void print_int(const void *key, const void *valor) {
    printf("(%d: %d)", *(int*)key, *(int*)valor);
}

static void hash_int_to_int_insert(Hash *self, const void *key, const void *valor) {
    hash_insert(&self->ctx, key, sizeof(int), valor, sizeof(int));
}

static void hash_int_to_int_update_valor(Hash *self, const void *key, const void *valor) {
    hash_update_valor(&self->ctx, key, valor, sizeof(int));
}

static void hash_int_to_int_print(Hash *self) {
    hash_print(&self->ctx, print_int);
}

static void hash_int_to_int_create(Hash *hash, int size) {
    hash_inicialize(&hash->ctx, size, int_generator, int_compare);
    hash->insert = hash_int_to_int_insert;
    hash->update_valor = hash_int_to_int_update_valor;
    hash->remove = hash_remove_valor;
    hash->destroy = hash_destroy_table;
    hash->print = hash_int_to_int_print;
}

// ==========================================================
// Funções da hash que mapeia chave STRING pra valor INT
// ==========================================================

static int str_generator(const void *key) {
     const unsigned char *str = key;
     int hash = 5381;
     int c;

     while ((c = *str++)) {
         hash = ((hash << 5) + hash) + c;
     }
     // Retorna o número bruto
     return hash;
}

static void hash_string_to_int_insert(Hash *self, const void *key, const void *valor) {
    hash_insert(&self->ctx, key, strlen(key)+1, valor, sizeof(int));
}

static void hash_string_to_int_update_valor(Hash *self, const void *key, const void *valor) {
    hash_update_valor(&self->ctx, key, valor, sizeof(int));
}

static void print_str_to_int(const void *key, const void *valor) {
    printf("(%s: %d)", (char*)key, *(int*)valor);
}

static void hash_string_to_int_print(Hash *self) {
    hash_print(&self->ctx, print_str_to_int);
}

static void hash_string_to_int_create(Hash *hash, int size) {
    hash_inicialize(&hash->ctx, size, str_generator, str_compare);
    hash->insert = hash_string_to_int_insert;
    hash->update_valor = hash_string_to_int_update_valor;
    hash->remove = hash_remove_valor;
    hash->destroy = hash_destroy_table;
    hash->print = hash_string_to_int_print;
}

// ==========================================================
// Funções da hash que mapeia chave STRING pra valor STRING
// ==========================================================

static void hash_str_to_str_insert(Hash *self, const void *key, const void *valor) {
    hash_insert(&self->ctx, key, strlen(key)+1, valor, strlen(valor)+1);
}

static void hash_str_to_str_update_valor(Hash *self, const void *key, const void *valor) {
    hash_update_valor(&self->ctx, key, valor, strlen(valor)+1);
}

static void print_str_to_str(const void *key, const void *valor) {
    printf("(%s: %s)", (char*)key, (char*)valor);
}

static void hash_str_to_str_print(Hash *self) {
    hash_print(&self->ctx, print_str_to_str);
}

static void hash_str_to_str_create(Hash *hash, int size) {
    hash_inicialize(&hash->ctx, size, str_generator, str_compare);
    hash->insert = hash_str_to_str_insert;
    hash->update_valor = hash_str_to_str_update_valor;
    hash->remove = hash_remove_valor;
    hash->destroy = hash_destroy_table;
    hash->print = hash_str_to_str_print;
}

// ==========================================================
// Funções da hash que mapeia chave TUPLA DE UINT32 pra valor INT
// ==========================================================


static int tuple_generator(const void *key) {
    const TupleIDs *dupla = (const TupleIDs*)key;
    uint32_t hash_bruto = (dupla->val1 * 31) + dupla->val2;
    return (int)(hash_bruto & 0x7FFFFFFF);
}

static void hash_tuple_id_to_int_insert(Hash *self, const void *key, const void *valor) {
    hash_insert(&self->ctx, key, sizeof(TupleIDs), valor, sizeof(int));
}

static void hash_tuple_id_to_int_update_valor(Hash *self, const void *key, const void *valor) {
    hash_update_valor(&self->ctx, key, valor, sizeof(int));
}

static void print_tuple_id_to_int(const void *key, const void *valor) {
    TupleIDs *temp = (TupleIDs*)key;
    printf("([%u, %u]: %d)", temp->val1, temp->val2, *(int*)valor);
}

static void hash_tuple_id_to_int_print(Hash *self) {
    hash_print(&self->ctx, print_tuple_id_to_int);
}

static void hash_tuple_id_to_int_create(Hash *hash, int size) {
    hash_inicialize(&hash->ctx, size, tuple_generator, compare_tuple_ids);
    hash->insert = hash_tuple_id_to_int_insert;
    hash->update_valor = hash_tuple_id_to_int_update_valor;
    hash->remove = hash_remove_valor;
    hash->destroy = hash_destroy_table;
    hash->print = hash_tuple_id_to_int_print;
}

// ==============================
// Interface para criação da hash
// ==============================


void hash_create(Hash *hash, HashType type, int size) {
    hash->lookup = lookup;
    switch(type) {
        case INT_INT:
            hash_int_to_int_create(hash, size);
            break;
        case STRING_INT:
            hash_string_to_int_create(hash, size);
            break;
        case STR_STR:
            hash_str_to_str_create(hash, size);
            break;
        case TUPLE_INT:
            hash_tuple_id_to_int_create(hash, size);
    }
}