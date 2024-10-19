#pragma once

#include <stdbool.h>

struct HSM_state_s;
struct HSM_event_s;

typedef enum {
    HSM_ERROR_OK,
    HSM_ERROR_STATE_NOT_IN_MACHINE,
    HSM_ERROR_EVENT_NOT_HANDLED,
} HSM_error_t;

typedef void (* state_handler_t)(struct HSM_state_s * state);
typedef bool (* event_handler_t)(struct HSM_state_s * state, struct HSM_event_s * event);

typedef struct HSM_state_s {
    char const * name;
    struct HSM_state_s * parent;
    struct HSM_state_s * active_substate;

    state_handler_t on_entry;
    state_handler_t on_exit;
    state_handler_t on_initialize;
    event_handler_t on_event;

    void * data;
} HSM_state_t;

typedef struct HSM_event_s {
    unsigned int id;
    char const * name;
    void * data;
} HSM_event_t;

HSM_error_t transition_to_state(HSM_state_t * machine, HSM_state_t * state);
HSM_error_t transition_to_shallow_history(HSM_state_t * machine, HSM_state_t * state);
HSM_error_t transition_to_deep_history(HSM_state_t * machine, HSM_state_t * state);
HSM_error_t handle_event(HSM_state_t * machine, HSM_event_t * event);
HSM_state_t * active_state(HSM_state_t * state);
char const * active_state_name(HSM_state_t * machine);
