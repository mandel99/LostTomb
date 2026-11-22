#ifndef PHARAOH_SCENARIO_MISSION_RECORDS_H
#define PHARAOH_SCENARIO_MISSION_RECORDS_H

#include <stdint.h>

enum {
    PHARAOH_MISSION_LEGION_KINDS = 3,
    PHARAOH_MISSION_LEGION_NONE = 0xff
};

typedef struct PharaohMissionLegionRecord {
    uint8_t legion_id;
    uint8_t reserved_01[3];
    int32_t experience;
    uint8_t soldiers;
    uint8_t reserved_09[3];
} PharaohMissionLegionRecord;

typedef struct PharaohMissionCarryRecords {
    int32_t savings;
    PharaohMissionLegionRecord legions[PHARAOH_MISSION_LEGION_KINDS];
} PharaohMissionCarryRecords;

typedef struct PharaohMissionRecordsState {
    PharaohMissionCarryRecords finished;
    PharaohMissionCarryRecords started;
} PharaohMissionRecordsState;

void pharaoh_mission_records_reset(PharaohMissionRecordsState *state);

#endif
