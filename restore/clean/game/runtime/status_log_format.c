#include "pharaoh/runtime/status_log_format.h"

#include <string.h>

enum {
    STATUS_LOG_TIMESTAMP_FIRST_LINE_LENGTH = 10,
    STATUS_LOG_TIMESTAMP_SECOND_LINE_OFFSET = 10,
    STATUS_LOG_TIMESTAMP_DETAIL_OFFSET = 14,
    STATUS_LOG_TIMESTAMP_LAST_FIELD_OFFSET = 18,
    STATUS_LOG_TIMESTAMP_LAST_FIELD_LENGTH = 2,
    STATUS_LOG_TIMESTAMP_SPACE_OFFSET = 10,
    STATUS_LOG_SPACES = 0x2020,
    STATUS_LOG_DASH_CHARACTER = '-',
    STATUS_LOG_DASH_CHARACTER_COUNT = 60,
    STATUS_LOG_CARRIAGE_RETURN = 0x0d,
    STATUS_LOG_LINE_FEED = 0x0a,
    STATUS_LOG_DASH_LINE_LENGTH = 62,
    STATUS_LOG_INTEGER_TEXT_WORD_COUNT = 64,
    STATUS_LOG_INTEGER_TEXT_WORD_SIZE = 4,
    STATUS_LOG_INTEGER_RADIX = 10,
    STATUS_LOG_POSITIVE_NINE_DIGIT_LIMIT = 999999999
};

_Static_assert(STATUS_LOG_INTEGER_TEXT_WORD_COUNT *
        STATUS_LOG_INTEGER_TEXT_WORD_SIZE ==
        PHARAOH_STATUS_LOG_INTEGER_TEXT_CAPACITY,
    "Native integer scratch is cleared as 64 four-byte words");
_Static_assert(PHARAOH_STATUS_LOG_TIMESTAMP_TEXT_LENGTH == 26,
    "Native timestamp text extent is 26 bytes");
_Static_assert(PHARAOH_STATUS_LOG_RECORD_CAPACITY >=
        PHARAOH_STATUS_LOG_TIMESTAMP_PREFIX_LENGTH +
            PHARAOH_STATUS_LOG_TEXT_COPY_LIMIT * 2 +
            PHARAOH_STATUS_LOG_TEXT_COPY_LIMIT + 2 * 2 +
            PHARAOH_STATUS_LOG_LINE_END_LENGTH,
    "Record buffer covers the native per-string copy limits");

static int32_t status_log_integer_digit_count(int32_t value)
{
    if (value < 10) {
        return 1;
    }
    if (value < 100) {
        return 2;
    }
    if (value < 1000) {
        return 3;
    }
    if (value < 10000) {
        return 4;
    }
    if (value < 100000) {
        return 5;
    }
    if (value < 1000000) {
        return 6;
    }
    if (value < 10000000) {
        return 7;
    }
    if (value < 100000000) {
        return 8;
    }
    return value > STATUS_LOG_POSITIVE_NINE_DIGIT_LIMIT ? 0 : 9;
}

int32_t pharaoh_status_log_integer_to_text(
    char text[PHARAOH_STATUS_LOG_INTEGER_TEXT_CAPACITY], int32_t value,
    int32_t output_offset, int32_t positive_sign)
{
    memset(text, 0, PHARAOH_STATUS_LOG_INTEGER_TEXT_CAPACITY);

    if (value < 0) {
        /* The native two's-complement negation leaves INT32_MIN unchanged. */
        value = (int32_t)(0u - (uint32_t)value);
        text[output_offset++] = '-';
    } else if (positive_sign != 0) {
        text[output_offset++] = '+';
    }

    const int32_t digits = status_log_integer_digit_count(value);
    for (int32_t digit = digits - 1; digit >= 0; --digit) {
        text[output_offset + digit] = (char)(value % STATUS_LOG_INTEGER_RADIX + '0');
        value /= STATUS_LOG_INTEGER_RADIX;
    }
    return output_offset + digits;
}

static int32_t status_log_copy_text(uint8_t *destination,
    const char *source)
{
    int32_t copied = 0;

    while (copied < PHARAOH_STATUS_LOG_TEXT_COPY_LIMIT && source[copied] != '\0') {
        destination[copied] = (uint8_t)source[copied];
        ++copied;
    }

    return copied;
}

static void status_log_write_spaces(uint8_t *destination)
{
    destination[0] = (uint8_t)(STATUS_LOG_SPACES & 0xff);
    destination[1] = (uint8_t)((STATUS_LOG_SPACES >> 8) & 0xff);
}

static void status_log_write_line_end(uint8_t *destination)
{
    destination[0] = STATUS_LOG_CARRIAGE_RETURN;
    destination[1] = STATUS_LOG_LINE_FEED;
}

static void status_log_append(PharaohStatusLogFormatState *state,
    const PharaohStatusLogFormatOperations *operations, uint32_t byte_count)
{
    pharaoh_status_log_append(state->append_state, state->record_buffer,
        byte_count, operations->append_operations);
}

static void status_log_format_detailed_record(const char *message,
    const char *detail, int32_t detail_value,
    PharaohStatusLogFormatState *state,
    const PharaohStatusLogFormatOperations *operations)
{
    uint8_t *const record = state->record_buffer;
    memcpy(record,
        &state->timestamp_text[STATUS_LOG_TIMESTAMP_SECOND_LINE_OFFSET], 4);
    memcpy(record + 4,
        &state->timestamp_text[STATUS_LOG_TIMESTAMP_DETAIL_OFFSET], 4);
    memcpy(record + 8,
        &state->timestamp_text[STATUS_LOG_TIMESTAMP_LAST_FIELD_OFFSET],
        STATUS_LOG_TIMESTAMP_LAST_FIELD_LENGTH);
    status_log_write_spaces(record + STATUS_LOG_TIMESTAMP_SPACE_OFFSET);

    int32_t length = PHARAOH_STATUS_LOG_TIMESTAMP_PREFIX_LENGTH;
    length += status_log_copy_text(record + length, message);

    if (detail != NULL) {
        length += status_log_copy_text(record + length, detail);
        status_log_write_spaces(record + length);
        length += 2;
    }

    if (detail_value != 0) {
        length += status_log_copy_text(record + length, state->integer_text);
        status_log_write_spaces(record + length);
        length += 2;
    }

    status_log_write_line_end(record + length);
    length += PHARAOH_STATUS_LOG_LINE_END_LENGTH;
    status_log_append(state, operations, (uint32_t)length);
}

static void status_log_format_initial_record(const char *message,
    PharaohStatusLogFormatState *state,
    const PharaohStatusLogFormatOperations *operations)
{
    uint8_t *const record = state->record_buffer;
    int32_t length = status_log_copy_text(record, message);

    memcpy(record + length, state->timestamp_text,
        STATUS_LOG_TIMESTAMP_FIRST_LINE_LENGTH);
    length += STATUS_LOG_TIMESTAMP_FIRST_LINE_LENGTH;
    status_log_write_line_end(record + length);
    status_log_append(state, operations,
        (uint32_t)(length + PHARAOH_STATUS_LOG_LINE_END_LENGTH));

    for (int32_t index = 0; index < STATUS_LOG_DASH_CHARACTER_COUNT; ++index) {
        record[index] = STATUS_LOG_DASH_CHARACTER;
    }
    status_log_write_line_end(record + PHARAOH_STATUS_LOG_SEPARATOR_LENGTH);
    status_log_append(state, operations, STATUS_LOG_DASH_LINE_LENGTH);
}

void pharaoh_status_log_format(const char *message, const char *detail,
    int32_t detail_value, PharaohStatusLogFormatState *state,
    const PharaohStatusLogFormatOperations *operations)
{
    operations->refresh_timestamp_text(operations->user_data,
        state->timestamp_text);

    if (detail_value > 0) {
        (void)pharaoh_status_log_integer_to_text(state->integer_text,
            detail_value - 1, 0, 0);
    }

    if (state->append_state->status_file_initialized != 0) {
        status_log_format_detailed_record(message, detail, detail_value,
            state, operations);
        return;
    }

    status_log_format_initial_record(message, state, operations);
}
