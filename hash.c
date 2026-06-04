//
// Created by luizao on 01/06/2026.
//

#include "hash.h"
#include "file_manager.h"

#include <stdio.h>
#include <string.h>

#define TAM_BUFFER_SIZE 1024

// djb2
uint32_t hash(const char *str, uint32_t num_buckets) {
    uint32_t hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    hash = hash % num_buckets;
    // Retorna a posiçao já no arquivo
    return hash * sizeof(uint32_t);
}

void hash_initialize(char *table, char *data, uint32_t num_buckets) {
    FILE *fp = open_file(table, "wb");
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
    fp = open_file(data, "wb");
    fclose(fp);
}

void hash_insert(char *table, char *data, uint32_t num_buckets, const char *string, uint32_t id) {
    FILE *fph = open_file(table, "rb+");
    FILE *fpd = open_file(data, "rb+");

    uint32_t h = hash(string, num_buckets);

    uint32_t offset_list_head;
    read_data(fph, h, &offset_list_head, sizeof(uint32_t));

    HashData new_data;
    new_data.id = id;
    strcpy(new_data.name, string);
    new_data.next = DISK_NULL;
    new_data.valid = 1;


    // Ainda não foi inserido nenhum elemento no bucket
    if(offset_list_head == DISK_NULL) {
        uint32_t offset_new_data = file_size(fpd);
        write_data(fph, h, &offset_new_data, sizeof(uint32_t));
        write_data(fpd, offset_new_data, &new_data, sizeof(HashData));
    } else {
        HashData current;
        uint32_t offset_current = offset_list_head, offset_last = DISK_NULL;
        uint32_t offset_first_free_position = DISK_NULL;

        // Busca de espaço livre e se o elemento já tá na hash.
        bool data_found = 0;
        do {
            offset_last = offset_current;
            read_data(fpd, offset_current, &current, sizeof(HashData));
            if(current.valid && strcmp(new_data.name, current.name) == 0) {
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
                offset_first_free_position = file_size(fpd);
                current.next = offset_first_free_position;
                write_data(fpd, offset_last, &current, sizeof(HashData));
            }
            write_data(fpd, offset_first_free_position, &new_data, sizeof(HashData));
        }
    }

    fclose(fph);
    fclose(fpd);
}

void remove_data(char *table, char *data, uint32_t num_buckets, const char *string) {
    FILE *fph = open_file(table, "rb+");
    FILE *fpd = open_file(data, "rb+");

    uint32_t h = hash(string, num_buckets);

    HashData current;
    uint32_t offset_current;
    read_data(fph, h, &offset_current, sizeof(uint32_t));
    fclose(fph);

    while(offset_current != DISK_NULL) {
        read_data(fpd, offset_current, &current, sizeof(HashData));
        if(current.valid && strcmp(current.name, string) == 0) {
            current.valid = 0;
            write_data(fpd, offset_current, &current, sizeof(HashData));
            break;
        }
        offset_current = current.next;
    }

    fclose(fpd);
}

uint32_t get_id(char *table, char *data, uint32_t num_buckets, const char *string) {
    FILE *fph = open_file(table, "rb+");
    FILE *fpd = open_file(data, "rb+");

    uint32_t h = hash(string, num_buckets);

    HashData current;
    uint32_t offset_current;
    read_data(fph, h, &offset_current, sizeof(uint32_t));
    fclose(fph);

    while(offset_current != DISK_NULL) {
        read_data(fpd, offset_current, &current, sizeof(HashData));
        if(current.valid && strcmp(current.name, string) == 0) {
            fclose(fpd);
            return current.id;
        }
        offset_current = current.next;
    }

    fclose(fpd);
    return DISK_NULL;
}