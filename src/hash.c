#include "../include/hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hash(int num, int table_size) {
    return num % table_size;
}

HashNode* hash_list_create(size_t key_size, size_t value_size) {
    HashNode *new = malloc(sizeof(HashNode));
    if(!new) {
        perror("Falha ao alocar hash_list");
        exit(EXIT_FAILURE);
    }

    new->key = malloc(key_size);
    new->value = malloc(value_size);
    if(!new->value || !new->key) {
        perror("Falha ao alocar hash_list");
        exit(EXIT_FAILURE);
    }
    new->next = NULL;
    return new;
}

HashNode* hash_list_destroy(HashNode *hash_list) {
    free(hash_list->key);
    free(hash_list->value);
    free(hash_list);
    return NULL;
}

void hash_inicialize(HashCtx *ctx, int table_size, HashFunction generate_number, CompareFunction equal) {
    ctx->table = malloc(sizeof(HashNode*) * table_size);
    ctx->table_size = table_size;
    if(!ctx->table) {
        perror("Falha ao alocar hash_list");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < table_size; i++) ctx->table[i] = NULL;
    ctx->equal = equal;
    ctx->generate_number = generate_number;
}

void hash_insert(HashCtx *ctx, const void *key, size_t key_size, const void *value, size_t value_size) {

    int brute_number = ctx->generate_number(key);
    int h = hash(brute_number, ctx->table_size);

    HashNode *current = ctx->table[h];

    while(current != NULL) {
        // Não permite chaves repetidas
        if(ctx->equal(current->key, key)) return;
        current = current->next;
    }

    HashNode *head = ctx->table[h];
    HashNode *new = hash_list_create(key_size, value_size);

    memcpy(new->key, key, key_size);
    memcpy(new->value, value, value_size);
    new->next = head;
    ctx->table[h] = new;
}

void* hash_lookup(HashCtx *ctx, const void *key) {
    int brute_number = ctx->generate_number(key);
    int h = hash(brute_number, ctx->table_size);

    HashNode *current = ctx->table[h];

    while(current != NULL) {
        if(ctx->equal(current->key, key)) return current->value;
        current = current->next;
    }

    return NULL;
}

void hash_update_valor(HashCtx *ctx, const void *key, const void *value, size_t value_size) {
    int brute_number = ctx->generate_number(key);
    int h = hash(brute_number, ctx->table_size);

    HashNode *current = ctx->table[h];
    while(current != NULL && !ctx->equal(current->key, key)) current = current->next;

    // Chave não encontrada
    if(!current) return;

    memcpy(current->value, value, value_size);
}

void hash_remove(HashCtx *ctx, const void *key) {
    int brute_number = ctx->generate_number(key);
    int h = hash(brute_number, ctx->table_size);

    HashNode *current = ctx->table[h];
    HashNode *prev = NULL;

    if(!current) return;
    if(ctx->equal(current->key, key)) {
        prev = current;
        current = current->next;
        prev = hash_list_destroy(prev);
        ctx->table[h] = current;
        return;
    }

    while((current != NULL) && !ctx->equal(current->key, key)) {
        prev = current;
        current = current->next;
    }

    if(current == NULL) return;

    prev->next = current->next;
    current = hash_list_destroy(current);
}

void hash_destroy(HashCtx *ctx) {
    for(int i = 0; i < ctx->table_size; i++) {
        if(ctx->table[i]) {
            HashNode *current = ctx->table[i], *temp = NULL;
            while(current != NULL) {
                temp = current;
                current = current->next;
                temp = hash_list_destroy(temp);
            }
            ctx->table[i] = NULL;
        }
    }
    free(ctx->table);
}

void hash_print(HashCtx *ctx, PrintFunction print) {
    HashNode *current;
    for(int i = 0; i < ctx->table_size; i++) {
        printf("%d: ", i);
        current = ctx->table[i];
        while(current != NULL) {
            print(current->key, current->value);
            printf(" -> ");
            current = current->next;
        }
        printf("NULL\n");
    }
}