#include <unity.h>

#include "hsm.h"

HSM_state_t * common_parent(HSM_state_t * left, HSM_state_t * right);

/**
 * @brief test common_parent()
 * 
 * Test with the following state machine:
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
void test_common_parent(void) {
    HSM_state_t A = {
        .name = "A",
        .parent = NULL,
    };
    HSM_state_t B = {
        .name = "B",
        .parent = &A,
    };
    HSM_state_t C = {
        .name = "C",
        .parent = &A,
    };
    HSM_state_t D = {
        .name = "D",
        .parent = &C,
    };
    HSM_state_t E = {
        .name = "E",
        .parent = &C,
    };

    // Common parent of any state and itself is itself.
    TEST_ASSERT_EQUAL_PTR(&A, common_parent(&A, &A));
    TEST_ASSERT_EQUAL_PTR(&B, common_parent(&B, &B));
    TEST_ASSERT_EQUAL_PTR(&C, common_parent(&C, &C));
    TEST_ASSERT_EQUAL_PTR(&D, common_parent(&D, &D));
    TEST_ASSERT_EQUAL_PTR(&E, common_parent(&E, &E));

    // Common parent of A and any other state is A.
    TEST_ASSERT_EQUAL_PTR(&A, common_parent(&A, &B));
    TEST_ASSERT_EQUAL_PTR(&A, common_parent(&E, &A));

    // Common parent of substates of A is A.
    TEST_ASSERT_EQUAL_PTR(&A, common_parent(&B, &C));
    TEST_ASSERT_EQUAL_PTR(&A, common_parent(&D, &B));

    // Common parent of C and its substates is C.
    TEST_ASSERT_EQUAL_PTR(&C, common_parent(&C, &D));
    TEST_ASSERT_EQUAL_PTR(&C, common_parent(&E, &C));

    // Common parent of substates of C is C.
    TEST_ASSERT_EQUAL_PTR(&C, common_parent(&D, &E));
}