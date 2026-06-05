//
// Created by luizao on 29/05/2026.
//

#ifndef TRABALHO_EDA_26_1_PERSON_H
#define TRABALHO_EDA_26_1_PERSON_H

#define NAME_LENGTH 50
#include "stdint.h"

typedef struct {
    char name[NAME_LENGTH];
    uint16_t year;
} Person;

Person* person_allocate();
void person_copy(Person *p, Person *copy);
void person_print(Person *p);

#endif //TRABALHO_EDA_26_1_PERSON_H