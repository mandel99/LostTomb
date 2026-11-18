#ifndef PHARAOH_RUNTIME_GAME_STATE_INITIALIZATION_H
#define PHARAOH_RUNTIME_GAME_STATE_INITIALIZATION_H

#include <stdint.h>

#include "pharaoh/runtime/text_input_state.h"
#include "pharaoh/runtime/city_warning_clear.h"
#include "pharaoh/runtime/city_view.h"
#include "pharaoh/runtime/map_ring.h"
#include "pharaoh/runtime/random_generator.h"

enum {
    PHARAOH_STARTUP_TEXT_FIELD_SCENARIO_NAME = 1,
    PHARAOH_STARTUP_TEXT_FIELD_FILE_DIALOG_NAME = 2,
    PHARAOH_STARTUP_TEXT_FIELD_MAP_NAME = 4,
    PHARAOH_STARTUP_TEXT_FIELD_ENTRY_5 = 5,
    PHARAOH_STARTUP_TEXT_FIELD_ENTRY_6 = 6,
    PHARAOH_STARTUP_TEXT_FIELD_CHEAT = 7
};

typedef struct PharaohGameStateInitState {
    uint8_t startup_state_byte;
    int32_t startup_state_word;
    int32_t scenario_climate;
    int32_t debug_evolve_info_enabled;
    int32_t debug_state_word;
    int32_t debug_walker_info_enabled;
    int32_t city_view_orientation;
    int32_t map_view_orientation;
    int32_t keyboard_field_index;
} PharaohGameStateInitState;

typedef struct PharaohGameStateInitTextBuffers {
    char *field_1_buffer;
    char *file_dialog_name;
    char *scenario_map_name;
    char *field_5_buffer;
    char *field_6_buffer;
    char *cheat_input_text;
} PharaohGameStateInitTextBuffers;

typedef struct PharaohGameStateInitContext {
    PharaohGameStateInitState *state;
    const PharaohGameStateInitTextBuffers *text_buffers;
    PharaohTextInputState *text_input_state;
    const PharaohCityWarningClearContext *city_warning_clear;
    const PharaohMapRingInitContext *map_ring;
    PharaohCityViewContext *city_view;
    const PharaohRandomGeneratorContext *random_generator;
    int32_t viewport_mode;
    uint32_t text_format_primary;
    uint32_t text_format_secondary;
} PharaohGameStateInitContext;

void pharaoh_game_state_initialize(const PharaohGameStateInitContext *context);

#endif
