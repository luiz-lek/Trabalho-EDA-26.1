//
// Created by luizao on 01/06/2026.
//

#ifndef TRABALHO_EDA_26_1_RELATIONSHIP_H
#define TRABALHO_EDA_26_1_RELATIONSHIP_H

#define RELATIONSHIP_SIZE sizeof(Relationship)

#include <stdint.h>

typedef enum {
    ACTED_IN,
    DIRECTED,
    PRODUCED,
    WROTE
} RelationshipType;

typedef struct {
    uint32_t person_id;
    uint8_t relationship_type;
    uint32_t movie_id;
    char role[50];
} Relationship;


RelationshipType parse_relationship_STRING(const char *string);

void print_file_relationships();

#endif //TRABALHO_EDA_26_1_RELATIONSHIP_H