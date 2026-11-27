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

static int32_t wrapped_add3(int32_t a, int32_t b, int32_t c)
{
    return (int32_t)((uint32_t)a + (uint32_t)b + (uint32_t)c);
}

static int32_t scaled_difference(int32_t difference, int32_t frame,
    int32_t last_frame)
{
    return (int32_t)((uint32_t)difference * (uint32_t)frame) / last_frame;
}

static void get_draw_info_for_view(
    const PharaohMonumentSpotDrawContext *context,
    const PharaohMonumentSpotAnimation *spot,
    int32_t *x, int32_t *y, int32_t *image, int32_t view)
{
    const PharaohMonumentAnimScript *script;
    const PharaohMonumentAnimSequence *sequence;
    const PharaohMonumentAnimImages *images;
    const PharaohMonumentAnimPosition *position_from;
    const PharaohMonumentAnimPosition *position_to;
    int32_t image_add;
    int32_t known_image;
    int32_t difference;

    /* Native write order is observable if output pointers alias. */
    *x = 0;
    *y = 0;
    *image = 0;
    if (spot->mode == PHARAOH_MONUMENT_SPOT_IDLE || spot->script < 0 ||
        spot->script >= context->script_count) return;

    script = &context->scripts[spot->script];
    if (spot->sequence < 0 || spot->sequence >= script->sequence_count) return;
    sequence = &script->sequences[spot->sequence];
    image_add = spot->reverse != 0 ? sequence->image_add_reverse[view] :
        sequence->image_add[view];
    if (image_add < 0 || sequence->images < 0 ||
        sequence->images >= script->image_count) return;
    if (sequence->position_from < 0 ||
        sequence->position_from >= script->position_count ||
        sequence->position_to < 0 ||
        sequence->position_to >= script->position_count) return;
    if (spot->frame < 0 || spot->frame > sequence->last_frame) return;

    images = &script->images[sequence->images];
    position_from = &script->positions[
        sequence->position_from * PHARAOH_MONUMENT_ANIMATION_VIEWS + view];
    position_to = &script->positions[
        sequence->position_to * PHARAOH_MONUMENT_ANIMATION_VIEWS + view];

    *image = (int32_t)((uint32_t)(spot->frame % images->frames) *
        (uint32_t)images->frame_step +
        (uint32_t)context->image_group_first[images->image_group] +
        (uint32_t)images->image_offset + (uint32_t)image_add);
    difference = (int32_t)((uint32_t)position_to->x -
        (uint32_t)position_from->x);
    *x = wrapped_add3(scaled_difference(difference, spot->frame,
        sequence->last_frame), script->x_offset, position_from->x);
    difference = (int32_t)((uint32_t)position_to->y -
        (uint32_t)position_from->y);
    *y = wrapped_add3(scaled_difference(difference, spot->frame,
        sequence->last_frame), script->y_offset, position_from->y);

    known_image = context->image_record_count <= *image ? 0 : *image;
    *x = (int32_t)((uint32_t)*x -
        (uint32_t)(int32_t)context->sprite_offset_x[known_image]);
    known_image = context->image_record_count <= *image ? 0 : *image;
    *y = (int32_t)((uint32_t)*y -
        (uint32_t)(int32_t)context->sprite_offset_y[known_image]);
}

void pharaoh_monument_spot_get_draw_info(
    const PharaohMonumentSpotDrawContext *context,
    const PharaohMonumentSpotAnimation *spot,
    int32_t *x, int32_t *y, int32_t *image)
{
    get_draw_info_for_view(context, spot, x, y, image,
        context->map_orientation / 2);
}

void pharaoh_monument_spot_get_draw_info_for_direction(
    const PharaohMonumentSpotDrawContext *context,
    const PharaohMonumentSpotAnimation *spot,
    int32_t *x, int32_t *y, int32_t *image, int32_t direction)
{
    int32_t view = direction - context->map_orientation / 2;
    if (view < 0) view += PHARAOH_MONUMENT_ANIMATION_VIEWS;
    get_draw_info_for_view(context, spot, x, y, image, view);
}
