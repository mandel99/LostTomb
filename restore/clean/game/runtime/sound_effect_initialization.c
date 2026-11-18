#include "pharaoh/runtime/sound_effect_initialization.h"

#include <stdint.h>

static const PharaohSoundEffectFileBinding interface_sound_bindings[
    PHARAOH_INTERFACE_SOUND_BINDING_COUNT] = {
    { 0, "audio\\wavs\\button.wav" },
    { 1, "audio\\wavs\\no.wav" },
    { 2, "audio\\wavs\\dig.wav" },
    { 3, "audio\\wavs\\fire.wav" },
    { 4, "audio\\wavs\\crash.wav" },
    { 5, "audio\\wavs\\splash.wav" },
    { 6, "audio\\wavs\\menu.wav" },
    { 7, "audio\\wavs\\build.wav" },
    { 8, "audio\\wavs\\build.wav" },
    { 9, "audio\\wavs\\fanfare1.wav" },
    { 10, "audio\\wavs\\fanfare2.wav" },
    { 17, "audio\\wavs\\crash.wav" },
    { 18, "audio\\wavs\\fire.wav" },
    { 11, "audio\\wavs\\ship_collision_beam.wav" },
    { 12, "audio\\wavs\\ship_collision_fore_quarter.wav" },
    { 13, "audio\\wavs\\ship_collision_aft_quarter.wav" },
    { 14, "audio\\wavs\\ship_collision_bow.wav" },
    { 15, "audio\\wavs\\ship_collision_stern.wav" },
    { 16, "audio\\wavs\\ship_order_top_speed.wav" }
};

void pharaoh_sound_effect_files_initialize(
    PharaohSoundEffectFileIds *file_ids,
    const PharaohSoundFileIdLoadContext *load_context)
{
    for (int32_t slot = 0;
         slot < PHARAOH_SOUND_EFFECT_FILE_SLOT_COUNT; ++slot) {
        file_ids->values[slot] = UINT32_MAX;
    }

    for (uint32_t index = 0;
         index < PHARAOH_INTERFACE_SOUND_BINDING_COUNT; ++index) {
        const PharaohSoundEffectFileBinding *binding =
            &interface_sound_bindings[index];
        pharaoh_sound_effect_bind_file(file_ids, binding->slot,
            binding->filename, load_context);
    }
}
