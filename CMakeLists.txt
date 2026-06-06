cmake_minimum_required(VERSION 4.1)
project(Trabalho_EDA_26_1 C)

set(CMAKE_C_STANDARD 11)

add_executable (
    Trabalho_EDA_26_1
        src/main.c
        include/file_manager.h
        include/person.h
        src/person.c
        src/file_manager.c
        include/b_plus_tree.h
        src/b_plus_tree.c
        include/importer.h
        src/importer.c
        src/movie.c
        include/movie.h
        include/hash_disk.h
        src/hash_disk.c
        include/relationship.h
        src/relationship.c
        src/data_structures_manipulation.c
        include/data_structures_manipulation.h
        src/teste.c
        include/teste.h
        src/resposta_questoes.c
        include/resposta_questoes.h
        include/hash.h
        src/hash.c
        src/hash_manipulation.c
        include/hash_manipulation.h
)
