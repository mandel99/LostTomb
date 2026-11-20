#include "pharaoh/ui/map_selection_rating.h"

static int32_t add_native_i32(int32_t left, int32_t right)
{
    return (int32_t)((uint32_t)left + (uint32_t)right);
}

int32_t pharaoh_map_selection_rating(
    const PharaohMapSelectionRatingInput *input)
{
    int32_t rating = add_native_i32(input->rating_terms[3],
        (int32_t)((uint32_t)add_native_i32(input->rating_terms[1],
            add_native_i32((int32_t)((uint32_t)input->rating_terms[2] * 10u),
                input->rating_terms[0])) * 2u));
    int32_t marked_event_count = 0;
    int32_t god_weight = 0;

    if (input->population_rating_enabled != 0 && input->population > 500) {
        if (input->population < 2001) rating = add_native_i32(rating, 20);
        else if (input->population < 4001) rating = add_native_i32(rating, 50);
        else if (input->population < 8001) rating = add_native_i32(rating, 80);
        else rating = add_native_i32(rating, 100);
    }

    if (input->invasion_count > 0) {
        if (input->invasion_count < 3) rating = add_native_i32(rating, 40);
        else if (input->invasion_count < 5) rating = add_native_i32(rating, 60);
        else if (input->invasion_count < 11) rating = add_native_i32(rating, 80);
        else rating = add_native_i32(rating, 100);
    }

    /* The native reverse scan counts event records 1..count-1, not record 0. */
    if (input->event_count > 0) {
        for (int32_t index = input->event_count - 1; index > 0; --index) {
            if (input->events[index].kind == 1) ++marked_event_count;
        }
    }
    if (marked_event_count > 5) {
        if (marked_event_count < 11) rating = add_native_i32(rating, 40);
        else if (marked_event_count < 21) rating = add_native_i32(rating, 60);
        else if (marked_event_count < 31) rating = add_native_i32(rating, 80);
        else rating = add_native_i32(rating, 100);
    } else {
        rating = add_native_i32(rating, 20);
    }

    for (int32_t index = 0; index < 5; ++index) {
        if (input->god_settings[index] == 1) ++god_weight;
        else if (input->god_settings[index] == 2) god_weight += 2;
    }
    if (god_weight > 0) {
        if (god_weight < 3) return add_native_i32(rating, 20);
        if (god_weight < 6) return add_native_i32(rating, 50);
        if (god_weight < 9) return add_native_i32(rating, 80);
        rating = add_native_i32(rating, 100);
    }
    return rating;
}
