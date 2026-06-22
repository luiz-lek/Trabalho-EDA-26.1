//
// Created by luizao on 21/06/2026.
//

#include "../include/utils.h"

#include <stdint.h>
#include <string.h>

bool str_compare(const void *a, const void *b) {
    return strcmp((char*)a, (char*)b) == 0;
}

bool int_compare(const void *a, const void *b) {
    return (*(int*)a == *(int*)b);
}

bool uint32_compare(const void *a, const void *b) {
    return (*(uint32_t*)a == *(uint32_t*)b);
}

bool compare_tuple_ids(const void *a, const void *b) {
    const TupleIDs *t1 = (TupleIDs*)a;
    const TupleIDs *t2 = (TupleIDs*)b;

    return (t1->val1 == t2->val1) && (t1->val2 == t2->val2);
}