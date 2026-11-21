#include "pharaoh/building/construction_warnings.h"

enum { RESOURCE_NONE = 0 };

typedef struct WarningRule {
    int32_t tool, industry_input, stored_product, stored_input;
    int32_t produced_input, imported_input;
    int32_t missing_message, producer_message;
    int32_t import_disabled_message, no_route_message;
} WarningRule;

static void check_warning(PharaohConstructionWarningState *state,
    int32_t tool, const WarningRule *rule)
{
    int16_t trade;
    if (state->editor_active == 1u || tool != rule->tool ||
        state->industry_active[rule->industry_input] > 0) return;
    if (state->stored[rule->stored_product] > 0 ||
        (rule->stored_input != RESOURCE_NONE &&
         state->stored[rule->stored_input] > 0)) return;
    state->show_warning(state->user_data, rule->missing_message, 0, 0);
    if (state->can_produce(state->user_data, rule->produced_input) != 0) {
        state->show_warning(state->user_data, rule->producer_message, 0, 0);
        return;
    }
    if (state->can_import(state->user_data, rule->imported_input) != 0) {
        trade = state->trade_status[rule->imported_input];
        if (trade == 1 || trade == 3) return;
        state->show_warning(state->user_data,
            rule->import_disabled_message, 0, 0);
        return;
    }
    state->show_warning(state->user_data, rule->no_route_message, 0, 0);
}

#define WARNING_FUNCTION(name, ...) \
    void pharaoh_warning_##name(PharaohConstructionWarningState *state, \
        int32_t tool) { static const WarningRule rule = { __VA_ARGS__ }; \
        check_warning(state, tool, &rule); }

WARNING_FUNCTION(brewery, 0x6e, 14, 15, 14, 14, 14, 0x1b, 0x22, 0x20, 0x1f)
WARNING_FUNCTION(weaver, 0x6f, 16, 17, 16, 16, 16, 0x1c, 0x23, 0x20, 0x1f)
WARNING_FUNCTION(jeweler, 0x71, 18, 19, 18, 18, 18, 0x1e, 0x25, 0x20, 0x1f)
WARNING_FUNCTION(potter, 0x72, 11, 13, 11, 11, 11, 0x1d, 0x24, 0x20, 0x1f)
WARNING_FUNCTION(papyrus, 0xcb, 22, 23, 22, 22, 22, 0x50, 0x51, 0x20, 0x1f)
WARNING_FUNCTION(paint, 0xe9, 32, 33, 32, 32, 32, 0x105, 0x106, 0x107, 0x108)
WARNING_FUNCTION(cattle, 0xc2, 1, 9, RESOURCE_NONE, 1, 9, 0x52, 0x53, 0x20, 0x1f)
WARNING_FUNCTION(carpenters, 0xb1, 20, 20, RESOURCE_NONE, 20, 20, 0x8f, 0x90, 0x20, 0x1f)

#undef WARNING_FUNCTION
