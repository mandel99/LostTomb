#ifndef PHARAOH_RUNTIME_DIGITAL_SOUND_DEVICE_H
#define PHARAOH_RUNTIME_DIGITAL_SOUND_DEVICE_H

#include <stdint.h>

typedef struct PharaohDigitalSoundWaveFormat {
    uint16_t format_tag;
    uint16_t channel_count;
    uint32_t samples_per_second;
    uint32_t average_bytes_per_second;
    uint16_t block_align;
    uint16_t bits_per_sample;
} PharaohDigitalSoundWaveFormat;

typedef struct PharaohDigitalSoundSample {
    /* The native reset writes ten dwords; their individual meanings remain
       unresolved, so retain their observed positional identity. */
    uint32_t native_word_00;
    uint32_t native_word_04;
    uint32_t native_word_08;
    uint32_t native_word_0c;
    uint32_t native_word_10;
    uint32_t native_word_14;
    uint32_t native_word_18;
    uint32_t native_word_1c;
    uint32_t native_word_20;
    uint32_t native_word_24;
} PharaohDigitalSoundSample;

typedef struct PharaohDigitalSoundCacheEntry {
    int32_t file_id;
    uint32_t locked_data;
    int32_t state;
} PharaohDigitalSoundCacheEntry;

/* Fields used by the native 0x0055E6C4 sound object. Opaque byte ranges
   preserve the observed offsets without assigning names to unrelated state. */
typedef struct PharaohDigitalSoundDeviceState {
    uint8_t opaque_before_stream_volumes[0x14c];
    int32_t stream_music_volume;
    int32_t volume_other;
    int32_t stream_speech_volume;
    int32_t stream_city_volume;
    uint8_t opaque_15c_to_177[0x1c];
    uint32_t wave_device;
    PharaohDigitalSoundWaveFormat wave_format;
    int32_t sample_rate;
    int32_t bits_per_sample_source;
    int32_t channels;
    int32_t sample_count;
    uint32_t *sample_handles;
    uint32_t *auxiliary_handles;
    uint32_t reserved_1a4;
    PharaohDigitalSoundSample *samples;
    PharaohDigitalSoundCacheEntry *cache_entries;
    uint8_t opaque_1b0_to_1d7[0x28];
    uint32_t stream_handles[3];
    int32_t spatial_sample_count;
    uint8_t opaque_1e8_to_1ff[0x18];
    uint32_t sound_disabled;
} PharaohDigitalSoundDeviceState;

typedef struct PharaohDigitalSoundDeviceOperations {
    void *user_data;
    int32_t (*get_windows_version)(void *user_data,
        uint32_t *platform_id, uint32_t *major_version);
    void (*ail_startup)(void *user_data);
    int32_t (*ail_set_preference)(void *user_data,
        uint32_t preference_id, uint32_t value);
    int32_t (*wave_out_open)(void *user_data, uint32_t *wave_device,
        uint32_t device_id, uint32_t flags,
        const PharaohDigitalSoundWaveFormat *format);
    void (*output_debug_string)(void *user_data, const char *message);
    void (*stop_sample)(void *user_data,
        PharaohDigitalSoundDeviceState *state, int32_t sample_index);
    void (*stop_spatial_sample)(void *user_data,
        PharaohDigitalSoundDeviceState *state, int32_t sample_index);
    void *(*allocate_bytes)(void *user_data, uint32_t byte_count);
    void (*deallocate_bytes)(void *user_data, void *allocation);
    uint32_t (*allocate_sample_handle)(void *user_data,
        uint32_t wave_device);
    void (*release_sample_handle)(void *user_data, uint32_t sample_handle);
    void (*free_locked_memory)(void *user_data, uint32_t locked_data);
} PharaohDigitalSoundDeviceOperations;

typedef struct PharaohDigitalSoundDeviceContext {
    PharaohDigitalSoundDeviceState *state;
    const PharaohDigitalSoundDeviceOperations *operations;
} PharaohDigitalSoundDeviceContext;

/* Native entry 0x0055E5D2: resets one ten-dword sample record. */
void pharaoh_digital_sound_sample_reset(
    PharaohDigitalSoundDeviceState *state, int32_t sample_index);

/* Native entry 0x0055EC7C: stop sample and spatial-sample ranges unless the
   native sound-disabled field is set. */
void pharaoh_digital_sound_stop_all(
    PharaohDigitalSoundDeviceState *state,
    const PharaohDigitalSoundDeviceOperations *operations);

/* Native entry 0x0055DEA2: replace the device's four sample-pool arrays. */
int32_t pharaoh_digital_sound_pool_resize(
    PharaohDigitalSoundDeviceState *state, int32_t requested_sample_count,
    const PharaohDigitalSoundDeviceOperations *operations);

/* Native entry 0x0055E6C4: OS gate, Miles setup, wave format, wave output,
   five-sample pool, and cache initialization. */
int32_t pharaoh_digital_sound_device_initialize(
    const PharaohDigitalSoundDeviceContext *context);

#endif
