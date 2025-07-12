#include <stdio.h>
#include "hsm.h"

/* Forward declarations for visibility by state structures. */
void turnstile_on_initialize(HSM_state_t * machine);
bool turnstile_on_event(HSM_state_t * state, HSM_event_t * event);
void locked_on_entry(HSM_state_t * machine);
bool locked_on_event(HSM_state_t * state, HSM_event_t * event);
void unlocked_on_entry(HSM_state_t * machine);
bool unlocked_on_event(HSM_state_t * state, HSM_event_t * event);

/**
 * Events
 */
enum {
    INSERT_TOKEN_EVENT,
    ENTER_EVENT,
};

char const * event_name(unsigned int event) {
    switch (event) {
    case INSERT_TOKEN_EVENT:
        return "token inserted";
    case ENTER_EVENT:
        return "turnstile entered";
    default:
        return "unknown";
    }
    return NULL;
}

/**
 * State machine is the parent of all states.
 * It is the only HSM_state_t to not have a parent.
 */
HSM_state_t turnstile_state_machine = { 
    .name = "turnstile state machine", 
    .parent = NULL,
    .on_initialize = turnstile_on_initialize,
    .on_event = turnstile_on_event,
};

/**
 * Locked state.
 */
HSM_state_t locked_state = {
    .name = "locked",
    .parent = &turnstile_state_machine,
    .on_entry = locked_on_entry,
    .on_event = locked_on_event,
};

/**
 * Unlocked state.
 */
HSM_state_t unlocked_state = {
    .name = "unlocked",
    .parent = &turnstile_state_machine,
    .on_entry = unlocked_on_entry,
    .on_event = unlocked_on_event,
};

/**
 * Turnstile initialization.
 */
void turnstile_on_initialize(HSM_state_t * machine) {
    printf("%s on initialize() called\n", machine->name);
    transition_to_state(&turnstile_state_machine, &locked_state);
}

bool turnstile_on_event(HSM_state_t * state, HSM_event_t * event) {
    printf("%s state handling %s event\n", state->name, event->name);
    return true;
}

/**
 * Locked state on entry handler.
 */
void locked_on_entry(HSM_state_t * state) {
    printf("%s state on entry: latching turnstile\n", state->name);
}

/**
 * Locked state event handler.
 */
bool locked_on_event(HSM_state_t * state, HSM_event_t * event) {
    printf("%s state handling %s event.\n", state->name, event->name);

    switch (event->id) {
    case INSERT_TOKEN_EVENT:
        printf("keeping token\n");
        transition_to_state(&turnstile_state_machine, &unlocked_state);
        return true;
    case ENTER_EVENT:
        printf("entry disallowed\n");
        return true;
    }

    return false;
}

/**
 * Unlocked state on entry handler.
 */
void unlocked_on_entry(HSM_state_t * state) {
    printf("%s state on entry: unlatching turnstile\n", state->name);
}

/**
 * Unlocked state event handler.
 */
bool unlocked_on_event(HSM_state_t * state, HSM_event_t * event) {
    printf("%s state handling %s event.\n", state->name, event->name);

    switch (event->id) {
    case INSERT_TOKEN_EVENT:
        printf("returning token\n");
        return true;
    case ENTER_EVENT:
        printf("entry allowed\n");
        transition_to_state(&turnstile_state_machine, &locked_state);
        return true;
    }

    return false;
}

int display_help() {
    printf("1) Insert token\n");
    printf("2) Enter turnstile\n");
    printf("3) Something unexpected\n");
    printf("?) Print this help\n");
}
int main() {
    display_help();

    HSM_event_t token_event = { .id = INSERT_TOKEN_EVENT, .name = event_name(INSERT_TOKEN_EVENT) };
    HSM_event_t enter_event = { .id = ENTER_EVENT, .name = event_name(ENTER_EVENT) };
    HSM_event_t unknown_event = { .id = ENTER_EVENT + 1, .name = event_name(ENTER_EVENT + 1) };

    // Initialize the state machine.
    transition_to_state(&turnstile_state_machine, &turnstile_state_machine);

    // Generate events.
    bool show_prompt = true;
    while (true) {
        if (show_prompt)
            printf("> ");

        int cmd = getchar();
        switch (cmd) {
            case '1':
                handle_event(&turnstile_state_machine, &token_event);
                show_prompt = false;
                break;
            case '2':
                handle_event(&turnstile_state_machine, &enter_event);
                show_prompt = false;
                break;
            case '3':
                handle_event(&turnstile_state_machine, &unknown_event);
                show_prompt = false;
                break;
            case '\n':
                // Ignore the newline.
                show_prompt = true;
                break;
            default:
                display_help();
                show_prompt = false;
                break;
        }
    }
    return 0;
}