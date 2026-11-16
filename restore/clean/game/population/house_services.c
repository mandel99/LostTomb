#include "pharaoh/population/house_services.h"

static void decay(uint8_t *value) { *value = *value > 1 ? (uint8_t)(*value - 1) : 0; }

void pharaoh_house_decay_culture(PharaohServiceBuilding *buildings, size_t count)
{
    size_t i, god;
    for (i = 0; i < count; ++i) {
        PharaohHouseServices *h;
        if (!buildings[i].in_use || !buildings[i].house_size) continue;
        h = &buildings[i].house;
        decay(&h->juggler); decay(&h->musician); decay(&h->dancer);
        decay(&h->senet); decay(&h->zoo); decay(&h->courthouse);
        decay(&h->school); decay(&h->library); decay(&h->dentist);
        decay(&h->apothecary); decay(&h->physician); decay(&h->water_supply);
        decay(&h->mortuary);
        for (god = 0; god < PHARAOH_HOUSE_GODS; ++god) decay(&h->temple[god]);
    }
}

void pharaoh_house_calculate_culture(PharaohServiceBuilding *buildings,
    size_t count, const int32_t venue_coverage[PHARAOH_HOUSE_GODS])
{
    size_t i, god;
    int32_t average = 0;
    for (god = 0; god < PHARAOH_HOUSE_GODS; ++god) average += venue_coverage[god];
    average /= PHARAOH_HOUSE_GODS;
    for (i = 0; i < count; ++i) {
        PharaohHouseServices *h;
        int8_t entertainment;
        if (!buildings[i].in_use || !buildings[i].house_size) continue;
        h = &buildings[i].house;
        h->education = 0; h->health = 0; h->num_gods = 0;
        entertainment = (int8_t)(average / 5);
        if (h->juggler) entertainment = (int8_t)((uint8_t)entertainment + 10u);
        if (h->musician) entertainment = (int8_t)((uint8_t)entertainment + 20u);
        if (h->dancer) entertainment = (int8_t)((uint8_t)entertainment + 30u);
        if (h->senet) entertainment = (int8_t)((uint8_t)entertainment + 40u);
        if (h->zoo) entertainment = entertainment <= 60
            ? (int8_t)((uint8_t)entertainment + 40u) : (int8_t)100;
        if (entertainment > 100) entertainment = 100;
        h->entertainment = entertainment;
        if (h->school || h->library) h->education = (h->school && h->library) ? 2u : 1u;
        for (god = 0; god < PHARAOH_HOUSE_GODS; ++god) if (h->temple[god]) ++h->num_gods;
        if (h->physician) ++h->health;
        if (h->mortuary) ++h->health;
    }
}

void pharaoh_buildings_update_desirability(PharaohServiceBuilding *buildings,
    size_t count, const int8_t *desirability, const uint8_t *elevation,
    int32_t (*square_offset)(void *, int32_t, int32_t), void *user_data)
{
    static const uint8_t elevation_bonus[6] = {0, 10, 12, 14, 16, 18};
    size_t i;
    for (i = 0; i < count; ++i) {
        PharaohServiceBuilding *b = &buildings[i];
        int32_t value, row, col;
        uint8_t level;
        if (!b->in_use) continue;
        if (b->size == 1) value = desirability[b->grid_offset];
        else {
            value = -9999;
            for (row = 0; row < b->size; ++row) for (col = 0; col < b->size; ++col) {
                int32_t candidate = desirability[b->grid_offset + square_offset(user_data, row, col)];
                if (candidate > value) value = candidate;
            }
        }
        b->desirability = (int8_t)value;
        if (b->adjacent_to_water) b->desirability = (int8_t)((uint8_t)b->desirability + 10u);
        level = elevation[b->grid_offset];
        if (level > 5) level = 5;
        b->desirability = (int8_t)((uint8_t)b->desirability + elevation_bonus[level]);
    }
}
