#ifndef PHARAOH_SIMULATION_FLOOD_BUILD_H
#define PHARAOH_SIMULATION_FLOOD_BUILD_H

#include "pharaoh/simulation/flood_simulation.h"
#include "pharaoh/map/surface_tiles.h"

typedef struct PharaohFloodSortedLink {
    struct PharaohFloodSortedLink *next, *previous;
    int32_t key;
    PharaohFloodTile *item;
} PharaohFloodSortedLink;

typedef struct PharaohFloodWorld {
    PharaohFloodTile *tiles[51984];
    uint8_t reserved_32c40[36];
    PharaohFloodList lists[30];
    int32_t current_list, season_month;
    uint32_t reserved_32fb4;
    int32_t progress;
    uint8_t has_floodplain;
} PharaohFloodWorld;

typedef struct PharaohFloodMemory {
    void *user_data;
    void *(*allocate)(void *user_data, uint32_t bytes);
    void (*release)(void *user_data, void *allocation);
} PharaohFloodMemory;

typedef struct PharaohFloodListsContext {
    PharaohFloodWorld *world;
    const uint8_t *random;
    const PharaohFloodMemory *memory; /* NULL selects the CRT heap. */
} PharaohFloodListsContext;

typedef struct PharaohFloodBuildOperations {
    void *user_data;
    void (*reset)(void *user_data); /* 004BE0F0, native world receiver */
    PharaohFloodTile *(*insert)(void *user_data, PharaohFloodList *list,
        int32_t tile, int32_t x, int32_t y, int32_t depth,
        uint8_t is_floodplain); /* 004BD740, native list receiver */
} PharaohFloodBuildOperations;

typedef struct PharaohFloodBuildContext {
    const PharaohMapTerrainView *map;
    PharaohMapSurfaceContext *surface;
    PharaohFloodList *lists; /* 30 records, 28 bytes each on Win32 */
    PharaohFloodTile **tiles; /* 51984 live tile pointers */
    int32_t *progress;
    uint8_t *has_floodplain;
    const PharaohFloodBuildOperations *operations;
} PharaohFloodBuildContext;

/* Complete typed reuse of Unit01558.c, qualified against native004BE160.
 * The native map border makes every eight-neighbor access valid. */
void pharaoh_floods_build(PharaohFloodBuildContext *context,
    int32_t x0, int32_t y0, int32_t x1, int32_t y1);

void pharaoh_flood_list_clear(PharaohFloodListsContext *context,
    PharaohFloodList *list); /*004BD6A0 */
PharaohFloodTile *pharaoh_flood_list_insert(PharaohFloodListsContext *context,
    PharaohFloodList *list, int32_t tile, int32_t x, int32_t y,
    int32_t depth, uint8_t is_floodplain); /*004BD740 */
void pharaoh_floods_reset(PharaohFloodListsContext *context); /*004BE0F0 */
void pharaoh_flood_build_bind_lists(PharaohFloodBuildContext *build,
    PharaohFloodBuildOperations *operations, PharaohFloodListsContext *lists);

#endif
