//
// Created by luizao on 29/05/2026.
//

#include "../include/importer.h"
#include "../include/file_manager.h"
#include "../include/person.h"
#include "../include/movie.h"

#include <string.h>

void trim(char* str) {
    if(!str) return;

    size_t len = strlen(str);
    while (len > 0 && str[len - 1] == ' ') len--;
    str[len] = '\0';

    size_t inicio = 0;
    while(str[inicio] != '\0' && str[inicio] == ' ') inicio++;

    if(inicio > 0) memmove(str, str + inicio, len - inicio + 1);
}

uint16_t str_to_uint16(const char* line) {
    uint16_t ano = 0;
    int i = 0;
    while(line[i] != '\0') {
        ano *= 10;
        ano += line[i] - '0';
        i++;
    }
    return ano;
}

void jump_token(char *line, char separator) {
    int i = 0;
    while(line[i] != '\0' && line[i] != separator) i++;
    if(line[i] == separator) i++;
    memmove(line, line + i, strlen(line) - i + 1); // Sobrescreve o trecho já lido com o restante da string.
}

void get_token(char *line, char *token_buffer, char separator) {
    int i = 0;
    while (line[i] != separator && line[i] != '\0') {
        token_buffer[i] = line[i];
        i++;
    }
    token_buffer[i] = '\0';
    if(line[i] == separator) i++;
    memmove(line, line + i, strlen(line) - i + 1); // Sobrescreve o trecho já lido com o restante da string.
}

void get_token_formated(char *line, char *token_buffer, char separator) {
    get_token(line, token_buffer, separator);
    trim(token_buffer);
}

void fill_person(Person *p, char *line) {
    char token_buffer[LINE_LENGTH];
    jump_token(line, '|'); // Pula a parte do identificador da entidade (Person ou Movie).
    get_token_formated(line, p->name, '|'); // Lê o nome da pessoa pro buffer.
    get_token_formated(line, token_buffer, '|'); // Lê a string do ano.
    p->year = str_to_uint16(token_buffer);
}

void fill_movie(Movie *m, char *line) {
    char token_buffer[LINE_LENGTH];
    jump_token(line, '|'); // Pula a parte com o tipo da entidade (Person ou Movie).
    get_token_formated(line, m->title, '|'); // Lê o título.
    get_token_formated(line, token_buffer, '|'); // Lê a string ano.
    m->year = str_to_uint16(token_buffer);
    get_token_formated(line, m->subtitle, '|'); // Lê o subtítulo.
}

void import_movies_and_persons(DB *db) {
    FILE *brute_data = open_file(PATH_NODES, "r");

    char buffer[LINE_LENGTH];

    Person p;
    Movie m;
    uint32_t person_id = -1,
             movie_id = -1,
             offset_movie_data = 0,
             offset_person_data = 0;

    while (fgets(buffer, LINE_LENGTH, brute_data) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if(buffer[0] == 'P') {
            person_id++;
            fill_person(&p, buffer);

            offset_person_data = file_size(db->tree_person_context.fp_data);
            write_data(db->tree_person_context.fp_data, offset_person_data, &p, sizeof(Person));
            tree_insert(&db->tree_person_context, person_id, offset_person_data);
            hash_disk_insert(&db->hash_person_lookup_context, p.name, person_id);
        }
        if(buffer[0] == 'M') {
            movie_id++;
            fill_movie(&m, buffer);

            offset_movie_data = file_size(db->tree_movie_context.fp_data);
            write_data(db->tree_movie_context.fp_data, offset_movie_data, &m, sizeof(Movie));
            tree_insert(&db->tree_movie_context, movie_id, offset_movie_data);
            hash_disk_insert(&db->hash_movie_lookup_context, m.title, movie_id);
        }
    }

    fclose(brute_data);
}

void import_relationships(DB *db) {
    FILE *fp = open_file(PATH_RELATIONSHIPS, "r");

    char line[LINE_LENGTH];
    char token_buffer[LINE_LENGTH];
    Relationship relationship;

    while(fgets(line, LINE_LENGTH, fp) != NULL) {
        relationship_initilize(&relationship);
        line[strcspn(line, "\n")] = '\0';

        jump_token(line, '|');
        get_token_formated(line, token_buffer, '|');
        relationship.person_id = hash_disk_get_value(&db->hash_person_lookup_context, token_buffer);

        get_token_formated(line, token_buffer, '|');
        relationship.relationship_type = parse_relationship_STRING(token_buffer);

        jump_token(line, '|');
        get_token_formated(line, token_buffer, '|');
        relationship.movie_id = hash_disk_get_value(&db->hash_movie_lookup_context, token_buffer);

        // Só tem papel se for relação do tipo atuação
        if(relationship.relationship_type == ACTED_IN) {
            jump_token(line, ':');
            get_token_formated(line, token_buffer, '\0');
            strncpy(relationship.role, token_buffer, sizeof(relationship.role)-1);
        } else {
            relationship.role[0] = '\0';
        }

        // Insere no arquivo de relacionamento com as listas encadeadas de folme pra pessoa e pessoa pra filme
        insert_relation(db->relation, &relationship,
            &db->hash_person_relations_context,
            &db->hash_movie_relations_context);
    }

    fclose(fp);
}

void import_data(DB *db) {
    import_movies_and_persons(db);

    fflush(db->tree_movie_context.fp_index);
    fflush(db->tree_movie_context.fp_metadata);

    fflush(db->tree_person_context.fp_index);
    fflush(db->tree_person_context.fp_metadata);

    fflush(db->hash_person_lookup_context.fph);
    fflush(db->hash_person_lookup_context.fpd);

    fflush(db->hash_movie_lookup_context.fph);
    fflush(db->hash_movie_lookup_context.fpd);

    import_relationships(db);

    fflush(db->hash_person_relations_context.fph);
    fflush(db->hash_person_relations_context.fpd);

    fflush(db->hash_movie_relations_context.fph);
    fflush(db->hash_movie_relations_context.fpd);

    fflush(db->relation);
}