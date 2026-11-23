#include "pharaoh/rendering/monument_spot_animation.h"

#include <stddef.h>

enum { PHARAOH_MONUMENT_SPOT_IDLE = 3 };

_Static_assert(sizeof(PharaohMonumentWorkSpotBinding) == 16 &&
               offsetof(PharaohMonumentWorkSpotBinding,
                        initial_work_remaining) == 12 &&
               offsetof(PharaohMonumentWorkSpotBinding, spot) == 8,
               "native monument work-spot binding layout changed");

void pharaoh_monument_spot_start(
    PharaohMonumentSpotAnimation *spot, int16_t mode,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random)
{
    const PharaohMonumentAnimScript *script;
    const PharaohMonumentAnimTransition *transitions;
    const PharaohMonumentAnimTransition *choice;
    int32_t row;
    int32_t choice_count;

    spot->mode = mode;
    if (spot->script < 0 || spot->script >= script_count)
        return;

    script = &scripts[spot->script];
    spot->sequence = -1;
    if (script->transition_count <= 0)
        return;

    transitions = (const PharaohMonumentAnimTransition *)script->transitions;
    for (row = 0; row < script->transition_count; ++row) {
        if (transitions[row * 5].sequence < 0)
            break;
    }
    if (row == script->transition_count)
        return;

    choice = &transitions[row * 5 + 1];
    for (choice_count = 0; choice_count < 4; ++choice_count) {
        if (choice[choice_count].sequence < 0)
            break;
    }
    if (choice_count == 0)
        return;

    choice += (int32_t)(pharaoh_crt_random_next(random) %
        (uint32_t)choice_count);
    spot->sequence = choice->sequence;
    spot->reverse = choice->reverse;
    if (spot->sequence < 0 || spot->sequence >= script->sequence_count) {
        spot->sequence = -1;
        return;
    }
    spot->frame = spot->reverse != 0 ?
        script->sequences[spot->sequence].last_frame : 0;
}

static void reset_work_spots(const PharaohMonumentWorkSpotBinding *bindings,
    int32_t count, const PharaohMonumentAnimScript *scripts,
    int32_t script_count, PharaohCrtRandom *random)
{
    int32_t index;
    for (index = 0; index < count; ++index) {
        PharaohMonumentSpotAnimation *spot = bindings[index].spot;
        spot->walker_id = 0;
        pharaoh_monument_spot_start(spot, PHARAOH_MONUMENT_SPOT_IDLE,
            scripts, script_count, random);
        spot->work_remaining = bindings[index].initial_work_remaining;
    }
}

void pharaoh_sphinx_reset_work_spots(const PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random)
{
    reset_work_spots(bindings, 22, scripts, script_count, random);
}

void pharaoh_sun_temple_reset_work_spots(const PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random)
{
    reset_work_spots(bindings, 13, scripts, script_count, random);
}

void pharaoh_library_reset_work_spots(const PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random)
{
    reset_work_spots(bindings, 27, scripts, script_count, random);
}

void pharaoh_abu_simbel_reset_work_spots(const PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random)
{
    reset_work_spots(bindings, 56, scripts, script_count, random);
}

void pharaoh_caesareum_reset_work_spots(const PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random)
{
    reset_work_spots(bindings, 27, scripts, script_count, random);
}

void pharaoh_lighthouse_reset_work_spots(const PharaohMonumentWorkSpotBinding *bindings,
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random)
{
    reset_work_spots(bindings, 15, scripts, script_count, random);
}

void pharaoh_royal_tomb_reset_work_spots(
    const PharaohMonumentWorkSpotGroup groups[25],
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random)
{
    int32_t group_index;
    for (group_index = 0; group_index < 25; ++group_index) {
        const PharaohMonumentWorkSpotBinding *bindings = groups[group_index].bindings;
        int32_t spot_index;
        for (spot_index = 0; spot_index < 3; ++spot_index) {
            PharaohMonumentSpotAnimation *spot = bindings[spot_index].spot;
            spot->walker_id = 0;
            pharaoh_monument_spot_start(spot, PHARAOH_MONUMENT_SPOT_IDLE,
                scripts, script_count, random);
            spot->work_remaining = groups[group_index].initial_work_override != NULL ?
                groups[group_index].initial_work_override[spot_index].initial_work_remaining :
                bindings[spot_index].initial_work_remaining;
        }
    }
}

void pharaoh_obelisk_reset_work_spots(int16_t building_type,
    const PharaohMonumentWorkSpotBinding small_bindings[10],
    const PharaohMonumentWorkSpotBinding large_bindings[18],
    const PharaohMonumentAnimScript *scripts, int32_t script_count,
    PharaohCrtRandom *random)
{
    const PharaohMonumentWorkSpotBinding *bindings =
        building_type == 207 ? small_bindings : large_bindings;
    reset_work_spots(bindings, building_type == 207 ? 10 : 18,
        scripts, script_count, random);
}
