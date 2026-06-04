//
// Created by luizao on 01/06/2026.
//

#include "hash.h"

#include <stdio.h>
#include <stdlib.h>

#include "file_manager.h"

void hash_initialize(char *hash, char *data) {
    FILE *fp = open_file(hash, "wb");
    fclose(fp);
    fp = open_file(data, "wb");
    fclose(fp);
}

void hash_insert(char *hash, char *data, char *name, uint32_t id);

void remove_data(char *hash, char *data, uint32_t id);

uint32_t get_id(char *hash, char *data, char *name) {
    return 1;
}