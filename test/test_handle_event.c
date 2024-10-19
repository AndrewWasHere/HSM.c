#include <stdbool.h>
#include <unity.h>

#include "hsm.h"

enum event_enum_s {
    UNHANDLED,
    HANDLED
};

typedef struct handled_data_s {
    bool handled;
} handled_data_t;

bool machine_on_event(HSM_state_t * machine, HSM_event_t * event) {
    handled_data_t * data = (handled_data_t *) machine->data;
    bool handled = false;

    switch (event->id) {
        case HANDLED:
            data->handled = true;
            handled = true;
            break;
    }

    return handled;
}

/*

*/
void test_handle_event_unhandled_event(void) {
    // Set up
    handled_data_t data;
    HSM_state_t machine = { .name = "machine", .parent = NULL, .on_event = machine_on_event, .data = &data };
    HSM_event_t event = { .id = UNHANDLED, .name = "event that should not be handled" };

    // Execute
    HSM_error_t res = handle_event(&machine, &event);

    // Verify
    TEST_ASSERT_EQUAL_INT(HSM_ERROR_EVENT_NOT_HANDLED, res);
    TEST_ASSERT_FALSE(data.handled);
}

/*
*/
void test_handle_event_handled_event(void) {
    // Set up
    handled_data_t data;
    HSM_state_t machine = { .name = "machine", .parent = NULL, .on_event = machine_on_event, .data = &data };
    HSM_event_t event = { .id = HANDLED, .name = "event that should be handled" };

    // Execute
    HSM_error_t res = handle_event(&machine, &event);

    // Verify
    TEST_ASSERT_EQUAL_INT(HSM_ERROR_OK, res);
    TEST_ASSERT_TRUE(data.handled);
}

bool l1_on_event(HSM_state_t * machine, HSM_event_t * event) {
    handled_data_t * data = (handled_data_t *) machine->data;
    bool handled = false;

    switch (event->id) {
        case HANDLED:
            data->handled = true;
            handled = true;
            break;
    }

    return handled;
}

bool l2_on_event(HSM_state_t * machine, HSM_event_t * event) {
    return false;
}

/*
*/
void test_handle_event_parent_handles_event(void) {
    // Set up
    handled_data_t machine_data;
    handled_data_t l1_data;
    handled_data_t l2_data;
    HSM_state_t machine = { .name = "machine", .parent = NULL, .on_event = machine_on_event, .data = &machine_data };
    HSM_state_t l1 = { .name = "l1", .parent = &machine, .on_event = l1_on_event, .data = &l1_data };
    HSM_state_t l2 = { .name = "l2", .parent = &l1, .on_event = l2_on_event, .data = &l2_data };
    HSM_event_t event = { .id = HANDLED, .name = "event that should be handled" };
    
    HSM_error_t res = transition_to_state(&machine, &l2);
    TEST_ASSERT_EQUAL_INT(HSM_ERROR_OK, res);

    // Execute
    res = handle_event(&machine, &event);

    // Verify
    TEST_ASSERT_EQUAL_INT(HSM_ERROR_OK, res);
    TEST_ASSERT_FALSE(machine_data.handled);
    TEST_ASSERT_TRUE(l1_data.handled);
    TEST_ASSERT_FALSE(l2_data.handled);
}