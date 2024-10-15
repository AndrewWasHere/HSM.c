add_executable(hsm_test)
target_link_libraries(hsm_test hsm)
target_include_directories(hsm_test PUBLIC "../src" "Unity/src")
target_sources(
    hsm_test 
    PUBLIC 
    "Unity/src/unity.c"
    "test_main.c"
)