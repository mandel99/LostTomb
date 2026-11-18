#ifndef PHARAOH_RUNTIME_SOUND_INITIALIZATION_H
#define PHARAOH_RUNTIME_SOUND_INITIALIZATION_H

#include <stdint.h>

#include "pharaoh/runtime/music_population.h"
#include "pharaoh/runtime/sound_effect_initialization.h"
#include "pharaoh/runtime/sound_file_table.h"
#include "pharaoh/runtime/digital_sound_device.h"

enum {
    PHARAOH_SOUND_INITIALIZATION_FILE_CAPACITY = 500,
    PHARAOH_SOUND_INITIALIZATION_FORCE_TRACK_SELECTION = 1
};

typedef struct PharaohSoundInitializationState {
    int32_t digital_output_result;
    int32_t post_configuration_sentinel;
} PharaohSoundInitializationState;

typedef struct PharaohSoundInitializationOperations {
    void *user_data;
    void (*load_music_configuration)(void *user_data,
                                     PharaohMusicManager *music_manager);
    void (*register_object_sound_files)(void *user_data);
    void (*register_environment_sound_files)(void *user_data);
    void (*register_figure_sound_files)(void *user_data);
} PharaohSoundInitializationOperations;

typedef struct PharaohSoundInitializationContext {
    PharaohSoundInitializationState *state;
    const PharaohDigitalSoundDeviceContext *digital_sound;
    PharaohSoundFileTable *sound_file_table;
    const PharaohSoundFileMemoryOperations *file_memory;
    PharaohMusicManager *music_manager;
    int32_t music_category;
    int32_t city_population;
    PharaohSoundEffectFileIds *interface_sound_file_ids;
    const PharaohSoundFileIdLoadContext *interface_sound_file_load_context;
    const PharaohSoundInitializationOperations *operations;
} PharaohSoundInitializationContext;

/* Native entry 0x005394F0. Inputs formerly read from process globals are
   explicit; four remaining subprocedures stay typed operations. */
void pharaoh_sound_initialize(
    const PharaohSoundInitializationContext *context);

#endif
