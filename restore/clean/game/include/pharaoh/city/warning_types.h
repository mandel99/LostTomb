#ifndef PHARAOH_CITY_WARNING_TYPES_H
#define PHARAOH_CITY_WARNING_TYPES_H

#include <stddef.h>
#include <stdint.h>

enum {
    PHARAOH_CITY_WARNING_COUNT = 5,
    PHARAOH_CITY_WARNING_POSITION_COMPONENTS = 2,
    PHARAOH_CITY_WARNING_TEXT_BYTES = 100
};

#pragma pack(push, 1)
typedef struct PharaohCityWarning {
    int16_t warning_id;
    int16_t x;
    int16_t y;
    int16_t in_use;
    int16_t text_length;
    int16_t box_width;
    int32_t time;
    uint8_t reserved_1[2];
    char text[PHARAOH_CITY_WARNING_TEXT_BYTES];
    uint8_t reserved_2[2];
} PharaohCityWarning;
#pragma pack(pop)

typedef int32_t PharaohCityWarningPositions[
    PHARAOH_CITY_WARNING_COUNT][PHARAOH_CITY_WARNING_POSITION_COMPONENTS];
typedef PharaohCityWarning PharaohCityWarningTable[
    PHARAOH_CITY_WARNING_COUNT];

_Static_assert(sizeof(PharaohCityWarning) == 0x78,
    "native city warning records are 120 bytes");
_Static_assert(offsetof(PharaohCityWarning, x) == 2 &&
    offsetof(PharaohCityWarning, y) == 4 &&
    offsetof(PharaohCityWarning, text) == 18,
    "city warning fields preserve their packed native offsets");

#endif
