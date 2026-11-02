#include "city_history.h"
#include <string.h>

static int32_t read_i32(const void *city, uint32_t offset)
{
    int32_t value;
    memcpy(&value, (const uint8_t *)city + offset, sizeof(value));
    return value;
}

static void write_u32(void *city, uint32_t offset, uint32_t value)
{
    memcpy((uint8_t *)city + offset, &value, sizeof(value));
}

int32_t restore_city_sum_bins(const void *city, uint32_t first, uint32_t count)
{
    uint32_t sum = 0, i;
    for (i = 0; i < count; ++i) {
        int16_t value;
        memcpy(&value, (const uint8_t *)city + 0x3e34u + (first + i) * 2u, sizeof(value));
        sum += (uint32_t)(int32_t)value;
    }
    return (int32_t)sum;
}

void restore_city_history_append(void *city)
{
    uint32_t cursor = (uint32_t)read_i32(city, 0x3e2cu);
    write_u32(city, 0x18acu + cursor * 4u, (uint32_t)read_i32(city, 0x1894u));
    ++cursor;
    if ((int32_t)cursor >= 2400) cursor = 0;
    write_u32(city, 0x3e2cu, cursor);
    write_u32(city, 0x3e30u, (uint32_t)read_i32(city, 0x3e30u) + 1u);
}

int32_t restore_city_history_get(const void *city, int32_t history_offset, int32_t ring_index)
{
    if (history_offset < read_i32(city, 0x3e30u)) {
        history_offset = (int32_t)((uint32_t)read_i32(city, 0x3e2cu) - (uint32_t)history_offset);
        if (history_offset < 0) history_offset = (int32_t)((uint32_t)history_offset + 2400u);
    } else {
        history_offset = 0;
    }
    ring_index = (int32_t)((uint32_t)ring_index + (uint32_t)history_offset);
    if (ring_index >= 2400) ring_index = (int32_t)((uint32_t)ring_index - 2400u);
    return read_i32(city, 0x18acu + (uint32_t)ring_index * 4u);
}
