cmake_minimum_required(VERSION 3.30)

project(
    HSM
    VERSION 0.1
    LANGUAGES C
)

option(LOGGING "Enable logging in hsm")

add_subdirectory(test)
add_subdirectory(examples/turnstile)
add_library(hsm src/hsm.c)
if(LOGGING)
    target_compile_definitions(hsm PRIVATE HSM_LOGGING)
endif()