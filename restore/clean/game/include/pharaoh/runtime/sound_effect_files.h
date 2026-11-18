#ifndef PHARAOH_RUNTIME_SOUND_EFFECT_FILES_H
#define PHARAOH_RUNTIME_SOUND_EFFECT_FILES_H

#include <stdint.h>

#include "pharaoh/runtime/sound_file_id_load.h"

enum {
    PHARAOH_SOUND_EFFECT_FILE_SLOT_COUNT = 100
};

typedef struct PharaohSoundEffectFileIds {
    uint32_t values[PHARAOH_SOUND_EFFECT_FILE_SLOT_COUNT];
} PharaohSoundEffectFileIds;

_Static_assert(sizeof(PharaohSoundEffectFileIds) == 400,
    "native sound-effect file-ID table size");

/* Native entry 0x0053AF70. The native routine stores the loader's full
   32-bit result at the supplied slot without a range check. */
void pharaoh_sound_effect_bind_file(
    PharaohSoundEffectFileIds *file_ids, int32_t slot,
    const char *filename,
    const PharaohSoundFileIdLoadContext *load_context);

#endif
