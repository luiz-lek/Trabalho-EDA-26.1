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

HashNode** hash_inicialize(HashNode** table, int table_size) {
    table = malloc(sizeof(HashNode*) * table_size);
    if(!table) {
        perror("Falha ao alocar hash_list");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < table_size; i++) table[i] = NULL;
    return table;
}

void hash_insert(HashNode** table, int table_size, HashFunction generate_number, CompareFunction equal,
    const void *key, size_t key_size, const void *value, size_t value_size) {

    int brute_number = generate_number(key);
    int h = hash(brute_number, table_size);

    HashNode *current = table[h];

    while(current != NULL) {
        // Não permite chaves repetidas
        if(equal(current->key, key)) return;
        current = current->next;
    }

    HashNode *head = table[h];
    HashNode *new = hash_list_create(key_size, value_size);

    memcpy(new->key, key, key_size);
    memcpy(new->value, value, value_size);
    new->next = head;
    table[h] = new;
}

void hash_update_valor(HashNode** table, int table_size, HashFunction generate_number, CompareFunction equal,
    const void *key, const void *value, size_t value_size) {

    int brute_number = generate_number(key);
    int h = hash(brute_number, table_size);

    HashNode *current = table[h];
    while(current != NULL && !equal(current->key, key)) current = current->next;

    // Chave não encontrada
    if(!current) return;

    memcpy(current->value, value, value_size);
}

void hash_remove(HashNode** table, int table_size, HashFunction generate_number, CompareFunction equal, const void *key) {
    int brute_number = generate_number(key);
    int h = hash(brute_number, table_size);

    HashNode *current = table[h];
    HashNode *prev = NULL;

    if(!current) return;
    if(equal(current->key, key)) {
        prev = current;
        current = current->next;
        prev = hash_list_destroy(prev);
        table[h] = current;
        return;
    }

    while((current != NULL) && !equal(current->key, key)) {
        prev = current;
        current = current->next;
    }

    if(current == NULL) return;

    prev->next = current->next;
    current = hash_list_destroy(current);
}

void hash_destroy(HashNode** table, int table_size) {
    for(int i = 0; i < table_size; i++) {
        if(table[i]) {
            HashNode *current = table[i], *temp = NULL;
            while(current != NULL) {
                temp = current;
                current = current->next;
                temp = hash_list_destroy(temp);
            }
            table[i] = NULL;
        }
    }
    free(table);
}

void hash_print(HashNode** table, int table_size, PrintFunction print) {
    HashNode *current;
    for(int i = 0; i < table_size; i++) {
        printf("%d: ", i);
        current = table[i];
        while(current != NULL) {
            print(current->key, current->value);
            printf(" -> ");
            current = current->next;
        }
        printf("NULL\n");
    }
}