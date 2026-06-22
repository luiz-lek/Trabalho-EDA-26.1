//
// Created by luizao on 21/06/2026.
//

#ifndef TRABALHO_EDA_26_1_LINKEDLIST_H
#define TRABALHO_EDA_26_1_LINKEDLIST_H

#include "utils.h"
#include <stdlib.h>

typedef struct LinkedList LinkedList;

typedef enum {
    UINT32,
    STR,
    INT,
} ListType;

typedef struct list_node {
    void *data;
    struct list_node *next;
} Node;

struct LinkedList{
    Node *head;
    void (*add_first)(LinkedList *list, void *data);
    void (*add_last)(LinkedList *list, void *data);
    bool (*lookup)(LinkedList *list, void *data);
    void (*clear)(LinkedList *list);
    CompareFunction compare_function;
    ListType type;
    size_t data_size;
};

void list_initialize(LinkedList *list, ListType type);

#endif //TRABALHO_EDA_26_1_LINKEDLIST_H
