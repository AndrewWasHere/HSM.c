add_executable(hsm_test)
target_link_libraries(hsm_test hsm)
target_include_directories(
    hsm_test PUBLIC 
    "include" 
    "../src" 
    "Unity/src"
)
target_sources(
    hsm_test 
    PUBLIC 
    "Unity/src/unity.c"
    "test_main.c"
    "test_active_state.c"
    "test_common_parent.c"
    "test_handle_event.c"
    "test_root_state.c"
    "test_transition_to_deep_history.c"
    "test_transition_to_shallow_history.c"
    "test_transition_to_state.c"
    "test_transition_to_state_with_initialize.c"
)