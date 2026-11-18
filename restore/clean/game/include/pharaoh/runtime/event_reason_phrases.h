#ifndef PHARAOH_RUNTIME_EVENT_REASON_PHRASES_H
#define PHARAOH_RUNTIME_EVENT_REASON_PHRASES_H

#include <stdint.h>

enum {
    PHARAOH_EVENT_REASON_PHRASE_ROWS = 54,
    PHARAOH_EVENT_REASON_PHRASE_COLUMNS = 36,
    PHARAOH_EVENT_REASON_TEXT_BUFFER_BYTES = 100000
};

typedef struct PharaohEventReasonPhraseTable {
    uint8_t value[PHARAOH_EVENT_REASON_PHRASE_ROWS]
                 [PHARAOH_EVENT_REASON_PHRASE_COLUMNS];
} PharaohEventReasonPhraseTable;

typedef struct PharaohEventReasonPhraseOperations {
    void *user_data;
    int32_t (*load_text_file)(void *user_data, const char *filename,
                              char *destination, uint32_t capacity,
                              int32_t mode);
    int32_t (*text_file_size)(void *user_data, const char *filename);
    int32_t (*show_yes_no_prompt)(void *user_data, void *owner_window,
                                  const char *message, const char *title);
    void (*report_status)(void *user_data, const char *message);
} PharaohEventReasonPhraseOperations;

typedef struct PharaohEventReasonPhraseContext {
    char *text_buffer;
    uint32_t text_buffer_capacity;
    void *owner_window;
    PharaohEventReasonPhraseTable *table;
    const PharaohEventReasonPhraseOperations *operations;
} PharaohEventReasonPhraseContext;

/* Native entry 0x0044EFA0. Fills the 54-by-36 byte reason table. */
int32_t pharaoh_event_reason_phrases_load(
    const PharaohEventReasonPhraseContext *context);

#endif
