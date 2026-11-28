#include "pharaoh/ui/small_button_callbacks.h"

#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohMapViewPoint) == 32 &&
    offsetof(PharaohMapViewPoint, x) == 0 &&
    offsetof(PharaohMapViewPoint, y) == 2,
    "native map-view point layout");

void pharaoh_small_button_show_message_29(PharaohSmallButtonState *state)
{
    state->open_message(state->user_data, 29, 1, 0);
    state->redraw_requested = 1;
}

void pharaoh_small_button_select_secondary_window(
    PharaohSmallButtonState *state)
{
    state->redraw_requested = 1;
    state->current_window = state->secondary_parameter != 0u ? 9 : 7;
}

void pharaoh_small_button_show_message_32(PharaohSmallButtonState *state)
{
    state->open_message(state->user_data, 32, 1, 0);
}

void pharaoh_small_button_start_timer(PharaohSmallButtonState *state)
{
    if (state->timer_active != 0u) return;
    state->start_timer(state->user_data, 0, 59, 1000);
    state->timer_first = 0;
    state->timer_mode = 0;
    state->timer_value = 0;
    state->timer_last = 0;
    state->redraw_requested = 1;
}

void pharaoh_small_button_show_selected_message(
    PharaohSmallButtonState *state)
{
    const int32_t message = state->message_id > 0 ? state->message_id : 10;
    state->open_message(state->user_data, message, 0, 0);
    state->redraw_requested = 1;
}

void pharaoh_small_button_commit_map_view_borrowed(
    const PharaohSmallButtonMapViewBorrowedState *state)
{
    if(*state->enabled==1u) {
        const uint32_t index=*state->selected_map_point;
        int16_t x,y;
        memcpy(&x,state->scroll_x_word,sizeof(x));
        memcpy(&y,state->scroll_y_word,sizeof(y));
        *state->committed_map_point=(int16_t)index;
        state->map_points[index].x=x;
        state->map_points[index].y=y;
        state->refresh_map_point(state->user_data);
    }
    *state->current_window=1;
    *state->redraw_requested=1;
}

/* Existing normalized API shares the whole40A090/409520 algorithm. */
void pharaoh_small_button_commit_map_view(PharaohSmallButtonState *state)
{
    const PharaohSmallButtonMapViewBorrowedState borrowed={
        &state->map_view_commit_enabled,&state->selected_map_point,
        &state->hit_scroll_x,&state->hit_scroll_y,&state->committed_map_point,
        state->map_points,&state->current_window,&state->redraw_requested,
        state->refresh_map_point,state->user_data
    };
    pharaoh_small_button_commit_map_view_borrowed(&borrowed);
}
