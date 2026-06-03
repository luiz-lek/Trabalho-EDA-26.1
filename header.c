//
// Created by luizao on 02/06/2026.
//

#include "header.h"
#include "file_manager.h"

#include <stdio.h>
#include <stdlib.h>

void load_header(char *metadata_file, Header *h) {
    FILE *fp = fopen(metadata_file, "rb");
    if (!fp) {
        perror("(fopen) erro ao abir arquivo de metadados para salvar header.");
        exit(EXIT_FAILURE);
    }

    load_data(fp, 0, h, HEADER_SIZE);
    fclose(fp);
}

void save_header(char *metadata_file, Header *h) {
    FILE *fp = fopen(metadata_file, "wb");
    if (!fp) {
        perror("(fopen) erro ao abir arquivo de metadados para ler header.");
        exit(EXIT_FAILURE);
    }

    save_data(fp, 0, h, HEADER_SIZE);
    fclose(fp);
}