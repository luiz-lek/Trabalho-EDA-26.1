//
// Created by luizao on 30/05/2026.
//

#include "../include/movie.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Movie* movie_allocate() {
    Movie *movie = malloc(sizeof(Movie));
    if(!movie) {
        perror("(malloc) falha ao alocar filme");
        exit(-1);
    }
    return movie;
}

void movie_copy(Movie *dest, Movie *src) {
    memcpy(dest, src, sizeof(Movie));
}

void movie_print(Movie *movie) {
    if(movie) {
        printf("\n----- FILME -----\n\tTitle: %s\n", movie->title);
        printf("\tSubtitle: %s\n", movie->subtitle);
        printf("\tYear: %d\n", movie->year);
    }
}