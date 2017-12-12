cmake_minimum_required(VERSION 3.8)
project(C)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES
        cipher.c
        cipher.h
        main.c
        valid.c
        valid.h
        copy.c
        copy.h
        types.h freq.c freq.h dico.c dico.h test.c)

add_executable(C ${SOURCE_FILES})