#ifndef PHARAOH_RUNTIME_TRADE_RECOMMENDATIONS_H
#define PHARAOH_RUNTIME_TRADE_RECOMMENDATIONS_H

#include <stdint.h>

#include "pharaoh/runtime/model_text.h"

enum {
    PHARAOH_TRADE_RECOMMENDATION_ROWS = 4,
    PHARAOH_TRADE_RECOMMENDATION_COLUMNS = 6,
    PHARAOH_TRADE_MODEL_SCRATCH_BYTES = 100000
};

typedef enum PharaohTradeModelStatus {
    PHARAOH_TRADE_MODEL_NO_SPARE_SCREEN,
    PHARAOH_TRADE_MODEL_FILE_MISSING,
    PHARAOH_TRADE_MODEL_RECOMMENDATION_COUNT_MISMATCH,
    PHARAOH_TRADE_MODEL_LOADED
} PharaohTradeModelStatus;

typedef struct PharaohTradeModelOperations {
    void *user_data;
    void (*clear_bytes)(void *, void *, uint32_t, uint8_t);
    int32_t (*load_text)(void *, const char *, void *, uint32_t, int32_t);
    uint32_t (*text_length)(void *, const char *);
    int32_t (*continue_after_count_mismatch)(void *, uint32_t actual_plus_one,
                                              uint32_t expected);
    void (*report_status)(void *, PharaohTradeModelStatus);
} PharaohTradeModelOperations;

typedef struct PharaohTradeModelLoadContext {
    const PharaohTradeModelOperations *operations;
    void *spare_screen;
    int32_t recommendations[PHARAOH_TRADE_RECOMMENDATION_ROWS]
                           [PHARAOH_TRADE_RECOMMENDATION_COLUMNS];
} PharaohTradeModelLoadContext;

int32_t pharaoh_trade_recommendations_load(PharaohTradeModelLoadContext *context);

#endif
