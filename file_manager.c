//
// Created by luizao on 29/05/2026.
//

#include "file_manager.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Lê os campos na ordem q estão declarados na struct do nó.
bool tree_node_load(TreeNode *node, uint8_t t, FILE *fp, uint32_t offset) {
    if(offset == DISK_NULL) return false;

    int keys_length = 2 * t - 1, child_length = 2 * t;

    // Cálculo da quantidades de bytes que o nó ocupa
    size_t buffer_size = sizeof(uint8_t) + sizeof(uint16_t)
                         + (keys_length * sizeof(uint32_t))
                         + (child_length * sizeof(uint32_t))
                         + sizeof(uint32_t);

    uint8_t *buffer = malloc(buffer_size); // uint8_t porque cada índice do vetor aramazena 1 byte
    if(!buffer) {
        perror("(malloc) falha ao alocar buffer para leitura de nó.");
        exit(-1);
    }

    fseek(fp, offset, SEEK_SET);
    size_t num_bytes = fread(buffer, 1, buffer_size, fp); // Lê todos o bytes da struct gravada no disco

    if(num_bytes != buffer_size) {
        free(buffer);
        return false;
    }

    uint32_t pos = 0;

    // Copia os bytes de is_leaf do buffer
    memcpy(&node->is_leaf, buffer + pos, sizeof(uint8_t));
    pos += sizeof(uint8_t);

    // Copia os bytes de num_keys do buffer
    memcpy(&node->num_keys, buffer + pos, sizeof(uint16_t));
    pos += sizeof(uint16_t);

    // Copia os bytes do vetor de chaves
    memcpy(node->key, buffer + pos, sizeof(uint32_t) * keys_length);
    pos += sizeof(uint32_t) * keys_length;

    // Copia o vetor de offsets
    memcpy(node->offset, buffer + pos, sizeof(uint32_t) * child_length);
    pos += sizeof(uint32_t) * child_length;

    // Copia o offset do proximo
    memcpy(&node->next, buffer + pos, sizeof(uint32_t));

    free(buffer);
    return true;
}

// Mesma lógica de cima, mas pra escrita
void tree_node_save(TreeNode *node, uint8_t t, FILE *fp, uint32_t offset) {
    int keys_length = 2 * t - 1, child_length = 2 * t;
    size_t buffer_size = sizeof(uint8_t) + sizeof(uint16_t)
                         + (keys_length * sizeof(uint32_t))
                         + (child_length * sizeof(uint32_t))
                         + sizeof(uint32_t);

    uint8_t *buffer = malloc(buffer_size);
    if(!buffer) {
        perror("(malloc) falha ao alocar buffer para escrita de nó.");
        exit(-1);
    }

    uint32_t pos = 0; // Marca o offset pro inicio do novo bloco de escrita

    // Escreve no buffer os bytes de is_leaf
    memcpy(buffer + pos, &node->is_leaf, sizeof(uint8_t));
    pos += sizeof(uint8_t);

    // Escreve no buffer os bytes de num_chaves
    memcpy(buffer + pos, &node->num_keys, sizeof(uint16_t));
    pos += sizeof(uint16_t);

    // Escreve no buffer os bytes do vetor de chaves
    memcpy(buffer + pos, node->key,  sizeof(uint32_t) * keys_length);
    pos += sizeof(uint32_t) * keys_length;

    // Escreve no buffer os bytes vetor de offsets
    memcpy(buffer + pos, node->offset,  sizeof(uint32_t) * child_length);
    pos += sizeof(uint32_t) * child_length;

    // Escreve no buffer os bytes do offset do proximo
    memcpy(buffer + pos, &node->next, sizeof(uint32_t));

    // Grava o buffer com todos os bytes de node
    fseek(fp, offset, SEEK_SET);
    fwrite(buffer, 1, buffer_size, fp);
    free(buffer);
}

void person_load(Person *p, FILE *fp, uint32_t offset) {
    fseek(fp, offset, SEEK_CUR);
    fread(p->name, sizeof(char), NAME_LENGTH, fp);
    fread(&p->year, sizeof(uint16_t), 1, fp);
}

void person_save(Person *p, FILE *fp, uint32_t offset) {
    fseek(fp, offset, SEEK_SET);
    fwrite(p->name, sizeof(char), NAME_LENGTH, fp);
    fwrite(&p->year, sizeof(uint16_t), 1, fp);
}

void movie_load(Movie *m, FILE *fp, uint32_t offset) {
    fseek(fp, offset, SEEK_SET);
    fread(m->title, sizeof(char), TITLE_LENGTH, fp);
    fread(m->subtitle, sizeof(char), SUB_TITLE_LENGTH, fp);
    fread(&m->year, sizeof(uint16_t), 1, fp);
}

void movie_save(Movie *m, FILE *fp, uint32_t offset) {
    fseek(fp, offset, SEEK_SET);
    fwrite(m->title, sizeof(char), TITLE_LENGTH, fp);
    fwrite(m->subtitle, sizeof(char), SUB_TITLE_LENGTH, fp);
    fwrite(&m->year, sizeof(uint16_t), 1, fp);
}

bool load_data(FILE *fp, uint32_t offset, void *data, size_t size) {
    fseek(fp, offset, SEEK_SET);
    return fread(data, size, 1, fp) == 1;
}

bool save_data(FILE *fp, uint32_t offset, void *data, size_t size) {
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

FILE* open_file(const char *filename, const char *mode) {
    FILE *fp = fopen(filename, mode);
    if(!fp) {
        fprintf(stderr,"ERROR: não foi possível abrir o arquivo \"%s\" no modo \"%s\"\n", filename, mode);
        perror("Detalhe: ");
        exit(EXIT_FAILURE);
    }
    return fp;
}