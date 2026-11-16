#ifndef PHARAOH_EVENTS_GIFT_BUTTONS_H
#define PHARAOH_EVENTS_GIFT_BUTTONS_H
#include <stddef.h>
#include <stdint.h>
typedef struct PharaohGiftEvent {
  int16_t id, type, amount, item, state, months_left;
  uint8_t active;
} PharaohGiftEvent;
typedef struct PharaohGiftContext {
  PharaohGiftEvent *events;
  size_t event_count;
  int16_t *message_phrases;
  uint8_t *resource_gift_received;
  uint32_t dialog_flags, accept_flag, refuse_flag, later_flag;
  int32_t gift_type, phrase_amount, phrase_item, phrase_buttons;
  int16_t state_accepted, state_refused, state_offered, state_offer_again;
  int16_t state_offered_again, state_offer_last, state_too_late;
  void (*process)(void *, PharaohGiftEvent *);
  void (*close_dialog)(void *);
  void (*determine_resources)(void *);
  void *user_data;
} PharaohGiftContext;
PharaohGiftEvent *pharaoh_gift_for_message(PharaohGiftContext *,
                                           const int16_t *);
void pharaoh_gift_accept(PharaohGiftContext *);
void pharaoh_gift_refuse(PharaohGiftContext *);
void pharaoh_gift_later(PharaohGiftContext *);
#endif
