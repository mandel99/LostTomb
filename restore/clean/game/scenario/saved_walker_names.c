#include "pharaoh/scenario/saved_walker_names.h"
#include "pharaoh/building/type_queries.h"

static int32_t normal_name_group(uint8_t type)
{
    switch (type) {
    case 1: case 2: case 4: case 5: case 9: case 35: case 38:
    case 54: case 55: case 56: case 75: case 79: case 80: case 81:
    case 85: case 87: case 90: case 96: case 98: case 108: return 254;
    case 3: return 255;
    case 7: return 277;
    case 8: return 275;
    case 10: return 274;
    case 15: case 18: case 105: return 263;
    case 16: return 264;
    case 17: return 265;
    case 19: case 21: return 262;
    case 20: return 261;
    case 22: case 23: case 24: return 256;
    case 26: case 39: return 259;
    case 29: case 30: return 271;
    case 31: case 32: case 33: return 272;
    case 34: return 273;
    case 40: return 257;
    case 42: return 280;
    case 66: return 260;
    case 73: return 258;
    case 76: return 279;
    case 88: return 276;
    case 89: return 278;
    case 92: case 93: case 100: case 101: return 289;
    case 99: return 290;
    default: return -2;
    }
}

void pharaoh_saved_walker_names_repair(PharaohWalkerQueryRecord walkers[2000],
    const PharaohBuildingTypeRecord *buildings,
    const PharaohFormationTotalsRecord *formations,
    PharaohWalkerNameCounters *counters)
{
    static const int16_t enemies[12] = {
        285, 287, 286, 288, 284, 283, 282, 281, 300, 301, 302, 303
    };
    for (int32_t id = 1; id < 2000; ++id) {
        PharaohWalkerQueryRecord *const walker = &walkers[id];
        uint8_t type = walker->type;
        if (type == 91) {
            uint8_t replacement;
            switch (buildings[walker->building_id].building_type) {
            case 30: case 34: replacement = 16; break;
            case 31: case 32: case 36: case 226: replacement = 15; break;
            case 33: case 35: replacement = 17; break;
            case 51: case 53: replacement = 29; break;
            case 60: case 61: case 62: case 63: case 64:
            case 65: case 66: case 67: case 68: case 69:
            case 151: case 156: replacement = 27; break;
            default: replacement = 40; break;
            }
            walker->counting_action_00b = replacement;
            type = replacement;
        }
        int32_t group;
        if (type == 27) {
            const int32_t god = pharaoh_building_group_index(
                buildings, walker->building_id);
            if ((uint32_t)god >= 5u) continue;
            group = 266 + god;
        } else if (type == 43 || type == 44 || type == 45) {
            const uint8_t enemy = formations[walker->herd_target_id_60].enemy_type;
            group = enemy < 12 ? enemies[enemy] : -1;
        } else {
            group = normal_name_group(type);
        }
        walker->name_group = group;
        if (group == -2) {
            walker->name_id = -1;
        } else if (group >= 0) {
            const int32_t slot = group >= 300 ? 303 - group : 294 - group;
            int32_t maximum = 15;
            if (group == 254) maximum = 127;
            else if (group == 255 || group == 257 || group == 259 ||
                group == 263 || group == 264 || group == 271 ||
                group == 272 || group == 280) maximum = 31;
            else if (group == 288) maximum = 16;
            if (counters->by_slot[slot] > maximum)
                counters->by_slot[slot] = 0;
        }
    }
}
