cmake_minimum_required(VERSION 3.30)

project(
    HSM
    VERSION 0.1
    LANGUAGES C
)

add_subdirectory(test)
add_subdirectory(examples/turnstile)
add_library(hsm src/hsm.c)
# Comment out the next line to disable logging in state machine
target_compile_definitions(hsm PRIVATE HSM_LOGGING)