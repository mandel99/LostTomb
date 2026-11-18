#ifndef PHARAOH_RUNTIME_TAX_SENTIMENT_MODEL_H
#define PHARAOH_RUNTIME_TAX_SENTIMENT_MODEL_H

#include <stdint.h>

#include "pharaoh/runtime/model_difficulty.h"

enum {
    PHARAOH_TAX_MODEL_TAX_PERCENTAGES = 26,
    PHARAOH_TAX_MODEL_INDEX_COUNT = 11,
    PHARAOH_TAX_MODEL_SCRATCH_BYTES = 100000
};

typedef enum PharaohTaxSentimentModelStatus {
    PHARAOH_TAX_MODEL_NO_SPARE_SCREEN,
    /* Native executable reports the Figure model filename error for this file. */
    PHARAOH_TAX_MODEL_FIGURE_FILE_MISSING,
    PHARAOH_TAX_MODEL_LINE_COUNT_MISMATCH,
    PHARAOH_TAX_MODEL_LOADED
} PharaohTaxSentimentModelStatus;

typedef struct PharaohTaxSentimentModelOperations {
    void *user_data;
    void (*clear_bytes)(void *, void *, uint32_t, uint8_t);
    int32_t (*load_text)(void *, const char *, void *, uint32_t, int32_t);
    uint32_t (*text_length)(void *, const char *);
    int32_t (*continue_after_count_mismatch)(void *, uint32_t actual_plus_one,
                                              uint32_t expected);
    void (*report_status)(void *, PharaohTaxSentimentModelStatus);
} PharaohTaxSentimentModelOperations;

typedef struct PharaohTaxSentimentModel {
    int32_t by_tax_percentage[PHARAOH_TAX_MODEL_TAX_PERCENTAGES]
                             [PHARAOH_TAX_MODEL_INDEX_COUNT];
} PharaohTaxSentimentModel;

typedef struct PharaohTaxSentimentModelLoadContext {
    const PharaohTaxSentimentModelOperations *operations;
    void *spare_screen;
    PharaohModelDifficulty difficulty;
    PharaohTaxSentimentModel *model;
} PharaohTaxSentimentModelLoadContext;

int32_t pharaoh_tax_sentiment_model_load(
    PharaohTaxSentimentModelLoadContext *context);

#endif
