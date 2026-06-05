//
// Created by luizao on 29/05/2026.
//

#include "../include/file_manager.h"

#include <stdlib.h>
#include <stdbool.h>

bool read_data(FILE *fp, uint32_t offset, void *data, size_t size) {
    fseek(fp, offset, SEEK_SET);
    return fread(data, size, 1, fp) == 1;
}

bool write_data(FILE *fp, uint32_t offset, const void *data, size_t size) {
    fseek(fp, offset, SEEK_SET);
    return fwrite(data, size, 1, fp) == 1;
}

uint32_t file_size(FILE *fp) {
    uint32_t size = 0;
    const uint32_t cur_position = ftell(fp);

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    fseek(fp, cur_position, SEEK_SET);
    return size;
}

FILE* open_file(const char *file_name, const char *mode) {
    FILE *fp = fopen(file_name, mode);
    if(!fp) {
        fprintf(stderr,"ERROR: não foi possível abrir o arquivo \"%s\" no modo \"%s\"\n", file_name, mode);
        perror("Detalhe");
        exit(EXIT_FAILURE);
    }
    return fp;
}

void create_file(const char *file_name) {
    FILE *fp = open_file(file_name, "wb");
    fclose(fp);
}