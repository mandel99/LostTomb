#ifndef PHARAOH_RUNTIME_SOUND_STREAM_FADE_H
#define PHARAOH_RUNTIME_SOUND_STREAM_FADE_H

#include "pharaoh/runtime/digital_sound_stream.h"

typedef struct PharaohSoundStreamFadeContext {
    PharaohDigitalSoundDeviceState *sound;
    const PharaohDigitalSoundStreamCloseOperations *stream_operations;
    const int32_t *sound_backend_active;
    int32_t *music_fade_active;
    int32_t *speech_fade_active;
} PharaohSoundStreamFadeContext;

/* Native entry 0x0053B6F0. Channel 0 is music, channel 1 is speech. */
void pharaoh_sound_stream_fade_down(
    const PharaohSoundStreamFadeContext *context, uint32_t channel);

#endif
