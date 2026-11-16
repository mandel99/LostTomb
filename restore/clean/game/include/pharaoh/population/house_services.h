#ifndef PHARAOH_POPULATION_HOUSE_SERVICES_H
#define PHARAOH_POPULATION_HOUSE_SERVICES_H

#include <stddef.h>
#include <stdint.h>

#define PHARAOH_HOUSE_GODS 5

typedef struct PharaohHouseServices {
    uint8_t juggler, musician, dancer, senet, zoo, courthouse;
    uint8_t school, library, dentist, apothecary, physician;
    uint8_t water_supply, mortuary, temple[PHARAOH_HOUSE_GODS];
    uint8_t education, health, num_gods;
    int8_t entertainment;
} PharaohHouseServices;

typedef struct PharaohServiceBuilding {
    uint8_t in_use, house_size, size, adjacent_to_water;
    int32_t grid_offset;
    int8_t desirability;
    PharaohHouseServices house;
} PharaohServiceBuilding;

void pharaoh_house_decay_culture(PharaohServiceBuilding *buildings,
    size_t count);
void pharaoh_house_calculate_culture(PharaohServiceBuilding *buildings,
    size_t count, const int32_t venue_coverage[PHARAOH_HOUSE_GODS]);
void pharaoh_buildings_update_desirability(PharaohServiceBuilding *buildings,
    size_t count, const int8_t *desirability, const uint8_t *elevation,
    int32_t (*square_offset)(void *, int32_t, int32_t), void *user_data);

#endif
