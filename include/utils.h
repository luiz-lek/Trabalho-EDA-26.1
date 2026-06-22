//
// Created by luizao on 21/06/2026.
//

#ifndef TRABALHO_EDA_26_1_UTILS_H
#define TRABALHO_EDA_26_1_UTILS_H

#include <stdbool.h>
#include <stdint.h>

typedef bool (*CompareFunction)(const void *a, const void *b);

typedef struct {
    uint32_t val1;
    uint32_t val2;
} TupleIDs;


bool str_compare(const void *a, const void *b);
bool int_compare(const void *a, const void *b);
bool uint32_compare(const void *a, const void *b);
bool compare_tuple_ids(const void *a, const void *b);
#endif //TRABALHO_EDA_26_1_UTILS_H
