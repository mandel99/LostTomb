#include "pharaoh/monuments/startup_geometry.h"

void pharaoh_abu_simbel_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_ABU_SIMBEL_PART_COUNT],
    const PharaohProjectionSettings *settings)
{
    pharaoh_projection_records_update(parts, PHARAOH_ABU_SIMBEL_PART_COUNT,
        settings);
}

void pharaoh_library_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_LIBRARY_PART_COUNT],
    const PharaohProjectionSettings *settings)
{
    pharaoh_projection_records_update(parts, PHARAOH_LIBRARY_PART_COUNT,
        settings);
}

void pharaoh_caesareum_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_CAESAREUM_PART_COUNT],
    const PharaohProjectionSettings *settings)
{
    pharaoh_projection_records_update(parts, PHARAOH_CAESAREUM_PART_COUNT,
        settings);
}

void pharaoh_lighthouse_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_LIGHTHOUSE_PART_COUNT],
    const PharaohProjectionSettings *settings)
{
    pharaoh_projection_records_update(parts, PHARAOH_LIGHTHOUSE_PART_COUNT,
        settings);
}

void pharaoh_small_tomb_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_SMALL_TOMB_PART_COUNT],
    const PharaohProjectionSettings *settings)
{
    pharaoh_projection_records_update(parts, PHARAOH_SMALL_TOMB_PART_COUNT,
        settings);
}

void pharaoh_medium_tomb_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_MEDIUM_TOMB_PART_COUNT],
    const PharaohProjectionSettings *settings)
{
    pharaoh_projection_records_update(parts, PHARAOH_MEDIUM_TOMB_PART_COUNT,
        settings);
}

void pharaoh_large_tomb_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_LARGE_TOMB_PART_COUNT],
    const PharaohProjectionSettings *settings)
{
    pharaoh_projection_records_update(parts, PHARAOH_LARGE_TOMB_PART_COUNT,
        settings);
}

void pharaoh_grand_tomb_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_GRAND_TOMB_PART_COUNT],
    const PharaohProjectionSettings *settings)
{
    pharaoh_projection_records_update(parts, PHARAOH_GRAND_TOMB_PART_COUNT,
        settings);
}

void pharaoh_sun_temple_part_geometry_initialize(
    PharaohProjectionRecord parts[PHARAOH_SUN_TEMPLE_PART_COUNT],
    const PharaohProjectionSettings *settings)
{
    pharaoh_projection_records_update(parts, PHARAOH_SUN_TEMPLE_PART_COUNT,
        settings);
}
