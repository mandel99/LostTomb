#include "pharaoh/runtime/sound_effect_request.h"

static void dispatch_to_player(void *data,
    const PharaohSoundEffectPlayRequest *request)
{
    const PharaohDigitalSoundDeviceContext *player=
        *(const PharaohDigitalSoundDeviceContext *const *)data;
    const PharaohDigitalSoundRequest actual={
        request->sample_match_tag,request->sound_file_id,request->volume,
        request->pan,request->loop_count};
    (void)pharaoh_digital_sound_play_sample(
        player->state,player->operations,&actual);
}

void pharaoh_sound_effect_request_play(PharaohSoundEffectRequestState *state,
    const PharaohDigitalSoundDeviceContext *player,int32_t effect_slot)
{
    const PharaohDigitalSoundDeviceContext *live=player;
    const PharaohSoundEffectRequestOperations operations={&live,dispatch_to_player};
    pharaoh_sound_effect_request(state,&operations,effect_slot);
}
