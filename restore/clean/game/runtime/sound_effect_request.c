#include "pharaoh/runtime/sound_effect_request.h"

void pharaoh_sound_effect_request(
    PharaohSoundEffectRequestState *state,
    const PharaohSoundEffectRequestOperations *operations,
    int32_t effect_slot)
{
    if (state->digital_sound_available == 0 ||
        effect_slot >= PHARAOH_SOUND_EFFECT_REQUEST_SLOT_COUNT ||
        state->native_sound_gate == 0) {
        return;
    }

    const int32_t sound_file_id = state->sound_file_ids[effect_slot];
    if (sound_file_id < 0) {
        return;
    }

    state->request->sample_match_tag = -1;
    state->request->sound_file_id = sound_file_id;
    state->request->volume = state->request_volume;
    state->request->pan = PHARAOH_SOUND_EFFECT_REQUEST_PAN_CENTER;
    state->request->loop_count = PHARAOH_SOUND_EFFECT_REQUEST_LOOP_COUNT;
    operations->dispatch(operations->user_data, state->request);
}
