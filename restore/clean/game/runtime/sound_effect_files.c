#include "pharaoh/runtime/sound_effect_files.h"

void pharaoh_sound_effect_bind_file(
    PharaohSoundEffectFileIds *file_ids, int32_t slot,
    const char *filename,
    const PharaohSoundFileIdLoadContext *load_context)
{
    file_ids->values[slot] = pharaoh_sound_file_id_load(load_context,
        filename);
}
