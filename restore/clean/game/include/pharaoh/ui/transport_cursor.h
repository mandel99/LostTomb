#ifndef PHARAOH_UI_TRANSPORT_CURSOR_H
#define PHARAOH_UI_TRANSPORT_CURSOR_H

#include <stddef.h>
#include <stdint.h>

enum {
    PHARAOH_TRANSPORT_CURSOR_TILE_ROW_STRIDE = 0xE4,
    PHARAOH_TRANSPORT_CURSOR_WALKER_RECORD_BYTES = 0x184,
    PHARAOH_TRANSPORT_CURSOR_WALKER_NEXT_OFFSET = 0x08,
    PHARAOH_TRANSPORT_CURSOR_WALKER_KIND_OFFSET = 0x0A,
    PHARAOH_TRANSPORT_CURSOR_WALKER_ACTION_OFFSET = 0x38,
    PHARAOH_TRANSPORT_CURSOR_WALKER_TAG_OFFSET = 0x60,
    PHARAOH_TRANSPORT_CURSOR_WALKER_CAN_SPECIAL_OFFSET = 0x130,
    PHARAOH_TRANSPORT_CURSOR_ELIGIBLE_WALKER_KIND_FIRST = 11,
    PHARAOH_TRANSPORT_CURSOR_ELIGIBLE_WALKER_KIND_LAST = 14,
    PHARAOH_TRANSPORT_CURSOR_WALKER_CORPSE_ACTION = 4,
    PHARAOH_TRANSPORT_CURSOR_NATIVE_FIGHT_ACTION = 11,
    PHARAOH_TRANSPORT_CURSOR_WALKER_NATIVE = 41,
    PHARAOH_TRANSPORT_CURSOR_WALKER_ENEMY_ARCHER = 43,
    PHARAOH_TRANSPORT_CURSOR_WALKER_KINGDOM_SOLDIER = 57,
    PHARAOH_TRANSPORT_CURSOR_WALKER_CROCODILE = 82,
    PHARAOH_TRANSPORT_CURSOR_WALKER_HYENA = 83,
    PHARAOH_TRANSPORT_CURSOR_WALKER_HIPPO = 84,
    PHARAOH_TRANSPORT_CURSOR_WALKER_ENEMY_TRANSPORT_SHIP = 92,
    PHARAOH_TRANSPORT_CURSOR_WALKER_ENEMY_WARSHIP = 93,
    PHARAOH_TRANSPORT_CURSOR_WALKER_BEDOUIN = 99,
    PHARAOH_TRANSPORT_CURSOR_WALKER_ENEMY_EGYPTIAN_WARSHIP = 100,
    PHARAOH_TRANSPORT_CURSOR_WALKER_ENEMY_EGYPTIAN_TRANSPORT_SHIP = 101,
    PHARAOH_TRANSPORT_CURSOR_WALKER_ASP = 102,
    PHARAOH_TRANSPORT_CURSOR_WALKER_LION = 103,
    PHARAOH_TRANSPORT_CURSOR_WALKER_SCORPION = 104,
    PHARAOH_TRANSPORT_CURSOR_WALKER_MUMMY = 109,
    PHARAOH_TRANSPORT_CURSOR_TILE_FLAG_MASK = 0x04
};

typedef struct PharaohTransportCursorWalkerRecord {
    uint8_t reserved_0000[8];
    int16_t next_walker_on_tile;
    uint8_t kind;
    uint8_t reserved_000B[45];
    uint8_t action_state;
    uint8_t reserved_0039[39];
    int16_t tag;
    uint8_t reserved_0062[206];
    uint8_t can_special;
    uint8_t reserved_0131[83];
} PharaohTransportCursorWalkerRecord;

_Static_assert(sizeof(PharaohTransportCursorWalkerRecord) ==
        PHARAOH_TRANSPORT_CURSOR_WALKER_RECORD_BYTES,
    "transport cursor walker rows preserve native stride");
_Static_assert(offsetof(PharaohTransportCursorWalkerRecord,
        next_walker_on_tile) == PHARAOH_TRANSPORT_CURSOR_WALKER_NEXT_OFFSET &&
    offsetof(PharaohTransportCursorWalkerRecord, kind) ==
        PHARAOH_TRANSPORT_CURSOR_WALKER_KIND_OFFSET &&
    offsetof(PharaohTransportCursorWalkerRecord, action_state) ==
        PHARAOH_TRANSPORT_CURSOR_WALKER_ACTION_OFFSET &&
    offsetof(PharaohTransportCursorWalkerRecord, tag) ==
        PHARAOH_TRANSPORT_CURSOR_WALKER_TAG_OFFSET &&
    offsetof(PharaohTransportCursorWalkerRecord, can_special) ==
        PHARAOH_TRANSPORT_CURSOR_WALKER_CAN_SPECIAL_OFFSET,
    "transport cursor walker members match observed native offsets");

typedef struct PharaohTransportCursorGrid {
    const int16_t *first_walker_id_by_tile;
    const PharaohTransportCursorWalkerRecord *walkers_by_id;
    const uint32_t *flags_by_tile;
    uint32_t current_column;
    uint32_t current_row;
    uint32_t tile_origin;
} PharaohTransportCursorGrid;

typedef struct PharaohTransportCursorContext {
    uint32_t cursor_enabled;
    const PharaohTransportCursorGrid *grid;
} PharaohTransportCursorContext;

/* Native 0x004B5520. Returns the first tag for walker kinds 11 through 14
   linked to the supplied tile; zero remains the no-match result. */
int32_t pharaoh_formation_legion_at_grid_offset(
    const PharaohTransportCursorGrid *grid, uint32_t tile_offset);

/* Native 0x004B5640. Returns the first fighter or dangerous walker ID on
   a tile, excluding corpse state. */
int32_t pharaoh_fighter_at_grid_offset(
    const PharaohTransportCursorGrid *grid, uint32_t tile_offset);

/* Native startup-frame helpers 0x004D6AA0 and 0x004D6B00. */
int32_t pharaoh_transport_cursor_can_embark(
    const PharaohTransportCursorContext *context, int32_t walker_id);
int32_t pharaoh_transport_cursor_can_land(
    const PharaohTransportCursorContext *context, int32_t walker_id);

#endif
