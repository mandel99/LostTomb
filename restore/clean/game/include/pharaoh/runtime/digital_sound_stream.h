#ifndef PHARAOH_RUNTIME_DIGITAL_SOUND_STREAM_H
#define PHARAOH_RUNTIME_DIGITAL_SOUND_STREAM_H

#include <stdint.h>
#include "pharaoh/runtime/digital_sound_device.h"

typedef struct PharaohDigitalSoundStreamCloseOperations {
    void *user_data;
    void (*ail_close_stream)(void *user_data, uint32_t stream_handle);
    int32_t (*ail_stream_status)(void *user_data, uint32_t stream_handle);
    int32_t (*ail_stream_volume)(void *user_data, uint32_t stream_handle);
    void (*ail_set_stream_volume)(void *user_data, uint32_t stream_handle,
                                   int32_t volume);
    uint32_t (*ail_open_stream)(void *user_data, uint32_t wave_device,
                                const char *filename, int32_t flags);
    void (*ail_set_stream_loop_count)(void *user_data,
                                      uint32_t stream_handle, uint32_t loops);
    void (*ail_start_stream)(void *user_data, uint32_t stream_handle);
} PharaohDigitalSoundStreamCloseOperations;

/* Native entry 0x0055F561. The caller supplies a native stream index. */
void pharaoh_digital_sound_stream_close(
    PharaohDigitalSoundDeviceState *state, uint32_t stream_index,
    const PharaohDigitalSoundStreamCloseOperations *operations);

/* Native entries 0x0055F5A1, 0x0055F5F4, 0x0055F639, 0x0055F449. */
int32_t pharaoh_digital_sound_stream_is_playing(
    const PharaohDigitalSoundDeviceState *state, uint32_t stream_index,
    const PharaohDigitalSoundStreamCloseOperations *operations);
int32_t pharaoh_digital_sound_stream_get_volume(
    const PharaohDigitalSoundDeviceState *state, uint32_t stream_index,
    const PharaohDigitalSoundStreamCloseOperations *operations);
void pharaoh_digital_sound_stream_set_volume(
    const PharaohDigitalSoundDeviceState *state, uint32_t stream_index,
    int32_t volume, const PharaohDigitalSoundStreamCloseOperations *operations);
int32_t pharaoh_digital_sound_stream_play(
    PharaohDigitalSoundDeviceState *state, const char *filename,
    uint32_t stream_index, uint32_t loops,
    const PharaohDigitalSoundStreamCloseOperations *operations);

void pharaoh_digital_sound_set_speech_volume(
    PharaohDigitalSoundDeviceState *state, int32_t volume,
    const PharaohDigitalSoundStreamCloseOperations *operations);
#endif
