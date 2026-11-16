#include "pharaoh/ui/monument_tokens.h"

#include <stdbool.h>
#include <stdio.h>

enum PharaohMonumentTokenAmounts {
    STORED_UNITS_PER_LOAD_COUNTED = 400,
    STORED_UNITS_COUNTED = 4,
    PROGRESS_PER_UNIT = 100,
    LOADS_PER_UNIT = 400,
    SCRIPTED_STOCK_LIMIT = 800,
    LIBRARY_STOCK_LIMIT = 4000,
    LIBRARY_STOCK_FIRST_PART = 3200,
    CAESAREUM_GRANITE_REMAINDER = 8,
    CAESAREUM_GRANITE_REMAINDER_LOADS = 2,
    LIBRARY_COPPER_REMAINDER_LOADS = 8,
    LIBRARY_REMAINDER_SMALL = 8,
    LIBRARY_REMAINDER_SMALL_LOADS = 2,
    SPHINX_STAGES = 16,
    MONUMENT_PROGRESS_COMPLETE = 100,
    AMOUNT_TEXT_LOAD = 54
};

static bool counted_in_loads(int32_t resource)
{
    return resource == PHARAOH_MONUMENT_RESOURCE_BRICKS ||
        resource == PHARAOH_MONUMENT_RESOURCE_COPPER;
}

static int32_t first_amount(
    const PharaohMonumentTokenContext *context, int32_t value)
{
    return counted_in_loads(context->resources.first) ?
        value * STORED_UNITS_PER_LOAD_COUNTED : value * STORED_UNITS_COUNTED;
}

static int32_t second_amount_bricks_only(
    const PharaohMonumentTokenContext *context, int32_t value)
{
    return context->resources.second == PHARAOH_MONUMENT_RESOURCE_BRICKS ?
        value * STORED_UNITS_PER_LOAD_COUNTED : value * STORED_UNITS_COUNTED;
}

static const char *amount_text(const PharaohMonumentTokenContext *context,
    PharaohTextBuffer buffer, int32_t resource, int32_t amount)
{
    const char *name = context->resource_name(context->user_data, resource);
    if (name == NULL) {
        return NULL;
    }
    (void) snprintf(buffer.data, buffer.capacity, "%d %s", amount, name);
    return buffer.data;
}

const char *pharaoh_monument_token_loads_word(
    const PharaohMonumentTokenContext *context, int32_t count)
{
    return context->lookup_text(context->user_data, PHARAOH_TEXT_GROUP_AMOUNTS,
        (count != 1) + AMOUNT_TEXT_LOAD);
}

static const char *loads_text(PharaohMonumentTokenContext *context,
    enum PharaohMonumentPhraseBufferIndex buffer_index, int32_t count)
{
    const char *word = pharaoh_monument_token_loads_word(context, count);
    PharaohTextBuffer buffer = context->phrase_buffers[buffer_index];
    (void) snprintf(buffer.data, buffer.capacity, "%d %s", count, word);
    return buffer.data;
}

static bool remaining_main_loads(
    const PharaohMonumentTokenContext *context, int32_t *count)
{
    int32_t value;
    if (context->building_type == PHARAOH_MONUMENT_PYRAMID) {
        *count = context->pyramid.main_remaining;
        return true;
    }
    if (context->building_type == PHARAOH_MONUMENT_SUN_TEMPLE) {
        value = context->sun_temple_material;
    } else if (context->building_type == PHARAOH_MONUMENT_LIGHTHOUSE) {
        value = context->lighthouse.material;
    } else if (context->building_type == PHARAOH_MONUMENT_CAESAREUM) {
        value = context->caesareum.material;
        if (context->resources.first == PHARAOH_MONUMENT_RESOURCE_GRANITE &&
            value > SCRIPTED_STOCK_LIMIT) {
            *count = CAESAREUM_GRANITE_REMAINDER_LOADS;
            return true;
        }
    } else if (context->building_type == PHARAOH_MONUMENT_ALEXANDRIA_LIBRARY) {
        int32_t stock = context->library.material;
        if (context->resources.first == PHARAOH_MONUMENT_RESOURCE_COPPER) {
            if (stock > LIBRARY_STOCK_LIMIT) {
                *count = LIBRARY_COPPER_REMAINDER_LOADS;
                return true;
            }
            value = stock - SCRIPTED_STOCK_LIMIT;
        } else if (stock > LIBRARY_STOCK_LIMIT) {
            value = stock - LIBRARY_STOCK_FIRST_PART;
        } else {
            *count = stock < SCRIPTED_STOCK_LIMIT ? 0 :
                LIBRARY_REMAINDER_SMALL_LOADS;
            return true;
        }
    } else if (context->building_type == PHARAOH_MONUMENT_MAUSOLEUM) {
        value = context->mausoleum.material;
    } else {
        return false;
    }
    *count = value / LOADS_PER_UNIT;
    return true;
}

static bool remaining_main_amount(const PharaohMonumentTokenContext *context,
    int32_t resource, int32_t *amount)
{
    int32_t value;
    if (context->building_type == PHARAOH_MONUMENT_PYRAMID) {
        *amount = first_amount(context, context->pyramid.main_remaining);
        return true;
    }
    if (context->building_type == PHARAOH_MONUMENT_SUN_TEMPLE) {
        value = context->sun_temple_material;
    } else if (context->building_type == PHARAOH_MONUMENT_CAESAREUM) {
        value = context->caesareum.material;
        if (resource == PHARAOH_MONUMENT_RESOURCE_GRANITE &&
            value > SCRIPTED_STOCK_LIMIT) {
            *amount = CAESAREUM_GRANITE_REMAINDER;
            return true;
        }
    } else if (context->building_type == PHARAOH_MONUMENT_LIGHTHOUSE) {
        value = context->lighthouse.material;
    } else if (context->building_type == PHARAOH_MONUMENT_ALEXANDRIA_LIBRARY) {
        int32_t stock = context->library.material;
        if (resource == PHARAOH_MONUMENT_RESOURCE_COPPER) {
            *amount = stock > LIBRARY_STOCK_LIMIT ?
                LIBRARY_STOCK_FIRST_PART : stock - SCRIPTED_STOCK_LIMIT;
            return true;
        }
        if (stock <= LIBRARY_STOCK_LIMIT) {
            *amount = stock < SCRIPTED_STOCK_LIMIT ? 0 : LIBRARY_REMAINDER_SMALL;
            return true;
        }
        value = stock - LIBRARY_STOCK_FIRST_PART;
    } else if (context->building_type == PHARAOH_MONUMENT_MAUSOLEUM) {
        value = context->mausoleum.material;
    } else {
        return false;
    }
    *amount = value / PROGRESS_PER_UNIT;
    return true;
}

const char *pharaoh_monument_token_name(
    const PharaohMonumentTokenContext *context)
{
    return context->monument_name(context->user_data, context->building_id);
}

const char *pharaoh_monument_token_percent_complete(
    PharaohMonumentTokenContext *context)
{
    int32_t value;
    switch (context->building_type) {
        case PHARAOH_MONUMENT_PYRAMID:
            value = context->pyramid.percent;
            break;
        case PHARAOH_MONUMENT_SMALL_OBELISK:
        case PHARAOH_MONUMENT_LARGE_OBELISK:
            value = context->obelisk_progress;
            break;
        case PHARAOH_MONUMENT_SPHINX:
            value = ((int32_t) context->sphinx_stage * MONUMENT_PROGRESS_COMPLETE) /
                SPHINX_STAGES;
            break;
        case PHARAOH_MONUMENT_SUN_TEMPLE:
            value = context->sun_temple_percent;
            break;
        case PHARAOH_MONUMENT_MAUSOLEUM:
            value = context->mausoleum.percent;
            break;
        case PHARAOH_MONUMENT_ALEXANDRIA_LIBRARY:
            value = context->library.percent;
            break;
        case PHARAOH_MONUMENT_CAESAREUM:
            value = context->caesareum.percent;
            break;
        case PHARAOH_MONUMENT_LIGHTHOUSE:
            value = context->lighthouse.percent;
            break;
        case PHARAOH_MONUMENT_SMALL_ROYAL_TOMB:
        case PHARAOH_MONUMENT_MEDIUM_ROYAL_TOMB:
        case PHARAOH_MONUMENT_LARGE_ROYAL_TOMB:
        case PHARAOH_MONUMENT_GRAND_ROYAL_TOMB:
            value = context->tomb_percent;
            break;
        case PHARAOH_MONUMENT_ABU_SIMBEL:
            value = context->abu_simbel_percent;
            break;
        default:
            return NULL;
    }
    if (value < 0) {
        value = 0;
    } else if (value > MONUMENT_PROGRESS_COMPLETE) {
        value = MONUMENT_PROGRESS_COMPLETE;
    }
    (void) snprintf(context->percent_buffer.data,
        context->percent_buffer.capacity, "%d%%", value);
    return context->percent_buffer.data;
}

const char *pharaoh_monument_token_courses_complete(
    PharaohMonumentTokenContext *context)
{
    (void) snprintf(context->courses_buffer.data,
        context->courses_buffer.capacity, "%d", context->pyramid.height);
    return context->courses_buffer.data;
}

const char *pharaoh_monument_token_needed_current_main(
    PharaohMonumentTokenContext *context)
{
    return amount_text(context,
        context->phrase_buffers[PHARAOH_MONUMENT_BUFFER_NEEDED_CURRENT_MAIN],
        context->resources.first,
        first_amount(context, context->pyramid.frontier_main));
}

const char *pharaoh_monument_token_needed_current_secondary(
    PharaohMonumentTokenContext *context)
{
    return amount_text(context,
        context->phrase_buffers[PHARAOH_MONUMENT_BUFFER_NEEDED_CURRENT_SECONDARY],
        context->resources.second,
        second_amount_bricks_only(context, context->pyramid.frontier_casing));
}

const char *pharaoh_monument_token_needed_remainder_main(
    PharaohMonumentTokenContext *context)
{
    int32_t amount;
    if (!remaining_main_amount(context, context->resources.first, &amount)) {
        return NULL;
    }
    return amount_text(context,
        context->phrase_buffers[PHARAOH_MONUMENT_BUFFER_NEEDED_REMAINDER_MAIN],
        context->resources.first, amount);
}

const char *pharaoh_monument_token_needed_remainder_secondary(
    PharaohMonumentTokenContext *context)
{
    int32_t amount = counted_in_loads(context->resources.second) ?
        context->pyramid.casing_remaining * STORED_UNITS_PER_LOAD_COUNTED :
        context->pyramid.casing_remaining * STORED_UNITS_COUNTED;
    return amount_text(context,
        context->phrase_buffers[PHARAOH_MONUMENT_BUFFER_NEEDED_REMAINDER_SECONDARY],
        context->resources.second, amount);
}

const char *pharaoh_monument_token_loads_current_main(
    PharaohMonumentTokenContext *context)
{
    int32_t count = context->pyramid.frontier_main;
    const char *word = pharaoh_monument_token_loads_word(context, count);
    PharaohTextBuffer buffer =
        context->phrase_buffers[PHARAOH_MONUMENT_BUFFER_LOADS_CURRENT_MAIN];
    if (word == NULL) {
        return NULL;
    }
    (void) snprintf(buffer.data, buffer.capacity, "%d %s", count, word);
    return buffer.data;
}

const char *pharaoh_monument_token_loads_current_secondary(
    PharaohMonumentTokenContext *context)
{
    return loads_text(context, PHARAOH_MONUMENT_BUFFER_LOADS_CURRENT_SECONDARY,
        context->pyramid.frontier_casing);
}

const char *pharaoh_monument_token_loads_remainder_main(
    PharaohMonumentTokenContext *context)
{
    int32_t count;
    if (!remaining_main_loads(context, &count)) {
        return NULL;
    }
    return loads_text(
        context, PHARAOH_MONUMENT_BUFFER_LOADS_REMAINDER_MAIN, count);
}

const char *pharaoh_monument_token_loads_remainder_secondary(
    PharaohMonumentTokenContext *context)
{
    return loads_text(context, PHARAOH_MONUMENT_BUFFER_LOADS_REMAINDER_SECONDARY,
        context->pyramid.casing_remaining);
}

const char *pharaoh_monument_token_total_main(
    PharaohMonumentTokenContext *context)
{
    int32_t amount;
    if (context->building_type == PHARAOH_MONUMENT_PYRAMID) {
        amount = first_amount(context, context->pyramid.main_total);
    } else if (context->building_type == PHARAOH_MONUMENT_SUN_TEMPLE) {
        amount = context->sun_temple_material_total / PROGRESS_PER_UNIT;
    } else if (context->building_type == PHARAOH_MONUMENT_MAUSOLEUM) {
        amount = context->mausoleum.material_total / PROGRESS_PER_UNIT;
    } else {
        return NULL;
    }
    return amount_text(context,
        context->phrase_buffers[PHARAOH_MONUMENT_BUFFER_TOTAL_MAIN],
        context->resources.first, amount);
}

const char *pharaoh_monument_token_total_secondary(
    PharaohMonumentTokenContext *context)
{
    return amount_text(context,
        context->phrase_buffers[PHARAOH_MONUMENT_BUFFER_TOTAL_SECONDARY],
        context->resources.second,
        second_amount_bricks_only(context, context->pyramid.casing_total));
}
