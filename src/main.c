#include <stdio.h>

#include "../include/importer.h"
#include "../include/data_base_manipulation.h"
#include "../include/teste.h"
#include "../include/resposta_questoes.h"

int main() {
    int t = 3;

    db_inicialize(t);
    DB db;
    db_load_context(&db);

    import_data(&db);
    tree_print(&db.tree_movie_context);
    printf("\n\n\n");
    tree_print(&db.tree_person_context);
    // print_movies_of_a_person(&db,"Keanu Reeves");
    // print_person_of_a_movie(&db,"The Matrix");
    //
    // db_close_context(&db);
    // db_destroy();
    // hash_test();
    // printf("-------- Pessoas que trabalharam juntas --------\n\n");
    // pessoas_trabalharam_juntas(&db);
    return 0;
}