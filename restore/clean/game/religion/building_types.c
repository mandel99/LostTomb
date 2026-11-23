#include "pharaoh/religion/building_types.h"

enum {
    PHARAOH_GODS_WITH_TEMPLE_TYPES = 10,
    PHARAOH_BUILDING_SHRINE_OSIRIS = 140
};

int32_t pharaoh_religion_building_type_for_god(
    int32_t god, PharaohGodBuildingKind kind)
{
    static const int16_t temples[PHARAOH_GODS_WITH_TEMPLE_TYPES] = {
        60, 61, 62, 63, 64, 151, 152, 153, 154, 155
    };
    static const int16_t complexes[PHARAOH_GODS_WITH_TEMPLE_TYPES] = {
        65, 66, 67, 68, 69, 156, 157, 158, 159, 160
    };

    if (god < 0 || god >= PHARAOH_GODS_WITH_TEMPLE_TYPES) {
        return -1;
    }
    switch (kind) {
        case PHARAOH_GOD_BUILDING_TEMPLE:
            return temples[god];
        case PHARAOH_GOD_BUILDING_TEMPLE_COMPLEX:
            return complexes[god];
        case PHARAOH_GOD_BUILDING_SHRINE:
            return PHARAOH_BUILDING_SHRINE_OSIRIS + god;
        default:
            return -1;
    }
}

int32_t pharaoh_city_god_with_status(
    const int16_t statuses[5], int32_t status)
{
    int32_t god;
    for (god = 0; god < 5; ++god) {
        if (statuses[god] == status) return god;
    }
    return -1;
}
