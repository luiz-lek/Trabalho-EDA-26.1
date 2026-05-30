//
// Created by luizao on 29/05/2026.
//

#include "person.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Person* person_allocate() {
    Person* person = malloc(sizeof(Person));
    if(!person) {
        perror("(malloc) falha ao alocar pessoa.");
        exit(1);
    }
    return person;
}

void person_copy(Person *p, Person *copy) {
    memcpy(copy, p, sizeof(Person));
}

void person_print(Person *p) {
    printf("\n----- PESSOA -----\n\tNome: %s\n", p->name);
    printf("\tYear: %d\n", p->year);
}