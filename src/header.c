//
// Created by luizao on 02/06/2026.
//

#include "../include/header.h"
#include "../include/file_manager.h"

#include <stdio.h>
#include <stdlib.h>

void load_header(char *metadata_file, Header *h) {
    FILE *fp = open_file(metadata_file, "rb");
    read_data(fp, 0, h, HEADER_SIZE);
    fclose(fp);
}

void save_header(char *metadata_file, Header *h) {
    FILE *fp = fopen(metadata_file, "wb");
    if (!fp) {
        perror("(fopen) erro ao abir arquivo de metadados para ler header.");
        exit(EXIT_FAILURE);
    }

    write_data(fp, 0, h, HEADER_SIZE);
    fclose(fp);
}