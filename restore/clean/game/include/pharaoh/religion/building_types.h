#ifndef PHARAOH_RELIGION_BUILDING_TYPES_H
#define PHARAOH_RELIGION_BUILDING_TYPES_H

#include <stdint.h>

typedef enum PharaohGodBuildingKind {
    PHARAOH_GOD_BUILDING_TEMPLE = 0,
    PHARAOH_GOD_BUILDING_TEMPLE_COMPLEX = 1,
    PHARAOH_GOD_BUILDING_SHRINE = 2
} PharaohGodBuildingKind;

/* Original address 0x004C06E0. */
int32_t pharaoh_religion_building_type_for_god(
    int32_t god, PharaohGodBuildingKind kind);

int32_t pharaoh_city_god_with_status(
    const int16_t statuses[5], int32_t status);

#endif
