#include <unity.h>

#include "hsm.h"

/*
State machine for these tests:

    @startuml
    state TestShallowHistoryMachine {
        state start
        state L1 {
            state L2 {
                state L3
            }
        }
    }
    @enduml
*/

/**
 * Shallow history transition to a state that has not been entered before is the
 * transitioned-to state. 
 */
void test_transition_to_shallow_history_unentered_state(void) {
    // Set up
    HSM_state_t machine = { .name = "machine", .parent = NULL };
    HSM_state_t start = { .name = "start", .parent = &machine };
    HSM_state_t L1 = { .name = "L1", .parent = &machine };
    HSM_state_t L2 = { .name = "L2", .parent = &L1 };
    HSM_state_t L3 = { .name = "L3", .parent = &L2 };

    HSM_error_t res = transition_to_state(&machine, &start);
    TEST_ASSERT_EQUAL_INT(HSM_ERROR_OK, res);

    // Execute
    res = transition_to_shallow_history(&machine, &L1);

    // Verify
    TEST_ASSERT_EQUAL_INT(HSM_ERROR_OK, res);
    TEST_ASSERT_EQUAL_INT(&L1, active_state(&machine));
}

/**
 * Shallow history transition to a state whose substate's child has been entered
 * before is the substate.
 */
void test_transition_to_shallow_history_previously_entered_state(void) {
    // Set up
    HSM_state_t machine = { .name = "machine", .parent = NULL };
    HSM_state_t start = { .name = "start", .parent = &machine };
    HSM_state_t L1 = { .name = "L1", .parent = &machine };
    HSM_state_t L2 = { .name = "L2", .parent = &L1 };
    HSM_state_t L3 = { .name = "L3", .parent = &L2 };

    HSM_error_t res = transition_to_state(&machine, &L3);
    TEST_ASSERT_EQUAL_INT(HSM_ERROR_OK, res);
    res = transition_to_state(&machine, &start);
    TEST_ASSERT_EQUAL_INT(HSM_ERROR_OK, res);

    // Execute
    res = transition_to_shallow_history(&machine, &L1);

    // Verify
    TEST_ASSERT_EQUAL_INT(HSM_ERROR_OK, res);
    TEST_ASSERT_EQUAL_INT(&L2, active_state(&machine));
}