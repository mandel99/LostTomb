#include "pharaoh/ui/building_info_actions_live.h"
#include "pharaoh/ui/menu_modal_buttons.h"
#include "pharaoh/runtime/platform_measure_speed.h"

_Static_assert(sizeof(PharaohWalkerQueryRecord) == 388 &&
    offsetof(PharaohWalkerQueryRecord, reserved_07c) + 1 == 0x7d &&
    offsetof(PharaohWalkerQueryRecord, reserved_131) == 0x131,
    "native info action walker phrase bytes");

static int32_t signed_bits(uint32_t value)
{
    return value <= INT32_MAX ? (int32_t)value :
        (int32_t)((int64_t)value - INT64_C(0x100000000));
}

void pharaoh_building_info_actions_select_walker(
    const PharaohBuildingInfoActionsLiveContext *c)
{
    c->window->selected_walker_index = signed_bits(*c->button_parameter);
    pharaoh_info_walker_selection_voice(c->selection_voice);
    *c->redraw = 1;
}

static void show_native_popup(const PharaohBuildingInfoActionsLiveContext *c,
    uint32_t key)
{
    /*4264D0's normalized persistent owner borrows these three shared inputs.
      Publish its window/redraw results to the caller's actual scalar views.
      Its other popup fields remain in the same persistent owner used by the
      real popup renderer/input.51F780 is the genuine original RET callback. */
    c->popup->current_window = *c->current_window;
    c->popup->mouse_x = *c->mouse_x;
    c->popup->mouse_y = *c->mouse_y;
    pharaoh_popup_window_open(c->popup, key,
        pharaoh_actor_walking_no_operation, 0);
    *c->current_window = c->popup->current_window;
    *c->redraw = c->popup->redraw_requested;
}

void pharaoh_building_info_actions_modal_40aa50(
    const PharaohBuildingInfoActionsLiveContext *c)
{
    const PharaohBuildingInfoActionPlayer *p = &c->players[*c->current_player];
    if (p->native_44f8 != 0) return;
    if (p->festival_placed == 0) {
        show_native_popup(c, 0x58);
        return;
    }
    if (p->treasury <= -5000) {
        show_native_popup(c, 0x60);
        return;
    }
    const uint32_t parameter = *c->button_parameter;
    *c->redraw = 1;
    *c->current_window = (uint8_t)(parameter != 0 ? 0x52 : 0x2b);
}

typedef struct ModalResetAdapter {
    const PharaohBuildingInfoActionsLiveContext *live;
    PharaohMenuModalState *modal;
} ModalResetAdapter;

static void reset_native_modal(void *user)
{
    ModalResetAdapter *a = user;
    /* The existing whole40AD30 core has already assigned its saved scalar
       fields. Publish both DWORDs before the actual4CF1F0 reset child. */
    *a->live->native_f53f0c = a->modal->saved_cursor_x;
    *a->live->native_f53f08 = a->modal->saved_cursor_y;
    pharaoh_input_transient_reset(a->live->input_transient);
}

void pharaoh_building_info_actions_modal_40ad30(
    const PharaohBuildingInfoActionsLiveContext *c)
{
    /* Reuse the existing complete normalized algorithm with one selected
       player/scalar view. This is not a player-bank or world copy. */
    PharaohMenuModalPlayer player = {0};
    player.gate = c->players[*c->current_player].mansion_placed;
    PharaohMenuModalState modal = {0};
    modal.player = &player;
    ModalResetAdapter adapter = {c, &modal};
    modal.reset_modal_state = reset_native_modal;
    modal.user_data = &adapter;
    if (player.gate != 0) {
        modal.button_parameter = *c->button_parameter;
        if (modal.button_parameter != 0) {
            modal.cursor_x = *c->native_e38da8;
            modal.cursor_y = *c->native_e38da4;
        }
    }
    pharaoh_menu_modal_open_2c_or_4d(&modal);
    if (player.gate != 0) {
        *c->current_window = modal.current_screen;
        *c->redraw = modal.redraw_requested;
    }
}

static void close_native_message(void *user)
{
    const PharaohBuildingInfoActionsLiveContext *c = user;
    pharaoh_message_dialog_close_live(c->dialog_close);
}

static void show_native_message(void *user, int32_t message,
    int32_t parameter, uint8_t templated)
{
    const PharaohBuildingInfoActionsLiveContext *c = user;
    /* This private provider is called only by the whole4E2360 core, which
       supplies parameter0. The genuine catalogue adapter binds message10's
       native DWORD60 before invoking the complete4C5BF0 algorithm. */
    (void)parameter;
    pharaoh_city_message_dialog_show(c->messages, message, templated);
}

void pharaoh_building_info_actions_show_message_help(
    const PharaohBuildingInfoActionsLiveContext *c)
{
    PharaohMenuMessageState state = {0};
    state.close_message_panel = close_native_message;
    state.show_message = show_native_message;
    state.user_data = (void *)c;
    pharaoh_menu_message_ten(&state);
}

void pharaoh_building_info_actions_next_phrase(
    const PharaohBuildingInfoActionsLiveContext *c)
{
    static const uint8_t phrase_groups[8] = {8, 13, 2, 1, 11, 5, 1, 10};
    const uint32_t selected = (uint32_t)c->window->selected_walker_index;
    PharaohWalkerQueryRecord *walker =
        &c->walkers[c->window->walker_ids[selected]];
    const uint8_t category_byte = walker->reserved_07c[1];
    const int32_t old = category_byte <= INT8_MAX ? category_byte :
        (int32_t)category_byte - 256;
    if (old != 9) {
        /* old-1 clears F55BA3; old8 clears F55BAC. The typed16-byte bank
           retains both neighboring DWORDs and exposes their actual bytes. */
        volatile uint8_t *const bank = (volatile uint8_t *)c->phrase_bank;
        bank[4 + old] = 0;
        uint8_t candidate = 0;
        for (uint8_t index = 0; index < 8; ++index) {
            /* Native compares a BYTE weight to the candidate INDEX. */
            if (c->phrase_bank->weights[index] > candidate) candidate = index;
        }
        if (c->phrase_bank->weights[candidate] > 1) {
            c->phrase_bank->weights[candidate] = 0;
            walker->reserved_07c[1] = candidate;
            c->window->walker_phrase = candidate;
            if (candidate != 9 && candidate != 8) {
                const uint8_t group = phrase_groups[candidate];
                *c->redraw = 1;
                walker->reserved_131 = group;
                return;
            }
            walker->reserved_131 = 13;
        }
    }
    *c->redraw = 1;
}
