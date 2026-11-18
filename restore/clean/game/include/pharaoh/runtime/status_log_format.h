#ifndef PHARAOH_RUNTIME_STATUS_LOG_FORMAT_H
#define PHARAOH_RUNTIME_STATUS_LOG_FORMAT_H

#include <stdint.h>

#include "pharaoh/runtime/status_log_append.h"

enum {
    PHARAOH_STATUS_LOG_TEXT_COPY_LIMIT = 100,
    PHARAOH_STATUS_LOG_INTEGER_TEXT_CAPACITY = 256,
    PHARAOH_STATUS_LOG_TIMESTAMP_TEXT_LENGTH = 26,
    PHARAOH_STATUS_LOG_RECORD_CAPACITY = 318,
    PHARAOH_STATUS_LOG_TIMESTAMP_PREFIX_LENGTH = 12,
    PHARAOH_STATUS_LOG_SEPARATOR_LENGTH = 60,
    PHARAOH_STATUS_LOG_LINE_END_LENGTH = 2
};

typedef struct PharaohStatusLogFormatState {
    PharaohStatusLogAppendState *append_state;
    /* Reuse one state object to retain the native process-global scratch text
       between calls, including the stale value used for negative details. */
    uint8_t record_buffer[PHARAOH_STATUS_LOG_RECORD_CAPACITY];
    char integer_text[PHARAOH_STATUS_LOG_INTEGER_TEXT_CAPACITY];
    char timestamp_text[PHARAOH_STATUS_LOG_TIMESTAMP_TEXT_LENGTH];
} PharaohStatusLogFormatState;

typedef struct PharaohStatusLogFormatOperations {
    void *user_data;
    /* Supplies the 26 bytes produced by the native CRT time conversion chain. */
    void (*refresh_timestamp_text)(void *user_data,
        char timestamp_text[PHARAOH_STATUS_LOG_TIMESTAMP_TEXT_LENGTH]);
    const PharaohStatusLogAppendOperations *append_operations;
} PharaohStatusLogFormatOperations;

/* Native entry 0x004CC5C0. The native callers supply an output offset within
   the 256-byte shared scratch text and a zero/nonzero positive-sign flag. */
int32_t pharaoh_status_log_integer_to_text(
    char text[PHARAOH_STATUS_LOG_INTEGER_TEXT_CAPACITY], int32_t value,
    int32_t output_offset, int32_t positive_sign);

/* Native entry 0x004CE6E0. Message and detail strings are read for at most
   100 bytes each; detail may be null. `detail_value` retains native signed
   behavior, including its positive-only integer conversion condition. */
void pharaoh_status_log_format(const char *message, const char *detail,
    int32_t detail_value, PharaohStatusLogFormatState *state,
    const PharaohStatusLogFormatOperations *operations);

#endif
