#ifndef PHARAOH_SCENARIO_CAMPAIGN_RECORDS_H
#define PHARAOH_SCENARIO_CAMPAIGN_RECORDS_H

#include "pharaoh/runtime/settings_ini.h"

#include <stddef.h>
#include <stdint.h>

typedef struct PharaohMissionMonumentRecord {
    uint8_t active;
    uint8_t reserved_01;
    uint8_t result_id;
    uint8_t reserved_03;
    uint16_t type;
    uint8_t reserved_06[0x22];
    uint8_t tie_breaker;
    uint8_t reserved_29[0x19];
    uint8_t eligible;
    uint8_t reserved_43[0x37];
    int16_t value;
    uint8_t reserved_7c[0x14];
} PharaohMissionMonumentRecord;

typedef struct PharaohMissionBuildingRecord {
    uint8_t active;
    uint8_t reserved_01[5];
    uint16_t x;
    uint16_t y;
    uint8_t reserved_0a[6];
    uint16_t type;
    int16_t parent_offset;
    uint8_t reserved_14[84];
    uint16_t packed_low;
    uint8_t reserved_6a[2];
    uint16_t packed_first;
    uint8_t reserved_6e[2];
    uint16_t packed_high;
    uint8_t reserved_72[150];
} PharaohMissionBuildingRecord;

typedef struct PharaohMissionWinner {
    uint8_t result_id;
    uint8_t reserved_01[3];
    int32_t value;
    uint8_t tie_breaker;
    uint8_t reserved_09[3];
} PharaohMissionWinner;

typedef struct PharaohMissionSpecialPositions {
    uint16_t type_e5_x;
    uint16_t type_e5_y;
    uint16_t type_ea_x;
    uint16_t type_ea_y;
    uint16_t type_eb_x;
    uint16_t type_eb_y;
    uint16_t type_de_packed;
    uint16_t type_de_first;
    uint16_t type_e1_x;
    uint16_t type_e1_y;
    uint16_t type_e4_x;
    uint16_t type_e4_y;
} PharaohMissionSpecialPositions;

typedef struct PharaohMissionRecordState {
    const PharaohSettingsFile *settings;
    uint32_t current_personal_savings;
    uint32_t *stored_personal_savings;
    const PharaohMissionMonumentRecord *monuments;
    int32_t monument_count;
    const PharaohMissionBuildingRecord *buildings;
    int32_t building_count;
    PharaohMissionWinner *winners;
    PharaohMissionSpecialPositions *special_positions;
} PharaohMissionRecordState;

typedef struct PharaohMissionRecordOperations {
    void *user_data;
    const char *(*select_text)(void *user_data, int32_t group,
        int32_t entry);
    void (*save_profile)(void *user_data);
} PharaohMissionRecordOperations;

_Static_assert(sizeof(PharaohMissionMonumentRecord) == 0x90,
    "native monument summary stride");
_Static_assert(offsetof(PharaohMissionMonumentRecord, type) == 4 &&
    offsetof(PharaohMissionMonumentRecord, tie_breaker) == 0x28 &&
    offsetof(PharaohMissionMonumentRecord, eligible) == 0x42 &&
    offsetof(PharaohMissionMonumentRecord, value) == 0x7a,
    "native monument summary offsets");
_Static_assert(sizeof(PharaohMissionBuildingRecord) == 0x108 &&
    offsetof(PharaohMissionBuildingRecord, type) == 0x10 &&
    offsetof(PharaohMissionBuildingRecord, packed_low) == 0x68 &&
    offsetof(PharaohMissionBuildingRecord, packed_first) == 0x6c &&
    offsetof(PharaohMissionBuildingRecord, packed_high) == 0x70,
    "native building summary layout");
_Static_assert(sizeof(PharaohMissionWinner) == 12,
    "native mission winner layout");

/* Complete typed translation of native 0x004188F0. */
void pharaoh_mission_records_store(PharaohMissionRecordState *state,
    const PharaohMissionRecordOperations *operations);

#endif
