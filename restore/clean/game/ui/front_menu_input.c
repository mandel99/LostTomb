#include "pharaoh/ui/front_menu_input.h"

enum { FRONT_MENU_BUTTON_COUNT = PHARAOH_MENU_BUTTON_COUNT };

void pharaoh_front_menu_handle_input(
    PharaohFrontMenuInputContext *context)
{
    PharaohMenuRect bounds[FRONT_MENU_BUTTON_COUNT];
    int32_t focus;

    for (int32_t index = 0; index < FRONT_MENU_BUTTON_COUNT; ++index) {
        context->buttons[index].left_action =
            pharaoh_front_menu_dispatch_action;
        context->buttons[index].right_action =
            pharaoh_front_menu_ignore_secondary_action;
        context->buttons[index].trigger =
            PHARAOH_GENERIC_BUTTON_TRIGGER_PRESS;
        context->buttons[index].primary_parameter = index;
        context->buttons[index].secondary_parameter = 0;
        bounds[index] = context->buttons[index].bounds;
    }

    context->action_context->state->input_state = context->input_state;
    context->input_state->action_context = context->action_context;

    focus = pharaoh_generic_buttons_get_focus(
        context->input_state->origin_x, context->input_state->origin_y,
        bounds, FRONT_MENU_BUTTON_COUNT,
        context->input_state->cursor_x, context->input_state->cursor_y,
        context->focus_state);
    *context->focus_button_id = pharaoh_ui_layout_buttons(
        context->layout_state, context->layout_operations,
        context->input_state->origin_x, context->input_state->origin_y,
        context->buttons, FRONT_MENU_BUTTON_COUNT, focus);
    (void)pharaoh_generic_buttons_handle_mouse(context->input_state,
        context->buttons, FRONT_MENU_BUTTON_COUNT);
}
