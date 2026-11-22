#include "pharaoh/runtime/sound_stream_fade.h"

enum {
    SOUND_STREAM_MUSIC_CHANNEL = 0,
    SOUND_STREAM_SPEECH_CHANNEL = 1,
    SOUND_STREAM_FADE_STEP = 5
};

/* Complete C algorithm reused from the prior LostTomb SoundFades owner.
 * Native 0x0053B690 completes the fade synchronously; 0x0053B6F0 advances
 * only one step and is a different entry. */
void pharaoh_sound_stream_fade_to_stop(
    const PharaohSoundStreamFadeContext *context, uint32_t channel)
{
    if (*context->sound_backend_active == 0 ||
        context->sound->stream_handles[channel] == 0 ||
        pharaoh_digital_sound_stream_is_playing(context->sound, channel,
            context->stream_operations) == 0) return;

    int32_t volume = pharaoh_digital_sound_stream_get_volume(
        context->sound, channel, context->stream_operations);
    while (volume > 0) {
        volume = (int32_t)((uint32_t)volume - SOUND_STREAM_FADE_STEP);
        pharaoh_digital_sound_stream_set_volume(context->sound, channel,
            volume, context->stream_operations);
    }
    pharaoh_digital_sound_stream_close(context->sound, channel,
        context->stream_operations);
}

void pharaoh_sound_stream_fade_down(
    const PharaohSoundStreamFadeContext *context, uint32_t channel)
{
    if (*context->sound_backend_active != 0 &&
        context->sound->stream_handles[channel] != 0 &&
        pharaoh_digital_sound_stream_is_playing(context->sound, channel,
            context->stream_operations) != 0) {
        const int32_t volume = pharaoh_digital_sound_stream_get_volume(
            context->sound, channel, context->stream_operations);
        const int32_t next_volume = (int32_t)((uint32_t)volume -
            SOUND_STREAM_FADE_STEP);
        if (next_volume >= 0) {
            pharaoh_digital_sound_stream_set_volume(context->sound, channel,
                next_volume, context->stream_operations);
            return;
        }
        pharaoh_digital_sound_stream_close(context->sound, channel,
            context->stream_operations);
    }

    if (channel == SOUND_STREAM_MUSIC_CHANNEL) {
        *context->music_fade_active = 0;
    } else if (channel == SOUND_STREAM_SPEECH_CHANNEL) {
        *context->speech_fade_active = 0;
    }
}
