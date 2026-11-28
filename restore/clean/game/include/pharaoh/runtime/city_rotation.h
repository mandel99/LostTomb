#ifndef PHARAOH_RUNTIME_CITY_ROTATION_H
#define PHARAOH_RUNTIME_CITY_ROTATION_H

#include "pharaoh/runtime/city_view.h"
#include "pharaoh/runtime/city_warning.h"
#include "pharaoh/map/native_leaf_queries.h"
#include "pharaoh/map/rotated_buildings.h"
#include "pharaoh/scenario/scene_routing.h"
#include "pharaoh/walker/tile_chain.h"

typedef struct PharaohCityRotationWalkerContext {
    PharaohWalkerTileChainContext *tile_chain;
    const PharaohBuildingTypeRecord *buildings;
    int16_t *slot_walkers; /* same live slot bank as004B3250 */
    const uint8_t *marked_tiles; /* NativeEBAB20, BYTE per map tile */
    const volatile int32_t *origin, *width, *height;
    const volatile uint32_t *current_walker; /* NativeD056F8, not the scan ID */
    int32_t *search_tile; /* NativeBC9768 */
    int32_t *result_x, *result_y; /* NativeD3B4E4/E0, same cells as rotation */
} PharaohCityRotationWalkerContext;

typedef struct PharaohCityRotationContext {
    PharaohCityViewContext *view;
    int32_t *city_orientation; /* actual saved NativeE94E04, chunk33 */
    int32_t *map_orientation; /* NativeBCA2E0 */
    const volatile uint32_t *button_parameter; /* NativeD3B198 */
    uint32_t *bridge_turn; /* NativeBC96C8 */
    uint32_t *undo_available; /* NativeBC9694 */
    uint8_t *redraw_requested; /* NativeE38DCA, BYTE */
    int32_t *result_x, *result_y; /* NativeD3B4E4/E0 */
    PharaohMapSurfaceContext *surface;
    const PharaohNativeMapLeafContext *map_leaf;
    PharaohRotatedBuildingContext *rotated_buildings;
    PharaohSceneRouting *routing;
    PharaohCityRotationWalkerContext *walker_repair;
    PharaohCityWarningContext *warning;
} PharaohCityRotationContext;

/* All referenced objects/pools are borrowed live storage. The map leaf's
 * corner_rotation/bridge_turn alias city_orientation/bridge_turn. Surface
 * orientation and warning orientation alias map_orientation; every projection
 * consumer uses view->projection. The result cells alias walker_repair's.
 * Existing rotated-image providers must be bound to the actual complete C
 * adapters and their initialized scene/asset contexts, never substitute ops.
 * No operation is skipped when a required pointer/provider is unavailable. */
void pharaoh_city_view_rotate_left(PharaohCityRotationContext *context); /*00430B20*/
void pharaoh_city_view_rotate_right(PharaohCityRotationContext *context); /*00430BE0*/
void pharaoh_city_rotation_repair_sentries(PharaohCityRotationWalkerContext *context); /*00497B90*/
void pharaoh_city_rotation_rebuild(PharaohCityRotationContext *context); /*0047D6E0*/
void pharaoh_city_rotate_command(PharaohCityRotationContext *context); /*00409EB0*/

#endif
