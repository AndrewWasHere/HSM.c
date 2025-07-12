#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "hsm.h"
#include "hsm_log.h"

HSM_state_t * root_state(HSM_state_t * state) {
    if (state == NULL) {
        HSM_LOG("ERROR: root state requested for NULL.");
        return NULL;
    }

    while (state->parent != NULL) {
        state = state->parent;
    }

    return state;
}

HSM_state_t * active_state(HSM_state_t * machine) {
    if (machine == NULL) {
        HSM_LOG("ERROR: active state requested for NULL.")
        return NULL;
    }

    HSM_state_t * state = root_state(machine);
    while (state->active_substate != NULL) {
        state = state->active_substate;
    }

    return state;
}

HSM_state_t * common_parent(HSM_state_t * left, HSM_state_t * right) {
    if (left == NULL || right == NULL) {
        HSM_LOG("ERROR: common parent requested for NULL.");
        return NULL;
    }

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

HSM_error_t transition_to_state(HSM_state_t * machine, HSM_state_t * state) {
    if (machine == NULL || state == NULL) {
        HSM_LOG("ERROR: transition to state requested for NULL.");
        return HSM_ERROR_STATE_NOT_IN_MACHINE;
    }

    HSM_state_t * s = active_state(machine);
    HSM_state_t * cp = common_parent(s, state);

    if (cp == NULL) {
        HSM_LOG(
            "ERROR: cannot transition to state %s. Not in machine %s.", 
            state->name, 
            machine->name
        );
        return HSM_ERROR_STATE_NOT_IN_MACHINE;
    }

    HSM_LOG("Transitioning %s to %s", machine->name, state->name);

    // Call `on_exit()` from active state up to, but not including, common
    // parent.
    while (s != cp) {
        if (s->on_exit != NULL) {
            s->on_exit(s);
        }
        s = s->parent;
    }

    // Update active state pointers from common parent to `state`.
    state->active_substate = NULL;
    s = state;
    while (s != cp) {
        HSM_state_t * p = s->parent;
        p->active_substate = s;
        s = s->parent;
    }

    // Call `on_entry()` for common parent's active state to `state`.
    s = cp->active_substate;
    while (s != NULL) {
        if (s->on_entry != NULL) {
            s->on_entry(s);
        }
        s = s->active_substate;
    }

    if (state->on_initialize != NULL) {
        state->on_initialize(state);
    }

    return HSM_ERROR_OK;
}

HSM_error_t transition_to_shallow_history(HSM_state_t * machine, HSM_state_t * state) {
    if (machine == NULL || state == NULL) {
        HSM_LOG("ERROR: transition to shallow history requested for NULL.");
        return HSM_ERROR_STATE_NOT_IN_MACHINE;
    }

    HSM_LOG("Transitioning %s to shallow history of state %s", machine->name, state->name);

    HSM_state_t * dest = (state->active_substate == NULL) ? 
        state : state->active_substate;
    return transition_to_state(machine, dest);
}

HSM_error_t transition_to_deep_history(HSM_state_t * machine, HSM_state_t * state) {
    if (machine == NULL || state == NULL) {
        HSM_LOG("ERROR: transition to deep history requested for NULL.");
        return HSM_ERROR_STATE_NOT_IN_MACHINE;
    }

    HSM_LOG("Transitioning %s to deep history of state %s", machine->name, state->name);

    HSM_state_t * dest = state;
    while (dest->active_substate != NULL) {
        dest = dest->active_substate;
    }
    return transition_to_state(machine, dest);
}

HSM_error_t handle_event(HSM_state_t * machine, HSM_event_t * event) {
    if (machine == NULL || event == NULL) {
        if (machine == NULL) {
            HSM_LOG("ERROR: handle event requested for NULL state.");
        }
        if (event == NULL) {
            HSM_LOG("ERROR: handle event requested with NULL event.");
        }
        return HSM_ERROR_EVENT_NOT_HANDLED;
    }

    bool handled = false;
    HSM_state_t * s = active_state(machine);
    while (s != NULL && !handled) {
        handled = s->on_event(s, event);
        s = s->parent;
    }

    if (!handled) {
        HSM_LOG(
            "ERROR: Event %s not handled by state machine %s", 
            event->name, 
            machine->name
        );
        return HSM_ERROR_EVENT_NOT_HANDLED;
    }
    return HSM_ERROR_OK;
}

char const * active_state_name(HSM_state_t * machine) {
    if (machine == NULL) {
        return "NULL";
    }

    HSM_state_t * state = active_state(machine);
    return state->name;
}
