#ifndef PHARAOH_UI_CITY_MESSAGE_PUBLISH_H
#define PHARAOH_UI_CITY_MESSAGE_PUBLISH_H
#include "pharaoh/ui/city_message_queue.h"
#include "pharaoh/ui/city_message_policy.h"
#include "pharaoh/runtime/sound_effect_request.h"

typedef struct PharaohCityMessagePublishContext {
    const PharaohCityMessageQueueContext *ui;
    const PharaohCityMessagePolicyContext *policy;
    PharaohSoundEffectRequestState *sound;
    const PharaohSoundEffectRequestOperations *sound_operations;
    const int32_t *current_year;
    const uint32_t *current_month;
    uint32_t *messages_used,*next_sequence,*posted_once;
    int32_t *newest_index; /* E951E8 */
    uint8_t *window_id; /* same live slot as ui->window_id/dialog */
    uint8_t *message_button_alert; /*5C755F*/
    uint32_t *special_message_present; /*E951D8*/
    uint8_t *special_message_flag; /*E38DCA*/
} PharaohCityMessagePublishContext;

/* Complete004E0CF0. UI, dialog, policy and media contexts borrow actual
   shared state; ui->popup aliases dialog->event54_popup. No snapshots. */
void pharaoh_city_message_post(const PharaohCityMessagePublishContext *,
    int32_t message,uint32_t argument0,uint32_t argument1,
    const int16_t *parameters,uint8_t force_popup);
#endif
