#include <unity.h>
#include "hsm.h"

#include "test_active_state.h"
#include "test_common_parent.h"
#include "test_handle_event.h"
#include "test_root_state.h"
#include "test_transition_to_deep_history.h"
#include "test_transition_to_shallow_history.h"
#include "test_transition_to_state.h"
#include "test_transition_to_state_with_initialize.h"

void setUp(void) {}
void tearDown(void) {}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_active_state_hierarchy);
    RUN_TEST(test_active_state_solo);
    RUN_TEST(test_common_parent);
    RUN_TEST(test_handle_event_unhandled_event);
    RUN_TEST(test_handle_event_handled_event);
    RUN_TEST(test_handle_event_parent_handles_event);
    RUN_TEST(test_handle_event_state_missing_on_event);
    RUN_TEST(test_root_state);
    RUN_TEST(test_transition_to_deep_history_unentered_state);
    RUN_TEST(test_transition_to_deep_history_previously_entered_state);
    RUN_TEST(test_transition_to_reachable);
    RUN_TEST(test_transition_to_unreachable);
    RUN_TEST(test_transition_to_shallow_history_unentered_state);
    RUN_TEST(test_transition_to_shallow_history_previously_entered_state);
    RUN_TEST(test_transition_to_reachable_with_initialize);
    RUN_TEST(test_transition_to_unreachable_with_initialize);
    return UNITY_END();
}
