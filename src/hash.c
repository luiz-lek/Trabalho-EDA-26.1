#include "../include/hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hash(int num) {
    return num % TAM_HASH;
}

HashList* hash_list_create(size_t key_size, size_t value_size) {
    HashList *new = malloc(sizeof(HashList));
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

HashList* hash_list_destroy(HashList *hash_list) {
    free(hash_list->key);
    free(hash_list->value);
    free(hash_list);
    return NULL;
}

void hash_inicilize(TH table) {
    for (int i = 0; i < TAM_HASH; i++) table[i] = NULL;
}

void hash_insert(TH table, HashFunction generate_number, CompareFunction equal,
    const void *key, size_t key_size, const void *value, size_t value_size) {

    int brute_number = generate_number(key);
    int h = hash(brute_number);

    HashList *current = table[h];

    while(current != NULL) {
        // Não permite chaves repetidas
        if(equal(current->key, key)) return;
        current = current->next;
    }

    HashList *head = table[h];
    HashList *new = hash_list_create(key_size, value_size);

    memcpy(new->key, key, key_size);
    memcpy(new->value, value, value_size);
    new->next = head;
    table[h] = new;
}

void hash_update_valor(TH table, HashFunction generate_number, CompareFunction equal, const void *key, const void *value, size_t value_size) {
    int brute_number = generate_number(key);
    int h = hash(brute_number);

    HashList *current = table[h];
    while(current != NULL && !equal(current->key, key)) current = current->next;

    // Chave não encontrada
    if(!current) return;

    memcpy(current->value, value, value_size);
}

void hash_remove(TH table, HashFunction generate_number, CompareFunction equal, const void *key) {
    int brute_number = generate_number(key);
    int h = hash(brute_number);

    HashList *current = table[h];
    HashList *prev = NULL;

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

void hash_destroy(TH table) {
    for(int i = 0; i < TAM_HASH; i++) {
        if(table[i]) {
            HashList *current = table[i], *temp = NULL;
            while(current != NULL) {
                temp = current;
                current = current->next;
                temp = hash_list_destroy(temp);
            }
        }
    }
}