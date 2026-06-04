#include <stdio.h>
#include <stdlib.h>

#include "b_plus_tree.h"
#include "file_manager.h"
#include "importer.h"
#include "relationship.h"

int main() {
    import_data(4);

    // printf("Árvore de filmes\n\n");
    // tree_print(PATH_INDEX_MOVIE_TREE, PATH_METADATA_MOVIE_TREE);
    // printf("\n\nÁrvore de pessoas\n\n");
    // tree_print(PATH_INDEX_PERSON_TREE, PATH_METADATA_PERSON_TREE);
    //
    // print_file_relationships();

    Person person;
    uint32_t offset_person = tree_search(PATH_INDEX_PERSON_TREE, PATH_METADATA_PERSON_TREE, 120);
    if(offset_person == DISK_NULL) {
        printf("No person found\n");
    } else {
        FILE *fp = fopen(PATH_DATA_PERSON_TREE, "rb");
        if(!fp) exit(EXIT_FAILURE);


        person_load(&person, fp, offset_person);
        fclose(fp);
        person_print(&person);
    }

    return 0;
}