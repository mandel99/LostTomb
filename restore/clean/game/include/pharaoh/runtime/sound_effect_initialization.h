#ifndef PHARAOH_RUNTIME_SOUND_EFFECT_INITIALIZATION_H
#define PHARAOH_RUNTIME_SOUND_EFFECT_INITIALIZATION_H

#include "pharaoh/runtime/sound_effect_files.h"

enum {
    PHARAOH_INTERFACE_SOUND_BINDING_COUNT = 19
};

typedef struct PharaohSoundEffectFileBinding {
    int32_t slot;
    const char *filename;
} PharaohSoundEffectFileBinding;

/* Native entry 0x0053AF90. Resets all 100 slots, then loads the 19
   interface/effect paths in native call order. */
void pharaoh_sound_effect_files_initialize(
    PharaohSoundEffectFileIds *file_ids,
    const PharaohSoundFileIdLoadContext *load_context);

#endif
