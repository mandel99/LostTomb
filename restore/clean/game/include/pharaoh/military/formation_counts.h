#ifndef PHARAOH_MILITARY_FORMATION_COUNTS_H
#define PHARAOH_MILITARY_FORMATION_COUNTS_H

#include "pharaoh/walker/record_queries.h"
#include "pharaoh/events/event_queries.h"

typedef union PharaohFormationCountRecord {
    uint32_t words[36];
    struct {
        uint8_t state,reserved_01[2],all_members_flag;
        int16_t type;
        uint8_t reserved_06[2];
        int16_t members[16];
        uint8_t member_count,reserved_29[25],flag_42,reserved_43[7];
        uint16_t damage_total,model_total;
        uint8_t reserved_4e[12];
        uint16_t ready;
        uint8_t reserved_5c[11],flag_67,reserved_68,skip_69,reserved_6a[14];
        int16_t event_id;
        uint8_t reserved_7a[22];
    } fields;
} PharaohFormationCountRecord;
typedef struct PharaohFormationUnitModel {
    uint16_t value;
    uint8_t opaque_02[54];
} PharaohFormationUnitModel;
typedef struct PharaohFormationCountContext {
    PharaohFormationCountRecord *formations; /* 50 recordsBCB9C8, stride144 */
    PharaohWalkerQueryRecord *walkers; /* 2000 recordsBCD5F8, stride388 */
    const PharaohFormationUnitModel *models; /* 7090D0, stride56 */
    uint32_t *totals; /* four DWORDsD05630..D0563C */
    int32_t *event_ids; /* scratchBCA45C, capacity49 */
    uint8_t *event_flags; /* scratchBCA428, capacity49 */
    uint8_t *player_counters; /* actualEA5ED5 + current player*49D8 */
    PharaohScenarioEvent *events; /* 785D74, stride124 */
    const int32_t *event_count; /* 785D70 */
} PharaohFormationCountContext;

/* Complete004B7C80. Borrow genuine shared native pools/counters. Retain
   fixed scans, BYTE/WORD wraps, native byte-count readiness scan and real C
   children. Event/model fields without known semantics remain opaque. */
void pharaoh_formation_calculate_figures(const PharaohFormationCountContext *);

#endif
