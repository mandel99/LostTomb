#include "pharaoh/runtime/digital_sound_device.h"

#include <stddef.h>

enum {
    DIGITAL_SOUND_NT_PLATFORM_ID = 2,
    DIGITAL_SOUND_MINIMUM_NT_MAJOR = 5,
    DIGITAL_SOUND_PCM_FORMAT_TAG = 1,
    DIGITAL_SOUND_BITS_PER_BYTE = 8,
    DIGITAL_SOUND_INITIAL_SAMPLE_COUNT = 5,
    DIGITAL_SOUND_PREFERENCE_0X28 = 0x28,
    DIGITAL_SOUND_PREFERENCE_0X0F = 0x0f,
    DIGITAL_SOUND_PREFERENCE_0X28_VALUE = 0x40000,
    DIGITAL_SOUND_PREFERENCE_0X0F_VALUE = 0,
    DIGITAL_SOUND_WAVE_DEVICE_ID = 0,
    DIGITAL_SOUND_SAMPLE_HANDLE_STRIDE = 4,
    DIGITAL_SOUND_SAMPLE_WORD_COUNT = 10,
    DIGITAL_SOUND_SAMPLE_WORD_STRIDE = 4,
    DIGITAL_SOUND_SAMPLE_CACHE_STRIDE = 12
};

#define DIGITAL_SOUND_WAVE_OPEN_FLAGS UINT32_MAX

static const char wave_output_failure_message[] =
    "Unable to initialize digital sound driver....\n";

_Static_assert(sizeof(void *) == 4,
    "Pharaoh sound-object pointers are 32-bit");
_Static_assert(sizeof(PharaohDigitalSoundWaveFormat) == 16,
    "Native wave format is 16 bytes");
_Static_assert(sizeof(PharaohDigitalSoundSample) == 0x28,
    "Native sound sample stride is 0x28 bytes");
_Static_assert(sizeof(PharaohDigitalSoundCacheEntry) == 0x0c,
    "Native sound cache stride is 0x0c bytes");
_Static_assert(offsetof(PharaohDigitalSoundDeviceState, wave_device) == 0x178,
    "Native wave device offset");
_Static_assert(offsetof(PharaohDigitalSoundDeviceState, wave_format) == 0x17c,
    "Native wave format offset");
_Static_assert(offsetof(PharaohDigitalSoundDeviceState, sample_rate) == 0x18c,
    "Native sample rate offset");
_Static_assert(offsetof(PharaohDigitalSoundDeviceState, sample_count) == 0x198,
    "Native sample count offset");
_Static_assert(offsetof(PharaohDigitalSoundDeviceState, sample_handles) == 0x19c,
    "Native sample handle array offset");
_Static_assert(offsetof(PharaohDigitalSoundDeviceState, samples) == 0x1a8,
    "Native sample array offset");
_Static_assert(offsetof(PharaohDigitalSoundDeviceState, cache_entries) == 0x1ac,
    "Native sound cache array offset");
_Static_assert(offsetof(PharaohDigitalSoundDeviceState,
    spatial_sample_count) == 0x1e4,
    "Native spatial sample count offset");
_Static_assert(offsetof(PharaohDigitalSoundDeviceState, sound_disabled) == 0x200,
    "Native sound-disabled offset");
_Static_assert(sizeof(PharaohDigitalSoundDeviceState) == 0x204,
    "Native sound object span used by these procedures");

void pharaoh_digital_sound_sample_reset(
    PharaohDigitalSoundDeviceState *state, int32_t sample_index)
{
    PharaohDigitalSoundSample *const sample = &state->samples[sample_index];

    sample->native_word_00 = UINT32_MAX;
    sample->native_word_04 = UINT32_MAX;
    sample->native_word_08 = UINT32_C(100);
    sample->native_word_0c = UINT32_C(0x40);
    sample->native_word_10 = UINT32_C(1);
    sample->native_word_14 = UINT32_C(0);
    sample->native_word_18 = UINT32_MAX;
    sample->native_word_1c = UINT32_C(0);
    sample->native_word_20 = UINT32_C(0);
    sample->native_word_24 = UINT32_C(0);
}

void pharaoh_digital_sound_stop_all(
    PharaohDigitalSoundDeviceState *state,
    const PharaohDigitalSoundDeviceOperations *operations)
{
    if (state->sound_disabled != 0) {
        return;
    }

    for (int32_t index = 0; index < state->sample_count; ++index) {
        operations->stop_sample(operations->user_data, state, index);
    }
    for (int32_t index = 0; index < state->spatial_sample_count; ++index) {
        operations->stop_spatial_sample(operations->user_data, state, index);
    }
}

int32_t pharaoh_digital_sound_pool_resize(
    PharaohDigitalSoundDeviceState *state, int32_t requested_sample_count,
    const PharaohDigitalSoundDeviceOperations *operations)
{
    if (requested_sample_count < 1) {
        return 0;
    }

    if (state->sample_handles != NULL) {
        pharaoh_digital_sound_stop_all(state, operations);
        for (int32_t index = 0; index < state->sample_count; ++index) {
            pharaoh_digital_sound_sample_reset(state, index);
            operations->release_sample_handle(operations->user_data,
                state->sample_handles[index]);
            if (state->cache_entries[index].locked_data != 0) {
                operations->free_locked_memory(operations->user_data,
                    state->cache_entries[index].locked_data);
                state->cache_entries[index].locked_data = 0;
            }
        }
    }

    if (state->sample_handles != NULL) {
        operations->deallocate_bytes(operations->user_data,
            state->sample_handles);
        state->sample_handles = NULL;
    }
    if (state->auxiliary_handles != NULL) {
        operations->deallocate_bytes(operations->user_data,
            state->auxiliary_handles);
        state->auxiliary_handles = NULL;
    }
    if (state->samples != NULL) {
        operations->deallocate_bytes(operations->user_data, state->samples);
        state->samples = NULL;
    }
    if (state->cache_entries != NULL) {
        operations->deallocate_bytes(operations->user_data,
            state->cache_entries);
        state->cache_entries = NULL;
    }

    const uint32_t count = (uint32_t)requested_sample_count;
    const uint32_t handle_array_bytes = count *
        DIGITAL_SOUND_SAMPLE_HANDLE_STRIDE;
    const uint32_t sample_array_bytes = count *
        (DIGITAL_SOUND_SAMPLE_WORD_COUNT * DIGITAL_SOUND_SAMPLE_WORD_STRIDE);
    const uint32_t cache_array_bytes = count * DIGITAL_SOUND_SAMPLE_CACHE_STRIDE;

    state->sample_handles = operations->allocate_bytes(
        operations->user_data, handle_array_bytes);
    if (state->sample_handles == NULL) {
        return 0;
    }
    state->auxiliary_handles = operations->allocate_bytes(
        operations->user_data, handle_array_bytes);
    if (state->auxiliary_handles == NULL) {
        return 0;
    }
    state->samples = operations->allocate_bytes(
        operations->user_data, sample_array_bytes);
    if (state->samples == NULL) {
        return 0;
    }
    state->cache_entries = operations->allocate_bytes(
        operations->user_data, cache_array_bytes);
    if (state->cache_entries == NULL) {
        return 0;
    }

    state->sample_count = requested_sample_count;
    for (int32_t index = 0; index < state->sample_count; ++index) {
        const uint32_t handle = operations->allocate_sample_handle(
            operations->user_data, state->wave_device);
        state->sample_handles[index] = handle;
        if (handle == 0) {
            break;
        }
    }
    return 1;
}

int32_t pharaoh_digital_sound_device_initialize(
    const PharaohDigitalSoundDeviceContext *context)
{
    PharaohDigitalSoundDeviceState *const state = context->state;
    const PharaohDigitalSoundDeviceOperations *const operations =
        context->operations;
    uint32_t platform_id;
    uint32_t major_version;

    if (operations->get_windows_version(operations->user_data,
            &platform_id, &major_version) == 0) {
        return 0;
    }
    if (platform_id == DIGITAL_SOUND_NT_PLATFORM_ID &&
        major_version < DIGITAL_SOUND_MINIMUM_NT_MAJOR) {
        state->sound_disabled = 1;
        return 0;
    }

    operations->ail_startup(operations->user_data);
    (void)operations->ail_set_preference(operations->user_data,
        DIGITAL_SOUND_PREFERENCE_0X28,
        DIGITAL_SOUND_PREFERENCE_0X28_VALUE);
    (void)operations->ail_set_preference(operations->user_data,
        DIGITAL_SOUND_PREFERENCE_0X0F,
        DIGITAL_SOUND_PREFERENCE_0X0F_VALUE);

    state->wave_format.format_tag = DIGITAL_SOUND_PCM_FORMAT_TAG;
    state->wave_format.channel_count = (uint16_t)state->channels;
    state->wave_format.samples_per_second = (uint32_t)state->sample_rate;
    const uint32_t sample_bytes =
        (uint32_t)(state->bits_per_sample_source /
            DIGITAL_SOUND_BITS_PER_BYTE);
    uint32_t bytes_per_second = (uint32_t)state->sample_rate * sample_bytes;
    bytes_per_second *= (uint32_t)state->channels;
    state->wave_format.average_bytes_per_second = bytes_per_second;
    state->wave_format.block_align = (uint16_t)(
        sample_bytes * (uint32_t)state->channels);
    state->wave_format.bits_per_sample =
        (uint16_t)state->bits_per_sample_source;

    if (operations->wave_out_open(operations->user_data,
            &state->wave_device, DIGITAL_SOUND_WAVE_DEVICE_ID,
            DIGITAL_SOUND_WAVE_OPEN_FLAGS, &state->wave_format) != 0) {
        operations->output_debug_string(operations->user_data,
            wave_output_failure_message);
        return 0;
    }

    if (pharaoh_digital_sound_pool_resize(state,
            DIGITAL_SOUND_INITIAL_SAMPLE_COUNT, operations) == 0) {
        return 0;
    }

    for (int32_t index = 0; index < state->sample_count; ++index) {
        pharaoh_digital_sound_sample_reset(state, index);
    }
    for (int32_t index = 0; index < state->sample_count; ++index) {
        state->cache_entries[index].file_id = -1;
        state->cache_entries[index].locked_data = 0;
        state->cache_entries[index].state = 0;
    }
    return 1;
}
