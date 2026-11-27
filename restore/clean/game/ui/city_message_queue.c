#include "pharaoh/ui/city_message_queue.h"
#include "pharaoh/core/static_objects.h"
#include "pharaoh/ui/window_state_actions.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohCityMessageMediaLayout) == 80 &&
    offsetof(PharaohCityMessageMediaLayout, media_offset) == 60 &&
    sizeof(PharaohMenuMessageRecord) == 48 &&
    offsetof(PharaohMenuMessageRecord, sequence) == 12,
    "native queued message layouts");

void pharaoh_city_message_dialog_show(const PharaohCityMessageQueueContext *c,
    int32_t message,uint8_t templated)
{
    PharaohMessageDialogOpenState dialog=*c->dialog;
    dialog.message_record_value=&c->layouts[message].media_offset;
    dialog.current_message_phrase_count=templated!=0?
        &c->messages[*c->shown_index].native_26:NULL;
    pharaoh_message_dialog_show(&dialog,message,0,templated);
}

void pharaoh_city_message_queue_process(const PharaohCityMessageQueueContext *c)
{
    if (*c->window_id != 1) return;
    if (*c->delay > 0) { --*c->delay; return; }
    uint32_t slot = 0;
    while (slot < 20 && c->queue[slot] == 0) ++slot;
    if (slot == 20) return;
    const int32_t sequence = c->queue[slot];
    c->queue[slot] = 0;
    if (sequence <= 0) return;
    uint32_t index = 0;
    /* Native exclusive end scans only the first999 records. */
    for (; index < 999; ++index) {
        if (c->messages[index].message == 0) return;
        if (c->messages[index].sequence == sequence) break;
    }
    if (index == 999) return;
    PharaohMenuMessageRecord *const event = &c->messages[index];
    *c->delay = 5;
    const int32_t message = event->message;
    pharaoh_city_message_set_category((uint32_t)message, c->category);
    event->read = 1;
    const uint32_t first = event->first;
    *c->year = event->kind;
    const uint32_t month = event->source;
    const uint32_t second = event->second;
    *c->modal_reset = 0;
    *c->shown_index = (int32_t)index;
    *c->month = month;
    *c->first = first;
    *c->second = second;
    *c->current_message = message;
    pharaoh_tooltip_invalidate(*c->current_tick, c->tooltip_id, c->tooltip_tick);
    if (pharaoh_file_exists_with_media_fallback(c->files,
            *c->strings + c->layouts[message].media_offset) == 0)
        /* Native passes6/7 to this verified RET-zero leaf. Its unused
           stack argument is outside the logical C interface. */
        (void)pharaoh_native_return_zero_0047d350();
    else *c->show_video = 1;
    *c->popup = 1;
    pharaoh_city_message_dialog_show(c, *c->current_message,
        event->template_id >= 0);
    *c->popup = 0;
    *c->redraw = 1;
}
