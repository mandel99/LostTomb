#ifndef PHARAOH_POPULATION_HEALTH_COVERAGE_H
#define PHARAOH_POPULATION_HEALTH_COVERAGE_H

#include <stddef.h>
#include <stdint.h>

typedef struct PharaohHealthCoverageBuilding {
    uint8_t state;
    uint8_t reserved_001[4];
    uint8_t house_size;
    uint8_t reserved_006[0x16];
    int16_t population;
    uint8_t reserved_01e[0x50];
    uint8_t coverage_6e;
    uint8_t reserved_06f[6];
    uint8_t coverage_75;
    uint8_t coverage_76;
    uint8_t reserved_077;
    uint8_t coverage_78;
    uint8_t reserved_079[0x8f];
} PharaohHealthCoverageBuilding;

void pharaoh_city_culture_update_health_coverage(
    const PharaohHealthCoverageBuilding *buildings, size_t building_count,
    int32_t *coverage_76, int32_t *coverage_78,
    int32_t *coverage_6e, int32_t *coverage_75);

#endif
