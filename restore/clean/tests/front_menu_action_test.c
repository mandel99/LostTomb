#include "pharaoh/ui/front_menu_action.h"

#include <stdio.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "line %d\n", __LINE__); \
    return 1; } } while (0)

typedef struct TestOperations { int32_t calls[5]; } TestOperations;

static void record(void *data, int32_t action)
{
    ++((TestOperations *)data)->calls[action];
}

static void open_player(void *data) { record(data, 0); }
static void open_support(void *data) { record(data, 1); }
static void open_maps(void *data) { record(data, 2); }
static void open_scores(void *data) { record(data, 3); }
static void request_exit(void *data) { record(data, 4); }

int main(void)
{
    PharaohGenericButtonInputState input = { 0 };
    uint8_t action_active = 1, save_selection_mode = 1, window = 0;
    uint32_t menu_action_reset = 0, continue_pending = 1;
    uint32_t campaign_mode = 0, scenario_changed = 0;
    uint32_t field_a = 7, field_b = 9;
    PharaohFrontMenuActionState state = { 0 };
    TestOperations test_operations = { { 0 } };
    const PharaohFrontMenuActionOperations operations = {
        &test_operations, open_player, open_support, open_maps,
        open_scores, request_exit
    };
    PharaohFrontMenuActionContext context = { &state, &operations };

    state.input_state = &input;
    state.action_active = &action_active;
    state.menu_action_reset = &menu_action_reset;
    state.continue_campaign_pending = &continue_pending;
    state.campaign_flow_mode = &campaign_mode;
    state.scenario_menu_changed = &scenario_changed;
    state.save_selection_mode = &save_selection_mode;
    state.new_campaign_field_a = &field_a;
    state.new_campaign_field_b = &field_b;
    state.current_window_id = &window;

    input.selected_primary_parameter = 0;
    pharaoh_front_menu_dispatch_action(&context);
    CHECK(test_operations.calls[0] == 1 && action_active == 0 &&
        menu_action_reset == 1 && continue_pending == 0);
    input.selected_primary_parameter = 1;
    pharaoh_front_menu_dispatch_action(&context);
    CHECK(test_operations.calls[1] == 1 && menu_action_reset == 1);
    input.selected_primary_parameter = 2;
    pharaoh_front_menu_dispatch_action(&context);
    CHECK(test_operations.calls[2] == 1 && campaign_mode == 2 &&
        scenario_changed == 1 && save_selection_mode == 0 &&
        continue_pending == 0 && field_a == 0 && field_b == 3);
    input.selected_primary_parameter = 3;
    pharaoh_front_menu_dispatch_action(&context);
    CHECK(test_operations.calls[3] == 1 && window == 0x53);
    input.selected_primary_parameter = 4;
    pharaoh_front_menu_dispatch_action(&context);
    CHECK(test_operations.calls[4] == 1);
    return 0;
}
