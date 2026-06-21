#include <stdio.h>

#include "../include/importer.h"
#include "../include/data_base_manipulation.h"
#include "../include/teste.h"

int main() {
    int t = 6;

    db_inicialize(t);
    DB db;
    db_load_context(&db);

    import_data(&db);
    // tree_print(&db.tree_movie_context);

    print_movies_of_a_person(&db,"Keanu Reeves");
    printf("\n\n\n");
    print_person_of_a_movie(&db,"The Matrix");

    db_close_context(&db);
    db_destroy();
    // hash_test();
    return 0;
}