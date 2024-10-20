#include <stdbool.h>
#include <unity.h>

#include "hsm.h"

/**
 * Event IDs
 */
enum event_enum_s {
    UNHANDLED,
    HANDLED
};

/**
 * State data container.
 */
typedef struct handled_data_s {
    bool handled;
} handled_data_t;

/**
 * Machine event handler.
 */
bool machine_on_event(HSM_state_t * machine, HSM_event_t * event) {
    handled_data_t * data = (handled_data_t *) machine->data;
    bool handled = false;

    switch (event->id) {
        case HANDLED:
            data->handled = true;
            handled = true;
            break;
        // UNHANDLED deliberately missing for test. In real code, the machine
        // state should handle all events as a catchall.
    }

    return handled;
}

/**
 * State L1 event handler.
 */
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

/**
 * State L2 event handler.
 * 
 * Goes nowhere, does nothing.
 */
bool l2_on_event(HSM_state_t * machine, HSM_event_t * event) {
    return false;
}

/**
 * Test outcome of an unhandled event.
 * 
 * Note: You should not do this in practice. The state machine should handle all
 * possible events to prevent unexpected behavior. This test just guarantees that
 * the expected behavior will occur.
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

/**
 *  Test handling an event.
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

/**
 * Test that events get passed to a parent state if the child state does not
 * handle the event.
 * 
 * State machine used for this test:
 * 
 * ```plantuml
 * @startuml
 * state machine {
 *   state L1 {
 *     state L2
 *   }
 * }
 * @enduml
 */
void test_handle_event_parent_handles_event(void) {
    // Set up
    handled_data_t machine_data;
    handled_data_t L1_data;
    handled_data_t L2_data;
    HSM_state_t machine = { .name = "machine", .parent = NULL, .on_event = machine_on_event, .data = &machine_data };
    HSM_state_t L1 = { .name = "L1", .parent = &machine, .on_event = l1_on_event, .data = &L1_data };
    HSM_state_t L2 = { .name = "L2", .parent = &L1, .on_event = l2_on_event, .data = &L2_data };
    HSM_event_t event = { .id = HANDLED, .name = "event that should be handled" };
    
    HSM_error_t res = transition_to_state(&machine, &L2);
    TEST_ASSERT_EQUAL_INT(HSM_ERROR_OK, res);

    // Execute
    res = handle_event(&machine, &event);

    // Verify
    TEST_ASSERT_EQUAL_INT(HSM_ERROR_OK, res);
    TEST_ASSERT_FALSE(machine_data.handled);
    TEST_ASSERT_TRUE(L1_data.handled);
    TEST_ASSERT_FALSE(L2_data.handled);
}