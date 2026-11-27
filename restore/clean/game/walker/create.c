#include "pharaoh/walker/create.h"
#include "pharaoh/scenario/scenario_new_game_setup.h"

#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohWalkerQueryRecord) == 0x184 &&
    offsetof(PharaohWalkerQueryRecord, state) == 0x0e &&
    offsetof(PharaohWalkerQueryRecord, request_event_id) == 0x132,
    "native walker creation layout");

static int16_t pharaoh_walker_create_low_i16(int32_t value)
{
    const uint16_t bits = (uint16_t)(uint32_t)value;
    int16_t result;
    memcpy(&result, &bits, sizeof(result));
    return result;
}

int32_t pharaoh_walker_create(PharaohWalkerCreateContext *context,
    uint8_t owner, int32_t type, int32_t x, int32_t y, int8_t direction)
{
    int32_t walker_id;

    for (walker_id = 1; walker_id < PHARAOH_WALKER_RECORD_COUNT;
         ++walker_id) {
        PharaohWalkerQueryRecord *const walker =
            &context->walkers[walker_id];
        uint32_t grid_offset;
        uint32_t sequence;
        uint32_t next_sequence;
        uint32_t random_direction;
        int16_t coordinate_x;
        int16_t coordinate_y;
        uint16_t pixel_x;
        uint16_t pixel_y;

        if (walker->state != 0 || pharaoh_walker_id_is_reserved(
                context->ferry_queues, context->group_children,
                walker_id) != 0) {
            continue;
        }

        memset(walker, 0, sizeof(*walker));
        walker->state = 1;
        walker->owner = owner;
        walker->type = (uint8_t)type;
        walker->use_cross_country = 0;
        walker->reserved_00d = 1;
        walker->request_event_id = -1;

        sequence = (uint32_t)*context->created_sequence;
        {
            const uint16_t sequence_bits = (uint16_t)sequence;
            memcpy(&walker->created_sequence, &sequence_bits,
                sizeof(sequence_bits));
        }
        next_sequence = sequence + 1u;
        memcpy(context->created_sequence, &next_sequence,
            sizeof(next_sequence));
        walker->direction = (uint8_t)direction;

        coordinate_x = pharaoh_walker_create_low_i16(x);
        coordinate_y = pharaoh_walker_create_low_i16(y);
        walker->x = coordinate_x;
        walker->y = coordinate_y;
        walker->destination_x = coordinate_x;
        walker->destination_y = coordinate_y;
        walker->source_x = coordinate_x;
        walker->source_y = coordinate_y;
        memcpy((uint8_t *)walker + 0x18, &coordinate_x,
            sizeof(coordinate_x));
        memcpy((uint8_t *)walker + 0x1a, &coordinate_y,
            sizeof(coordinate_y));

        grid_offset = (uint32_t)context->tile_grid_base_offset +
            (uint32_t)y * UINT32_C(0xe4) + (uint32_t)x;
        memcpy(&walker->grid_offset, &grid_offset, sizeof(grid_offset));

        pixel_x = (uint16_t)((uint32_t)(int32_t)coordinate_x * 15u);
        pixel_y = (uint16_t)((uint32_t)(int32_t)coordinate_y * 15u);
        memcpy((uint8_t *)walker + 0x4a, &pixel_x, sizeof(pixel_x));
        memcpy((uint8_t *)walker + 0x4c, &pixel_y, sizeof(pixel_y));
        walker->progress_on_tile = 15;

        random_direction = pharaoh_crt_random_next(context->random) & 3u;
        walker->reserved_07c[0] = (uint8_t)random_direction;
        walker->reserved_07c[2] = (uint8_t)random_direction;
        pharaoh_walker_name_set(context->walkers, context->formations,
            context->name_counters, walker_id);
        pharaoh_walker_attach_to_tile(context->tile_chain, walker_id);

        if (type == 0x13 || type == 0x14) {
            pharaoh_trader_create(context->trader_records,
                context->walkers, walker_id, context->next_trader_record);
        }
        if (*context->highest_walker_id < walker_id) {
            *context->highest_walker_id = walker_id;
        }
        return walker_id;
    }

    return 0;
}

void pharaoh_walker_reserved_initialize(
    PharaohWalkerReservedInitContext *context)
{
    PharaohWalkerCreateContext *const create = context->walker_create;
    int32_t walker_id;
    uint32_t tag;

    pharaoh_walker_init_scenario(create->walkers, create->highest_walker_id,
        create->created_sequence);
    pharaoh_scenario_invasion_amounts_clear(context->invasion_reset);

    for (tag = 1; tag <= 3; ++tag) {
        walker_id = pharaoh_walker_create(create, 1, 0x24, -1, -1, 0);
        create->walkers[walker_id].counting_action_00b = (uint8_t)tag;
    }

    (void)pharaoh_walker_create(create, 1, 0x24, -1, -1, 0);

    for (tag = 0x0c; tag <= 0x20; ++tag) {
        walker_id = pharaoh_walker_create(create, 1, 0x24, -1, -1, 0);
        create->walkers[walker_id].counting_action_00b = (uint8_t)tag;
    }

    for (tag = 0x32; tag <= 0x41; ++tag) {
        walker_id = pharaoh_walker_create(create, 1, 0x24, -1, -1, 0);
        create->walkers[walker_id].counting_action_00b = (uint8_t)tag;
    }
}
