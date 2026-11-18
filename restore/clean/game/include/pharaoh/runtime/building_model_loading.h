#ifndef PHARAOH_RUNTIME_BUILDING_MODEL_LOADING_H
#define PHARAOH_RUNTIME_BUILDING_MODEL_LOADING_H

#include <stdint.h>

#include "pharaoh/building/model_types.h"
#include "pharaoh/runtime/model_difficulty.h"

enum {
    PHARAOH_BUILDING_MODEL_EXPECTED_ROWS = 257,
    PHARAOH_BUILDING_MODEL_SCRATCH_BYTES = 100000
};

typedef enum PharaohBuildingModelStatus {
    PHARAOH_BUILDING_MODEL_NO_SPARE_SCREEN,
    PHARAOH_BUILDING_MODEL_FILE_MISSING,
    PHARAOH_BUILDING_MODEL_ROW_COUNT_MISMATCH,
    PHARAOH_BUILDING_MODEL_LOADED
} PharaohBuildingModelStatus;

typedef struct PharaohBuildingModelOperations {
    void *user_data;
    void (*clear_bytes)(void *, void *, uint32_t, uint8_t);
    int32_t (*load_text)(void *, const char *, void *, uint32_t, int32_t);
    uint32_t (*text_length)(void *, const char *);
    int32_t (*continue_after_count_mismatch)(void *, uint32_t actual_plus_one,
                                              uint32_t expected);
    void (*report_status)(void *, PharaohBuildingModelStatus);
} PharaohBuildingModelOperations;

typedef struct PharaohBuildingModelLoadContext {
    const PharaohBuildingModelOperations *operations;
    void *spare_screen;
    PharaohModelDifficulty difficulty;
    PharaohBuildingModelRecord *building_models;
    PharaohHouseModel *house_models;
} PharaohBuildingModelLoadContext;

int32_t pharaoh_building_model_load(PharaohBuildingModelLoadContext *context);

#endif
