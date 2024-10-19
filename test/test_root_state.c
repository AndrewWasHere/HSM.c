#include <stddef.h>
#include <unity.h>

#include <hsm.h>

HSM_state_t * root_state(HSM_state_t * state);

/**
 * @brief Test root_state()
 * 
 * Tests with the following state machine:
 * 
 * ```plantuml
 * @startuml
 * state A {
 *   state B
 *   state C {
 *     state D
 *     state E
 *   }
 * }
 * @enduml
 * ```
 */
void test_root_state(void) {
    HSM_state_t A = {
        .name = "A",
        .parent = NULL
    };
    HSM_state_t B = {
        .name = "B",
        .parent = &A
    };
    HSM_state_t C = {
        .name = "C",
        .parent = &A
    };
    HSM_state_t D = {
        .name = "D",
        .parent = &C
    };
    HSM_state_t E = {
        .name = "E",
        .parent = &C
    };

    TEST_ASSERT_EQUAL_PTR(&A, root_state(&A));
    TEST_ASSERT_EQUAL_PTR(&A, root_state(&B));
    TEST_ASSERT_EQUAL_PTR(&A, root_state(&C));
    TEST_ASSERT_EQUAL_PTR(&A, root_state(&D));
    TEST_ASSERT_EQUAL_PTR(&A, root_state(&E));
}