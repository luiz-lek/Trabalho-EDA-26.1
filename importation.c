//
// Created by luizao on 29/05/2026.
//

#include "importation.h"

#include <stdlib.h>
#include <string.h>

#include "person.h"
#include "file_manager.h"
#include "movie.h"

void trim(char* str) {
    if(!str) return;

    size_t len = strlen(str);
    while (len > 0 && str[len - 1] == ' ') len--;
    str[len] = '\0';

    size_t inicio = 0;
    while(str[inicio] != '\0' && str[inicio] == ' ') inicio++;

    if(inicio > 0) memmove(str, str + inicio, len - inicio + 1);
}

uint16_t str_to_uint16(char* line) {
    uint16_t ano = 0;
    int i = 0;
    while(line[i] != '\0') {
        ano *= 10;
        ano += line[i] - '0';
        i++;
    }
    return ano;
}

void fill_buffer_until_find_separator(char* line, char* buffer) {
    int i = 0;
    while (line[i] != '|' && line[i] != '\0') {
        buffer[i] = line[i];
        i++;
    }
    buffer[i] = '\0';
    if (line[i] == '|') i++;
    memmove(line, line + i, strlen(line) - i + 1); // Sobrescreve o trecho já lido com o restante da string.
}

void fill_person(Person *p, char* line) {
    char buffer[LINE_LENGTH];

    fill_buffer_until_find_separator(line, buffer); // Le o identificador da entidade (Person ou Movie) e jogar fora.

    fill_buffer_until_find_separator(line, buffer); // Lê o nome da pessoa pro buffer.
    trim(buffer);
    strcpy(p->name, buffer);

    fill_buffer_until_find_separator(line, buffer); // Lê a string do ano.
    trim(buffer);
    p->year = str_to_uint16(buffer);

    person_print(p);
}

void fill_movie(Movie *m, char* line) {
    char buffer[LINE_LENGTH];

    fill_buffer_until_find_separator(line, buffer); // Pula a parte com o tipo da entidade (Person ou Movie).

    fill_buffer_until_find_separator(line, buffer); // Lê o título.
    trim(buffer);
    strcpy(m->title, buffer);

    fill_buffer_until_find_separator(line, buffer); // Lê a string ano.
    trim(buffer);
    m->year = str_to_uint16(buffer);

    fill_buffer_until_find_separator(line, buffer);
    trim(buffer);
    strcpy(m->subtitle, buffer);

    movie_print(m);
}

void import_entities() {
    uint32_t person_id = -1;
    uint32_t movie_id = -1;

    FILE* fp = fopen(PATH_NODES, "r");
    if(!fp) {
        perror("Erro ao abrir arquivo de nodes.");
        exit(-1);
    }

    char buffer[LINE_LENGTH];

    Person p;
    Movie m;

    // pendente: adicionar as pessoas e filmes nas tabelas para relacionar seus ids.
    while (fgets(buffer, 256, fp) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if(buffer[0] == 'P') {
            person_id++;
            fill_person(&p, buffer);
        }
        if(buffer[0] == 'M') {
            movie_id++;
            fill_movie(&m, buffer);
        }
    }
}