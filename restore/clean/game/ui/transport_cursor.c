#include "pharaoh/ui/transport_cursor.h"

static uint32_t current_tile_offset(const PharaohTransportCursorGrid *grid)
{
    return grid->tile_origin + grid->current_column +
        grid->current_row * PHARAOH_TRANSPORT_CURSOR_TILE_ROW_STRIDE;
}

int32_t pharaoh_formation_legion_at_grid_offset(
    const PharaohTransportCursorGrid *grid, uint32_t tile_offset)
{
    int32_t walker_id = grid->first_walker_id_by_tile[tile_offset];

    while (walker_id != 0) {
        const PharaohTransportCursorWalkerRecord *const walker =
            &grid->walkers_by_id[(uint32_t)walker_id];

        if (walker->kind >=
                PHARAOH_TRANSPORT_CURSOR_ELIGIBLE_WALKER_KIND_FIRST &&
            walker->kind <=
                PHARAOH_TRANSPORT_CURSOR_ELIGIBLE_WALKER_KIND_LAST) {
            return walker->tag;
        }
        walker_id = walker->next_walker_on_tile;
    }
    return 0;
}

static int32_t fighter_kind_qualifies(
    const PharaohTransportCursorWalkerRecord *walker)
{
    if (walker->kind >= PHARAOH_TRANSPORT_CURSOR_WALKER_ENEMY_ARCHER &&
        walker->kind <= PHARAOH_TRANSPORT_CURSOR_WALKER_KINGDOM_SOLDIER) {
        return 1;
    }
    if (walker->kind == PHARAOH_TRANSPORT_CURSOR_WALKER_NATIVE &&
        walker->action_state == PHARAOH_TRANSPORT_CURSOR_NATIVE_FIGHT_ACTION) {
        return 1;
    }
    switch (walker->kind) {
    case PHARAOH_TRANSPORT_CURSOR_WALKER_CROCODILE:
    case PHARAOH_TRANSPORT_CURSOR_WALKER_HYENA:
    case PHARAOH_TRANSPORT_CURSOR_WALKER_HIPPO:
    case PHARAOH_TRANSPORT_CURSOR_WALKER_ENEMY_TRANSPORT_SHIP:
    case PHARAOH_TRANSPORT_CURSOR_WALKER_ENEMY_WARSHIP:
    case PHARAOH_TRANSPORT_CURSOR_WALKER_BEDOUIN:
    case PHARAOH_TRANSPORT_CURSOR_WALKER_ENEMY_EGYPTIAN_WARSHIP:
    case PHARAOH_TRANSPORT_CURSOR_WALKER_ENEMY_EGYPTIAN_TRANSPORT_SHIP:
    case PHARAOH_TRANSPORT_CURSOR_WALKER_ASP:
    case PHARAOH_TRANSPORT_CURSOR_WALKER_LION:
    case PHARAOH_TRANSPORT_CURSOR_WALKER_SCORPION:
    case PHARAOH_TRANSPORT_CURSOR_WALKER_MUMMY:
        return 1;
    default:
        return 0;
    }
}

int32_t pharaoh_fighter_at_grid_offset(
    const PharaohTransportCursorGrid *grid, uint32_t tile_offset)
{
    int32_t walker_id = grid->first_walker_id_by_tile[tile_offset];

    while (walker_id > 0) {
        const PharaohTransportCursorWalkerRecord *const walker =
            &grid->walkers_by_id[(uint32_t)walker_id];
        if (walker->action_state !=
                PHARAOH_TRANSPORT_CURSOR_WALKER_CORPSE_ACTION &&
            fighter_kind_qualifies(walker)) {
            return walker_id;
        }
        walker_id = walker->next_walker_on_tile;
    }
    return 0;
}

int32_t pharaoh_transport_cursor_can_embark(
    const PharaohTransportCursorContext *context, int32_t walker_id)
{
    if (context->cursor_enabled == 0 ||
        context->grid->walkers_by_id[(uint32_t)walker_id].can_special != 0) {
        return 0;
    }

    return pharaoh_formation_legion_at_grid_offset(context->grid,
        current_tile_offset(context->grid)) != 0;
}

int32_t pharaoh_transport_cursor_can_land(
    const PharaohTransportCursorContext *context, int32_t walker_id)
{
    if (context->cursor_enabled == 0 ||
        context->grid->walkers_by_id[(uint32_t)walker_id].can_special == 0) {
        return 0;
    }

    const uint32_t tile_flags =
        context->grid->flags_by_tile[current_tile_offset(context->grid)];
    return (tile_flags & PHARAOH_TRANSPORT_CURSOR_TILE_FLAG_MASK) ==
            PHARAOH_TRANSPORT_CURSOR_TILE_FLAG_MASK ? 0 : 1;
}
