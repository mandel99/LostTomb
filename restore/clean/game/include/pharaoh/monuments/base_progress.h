#ifndef PHARAOH_MONUMENTS_BASE_PROGRESS_H
#define PHARAOH_MONUMENTS_BASE_PROGRESS_H

#include "pharaoh/monuments/progress_reports.h"

typedef struct PharaohBaseMonumentProgress {
    PharaohBuildingTypeRecord *buildings;
    const PharaohWalkerQueryRecord *walkers;
    uint32_t parts; /*real receiver DWORD+4*/
} PharaohBaseMonumentProgress;

int32_t pharaoh_library_progress(PharaohBaseMonumentProgress *c,
    int32_t building,int32_t out[14]);
int32_t pharaoh_caesareum_progress(PharaohBaseMonumentProgress *c,
    int32_t building,int32_t out[8]);
int32_t pharaoh_lighthouse_progress(PharaohBaseMonumentProgress *c,
    int32_t building,int32_t out[8]);
int32_t pharaoh_abu_simbel_progress(const PharaohBuildingTypeRecord *buildings,
    int32_t building,int32_t out[4]);
uint8_t pharaoh_royal_tomb_worker_active(PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers,int32_t building);
int32_t pharaoh_royal_tomb_progress(PharaohBaseMonumentProgress *c,
    int32_t building,int32_t count,int32_t out[7]);
int32_t pharaoh_small_royal_tomb_progress(PharaohBaseMonumentProgress *c,
    int32_t building,int32_t out[7]);
int32_t pharaoh_medium_royal_tomb_progress(PharaohBaseMonumentProgress *c,
    int32_t building,int32_t out[7]);
int32_t pharaoh_large_royal_tomb_progress(PharaohBaseMonumentProgress *c,
    int32_t building,int32_t out[7]);
int32_t pharaoh_grand_royal_tomb_progress(PharaohBaseMonumentProgress *c,
    int32_t building,int32_t out[7]);

#endif
