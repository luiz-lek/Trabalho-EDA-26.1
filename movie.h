//
// Created by luizao on 30/05/2026.
//

#ifndef TRABALHO_EDA_26_1_MOVIE_H
#define TRABALHO_EDA_26_1_MOVIE_H

#define TITLE_LENGTH 30
#define SUB_TITLE_LENGTH 200
#define ACTORS_LENGTH 20
#define DIRECTION_LENGTH 20
#define PRODUTORS_LENGTH 5
#define WRITERS_LENGTH 5

#include <stdint.h>

typedef struct {
    char title[TITLE_LENGTH];
    char subtitle[SUB_TITLE_LENGTH];
    uint16_t year;
} Movie;

Movie* movie_allocate();
void movie_copy(Movie *dest, Movie *src);
void movie_print(Movie *movie);

#endif //TRABALHO_EDA_26_1_MOVIE_H