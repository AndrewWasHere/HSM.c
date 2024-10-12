#include <stddef.h>

#include "hsm.h"

static HSM_state_t * root_state(HSM_state_t * state) {
    while (state->parent != NULL) {
        state = state->parent;
    }

    return state;
}

static HSM_state_t * active_state(HSM_state_t * machine) {
    HSM_state_t * state = root_state(machine);
    while (state->active_substate != NULL) {
        state = state->active_substate;
    }

    return state;
}

static HSM_state_t * common_parent(HSM_state_t * left, HSM_state_t * right) {
    while (left != NULL) {
        HSM_state_t * r = right;
        while (r != NULL) {
            if (r == left) {
                // Common parent found.
                return r;
            }

            r = r->parent;
        }
        left = left->parent;
    }

    // No common parent.
    return NULL;
}

void transition_to_state(HSM_state_t * machine, HSM_state_t * state) {
    HSM_state_t * s = active_state(machine);
    HSM_state_t * cp = common_parent(s, state);

    assert(cp != NULL);

    // Call `on_exit()` from active state up to, but not including common
    // parent.
    while (s != cp) {
        if (s->on_exit != NULL) {
            s->on_exit(s);
        }
        s = s->parent;
    }

    // Update active state pointers from common parent to `state`.
    s->active_substate = NULL;
    while (s != cp) {
        HSM_state_t * p = s->parent;
        p->active_substate = s;
        s = s->parent;
    }

    // Call `on_entry()` for common parent's active state to `state`.
    s = s->active_substate;
    while (s != NULL) {
        if (s->on_entry != NULL) {
            s->on_entry(s);
        }
        s = s->active_substate;
    }

    if (s->on_initialize != NULL) {
        s->on_initialize(s);
    }
}

void transition_to_shallow_history(HSM_state_t * machine, HSM_state_t * state) {
    HSM_state_t * dest = (state->active_substate == NULL) ? 
        state : state->active_substate;
    transition_to_state(machine, dest);
}

void transition_to_deep_history(HSM_state_t * machine, HSM_state_t * state) {
    HSM_state_t * dest = state;
    while (dest->active_substate != NULL) {
        dest = dest->active_substate;
    }
    transition_to_state(machine, dest);
}

void handle_event(HSM_state_t * machine, HSM_event_t * event) {
    bool handled = false;
    HSM_state_t * s = active_state(machine);
    while (s != NULL && !(handled = s->on_event(s, event))) {
        s = s->parent;
    }

    assert(handled);
}

char const * active_state_name(HSM_state_t * machine) {
    HSM_state_t * state = active_state(machine);
    return state->name;
}
