#include "pharaoh/runtime/settings_ini.h"

#include <ctype.h>
#include <stdbool.h>
#include <string.h>

enum {
    PHARAOH_SETTINGS_INI_MODE_ELEMENT_BYTES = 20000,
    PHARAOH_SETTINGS_INI_MODE_ELEMENT_COUNT = 1
};

int32_t pharaoh_ini_compare_bytes(const char *left, const char *right,
                                  int32_t byte_count)
{
    for (int32_t i = 0; i < byte_count; ++i) {
        if (left[i] != right[i]) {
            return i + 1;
        }
    }
    return 0;
}

int32_t pharaoh_ini_find_key(const char *buffer, const char *key,
                             int32_t search_bytes)
{
    const size_t key_bytes = strlen(key);
    for (int32_t i = 0; i < search_bytes; ++i) {
        if (buffer[i] == key[0] &&
            pharaoh_ini_compare_bytes(buffer + i, key, (int32_t)key_bytes) == 0) {
            return i;
        }
    }
    return 0;
}

int32_t pharaoh_ini_find_byte(const char *buffer, char value,
                              int32_t search_bytes)
{
    for (int32_t i = 0; i < search_bytes; ++i) {
        if (buffer[i] == value) {
            return i + 1;
        }
    }
    return 0;
}

int32_t pharaoh_ini_parse_integer(const char *text)
{
    const unsigned char *cursor = (const unsigned char *)text;
    while (isspace(*cursor)) {
        ++cursor;
    }

    const bool negative = *cursor == '-';
    if (*cursor == '-' || *cursor == '+') {
        ++cursor;
    }

    uint32_t magnitude = 0;
    while (isdigit(*cursor)) {
        magnitude = magnitude * 10u + (uint32_t)(*cursor - (unsigned char)'0');
        ++cursor;
    }

    const uint32_t bits = negative ? 0u - magnitude : magnitude;
    int32_t value;
    memcpy(&value, &bits, sizeof(value));
    return value;
}

int32_t pharaoh_ini_to_integer(const char *text)
{
    return pharaoh_ini_parse_integer(text);
}

int32_t pharaoh_settings_read_ini(const PharaohSettingsIniContext *context)
{
    const PharaohSettingsIniOperations *operations = context->operations;
    char *buffer = operations->allocate(context->user_data,
        PHARAOH_SETTINGS_INI_BUFFER_BYTES);
    if (buffer == NULL) {
        return 0;
    }
    memset(buffer, 0, PHARAOH_SETTINGS_INI_BUFFER_BYTES);

    const PharaohFileReadOperations *file_operations =
        context->files->operations;
    PharaohReadOnlyFile *file = file_operations->open(
        context->files->user_data, "Pharaoh.ini");
    if (file == NULL) {
        operations->show_error_status(context->user_data,
            "Err:Could not read sierra.ini");
        operations->release(context->user_data, buffer);
        return 0;
    }

    (void)file_operations->read(context->files->user_data, buffer,
        PHARAOH_SETTINGS_INI_MODE_ELEMENT_BYTES,
        PHARAOH_SETTINGS_INI_MODE_ELEMENT_COUNT, file);
    file_operations->close(context->files->user_data, file);

    int32_t key_offset = pharaoh_ini_find_key(buffer, "RAM",
        PHARAOH_SETTINGS_INI_BUFFER_BYTES);
    int32_t equals_offset = pharaoh_ini_find_byte(
        buffer + key_offset, '=',
        PHARAOH_SETTINGS_INI_BUFFER_BYTES);
    if (key_offset + equals_offset > 0) {
        context->settings->ram = pharaoh_ini_to_integer(
            buffer + key_offset + equals_offset);
    }

    key_offset = pharaoh_ini_find_key(buffer, "CheckCD",
        PHARAOH_SETTINGS_INI_BUFFER_BYTES);
    if (key_offset > 0) {
        equals_offset = pharaoh_ini_find_byte(
            buffer + key_offset, '=',
            PHARAOH_SETTINGS_INI_BUFFER_BYTES);
        if (key_offset + equals_offset > 0) {
            context->settings->check_cd = (uint8_t)pharaoh_ini_to_integer(
                buffer + key_offset + equals_offset);
        }
    }

    key_offset = pharaoh_ini_find_key(buffer, "CPU",
        PHARAOH_SETTINGS_INI_BUFFER_BYTES);
    if (key_offset > 0) {
        equals_offset = pharaoh_ini_find_byte(
            buffer + key_offset, '=',
            PHARAOH_SETTINGS_INI_BUFFER_BYTES);
        if (key_offset + equals_offset > 0) {
            context->settings->cpu = pharaoh_ini_to_integer(
                buffer + key_offset + equals_offset);
        }
    }

    operations->release(context->user_data, buffer);
    return 1;
}
