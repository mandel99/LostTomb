#ifndef PHARAOH_RUNTIME_SOUND_EFFECT_REQUEST_H
#define PHARAOH_RUNTIME_SOUND_EFFECT_REQUEST_H

#include <stdint.h>

enum {
    PHARAOH_SOUND_EFFECT_REQUEST_SLOT_COUNT = 19,
    PHARAOH_SOUND_EFFECT_REQUEST_PAN_CENTER = 0x40,
    PHARAOH_SOUND_EFFECT_REQUEST_LOOP_COUNT = 1
};

typedef struct PharaohSoundEffectPlayRequest {
    int32_t sample_match_tag;
    int32_t sound_file_id;
    int32_t volume;
    int32_t pan;
    int32_t loop_count;
} PharaohSoundEffectPlayRequest;

_Static_assert(sizeof(PharaohSoundEffectPlayRequest) == 0x14,
               "native sound request size");

typedef struct PharaohSoundEffectRequestState {
    int32_t digital_sound_available;
    uint8_t native_sound_gate;
    const int32_t *sound_file_ids;
    int32_t request_volume;
    PharaohSoundEffectPlayRequest *request;
} PharaohSoundEffectRequestState;

typedef struct PharaohSoundEffectRequestOperations {
    void *user_data;
    void (*dispatch)(void *user_data,
                     const PharaohSoundEffectPlayRequest *request);
} PharaohSoundEffectRequestOperations;

/* Original entry 0x0053B090. The caller supplies a valid effect slot in
   [0, PHARAOH_SOUND_EFFECT_REQUEST_SLOT_COUNT). */
void pharaoh_sound_effect_request(
    PharaohSoundEffectRequestState *state,
    const PharaohSoundEffectRequestOperations *operations,
    int32_t effect_slot);

#endif
