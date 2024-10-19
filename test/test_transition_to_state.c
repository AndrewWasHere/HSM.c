#include <stddef.h>
#include <unity.h>

#include "hsm.h"

/*
    State machine for these tests:

    @startuml
    state TransitionTestsMachine {
        state Start
        state Dest

        Start : entry / set entered
        Start : exit / set exited
        Dest : entry / set entered
        Dest : exit / set exited
    }
    state Unreachable

    TransitionTestsMachine : entry / set entered
    TransitionTestsMachine : exit / set exited
    Unreachable : entry / set entered
    Unreachable : exit / set exited
*/

typedef struct data_s {
    bool entered;
    bool exited;
} data_t;

static data_t machine_data;
static data_t start_data;
static data_t dest_data;
static data_t unreachable_data;

void on_entry(HSM_state_t * state) {
    data_t * data = (data_t *) state->data;
    data->entered = true;
}

void on_exit(HSM_state_t * state) {
    data_t * data = (data_t *) state->data;
    data->exited = true;
}

void reset_state(HSM_state_t * state) {
    data_t * data = (data_t *) state->data;
    data->entered = data->exited = false;
}

HSM_state_t build_state(char const * name, HSM_state_t * parent, void * data) {
    HSM_state_t s = {
        .name = name,
        .parent = parent,
        .on_entry = on_entry,
        .on_exit = on_exit,
        .data = data,
    };

    return s;
}

/**
 * @brief Transition from `start` to `dest` should succeed.
 */
void test_transition_to_reachable(void) {
    // Set up
    HSM_state_t machine = build_state("machine", NULL, &machine_data);
    HSM_state_t start = build_state("start", &machine, &start_data);
    HSM_state_t dest = build_state("dest", &machine, &dest_data);
    HSM_state_t unreachable = build_state("unreachable", NULL, &unreachable_data);

    HSM_error_t res = transition_to_state(&machine, &start);
    TEST_ASSERT_EQUAL_INT(HSM_ERROR_OK, res);
    reset_state(&machine);
    reset_state(&start);
    reset_state(&dest);
    reset_state(&unreachable);

    // Execute
    res = transition_to_state(&machine, &dest);

    // Verify
    TEST_ASSERT_EQUAL_INT(HSM_ERROR_OK, res);
    TEST_ASSERT_EQUAL_INT(&dest, active_state(&machine));
    TEST_ASSERT_FALSE(machine_data.entered);
    TEST_ASSERT_FALSE(machine_data.exited);
    TEST_ASSERT_FALSE(start_data.entered);
    TEST_ASSERT_TRUE(start_data.exited);
    TEST_ASSERT_TRUE(dest_data.entered);
    TEST_ASSERT_FALSE(dest_data.exited);
    TEST_ASSERT_FALSE(unreachable_data.entered);
    TEST_ASSERT_FALSE(unreachable_data.exited);
}

/**
 * @brief Transition from Start to Unreachable should fail.
 */
void test_transition_to_unreachable(void) {
    // Set up
    HSM_state_t machine = build_state("machine", NULL, &machine_data);
    HSM_state_t start = build_state("start", &machine, &start_data);
    HSM_state_t dest = build_state("dest", &machine, &dest_data);
    HSM_state_t unreachable = build_state("unreachable", NULL, &unreachable_data);

    HSM_error_t res = transition_to_state(&machine, &start);
    TEST_ASSERT_EQUAL_INT(HSM_ERROR_OK, res);
    reset_state(&machine);
    reset_state(&start);
    reset_state(&dest);
    reset_state(&unreachable);

    // Execute
    res = transition_to_state(&machine, &unreachable);

    // Verify
    TEST_ASSERT_EQUAL_INT(HSM_ERROR_STATE_NOT_IN_MACHINE, res);
    TEST_ASSERT_EQUAL_INT(&start, active_state(&machine));
    TEST_ASSERT_FALSE(machine_data.entered);
    TEST_ASSERT_FALSE(machine_data.exited);
    TEST_ASSERT_FALSE(start_data.entered);
    TEST_ASSERT_FALSE(start_data.exited);
    TEST_ASSERT_FALSE(dest_data.entered);
    TEST_ASSERT_FALSE(dest_data.exited);
    TEST_ASSERT_FALSE(unreachable_data.entered);
    TEST_ASSERT_FALSE(unreachable_data.exited);
}