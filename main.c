#include <stdio.h>

#include "b_plus_tree.h"
#include "file_manager.h"
#include "importer.h"
#include "relationship.h"

int main() {
    import_data(4);

    printf("Árvore de filmes\n\n");
    // tree_print(PATH_INDEX_MOVIE_TREE, PATH_METADATA_MOVIE_TREE);
    // printf("\n\nÁrvore de pessoas\n\n");
    // tree_print(PATH_INDEX_PERSON_TREE, PATH_METADATA_PERSON_TREE);

    print_file_relationships();
    return 0;
}