#ifndef PHARAOH_PLATFORM_WINDOWS_SOUND_H
#define PHARAOH_PLATFORM_WINDOWS_SOUND_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "pharaoh/runtime/digital_sound_stream.h"
#include "pharaoh/runtime/sound_effect_request.h"

typedef struct PharaohWindowsSound {
    HMODULE module;
    int32_t ready;
    PharaohDigitalSoundDeviceState state;
    PharaohDigitalSoundDeviceOperations device_operations;
    PharaohDigitalSoundStreamCloseOperations stream_operations;
    void *button_data;
    int32_t (WINAPI *startup)(void);
    void (WINAPI *shutdown)(void);
    int32_t (WINAPI *preference)(uint32_t, uint32_t);
    int32_t (WINAPI *wave_open)(uint32_t *, void *, uint32_t,
        const PharaohDigitalSoundWaveFormat *);
    void (WINAPI *wave_close)(uint32_t);
    uint32_t (WINAPI *sample_allocate)(uint32_t);
    void (WINAPI *sample_release)(uint32_t);
    void (WINAPI *sample_init)(uint32_t);
    int32_t (WINAPI *sample_file)(uint32_t, const void *, int32_t);
    void (WINAPI *sample_volume)(uint32_t, int32_t);
    void (WINAPI *sample_pan)(uint32_t, int32_t);
    void (WINAPI *sample_loops)(uint32_t, int32_t);
    void (WINAPI *sample_start)(uint32_t);
    uint32_t (WINAPI *sample_status)(uint32_t);
    void (WINAPI *sample_end)(uint32_t);
    void (WINAPI *stream_close)(uint32_t);
    uint32_t (WINAPI *stream_open)(uint32_t, const char *, int32_t);
    int32_t (WINAPI *stream_status)(uint32_t);
    int32_t (WINAPI *stream_volume)(uint32_t);
    void (WINAPI *stream_set_volume)(uint32_t, int32_t);
    void (WINAPI *stream_loops)(uint32_t, uint32_t);
    void (WINAPI *stream_start)(uint32_t);
} PharaohWindowsSound;

int32_t pharaoh_windows_sound_open(PharaohWindowsSound *sound);
void pharaoh_windows_sound_close(PharaohWindowsSound *sound);
void pharaoh_windows_sound_button(PharaohWindowsSound *sound, int32_t volume);
void pharaoh_windows_sound_menu_music(PharaohWindowsSound *sound,
    int32_t enabled, int32_t volume);
#endif
