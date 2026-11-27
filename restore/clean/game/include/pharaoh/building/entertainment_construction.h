#ifndef PHARAOH_BUILDING_ENTERTAINMENT_CONSTRUCTION_H
#define PHARAOH_BUILDING_ENTERTAINMENT_CONSTRUCTION_H

#include "pharaoh/building/allocation.h"
#include "pharaoh/building/construction_undo.h"
#include "pharaoh/building/venue_geometry.h"
#include "pharaoh/map/venue_tiles.h"

typedef enum PharaohEntertainmentKind {
    PHARAOH_ENTERTAINMENT_BANDSTAND = 30,
    PHARAOH_ENTERTAINMENT_BOOTH = 31,
    PHARAOH_ENTERTAINMENT_PAVILION = 33,
    PHARAOH_ENTERTAINMENT_FESTIVAL_SQUARE = 209
} PharaohEntertainmentKind;

/* The actual 0x49D8 player bank, shared with allocation->players. */
typedef struct PharaohEntertainmentPlayer {
    uint8_t reserved_0000[0x47a0];
    uint32_t festival_square_x, festival_square_y;
    uint8_t reserved_47a8[0x4814 - 0x47a8];
    uint16_t festival_square_placed;
    uint8_t reserved_4816[0x49d8 - 0x4816];
} PharaohEntertainmentPlayer;

_Static_assert(sizeof(PharaohEntertainmentPlayer) == 0x49d8 &&
    offsetof(PharaohEntertainmentPlayer, festival_square_x) == 0x47a0 &&
    offsetof(PharaohEntertainmentPlayer, festival_square_placed) == 0x4814,
    "native entertainment player fields");

/* Native read-only rank data at 0x005D2960 and 0x005D2984. Context pointers
 * remain explicit so actual live tables and permitted aliases are retained. */
extern const int32_t pharaoh_entertainment_bandstand_ranks[9];
extern const int32_t pharaoh_entertainment_pavilion_ranks[16];
extern const uint8_t pharaoh_entertainment_pattern_2[4];
extern const uint8_t pharaoh_entertainment_pattern_3[9];
extern const uint8_t pharaoh_entertainment_pattern_4[16];
extern const uint8_t pharaoh_entertainment_pattern_5[25];

typedef struct PharaohEntertainmentConstructionContext {
    PharaohBuildingAllocationContext *allocation;
    PharaohVenueTileContext *venue_tiles;
    const PharaohConstructionUndoContext *backup;
    PharaohEntertainmentPlayer *players;
    const uint32_t *selection_x, *selection_y; /* BCA304/BCA308. */
    uint32_t *occupancy; /* 25 DWORDs, column-major: 775614. */
    const int32_t *bandstand_ranks; /* Nine live DWORDs. */
    const int32_t *pavilion_ranks; /* Sixteen live DWORDs. */
    const uint8_t *patterns_by_size[4]; /* Native BYTE masks for sizes2..5. */
    uint32_t *placed_count; /* BC971C, whole DWORD. */
} PharaohEntertainmentConstructionContext;

/* All surfaces, building/player records and undo backups alias the same real
 * scene. venue_tiles->cleared is the mutable native AE8E7C byte grid.
 * backup->saved_bytes[0..2] correspond to AE8E7C/B285CC/B67D1C in that order;
 * saved_terrain/saved_images are the previous map backup, not a new snapshot.
 * Actual image groups, geometry, allocator, undo and refresh providers required.
 * Bandstand/Pavilion retain the native even view-orientation domain0..6;
 * invalid image rules leave native private locals undefined and are excluded. */

/* Native 0x0043F410, complete logical DWORD 0/1. */
uint32_t pharaoh_entertainment_site_valid(
    const PharaohEntertainmentConstructionContext *context,
    PharaohEntertainmentKind kind);

/* Native 0x00441130 algorithm for actual constructor inputs. primary is one
 * of BANDSTAND, BOOTH or PAVILION, as in its actual constructor callsites;
 * secondary must also be BANDSTAND, BOOTH or PAVILION.
 * Returns UINT32_MAX for failure, otherwise the 0/1 rotation. The native
 * unknown-secondary failure reads private stack garbage and is outside this
 * typed interface. It is not converted into an invented input or result.
 * Output coordinate pointers may alias the real occupancy/rank storage. */
uint32_t pharaoh_entertainment_select_slot(
    const PharaohEntertainmentConstructionContext *context,
    int32_t x, int32_t y, int32_t *out_x, int32_t *out_y,
    PharaohEntertainmentKind primary, PharaohEntertainmentKind secondary);

/* Native 0x00440700/850/B10/ED0. The native second and third stack arguments
 * are unused; the logical C API exposes preview and the final x/y pair only.
 * Returns full DWORD 0/1. Allocation failure deliberately continues through
 * record zero, map stamping and links, just as the original constructors. */
uint32_t pharaoh_entertainment_build_booth(
    const PharaohEntertainmentConstructionContext *context,
    uint32_t preview, int32_t x, int32_t y);
uint32_t pharaoh_entertainment_build_bandstand(
    const PharaohEntertainmentConstructionContext *context,
    uint32_t preview, int32_t x, int32_t y);
uint32_t pharaoh_entertainment_build_pavilion(
    const PharaohEntertainmentConstructionContext *context,
    uint32_t preview, int32_t x, int32_t y);
uint32_t pharaoh_entertainment_build_festival_square(
    const PharaohEntertainmentConstructionContext *context,
    uint32_t preview, int32_t x, int32_t y);

#endif
