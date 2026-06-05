//
// Created by luizao on 01/06/2026.
//

#include "../include/relationship.h"

#include <stdio.h>
#include <string.h>

#include "../include/file_manager.h"

RelationshipType parse_relationship_STRING(const char *string) {
    if(strcmp(string, "ACTED_IN") == 0) return ACTED_IN;
    if(strcmp(string, "DIRECTED") == 0) return DIRECTED;
    if(strcmp(string, "PRODUCED") == 0) return PRODUCED;
    if(strcmp(string, "WROTE") == 0) return WROTE;

    return 0;
}

void parse_STRING_relationship(char *string, RelationshipType type) {
    switch(type) {
        case ACTED_IN:
            strcpy(string, "ACTED_IN");
            break;
        case DIRECTED:
            strcpy(string, "DIRECTED");
            break;
        case PRODUCED:
            strcpy(string, "PRODUCED");
            break;
        default:
            strcpy(string, "WROTE");
    }
}

void relationship_initilize(Relationship *r) {
    r->is_valid = true;
    r->offset_next_movie = DISK_NULL;
    r->offset_next_person = DISK_NULL;
}

void print_relationship(Relationship *r) {
    printf("\nRelacionamento\n");

    char relationship_type[20];
    parse_STRING_relationship(relationship_type, r->relationship_type);
    printf("%d %s %d\n", r->person_id, relationship_type, r->movie_id);
    if(r->role[0] != '\0') {
        printf("Role: %s\n", r->role);
    }
    printf("Proximo filme: %d\n"
           "Próxima pessoa: %d\n", r->offset_next_movie, r->offset_next_person);
}

void print_relationships_file() {
    FILE *fp = open_file(PATH_RELATIONS_DATA, "rb+");

    Relationship r;
    uint32_t current = 0;
    bool read = read_data(fp, current, &r, RELATIONSHIP_SIZE);

    while(read) {
        print_relationship(&r);
        current += RELATIONSHIP_SIZE;
        read = read_data(fp, current, &r, RELATIONSHIP_SIZE);
    }

    fclose(fp);
}