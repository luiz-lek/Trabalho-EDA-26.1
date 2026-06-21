#include "../include/hash_manipulation.h"

#include <stdio.h>
#include <string.h>

int int_generator(const void *key) {
    return *(int*)key;
}

bool int_comparate(const void *a, const void *b) {
    return (*(int*)a == *(int*)b);
}

void print_int(const void *key, const void *valor) {
    printf("(%d, %d)", *(int*)key, *(int*)valor);
}

void hash_int_to_int_insert(Hash *self, const void *key, const void *valor) {
    hash_insert(self->table, self->table_size, int_generator, int_comparate,
        key, sizeof(int), valor, sizeof(int));
}

void hash_int_to_int_update_valor(Hash *self, const void *key, const void *valor) {
    hash_update_valor(self->table, self->table_size, int_generator, int_comparate, key, valor, sizeof(int));
}

void hash_int_to_int_remove(Hash *self, const void *key) {
    hash_remove(self->table, self->table_size, int_generator, int_comparate, key);
}

void hash_int_to_int_print(Hash *self) {
    hash_print(self->table, self->table_size, print_int);
}

void hash_clear_int_to_int(Hash *self) {
    hash_destroy(self->table, self->table_size);
}

void hash_int_to_int_create(Hash *hash) {
    hash->insert = hash_int_to_int_insert;
    hash->update_valor = hash_int_to_int_update_valor;
    hash->remove = hash_int_to_int_remove;
    hash->destroy = hash_clear_int_to_int;
    hash->print = hash_int_to_int_print;
}







 int str_generator(const void *key) {
     const unsigned char *str = key;
     int hash = 5381;
     int c;

     while ((c = *str++)) {
         hash = ((hash << 5) + hash) + c;
     }
     // Retorna o número bruto
     return hash;
 }

 bool str_compare(const void *a, const void *b) {
     return strcmp((char*)a, (char*)b) == 0;
 }

 void print_str(const void *key, const void *valor) {
     printf("(%s, %d)", (char*)key, *(int*)valor);
 }


 void hash_string_to_int_insert(Hash *self, const void *key, const void *valor) {
     hash_insert(self->table, self->table_size, str_generator, str_compare, key, strlen(key)+1, valor, sizeof(int));
 }

 void hash_string_to_int_update_valor(Hash *self, const void *key, const void *valor) {
     hash_update_valor(self->table, self->table_size, str_generator, str_compare, key, valor, sizeof(int));
 }

 void hash_string_to_int_remove(Hash *self, const void *key) {
     hash_remove(self->table, self->table_size, str_generator, str_compare, key);
 }

 void hash_string_to_int_print(Hash *self) {
     hash_print(self->table, self->table_size, print_str);
 }

 void hash_clear_string_to_int(Hash *self) {
     hash_destroy(self->table, self->table_size);
 }

 void hash_string_to_int_create(Hash *hash) {
     hash->insert = hash_string_to_int_insert;
     hash->update_valor = hash_string_to_int_update_valor;
     hash->remove = hash_string_to_int_remove;
     hash->destroy = hash_clear_string_to_int;
     hash->print = hash_string_to_int_print;
}

void hash_create(Hash *hash, HashType type, int size) {
    hash->table_size = size;
    hash->table = hash_inicialize(hash->table, hash->table_size);
    switch(type) {
        case INT_INT:
            hash_int_to_int_create(hash);
            break;
        case STRING_INT:
            hash_string_to_int_create(hash);
    }
}