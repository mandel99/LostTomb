#ifndef PHARAOH_MONUMENTS_STARTUP_GEOMETRY_H
#define PHARAOH_MONUMENTS_STARTUP_GEOMETRY_H

#include "pharaoh/core/record_geometry.h"

enum {
    PHARAOH_ABU_SIMBEL_PART_COUNT = 16,
    PHARAOH_LIBRARY_PART_COUNT = 25,
    PHARAOH_CAESAREUM_PART_COUNT = 9,
    PHARAOH_LIGHTHOUSE_PART_COUNT = 2,
    PHARAOH_SMALL_TOMB_PART_COUNT = 167,
    PHARAOH_MEDIUM_TOMB_PART_COUNT = 158,
    PHARAOH_LARGE_TOMB_PART_COUNT = 437,
    PHARAOH_GRAND_TOMB_PART_COUNT = 513,
    PHARAOH_SUN_TEMPLE_PART_COUNT = 100
};

/* settings.scale is native mapTilePixelHeight; settings.step is mapTilePixelWidth. */
/* The native owners update the typed part-record array held by each monument. */
void pharaoh_abu_simbel_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_ABU_SIMBEL_PART_COUNT],
    const PharaohProjectionSettings *settings);
void pharaoh_library_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_LIBRARY_PART_COUNT],
    const PharaohProjectionSettings *settings);
void pharaoh_caesareum_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_CAESAREUM_PART_COUNT],
    const PharaohProjectionSettings *settings);
void pharaoh_lighthouse_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_LIGHTHOUSE_PART_COUNT],
    const PharaohProjectionSettings *settings);
void pharaoh_small_tomb_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_SMALL_TOMB_PART_COUNT],
    const PharaohProjectionSettings *settings);
void pharaoh_medium_tomb_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_MEDIUM_TOMB_PART_COUNT],
    const PharaohProjectionSettings *settings);
void pharaoh_large_tomb_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_LARGE_TOMB_PART_COUNT],
    const PharaohProjectionSettings *settings);
void pharaoh_grand_tomb_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_GRAND_TOMB_PART_COUNT],
    const PharaohProjectionSettings *settings);
void pharaoh_sun_temple_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_SUN_TEMPLE_PART_COUNT],
    const PharaohProjectionSettings *settings);

#endif
