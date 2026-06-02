cmake_minimum_required(VERSION 4.1)
project(Trabalho_EDA_26_1 C)

set(CMAKE_C_STANDARD 11)

add_executable (
    Trabalho_EDA_26_1
    main.c
    file_manager.h
    person.h
    person.c
    file_manager.c
    b_plus_tree.h
    b_plus_tree.c
    importer.h
    importer.c
    movie.c
    movie.h
    hash.h
    hash.c
    relationship.h
    relationship.c
)
