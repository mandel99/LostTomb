#include "pharaoh/platform/windows_sound.h"
#include "pharaoh/ui/front_menu_render.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int32_t windows_version(void *data, uint32_t *platform, uint32_t *major)
{
    OSVERSIONINFOA version = { 0 };
    (void)data;
    version.dwOSVersionInfoSize = sizeof(version);
#pragma warning(push)
#pragma warning(disable:4996)
    const BOOL result = GetVersionExA(&version);
#pragma warning(pop)
    *platform = version.dwPlatformId;
    *major = version.dwMajorVersion;
    return result != 0;
}
static void startup(void *data) { (void)((PharaohWindowsSound *)data)->startup(); }
static int32_t preference(void *data, uint32_t id, uint32_t value)
{ return ((PharaohWindowsSound *)data)->preference(id, value); }
static int32_t wave_open(void *data, uint32_t *driver, uint32_t device,
    uint32_t flags, const PharaohDigitalSoundWaveFormat *format)
{
    (void)device;
    return ((PharaohWindowsSound *)data)->wave_open(driver, NULL, flags, format);
}
static void debug_text(void *data, const char *text)
{ (void)data; OutputDebugStringA(text); }
static void *allocate(void *data, uint32_t bytes)
{ (void)data; return calloc(1, bytes); }
static void deallocate(void *data, void *pointer)
{ (void)data; free(pointer); }
static uint32_t sample_allocate(void *data, uint32_t driver)
{ return ((PharaohWindowsSound *)data)->sample_allocate(driver); }
static void sample_release(void *data, uint32_t sample)
{ if (sample != 0) ((PharaohWindowsSound *)data)->sample_release(sample); }
static void stop_sample(void *data, PharaohDigitalSoundDeviceState *state, int32_t index)
{ if (state->sample_handles[index] != 0)
    ((PharaohWindowsSound *)data)->sample_end(state->sample_handles[index]); }
static void close_stream(void *data, uint32_t stream)
{ if (stream != 0) ((PharaohWindowsSound *)data)->stream_close(stream); }
static uint32_t open_stream(void *data, uint32_t driver, const char *file, int32_t flags)
{ return ((PharaohWindowsSound *)data)->stream_open(driver, file, flags); }
static int32_t stream_status(void *data, uint32_t stream)
{ return ((PharaohWindowsSound *)data)->stream_status(stream); }
static int32_t stream_volume(void *data, uint32_t stream)
{ return ((PharaohWindowsSound *)data)->stream_volume(stream); }
static void set_stream_volume(void *data, uint32_t stream, int32_t volume)
{ ((PharaohWindowsSound *)data)->stream_set_volume(stream, volume); }
static void stream_loops(void *data, uint32_t stream, uint32_t loops)
{ ((PharaohWindowsSound *)data)->stream_loops(stream, loops); }
static void start_stream(void *data, uint32_t stream)
{ ((PharaohWindowsSound *)data)->stream_start(stream); }

static int32_t menu_music_playing(void *data, int32_t channel)
{
    PharaohWindowsSound *const sound = data;
    return pharaoh_digital_sound_stream_is_playing(&sound->state,
        (uint32_t)channel, &sound->stream_operations);
}
static void menu_music_play(void *data, const char *filename, int32_t channel,
    int32_t loops)
{
    PharaohWindowsSound *const sound = data;
    (void)pharaoh_digital_sound_stream_play(&sound->state, filename,
        (uint32_t)channel, (uint32_t)loops, &sound->stream_operations);
}

void pharaoh_windows_sound_menu_music(PharaohWindowsSound *sound,
    int32_t enabled, int32_t volume)
{
    const PharaohMainMenuSound menu_sound = {
        sound, NULL, menu_music_playing, menu_music_play };
    sound->state.stream_music_volume = volume;
    pharaoh_front_menu_update_music(sound->ready, enabled, &menu_sound);
}

int32_t pharaoh_windows_sound_open(PharaohWindowsSound *sound)
{
    memset(sound, 0, sizeof(*sound));
    sound->module = LoadLibraryA("mss32.dll");
    if (sound->module == NULL) return 0;
#define BIND(field, symbol) do { \
    FARPROC entry = GetProcAddress(sound->module, symbol); \
    if (entry == NULL) goto failed; \
    memcpy(&sound->field, &entry, sizeof(sound->field)); \
} while (0)
    BIND(startup, "_AIL_startup@0");
    BIND(shutdown, "_AIL_shutdown@0");
    BIND(preference, "_AIL_set_preference@8");
    BIND(wave_open, "_AIL_waveOutOpen@16");
    BIND(wave_close, "_AIL_waveOutClose@4");
    BIND(sample_allocate, "_AIL_allocate_sample_handle@4");
    BIND(sample_release, "_AIL_release_sample_handle@4");
    BIND(sample_init, "_AIL_init_sample@4");
    BIND(sample_file, "_AIL_set_sample_file@12");
    BIND(sample_volume, "_AIL_set_sample_volume@8");
    BIND(sample_pan, "_AIL_set_sample_pan@8");
    BIND(sample_loops, "_AIL_set_sample_loop_count@8");
    BIND(sample_start, "_AIL_start_sample@4");
    BIND(sample_status, "_AIL_sample_status@4");
    BIND(sample_end, "_AIL_end_sample@4");
    BIND(stream_close, "_AIL_close_stream@4");
    BIND(stream_open, "_AIL_open_stream@12");
    BIND(stream_status, "_AIL_stream_status@4");
    BIND(stream_volume, "_AIL_stream_volume@4");
    BIND(stream_set_volume, "_AIL_set_stream_volume@8");
    BIND(stream_loops, "_AIL_set_stream_loop_count@8");
    BIND(stream_start, "_AIL_start_stream@4");
#undef BIND
    sound->device_operations.user_data = sound;
    sound->device_operations.get_windows_version = windows_version;
    sound->device_operations.ail_startup = startup;
    sound->device_operations.ail_set_preference = preference;
    sound->device_operations.wave_out_open = wave_open;
    sound->device_operations.output_debug_string = debug_text;
    sound->device_operations.allocate_bytes = allocate;
    sound->device_operations.deallocate_bytes = deallocate;
    sound->device_operations.allocate_sample_handle = sample_allocate;
    sound->device_operations.release_sample_handle = sample_release;
    sound->device_operations.stop_sample = stop_sample;
    sound->state.sample_rate = 22050;
    sound->state.bits_per_sample_source = 16;
    sound->state.channels = 2;
    sound->state.stream_music_volume = 100;
    sound->state.volume_other = 100;
    sound->state.stream_speech_volume = 100;
    sound->state.stream_city_volume = 100;
    const PharaohDigitalSoundDeviceContext device = {
        &sound->state, &sound->device_operations };
    if (pharaoh_digital_sound_device_initialize(&device) == 0) goto failed;
    sound->ready = 1;
    sound->stream_operations.user_data = sound;
    sound->stream_operations.ail_close_stream = close_stream;
    sound->stream_operations.ail_open_stream = open_stream;
    sound->stream_operations.ail_stream_status = stream_status;
    sound->stream_operations.ail_stream_volume = stream_volume;
    sound->stream_operations.ail_set_stream_volume = set_stream_volume;
    sound->stream_operations.ail_set_stream_loop_count = stream_loops;
    sound->stream_operations.ail_start_stream = start_stream;
    FILE *file = NULL;
    if (fopen_s(&file, "audio\\wavs\\button.wav", "rb") == 0 && file != NULL) {
        if (fseek(file, 0, SEEK_END) == 0) {
            const long bytes = ftell(file);
            if (bytes > 0 && fseek(file, 0, SEEK_SET) == 0) {
                sound->button_data = malloc((size_t)bytes);
                if (sound->button_data != NULL &&
                    fread(sound->button_data, 1, (size_t)bytes, file) != (size_t)bytes) {
                    free(sound->button_data);
                    sound->button_data = NULL;
                }
            }
        }
        (void)fclose(file);
    }
    return 1;
failed:
    pharaoh_windows_sound_close(sound);
    return 0;
}

void pharaoh_windows_sound_button(PharaohWindowsSound *sound, int32_t volume)
{
    if (sound->ready == 0 || sound->button_data == NULL ||
        sound->state.sample_handles == NULL) return;
    const uint32_t sample = sound->state.sample_handles[0];
    if (sample == 0) return;
    /* Native 0055F0E8 suppresses an already playing request with the same
       file and match tag. This boot adapter owns only the button sample. */
    if (sound->sample_status(sample) == 4) return;
    sound->sample_end(sample);
    sound->sample_init(sample);
    if (sound->sample_file(sample, sound->button_data, 0) == 0) return;
    if (volume < 0) volume = 0;
    if (volume > 100) volume = 100;
    sound->sample_loops(sample, PHARAOH_SOUND_EFFECT_REQUEST_LOOP_COUNT);
    sound->sample_pan(sample, PHARAOH_SOUND_EFFECT_REQUEST_PAN_CENTER);
    /* Native 0055F0E8 forwards the request volume to Miles unchanged. */
    sound->sample_volume(sample, volume);
    sound->sample_start(sample);
}

void pharaoh_windows_sound_close(PharaohWindowsSound *sound)
{
    if (sound->module == NULL) return;
    for (uint32_t i = 0; i < 3; ++i) {
        if (sound->state.stream_handles[i] != 0)
            pharaoh_digital_sound_stream_close(&sound->state, i,
                &sound->stream_operations);
    }
    if (sound->state.sample_handles != NULL) {
        for (int32_t i = 0; i < sound->state.sample_count; ++i) {
            const uint32_t sample = sound->state.sample_handles[i];
            if (sample != 0) {
                sound->sample_end(sample);
                sound->sample_release(sample);
            }
        }
    }
    if (sound->state.wave_device != 0) sound->wave_close(sound->state.wave_device);
    if (sound->shutdown != NULL && sound->device_operations.user_data != NULL)
        sound->shutdown();
    free(sound->state.sample_handles);
    free(sound->state.auxiliary_handles);
    free(sound->state.samples);
    free(sound->state.cache_entries);
    free(sound->button_data);
    FreeLibrary(sound->module);
    memset(sound, 0, sizeof(*sound));
}
