#include <stddef.h>
#include <unity.h>

#include "hsm.h"

HSM_state_t * active_state(HSM_state_t * machine);

/**
 * Test active_state() with a no-state machine.
 * 
 * Test with the following state machine:
 * 
 * ```plantuml
 * @startuml
 * state A
 * @enduml
 */
void test_active_state_solo(void) {
    HSM_state_t A = {
        .name = "A",
        .parent = NULL,
    };

    TEST_ASSERT_EQUAL_PTR(&A, active_state(&A));
}

/**
 * Test active_state() with a heirarchical machine
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
void test_active_state_hierarchy(void) {
    HSM_state_t A = { .name = "A", .parent = NULL, };
    HSM_state_t B = { .name = "B", .parent = &A, };
    HSM_state_t C = { .name = "C", .parent = &A, };
    HSM_state_t D = { .name = "D", .parent = &C, };
    HSM_state_t E = { .name = "E", .parent = &C, };

    HSM_error_t err = transition_to_state(&A, &B);
    TEST_ASSERT_TRUE(err == HSM_ERROR_OK);
}