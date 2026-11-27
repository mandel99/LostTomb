#ifndef PHARAOH_UI_CITY_MESSAGE_QUEUE_H
#define PHARAOH_UI_CITY_MESSAGE_QUEUE_H

#include "pharaoh/ui/city_message_text.h"
#include "pharaoh/runtime/file_exists_media_fallback.h"

typedef struct PharaohCityMessageMediaLayout {
    union { uint8_t reserved_00[0x38];
        struct { int16_t native_00,subtype; uint8_t reserved_04[0x34]; }; };
    uint32_t native_38, media_offset;
    uint8_t reserved_40[0x10];
} PharaohCityMessageMediaLayout;

typedef struct PharaohCityMessageQueueContext {
    const uint8_t *window_id;
    int32_t *delay, *queue; /* twenty native DWORD queue slots */
    PharaohMenuMessageRecord *messages; /* one thousand native records */
    uint32_t *category, *first, *second, *month;
    int32_t *year, *current_message, *shown_index;
    uint8_t *modal_reset, *redraw;
    uint32_t *popup, *show_video;
    const int32_t *current_tick;
    int32_t *tooltip_id, *tooltip_tick;
    const PharaohCityMessageMediaLayout *layouts;
    const char *const *strings;
    const PharaohFileExistsMediaFallbackContext *files;
    PharaohMessageDialogOpenState *dialog;
} PharaohCityMessageQueueContext;

/* Native004E1320. All UI slots alias the corresponding live dialog state.
 * In particular popup aliases dialog->event54_popup; no snapshots/defaults. */
void pharaoh_city_message_queue_process(const PharaohCityMessageQueueContext *context);

/* Bind catalogue DWORD60 and selected record WORD38 for actual004C5BF0. */
void pharaoh_city_message_dialog_show(const PharaohCityMessageQueueContext *,
    int32_t message,uint8_t templated);

#endif
