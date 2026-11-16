#ifndef PHARAOH_UI_MONUMENT_TOKENS_H
#define PHARAOH_UI_MONUMENT_TOKENS_H

#include "pharaoh/ui/message_tokens.h"

#include <stdint.h>

enum PharaohMonumentBuildingType {
    PHARAOH_MONUMENT_PYRAMID = 183,
    PHARAOH_MONUMENT_SMALL_OBELISK = 207,
    PHARAOH_MONUMENT_LARGE_OBELISK = 208,
    PHARAOH_MONUMENT_SPHINX = 210,
    PHARAOH_MONUMENT_SUN_TEMPLE = 213,
    PHARAOH_MONUMENT_MAUSOLEUM = 222,
    PHARAOH_MONUMENT_ALEXANDRIA_LIBRARY = 225,
    PHARAOH_MONUMENT_CAESAREUM = 227,
    PHARAOH_MONUMENT_LIGHTHOUSE = 228,
    PHARAOH_MONUMENT_SMALL_ROYAL_TOMB = 229,
    PHARAOH_MONUMENT_ABU_SIMBEL = 230,
    PHARAOH_MONUMENT_MEDIUM_ROYAL_TOMB = 234,
    PHARAOH_MONUMENT_LARGE_ROYAL_TOMB = 235,
    PHARAOH_MONUMENT_GRAND_ROYAL_TOMB = 236
};

enum PharaohMonumentResource {
    PHARAOH_MONUMENT_RESOURCE_BRICKS = 12,
    PHARAOH_MONUMENT_RESOURCE_GRANITE = 26,
    PHARAOH_MONUMENT_RESOURCE_COPPER = 29
};

enum PharaohMonumentPhraseBufferIndex {
    PHARAOH_MONUMENT_BUFFER_TOTAL_SECONDARY,
    PHARAOH_MONUMENT_BUFFER_TOTAL_MAIN,
    PHARAOH_MONUMENT_BUFFER_LOADS_REMAINDER_SECONDARY,
    PHARAOH_MONUMENT_BUFFER_LOADS_REMAINDER_MAIN,
    PHARAOH_MONUMENT_BUFFER_LOADS_CURRENT_SECONDARY,
    PHARAOH_MONUMENT_BUFFER_LOADS_CURRENT_MAIN,
    PHARAOH_MONUMENT_BUFFER_NEEDED_REMAINDER_SECONDARY,
    PHARAOH_MONUMENT_BUFFER_NEEDED_REMAINDER_MAIN,
    PHARAOH_MONUMENT_BUFFER_NEEDED_CURRENT_SECONDARY,
    PHARAOH_MONUMENT_BUFFER_NEEDED_CURRENT_MAIN,
    PHARAOH_MONUMENT_BUFFER_COUNT
};

typedef struct PharaohPyramidProgress {
    int32_t height;
    int32_t frontier_main;
    int32_t frontier_casing;
    int32_t main_remaining;
    int32_t casing_remaining;
    int32_t main_total;
    int32_t casing_total;
    int32_t percent;
} PharaohPyramidProgress;

typedef struct PharaohScriptedMonumentProgress {
    int32_t material;
    int32_t percent;
    int32_t material_total;
} PharaohScriptedMonumentProgress;

typedef struct PharaohMonumentStockProgress {
    int32_t material;
    int32_t percent;
    int32_t material_total;
} PharaohMonumentStockProgress;

typedef struct PharaohMonumentResources {
    int32_t first;
    int32_t second;
} PharaohMonumentResources;

typedef const char *(*PharaohMonumentNameFunction)(
    void *user_data, int32_t building_id);
typedef const char *(*PharaohMonumentResourceNameFunction)(
    void *user_data, int32_t resource);

typedef struct PharaohMonumentTokenContext {
    int32_t building_id;
    int16_t building_type;
    uint8_t sphinx_stage;
    int32_t obelisk_progress;
    PharaohMonumentResources resources;
    PharaohPyramidProgress pyramid;
    PharaohScriptedMonumentProgress caesareum;
    PharaohScriptedMonumentProgress lighthouse;
    PharaohScriptedMonumentProgress mausoleum;
    PharaohMonumentStockProgress library;
    int32_t abu_simbel_percent;
    int32_t sun_temple_material;
    int32_t sun_temple_percent;
    int32_t sun_temple_material_total;
    int32_t tomb_percent;
    PharaohTextLookupFunction lookup_text;
    PharaohMonumentNameFunction monument_name;
    PharaohMonumentResourceNameFunction resource_name;
    void *user_data;
    PharaohTextBuffer percent_buffer;
    PharaohTextBuffer courses_buffer;
    PharaohTextBuffer phrase_buffers[PHARAOH_MONUMENT_BUFFER_COUNT];
} PharaohMonumentTokenContext;

/* Original addresses 0x00447C20 through 0x00448270. */
const char *pharaoh_monument_token_name(
    const PharaohMonumentTokenContext *context);
const char *pharaoh_monument_token_percent_complete(
    PharaohMonumentTokenContext *context);
const char *pharaoh_monument_token_courses_complete(
    PharaohMonumentTokenContext *context);
const char *pharaoh_monument_token_needed_current_main(
    PharaohMonumentTokenContext *context);
const char *pharaoh_monument_token_needed_current_secondary(
    PharaohMonumentTokenContext *context);
const char *pharaoh_monument_token_needed_remainder_main(
    PharaohMonumentTokenContext *context);
const char *pharaoh_monument_token_needed_remainder_secondary(
    PharaohMonumentTokenContext *context);
const char *pharaoh_monument_token_loads_word(
    const PharaohMonumentTokenContext *context, int32_t count);
const char *pharaoh_monument_token_loads_current_main(
    PharaohMonumentTokenContext *context);
const char *pharaoh_monument_token_loads_current_secondary(
    PharaohMonumentTokenContext *context);
const char *pharaoh_monument_token_loads_remainder_main(
    PharaohMonumentTokenContext *context);
const char *pharaoh_monument_token_loads_remainder_secondary(
    PharaohMonumentTokenContext *context);
const char *pharaoh_monument_token_total_main(
    PharaohMonumentTokenContext *context);
const char *pharaoh_monument_token_total_secondary(
    PharaohMonumentTokenContext *context);

#endif
