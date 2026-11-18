#include "pharaoh/runtime/event_reason_phrases.h"

#include "pharaoh/runtime/model_text.h"
#include "pharaoh/runtime/settings_ini.h"

#include <stdio.h>
#include <string.h>

enum {
    NATIVE_MESSAGE_BOX_NO = 7,
    NATIVE_TEXT_SCAN_LIMIT = 999,
    PHARAOH_REASON_ERROR_TEXT_BYTES = 100,
    PHARAOH_REASON_PROMPT_TEXT_BYTES = 100
};

static int32_t pharaoh_event_reason_find_byte_bounded(
    const char *text, char target, int32_t native_search_limit,
    uint32_t available_bytes)
{
    if (native_search_limit > 0 &&
        (uint32_t)native_search_limit <= available_bytes) {
        return pharaoh_ini_find_byte(text, target, native_search_limit);
    }
    for (int32_t i = 0; i < native_search_limit &&
         (uint32_t)i < available_bytes; ++i) {
        if (text[i] == target) {
            return i + 1;
        }
    }
    return 0;
}

static uint32_t pharaoh_event_reason_skip_number_bounded(
    const char *text, uint32_t available_bytes)
{
    if (available_bytes >= NATIVE_TEXT_SCAN_LIMIT) {
        return pharaoh_model_text_skip_to_number(text);
    }
    const uint32_t scan_limit = available_bytes < NATIVE_TEXT_SCAN_LIMIT
        ? available_bytes : NATIVE_TEXT_SCAN_LIMIT;
    for (uint32_t i = 0; i < scan_limit; ++i) {
        const uint8_t value = (uint8_t)text[i];
        if ((value >= (uint8_t)'0' && value <= (uint8_t)'9') ||
            value == (uint8_t)'-') {
            return i;
        }
    }
    return scan_limit;
}

static int32_t pharaoh_event_reason_number_bounded(
    const char *text, uint32_t available_bytes)
{
    uint32_t end = 0;
    while (end < available_bytes &&
           text[end] >= '0' && text[end] <= '9') {
        ++end;
    }
    if (end < available_bytes && text[0] == '-') {
        ++end;
        while (end < available_bytes &&
               text[end] >= '0' && text[end] <= '9') {
            ++end;
        }
    }
    if (end == 0) {
        return 0;
    }
    if (end == available_bytes) {
        return 0;
    }
    return pharaoh_ini_parse_integer(text);
}

static void pharaoh_event_reason_parse_table(
    const PharaohEventReasonPhraseContext *context, int32_t file_bytes)
{
    uint32_t cursor = 0;
    for (uint32_t row = 0; row < PHARAOH_EVENT_REASON_PHRASE_ROWS; ++row) {
        uint32_t available = context->text_buffer_capacity - cursor;
        int32_t span = pharaoh_event_reason_find_byte_bounded(
            context->text_buffer + cursor, '{', file_bytes, available);
        cursor += (uint32_t)span;
        for (uint32_t column = 0;
             column < PHARAOH_EVENT_REASON_PHRASE_COLUMNS; ++column) {
            available = context->text_buffer_capacity - cursor;
            const uint32_t number_offset =
                pharaoh_event_reason_skip_number_bounded(
                    context->text_buffer + cursor, available);
            cursor += number_offset;
            available = context->text_buffer_capacity - cursor;
            const int32_t value = pharaoh_event_reason_number_bounded(
                context->text_buffer + cursor, available);
            context->table->value[row][column] = (uint8_t)value;
            span = pharaoh_event_reason_find_byte_bounded(
                context->text_buffer + cursor, ',', file_bytes,
                available);
            cursor += (uint32_t)span;
        }
    }
}

int32_t pharaoh_event_reason_phrases_load(
    const PharaohEventReasonPhraseContext *context)
{
    const PharaohEventReasonPhraseOperations *operations =
        context->operations;
    if (context->text_buffer == NULL) {
        operations->report_status(operations->user_data,
            "ERR:no spare screen for reason table");
        return 0;
    }
    if (context->text_buffer_capacity <
        PHARAOH_EVENT_REASON_TEXT_BUFFER_BYTES) {
        return 0;
    }

    memset(context->text_buffer, 0,
        PHARAOH_EVENT_REASON_TEXT_BUFFER_BYTES);
    if (operations->load_text_file(operations->user_data,
            "auto reason phrases.txt", context->text_buffer,
            PHARAOH_EVENT_REASON_TEXT_BUFFER_BYTES, 0) == 0) {
        operations->report_status(operations->user_data,
            "ERR:No auto reason phrases file");
        return 0;
    }

    const int32_t file_bytes = operations->text_file_size(
        operations->user_data, "auto reason phrases.txt");
    uint32_t observed_rows = 0;
    uint32_t cursor = 0;
    int32_t span = pharaoh_event_reason_find_byte_bounded(
        context->text_buffer, '{', file_bytes,
        context->text_buffer_capacity);
    if (span != 0) {
        do {
            ++observed_rows;
            if (observed_rows > PHARAOH_EVENT_REASON_PHRASE_ROWS) {
                break;
            }
            cursor += (uint32_t)span;
            span = pharaoh_event_reason_find_byte_bounded(
                context->text_buffer + cursor, '{', file_bytes,
                context->text_buffer_capacity - cursor);
        } while (span != 0);
    }

    if (observed_rows != PHARAOH_EVENT_REASON_PHRASE_ROWS) {
        char error_text[PHARAOH_REASON_ERROR_TEXT_BYTES];
        char prompt_text[PHARAOH_REASON_PROMPT_TEXT_BYTES];
        (void)snprintf(error_text, sizeof(error_text),
            "ERR: reason file has too many lines, expected %d",
            PHARAOH_EVENT_REASON_PHRASE_ROWS);
        (void)snprintf(prompt_text, sizeof(prompt_text),
            "%s\r\n\r\nWould you still like to continue?", error_text);
        if (operations->show_yes_no_prompt(operations->user_data,
                context->owner_window, prompt_text,
                "reason phrase data invalid") == NATIVE_MESSAGE_BOX_NO) {
            operations->report_status(operations->user_data, error_text);
            return 0;
        }
    }

    pharaoh_event_reason_parse_table(context, file_bytes);
    operations->report_status(operations->user_data,
        " OK: reason table loaded");
    return 1;
}
