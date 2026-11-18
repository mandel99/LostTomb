#ifndef PHARAOH_RUNTIME_FIGURE_MODEL_LOADING_H
#define PHARAOH_RUNTIME_FIGURE_MODEL_LOADING_H

#include <stdint.h>

#include "pharaoh/runtime/model_difficulty.h"
#include "pharaoh/walker/model_types.h"

enum {
    PHARAOH_FIGURE_MODEL_EXPECTED_ROWS =
        PHARAOH_FIGURE_MODEL_RECORD_COUNT +
        PHARAOH_ENEMY_MODEL_NATION_COUNT * PHARAOH_ENEMY_MODEL_TYPE_COUNT,
    PHARAOH_FIGURE_MODEL_SCRATCH_BYTES = 100000
};

typedef enum PharaohFigureModelStatus {
    PHARAOH_FIGURE_MODEL_NO_SPARE_SCREEN,
    PHARAOH_FIGURE_MODEL_FILE_MISSING,
    PHARAOH_FIGURE_MODEL_ROW_COUNT_MISMATCH,
    PHARAOH_FIGURE_MODEL_LOADED
} PharaohFigureModelStatus;

typedef struct PharaohFigureModelOperations {
    void *user_data;
    void (*clear_bytes)(void *, void *, uint32_t, uint8_t);
    int32_t (*load_text)(void *, const char *, void *, uint32_t, int32_t);
    uint32_t (*text_length)(void *, const char *);
    int32_t (*continue_after_count_mismatch)(void *, uint32_t actual_plus_one,
                                              uint32_t expected);
    void (*report_status)(void *, PharaohFigureModelStatus);
} PharaohFigureModelOperations;

typedef struct PharaohFigureModelLoadContext {
    const PharaohFigureModelOperations *operations;
    void *spare_screen;
    PharaohModelDifficulty difficulty;
    PharaohFigureModelTable *figure_models;
    PharaohEnemyModelTable *enemy_models;
} PharaohFigureModelLoadContext;

int32_t pharaoh_figure_model_load(PharaohFigureModelLoadContext *context);

#endif
