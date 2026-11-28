#ifndef PHARAOH_PLATFORM_WINDOWS_GAME_INPUT_H
#define PHARAOH_PLATFORM_WINDOWS_GAME_INPUT_H

#include "pharaoh/runtime/mouse_update.h"
#include "pharaoh/runtime/sound_file_table.h"
#include "pharaoh/runtime/sound_effect_initialization.h"
#include "pharaoh/runtime/building_sound_files.h"

struct PharaohWindowsStartupHost;

typedef struct PharaohWindowsGameInput {
    PharaohMouseUpdateState mouse;
    PharaohSoundFileTable files;
    PharaohSoundEffectFileIds effects;
    PharaohBuildingSoundBank building_sounds[PHARAOH_BUILDING_SOUND_BANK_COUNT];
    PharaohSoundEffectPlayRequest request;
    uint32_t table_bytes;
    uint8_t files_initialized,left_message;
} PharaohWindowsGameInput;

/* Actual window-message WORD coordinates and raw button BYTE. */
void pharaoh_windows_game_input_message(PharaohWindowsGameInput *,
    uint32_t message,intptr_t parameter);
PharaohMouseUpdateStatus pharaoh_windows_game_input_update(
    PharaohWindowsGameInput *,struct PharaohWindowsStartupHost *);
void pharaoh_windows_game_input_effect(PharaohWindowsGameInput *,
    struct PharaohWindowsStartupHost *,int32_t slot);
void pharaoh_windows_game_input_bind_sound(PharaohWindowsGameInput *,
    struct PharaohWindowsStartupHost *,PharaohSoundEffectRequestState *,
    PharaohDigitalSoundDeviceContext *);
void pharaoh_windows_game_input_close(PharaohWindowsGameInput *,
    struct PharaohWindowsStartupHost *);

#endif
