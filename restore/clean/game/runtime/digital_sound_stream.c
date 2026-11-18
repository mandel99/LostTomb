#include "pharaoh/runtime/digital_sound_stream.h"

#include <stddef.h>

_Static_assert(offsetof(PharaohDigitalSoundDeviceState, stream_handles) == 0x1d8,
    "Native sound stream handles offset");
_Static_assert(offsetof(PharaohDigitalSoundDeviceState, stream_music_volume) == 0x14c,
    "Native music stream volume offset");
_Static_assert(offsetof(PharaohDigitalSoundDeviceState, stream_speech_volume) == 0x154,
    "Native speech stream volume offset");
_Static_assert(offsetof(PharaohDigitalSoundDeviceState, stream_city_volume) == 0x158,
    "Native city stream volume offset");

void pharaoh_digital_sound_stream_close(
    PharaohDigitalSoundDeviceState *state, uint32_t stream_index,
    const PharaohDigitalSoundStreamCloseOperations *operations)
{
    if (state->sound_disabled == 0) {
        operations->ail_close_stream(operations->user_data,
            state->stream_handles[stream_index]);
        state->stream_handles[stream_index] = 0;
    }
}

int32_t pharaoh_digital_sound_stream_is_playing(
    const PharaohDigitalSoundDeviceState *state, uint32_t stream_index,
    const PharaohDigitalSoundStreamCloseOperations *operations)
{
    if (state->sound_disabled != 0 || state->stream_handles[stream_index] == 0) {
        return 0;
    }
    return operations->ail_stream_status(operations->user_data,
        state->stream_handles[stream_index]) == 4;
}

int32_t pharaoh_digital_sound_stream_get_volume(
    const PharaohDigitalSoundDeviceState *state, uint32_t stream_index,
    const PharaohDigitalSoundStreamCloseOperations *operations)
{
    if (state->sound_disabled != 0 || state->stream_handles[stream_index] == 0) {
        return 0;
    }
    return operations->ail_stream_volume(operations->user_data,
        state->stream_handles[stream_index]);
}

void pharaoh_digital_sound_stream_set_volume(
    const PharaohDigitalSoundDeviceState *state, uint32_t stream_index,
    int32_t volume, const PharaohDigitalSoundStreamCloseOperations *operations)
{
    if (state->sound_disabled == 0 && state->stream_handles[stream_index] != 0) {
        operations->ail_set_stream_volume(operations->user_data,
            state->stream_handles[stream_index], volume);
    }
}

int32_t pharaoh_digital_sound_stream_play(
    PharaohDigitalSoundDeviceState *state, const char *filename,
    uint32_t stream_index, uint32_t loops,
    const PharaohDigitalSoundStreamCloseOperations *operations)
{
    if (stream_index >= 3) {
        return -1;
    }
    if (state->sound_disabled != 0) {
        return 1;
    }

    if (state->stream_handles[stream_index] != 0) {
        operations->ail_close_stream(operations->user_data,
            state->stream_handles[stream_index]);
        state->stream_handles[stream_index] = 0;
    }

    const uint32_t handle = operations->ail_open_stream(operations->user_data,
        state->wave_device, filename, 0);
    state->stream_handles[stream_index] = handle;
    if (handle != 0) {
        const int32_t volume = stream_index == 1 ?
            state->stream_speech_volume : stream_index == 2 ?
            state->stream_city_volume : state->stream_music_volume;
        operations->ail_set_stream_loop_count(operations->user_data,
            handle, loops);
        operations->ail_set_stream_volume(operations->user_data,
            handle, volume);
        operations->ail_start_stream(operations->user_data, handle);
    }
    return 1;
}
