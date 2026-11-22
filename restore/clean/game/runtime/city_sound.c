#include "pharaoh/runtime/city_sound.h"

#include <stddef.h>
#include <string.h>

void pharaoh_sound_city_initialize(const PharaohCitySoundInitializeContext *c)
{
    static const struct { uint8_t record; int32_t sound; } presets[63] = {
        {1,30},
        {2,34},
        {3,38},
        {4,42},
        {5,46},
        {6,50},
        {7,51},
        {8,52},
        {9,53},
        {10,54},
        {11,55},
        {12,56},
        {13,57},
        {14,58},
        {15,62},
        {16,63},
        {17,64},
        {18,65},
        {19,66},
        {20,67},
        {21,68},
        {22,69},
        {23,70},
        {24,74},
        {25,78},
        {26,79},
        {27,80},
        {28,81},
        {29,82},
        {30,83},
        {31,87},
        {32,89},
        {34,93},
        {35,95},
        {36,97},
        {37,98},
        {38,99},
        {39,100},
        {40,101},
        {41,102},
        {42,106},
        {43,110},
        {44,111},
        {45,112},
        {46,113},
        {47,114},
        {48,115},
        {49,116},
        {50,117},
        {51,118},
        {52,119},
        {53,120},
        {54,121},
        {55,122},
        {56,123},
        {57,124},
        {58,125},
        {59,126},
        {60,127},
        {61,128},
        {62,132},
        {63,133},
        {33,91}
    };
    *c->last_update_tick = c->get_tick(c->clock_user_data);
    memset(c->records, 0, 0x2300);
    const uint32_t tick = *c->last_update_tick;
    for (uint32_t i = 1; i < 70; ++i)
        c->records[i][20] = (int32_t)tick;
    for (uint32_t i = 0; i < 63; ++i) {
        int32_t *const record = c->records[presets[i].record];
        record[2] = 200;
        record[9] = 1;
        record[10] = presets[i].sound;
        record[18] = 1;
        record[21] = 30000;
    }
}

_Static_assert(sizeof(PharaohCitySoundViewRecord) == 0x80 &&
    offsetof(PharaohCitySoundViewRecord, direction_buildings) == 0x0c,
    "native city sound-view record layout");

/* Complete C translation of Pharaoh.exe 0x0041D410. */
void pharaoh_sound_city_mark_building_view(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_id, int32_t direction,
    const int32_t *building_type_sound_ids, int32_t sound_suppressed,
    PharaohCitySoundViewRecord *sound_views)
{
    const PharaohBuildingTypeRecord *building = &buildings[building_id];
    int32_t sound_id;
    if (building->state != 1u) return;
    sound_id = building_type_sound_ids[building->building_type];
    if (sound_id < 1) return;
    if (building->building_type == 0x1e || building->building_type == 0x1f ||
        building->building_type == 0x20 || building->building_type == 0x21 ||
        building->building_type == 0xe2) {
        if (sound_suppressed > 0 || building->num_workers < 1) return;
    }
    sound_views[sound_id].active = 1;
    ++sound_views[sound_id].total_buildings;
    ++sound_views[sound_id].direction_buildings[direction];
}

static int32_t tick_delta(int32_t current_tick, int32_t previous_tick)
{
    return (int32_t)((uint32_t)current_tick - (uint32_t)previous_tick);
}

/* Complete C translation of Pharaoh.exe 0x0041D560. The native sound leaf
 * 0x0047D350 always returns zero and has no side effects, so only the proven
 * scheduler state changes remain observable. */
void pharaoh_sound_city_play(int32_t records[70][32], int32_t current_tick,
    int32_t *last_play_tick, int32_t *last_sound_id)
{
    int32_t selected = 0;
    int32_t longest_elapsed = 0;

    for (int32_t i = 1; i < 70; ++i) {
        records[i][22] = 0;
        if (records[i][0] < 1) {
            records[i][1] = 0;
            for (int32_t field = 3; field <= 7; ++field)
                records[i][field] = 0;
        } else {
            records[i][0] = 0;
            if (records[i][2] <= records[i][1] &&
                records[i][21] <= tick_delta(current_tick, records[i][20]))
                records[i][22] = 1;
        }
    }
    if (tick_delta(current_tick, *last_play_tick) <= 1999) return;
    for (int32_t i = 1; i < 70; ++i) {
        const int32_t elapsed = tick_delta(current_tick, records[i][20]);
        if (records[i][22] > 0 && elapsed > longest_elapsed) {
            selected = i;
            longest_elapsed = elapsed;
        }
    }
    if (selected == 0) return;
    for (int32_t attempt = 0; attempt < 8; ++attempt) {
        if (++records[selected][8] >= records[selected][9])
            records[selected][8] = 0;
        *last_sound_id = records[selected][10 + records[selected][8]];
    }
    *last_play_tick = current_tick;
    records[selected][20] = current_tick;
    records[selected][1] = 0;
    for (int32_t field = 3; field <= 7; ++field)
        records[selected][field] = 0;
    ++records[selected][19];
}

/* Complete full-EAX leaf translation of Pharaoh.exe 0x0053B830. */
int32_t pharaoh_sound_city_building_ambient_category(int32_t building_type)
{
    switch (building_type) {
        case 0x06: case 0x0a: case 0x0b: case 0x0c: case 0x0d:
        case 0x0e: case 0x0f: case 0x10: case 0x11: case 0x12:
        case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
        case 0x18: case 0x19: case 0x1a: case 0x1b: case 0x1c:
        case 0x1d: case 0x1e: case 0x1f: case 0x20: case 0x21:
        case 0x22: case 0x23: case 0x24: case 0x25: case 0x26:
        case 0x27: case 0x28: case 0x29: case 0x2a: case 0x2b:
        case 0x2c: case 0x2d: case 0x2f: case 0x31: case 0x33:
        case 0x35: case 0x37: case 0x3a: case 0x3b: case 0x46:
        case 0x4d: case 0x4e: case 0x4f: case 0x51: case 0x5a:
        case 0x5c: case 0x5e: case 0x5f: case 0x87: case 0x8c:
        case 0x8d: case 0x8e: case 0x8f: case 0x90: case 0x91:
        case 0x97: case 0x98: case 0x99: case 0x9a: case 0x9b:
        case 0x9c: case 0x9d: case 0x9e: case 0x9f: case 0xa0:
        case 0xa7: case 0xa8: case 0xa9: case 0xaa: case 0xab:
        case 0xac: case 0xad: case 0xb8: case 0xce: case 0xe2:
            return 1;
        case 0x47: case 0x48: case 0x49: case 0x4a: case 0x4b:
        case 0x4c: case 0x6a: case 0x6b: case 0x6c: case 0x6d:
        case 0x6e: case 0x6f: case 0x70: case 0x71: case 0x72:
        case 0x73: case 0x88: case 0xa1: case 0xa2: case 0xb1:
        case 0xb2: case 0xb3: case 0xb5: case 0xb6: case 0xcc:
        case 0xcd: case 0xd9: case 0xdd: case 0xe7:
            return 2;
        case 0x56: case 0x57: case 0x65: case 0x66:
        case 0x67: case 0x68: case 0xe0:
            return 3;
        case 0xb7: case 0xd2:
            return 4;
        default:
            return 0;
    }
}
