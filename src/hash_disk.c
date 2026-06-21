//
// Created by luizao on 01/06/2026.
//

#include "../include/hash_disk.h"
#include "../include/file_manager.h"

#include <stdio.h>
#include <string.h>

#define TAM_BUFFER_SIZE 1024

uint32_t hash_function(HashFunction generate_brute_number, const void *key, int num_buckets) {
    const uint32_t brute_number = generate_brute_number(key);
    return brute_number % num_buckets * sizeof(uint32_t);
}

void hash_disk_initialize(const char *path_table, const char *path_data, int num_buckets) {
    FILE *fp = open_file(path_table, "wb");
    uint32_t buffer_ram[TAM_BUFFER_SIZE];

    for(int i = 0; i < TAM_BUFFER_SIZE; i++) buffer_ram[i] = DISK_NULL;

    uint32_t num_buckets_wrote = 0;

    while(num_buckets_wrote < num_buckets) {
        uint32_t to_write = num_buckets - num_buckets_wrote;
        if(to_write > TAM_BUFFER_SIZE) to_write = TAM_BUFFER_SIZE;

        fwrite(buffer_ram, sizeof(uint32_t), to_write, fp);

        num_buckets_wrote += to_write;
    }

    fclose(fp);
    create_file(path_data);
}

void hash_disk_insert(HashContext *ctx, const void *key, uint32_t valor) {
    uint32_t h = hash_function(ctx->generate_brute_number, key, ctx->num_buckets);

    uint32_t offset_list_head;
    read_data(ctx->fph, h, &offset_list_head, sizeof(uint32_t));

    HashDiskNode new_data;
    memset(&new_data.key, 0, sizeof(new_data.key));

    size_t copy_size = ctx->key_size;
    if (copy_size > sizeof(new_data.key)) {
        copy_size = sizeof(new_data.key);
    }

    memcpy(&new_data.key, key, copy_size);
    new_data.valor = valor;
    new_data.next = DISK_NULL;
    new_data.valid = 1;


    // Ainda não foi inserido nenhum elemento no bucket
    if(offset_list_head == DISK_NULL) {
        uint32_t offset_new_data = file_size(ctx->fpd);
        write_data(ctx->fph, h, &offset_new_data, sizeof(uint32_t));
        write_data(ctx->fpd, offset_new_data, &new_data, sizeof(HashDiskNode));
    } else {
        HashDiskNode current;
        uint32_t offset_current = offset_list_head, offset_last = DISK_NULL;
        uint32_t offset_first_free_position = DISK_NULL;

        // Busca de espaço livre e se o elemento já tá na hash.
        bool data_found = 0;
        do {
            offset_last = offset_current;
            read_data(ctx->fpd, offset_current, &current, sizeof(HashDiskNode));
            if(current.valid && ctx->comparator(&current.key, key)) {
                data_found = true;
                break;
            }
            if(offset_first_free_position == DISK_NULL && !current.valid) {
                offset_first_free_position = offset_current;
                new_data.next = current.next;
            }
            offset_current = current.next;
        } while(offset_current != DISK_NULL);

        // O nome ainda não tá na hash e pode ser inserido
        if(!data_found) {
            if(offset_first_free_position == DISK_NULL) {
                offset_first_free_position = file_size(ctx->fpd);
                current.next = offset_first_free_position;
                write_data(ctx->fpd, offset_last, &current, sizeof(HashDiskNode));
            }
            write_data(ctx->fpd, offset_first_free_position, &new_data, sizeof(HashDiskNode));
        }
    }
}

void hash_disk_remove_data(HashContext *ctx, int num_buckets, const void *key) {
    uint32_t h = hash_function(ctx->generate_brute_number, key, num_buckets);

    HashDiskNode current;
    uint32_t offset_current;
    read_data(ctx->fph, h, &offset_current, sizeof(uint32_t));

    while(offset_current != DISK_NULL) {
        read_data(ctx->fpd, offset_current, &current, sizeof(HashDiskNode));
        if(current.valid && ctx->comparator(&current.key, key)) {
            current.valid = 0;
            write_data(ctx->fpd, offset_current, &current, sizeof(HashDiskNode));
            break;
        }
        offset_current = current.next;
    }
}

uint32_t hash_disk_get_value(HashContext *ctx, const void *key) {
    uint32_t h = hash_function(ctx->generate_brute_number, key, ctx->num_buckets);

    HashDiskNode current;
    uint32_t offset_current;
    read_data(ctx->fph, h, &offset_current, sizeof(uint32_t));

    while(offset_current != DISK_NULL) {
        read_data(ctx->fpd, offset_current, &current, sizeof(HashDiskNode));
        if(current.valid && ctx->comparator(&current.key, key)) return current.valor;
        offset_current = current.next;
    }

    return DISK_NULL;
}

void hash_disk_change_value(HashContext *ctx, const void *key, uint32_t new_value) {
    uint32_t h = hash_function(ctx->generate_brute_number, key, ctx->num_buckets);

    HashDiskNode current;
    uint32_t offset_current;
    read_data(ctx->fph, h, &offset_current, sizeof(uint32_t));

    while(offset_current != DISK_NULL) {
        read_data(ctx->fpd, offset_current, &current, sizeof(HashDiskNode));
        if(current.valid && ctx->comparator(&current.key, key)) {
            current.valor = new_value;
            write_data(ctx->fpd, offset_current, &current, sizeof(HashDiskNode));
            break;
        }
        offset_current = current.next;
    }
}

void hash_disk_delete_table(const char *path_table, const char *path_data) {
    remove(path_table);
    remove(path_data);
}