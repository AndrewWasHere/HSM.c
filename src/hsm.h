#pragma once

typedef void (* state_handler_t)(void * self);
typedef void (* event_handler_t)(void * self, void * event);


typedef struct HSM_state_s {
    char const * name;
    struct state_s * parent;
    struct state_s * active_substate;

    state_handler_t on_entry;
    state_handler_t on_exit;
    state_handler_t on_initialize;
    event_handler_t on_event;
} HSM_state_t;

typedef struct HSM_event_s {
} HSM_event_t;

void transition_to_state(HSM_state_t * machine, HSM_state_t * state);
void transition_to_shallow_history(HSM_state_t * machine, HSM_state_t * state);
void transition_to_deep_history(HSM_state_t * machine, HSM_state_t * state);
void handle_event(HSM_state_t * machine, HSM_event_t * event);
char const * active_state_name(HSM_state_t * machine);
