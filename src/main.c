#include <stdio.h>

#include "../include/importer.h"
#include "../include/teste.h"

int main() {
    import_data(6);
    print_movies_of_a_person("Marshall Bell");

    printf("\n\n\n");
    print_person_of_a_movie("The Matrix");
    return 0;
}
