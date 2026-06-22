//
// Created by luizao on 21/06/2026.
//
#include "../include/linked_list.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static  Node* create_node(void *data, ListType type, size_t data_size) {
    Node *new = malloc(sizeof(Node));
    if (!new) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    if (type == STR) data_size = strlen((char*)data) + 1;

    new->data = malloc(data_size);
    if (!new->data) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    memcpy(new->data, data, data_size);
    new->next = NULL;
    return new;
}

static void list_add_first(LinkedList *list, void *data) {
    Node *head = list->head;
    Node *new = create_node(data, list->type, list->data_size);
    new->next = head;
    list->head = new;
}

static void list_add_last(LinkedList *list, void *data) {
    if (!list->head) {
        list_add_first(list, data);
        return;
    }

    Node *current = list->head, *new = create_node(data, list->type, list->data_size);

    while (current->next != NULL) current = current->next;

    current->next = new;
}

static bool list_lookup(LinkedList *list, void *data) {
    if (!list->compare_function) return false;

    Node *current = list->head;
    while (current) {
        if (list->compare_function(current->data, data)) return true;
        current = current->next;
    }

    return false;
}

static void list_clear(LinkedList *list) {
    Node *current = list->head, *prev = NULL;

    while (current) {
        prev = current;
        current = current->next;
        free(prev->data);
        free(prev);
    }

    list->head = NULL;
}

void list_initialize(LinkedList *list, ListType type) {
    list->head = NULL;
    list->type = type;
    list->add_first = list_add_first;
    list->add_last = list_add_last;
    list->lookup = list_lookup;
    list->clear = list_clear;

    switch (type) {
        case UINT32:
            list->compare_function = uint32_compare;
            list->data_size = sizeof(uint32_t);
            break;
        case STR:
            list->compare_function = str_compare;
            list->data_size = 0;
            break;
        case INT:
            list->compare_function = int_compare;
            list->data_size = sizeof(int);
            break;
    }
}