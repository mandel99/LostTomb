#include "pharaoh/events/gift_buttons.h"
PharaohGiftEvent *pharaoh_gift_for_message(PharaohGiftContext *c,
                                           const int16_t *p) {
  size_t i;
  for (i = 0; i < c->event_count; ++i) {
    PharaohGiftEvent *e = &c->events[i];
    if (e->type == c->gift_type && e->active &&
        p[c->phrase_amount] == e->amount && p[c->phrase_item] == e->item) {
      e->id = (int16_t)i;
      return e;
    }
  }
  return 0;
}
static PharaohGiftEvent *shown(PharaohGiftContext *c) {
  c->message_phrases[c->phrase_buttons] = 0;
  return pharaoh_gift_for_message(c, c->message_phrases);
}
void pharaoh_gift_accept(PharaohGiftContext *c) {
  PharaohGiftEvent *e;
  if (!(c->dialog_flags & c->accept_flag))
    return;
  e = shown(c);
  if (!e)
    return;
  e->state = c->state_accepted;
  c->process(c->user_data, e);
  c->close_dialog(c->user_data);
  c->resource_gift_received[e->item] = 1;
  c->determine_resources(c->user_data);
}
void pharaoh_gift_refuse(PharaohGiftContext *c) {
  PharaohGiftEvent *e;
  if (!(c->dialog_flags & c->refuse_flag))
    return;
  e = shown(c);
  if (!e)
    return;
  e->state = c->state_refused;
  c->process(c->user_data, e);
  c->close_dialog(c->user_data);
}
void pharaoh_gift_later(PharaohGiftContext *c) {
  PharaohGiftEvent *e;
  if (!(c->dialog_flags & c->later_flag))
    return;
  e = shown(c);
  if (!e)
    return;
  if (e->state == c->state_offered) {
    e->state = c->state_offer_again;
    e->months_left = 1;
  } else if (e->state == c->state_offered_again) {
    e->state = c->state_offer_last;
    e->months_left = 1;
  } else {
    e->state = c->state_too_late;
    c->process(c->user_data, e);
  }
  c->close_dialog(c->user_data);
}
