#ifndef PHARAOH_RUNTIME_GAME_FILE_UPGRADE_H
#define PHARAOH_RUNTIME_GAME_FILE_UPGRADE_H

#include <stdint.h>

#include "pharaoh/walker/record_queries.h"

enum {
    PHARAOH_UPGRADE_WALKER_RECORD_COUNT = 1999,
    PHARAOH_UPGRADE_EVENT_RECORD_COUNT = 150,
    PHARAOH_UPGRADE_EMPIRE_CITY_RECORD_COUNT = 61,
    PHARAOH_UPGRADE_EMPIRE_OBJECT_RECORD_COUNT = 200,
    PHARAOH_UPGRADE_STORAGE_RECORD_COUNT = 200,
    PHARAOH_UPGRADE_PLAYER_RECORD_COUNT = 2,
    PHARAOH_UPGRADE_BUILDING_RECORD_COUNT = 4000
};

/* Legacy clear cursor anchored at native walker+2, with388-byte stride. */
typedef struct PharaohUpgradeWalkerRecord {
    uint8_t cleared_field;
    uint8_t unknown_001[0x12f];
    uint16_t reset_word;
    uint8_t unknown_132[0x52];
} PharaohUpgradeWalkerRecord;

typedef struct PharaohUpgradeEventRecord {
    uint8_t reset_fields[4];
    uint8_t unknown_04[0x78];
} PharaohUpgradeEventRecord;

typedef struct PharaohUpgradeEmpireCityRecord {
    uint8_t bytes[0x6a];
} PharaohUpgradeEmpireCityRecord;

typedef struct PharaohUpgradeEmpireObjectRecord {
    uint8_t bytes[0x62];
} PharaohUpgradeEmpireObjectRecord;

typedef struct PharaohUpgradeStorageRecord {
    uint8_t bytes[0xc4];
} PharaohUpgradeStorageRecord;

typedef struct PharaohUpgradePlayerRecord {
    uint8_t bytes[0x49d8];
} PharaohUpgradePlayerRecord;

typedef struct PharaohUpgradeBuildingRecord {
    uint8_t bytes[0x108];
} PharaohUpgradeBuildingRecord;

typedef struct PharaohUpgradeFormationRecord {
    uint8_t unknown_00[0x84];
    int32_t ship_walker_id;
    uint8_t unknown_88[8];
} PharaohUpgradeFormationRecord;

_Static_assert(sizeof(PharaohUpgradeWalkerRecord) == 0x184,
    "native upgraded walker record size changed");
_Static_assert(sizeof(PharaohUpgradeEventRecord) == 0x7c,
    "native upgraded event record size changed");
_Static_assert(sizeof(PharaohUpgradeEmpireCityRecord) == 0x6a &&
    sizeof(PharaohUpgradeEmpireObjectRecord) == 0x62 &&
    sizeof(PharaohUpgradeStorageRecord) == 0xc4,
    "native relayout record size changed");
_Static_assert(sizeof(PharaohUpgradePlayerRecord) == 0x49d8,
    "native player record size changed");
_Static_assert(sizeof(PharaohUpgradeBuildingRecord) == 0x108,
    "native building record size changed");
_Static_assert(sizeof(PharaohUpgradeFormationRecord) == 0x90 &&
    offsetof(PharaohUpgradeFormationRecord, ship_walker_id) == 0x84,
    "native formation record size changed");

void pharaoh_game_file_upgrade_clear_walker_fields(
    PharaohUpgradeWalkerRecord records[PHARAOH_UPGRADE_WALKER_RECORD_COUNT]);
void pharaoh_game_file_upgrade_event_fields(
    PharaohUpgradeEventRecord records[PHARAOH_UPGRADE_EVENT_RECORD_COUNT]);
void pharaoh_game_file_relayout_empire_cities(
    PharaohUpgradeEmpireCityRecord records[PHARAOH_UPGRADE_EMPIRE_CITY_RECORD_COUNT]);
void pharaoh_game_file_upgrade_empire_object_flags(
    PharaohUpgradeEmpireObjectRecord records[PHARAOH_UPGRADE_EMPIRE_OBJECT_RECORD_COUNT]);
void pharaoh_game_file_relayout_storages(
    PharaohUpgradeStorageRecord records[PHARAOH_UPGRADE_STORAGE_RECORD_COUNT]);
void pharaoh_game_file_upgrade_widen_player_counts(
    PharaohUpgradePlayerRecord records[PHARAOH_UPGRADE_PLAYER_RECORD_COUNT]);
void pharaoh_game_file_upgrade_empire_object_states(
    PharaohUpgradeEmpireObjectRecord records[PHARAOH_UPGRADE_EMPIRE_OBJECT_RECORD_COUNT]);
void pharaoh_game_file_upgrade_building_orientation(
    PharaohUpgradeBuildingRecord records[PHARAOH_UPGRADE_BUILDING_RECORD_COUNT]);
void pharaoh_game_file_rebuild_ship_walker_ids(
    PharaohUpgradeFormationRecord formations[50],
    const PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT]);

#endif
