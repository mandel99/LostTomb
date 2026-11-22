#include "pharaoh/runtime/game_file_upgrade.h"

#include <string.h>

/* Complete C translation of Pharaoh.exe 0x004D32D0. */
void pharaoh_game_file_upgrade_clear_walker_fields(
    PharaohUpgradeWalkerRecord records[PHARAOH_UPGRADE_WALKER_RECORD_COUNT])
{
    for (int32_t index = 0; index < PHARAOH_UPGRADE_WALKER_RECORD_COUNT;
         ++index) {
        records[index].cleared_field = 0;
        records[index].reset_word = UINT16_C(0xffff);
    }
}

/* Complete C translation of Pharaoh.exe 0x004D3D80. */
void pharaoh_game_file_upgrade_event_fields(
    PharaohUpgradeEventRecord records[PHARAOH_UPGRADE_EVENT_RECORD_COUNT])
{
    for (int32_t index = 0; index < PHARAOH_UPGRADE_EVENT_RECORD_COUNT;
         ++index) {
        records[index].reset_fields[0] = 6;
        records[index].reset_fields[1] = 6;
        records[index].reset_fields[2] = 6;
        records[index].reset_fields[3] = 6;
    }
}

/* Complete zero-callee C translation of Pharaoh.exe 0x004D18F0. */
void pharaoh_game_file_relayout_empire_cities(
    PharaohUpgradeEmpireCityRecord records[PHARAOH_UPGRADE_EMPIRE_CITY_RECORD_COUNT])
{
    for (int32_t index = 0; index < PHARAOH_UPGRADE_EMPIRE_CITY_RECORD_COUNT;
         ++index) {
        uint8_t converted[0x6a] = {0};
        const uint8_t *source = records[index].bytes;
        memcpy(converted, source, 0x24);
        memcpy(converted + 0x2a, source + 0x24, 0x1e);
        memcpy(converted + 0x4e, source + 0x42, 0x1c);
        memcpy(records[index].bytes, converted, sizeof(converted));
    }
}

/* Complete zero-callee C translation of Pharaoh.exe 0x004D19E0. */
void pharaoh_game_file_upgrade_empire_object_flags(
    PharaohUpgradeEmpireObjectRecord records[PHARAOH_UPGRADE_EMPIRE_OBJECT_RECORD_COUNT])
{
    for (int32_t index = 0; index < PHARAOH_UPGRADE_EMPIRE_OBJECT_RECORD_COUNT;
         ++index) {
        uint8_t converted[0x62] = {0};
        const uint8_t *source = records[index].bytes;
        uint32_t high;
        uint32_t medium;
        uint32_t low;
        memcpy(converted, source, 0x4c);
        memcpy(&high, source + 0x40, sizeof(high));
        memcpy(&medium, source + 0x44, sizeof(medium));
        memcpy(&low, source + 0x48, sizeof(low));
        for (uint32_t bit = 0; bit < 32; ++bit) {
            const uint32_t mask = UINT32_C(1) << bit;
            converted[0x3e + bit] = (high & mask) != 0u ? 3u :
                (medium & mask) != 0u ? 2u : (low & mask) != 0u ? 1u : 0u;
        }
        memset(converted + 0x5e, 2, 4);
        memcpy(records[index].bytes, converted, sizeof(converted));
    }
}

/* Complete zero-callee C translation of Pharaoh.exe 0x004D3200. */
void pharaoh_game_file_relayout_storages(
    PharaohUpgradeStorageRecord records[PHARAOH_UPGRADE_STORAGE_RECORD_COUNT])
{
    for (int32_t index = 0; index < PHARAOH_UPGRADE_STORAGE_RECORD_COUNT;
         ++index) {
        uint8_t converted[0xc4] = {0};
        const uint8_t *source = records[index].bytes;
        memcpy(converted, source, 0x6c);
        memcpy(converted + 0x0a, source + 0x0a, 0x1e);
        memset(converted + 0x28, 0, 6); /* native004D323C/004D3244 */
        memcpy(converted + 0x2e, source + 0x28, 4);
        memcpy(converted + 0x32, source + 0x2c, 2);
        memcpy(converted + 0x34, source + 0x2e, 0x3c);
        memset(converted + 0x70, 0, 0x0c);
        memcpy(records[index].bytes, converted, sizeof(converted));
    }
}

/* Complete zero-callee C translation of Pharaoh.exe 0x004D18B0. */
void pharaoh_game_file_upgrade_widen_player_counts(
    PharaohUpgradePlayerRecord records[PHARAOH_UPGRADE_PLAYER_RECORD_COUNT])
{
    for (int32_t player = 0; player < PHARAOH_UPGRADE_PLAYER_RECORD_COUNT;
         ++player) {
        uint8_t *record = records[player].bytes;
        for (int32_t index = 0; index < 30; ++index) {
            int16_t first;
            int16_t second;
            const int32_t widened_first_offset = 0x4860 + index * 4;
            const int32_t widened_second_offset = 0x48f0 + index * 4;
            memcpy(&first, record + 0x2796 + index * 2, sizeof(first));
            memcpy(&second, record + 0x27de + index * 2, sizeof(second));
            {
                const int32_t widened = first;
                memcpy(record + widened_first_offset, &widened, sizeof(widened));
            }
            {
                const int32_t widened = second;
                memcpy(record + widened_second_offset, &widened, sizeof(widened));
            }
        }
    }
}

/* Complete zero-callee C translation of Pharaoh.exe 0x004D17E0. */
void pharaoh_game_file_upgrade_empire_object_states(
    PharaohUpgradeEmpireObjectRecord records[PHARAOH_UPGRADE_EMPIRE_OBJECT_RECORD_COUNT])
{
    for (int32_t index = 0; index < PHARAOH_UPGRADE_EMPIRE_OBJECT_RECORD_COUNT;
         ++index) {
        uint8_t *record = records[index].bytes;
        if (record[1] == 0 || record[0] != 1) continue;
        switch (record[0x18]) {
        case 0: case 3: case 5: record[0x18] = 4; break;
        case 1: record[0x18] = 0; break;
        case 2: record[0x18] = 1; break;
        case 4: case 6: record[0x18] = 6; break;
        default: break;
        }
    }
}

/* Complete zero-callee C translation of Pharaoh.exe 0x004D4180. */
void pharaoh_game_file_upgrade_building_orientation(
    PharaohUpgradeBuildingRecord records[PHARAOH_UPGRADE_BUILDING_RECORD_COUNT])
{
    /* Possible native bug: EDX is cleared once before the loop (004D4181),
     * so a flagged statue also marks later unflagged statues. Preserve it. */
    uint8_t persistent_flag = 0;
    for (int32_t id = 1; id < PHARAOH_UPGRADE_BUILDING_RECORD_COUNT; ++id) {
        uint8_t *record = records[id].bytes;
        int16_t type;
        uint32_t value;
        if (record[0] != 1) continue;
        memcpy(&type, record + 0x10, sizeof(type));
        if (type != 0x29 && type != 0x2a && type != 0x2b) continue;
        value = record[0xaa];
        if ((value & 0x40u) != 0u) {
            persistent_flag = 1;
            value -= 0x40u;
        }
        ++value;
        if (value == 4u) value = 0;
        record[0xaa] = (uint8_t)((persistent_flag << 4) + value);
    }
}

/* Complete zero-callee C translation of Pharaoh.exe 0x004D3D00. */
void pharaoh_game_file_rebuild_ship_walker_ids(
    PharaohUpgradeFormationRecord formations[50],
    const PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT])
{
    for (int32_t formation = 0; formation < 50; ++formation) {
        const int32_t walker_id = formations[formation].ship_walker_id;
        const uint8_t type = ((const uint8_t *)&walkers[walker_id])[0x0a];
        if (type != 0x4d && type != 0x5c && type != 0x65)
            formations[formation].ship_walker_id = 0;
    }
    for (int32_t walker_id = 1; walker_id < PHARAOH_WALKER_RECORD_COUNT;
         ++walker_id) {
        const uint8_t *walker = (const uint8_t *)&walkers[walker_id];
        if (walker[0x0a] == 0x4d && walker[0x0e] == 1 &&
            walker[0x38] != 4 && walker[0x130] != 0) {
            formations[walker[0x92]].ship_walker_id = walker_id;
        }
    }
}
