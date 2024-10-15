cmake_minimum_required(VERSION 3.30)

project(
    HSM
    VERSION 0.1
    LANGUAGES C
)

add_subdirectory(test)
add_library(hsm src/hsm.c)
