#ifndef PHARAOH_MILITARY_NATIVE_ORDERS_H
#define PHARAOH_MILITARY_NATIVE_ORDERS_H

#include "pharaoh/military/legion_orders.h"
#include "pharaoh/map/access_routing.h"
#include "pharaoh/map/native_leaf_queries.h"
#include "pharaoh/map/water_routing.h"
#include "pharaoh/runtime/city_warning.h"
#include "pharaoh/walker/record_queries.h"

typedef struct PharaohNativeOrderRouteContext {
    PharaohRoutingSearchState *search;
    const PharaohWaterTargetContext *friendly;
} PharaohNativeOrderRouteContext;

typedef struct PharaohNativeOrderLineContext {
    const PharaohWaterRoutingContext *water;
    const volatile uint32_t *terrain;
} PharaohNativeOrderLineContext;

/* Original formation and walker overlays are retained. In these owners,
 * formation BYTE66 is the order mode, BYTE73 is the disabled flag, WORD7E/80
 * are return flags, DWORD84 is the direct/standard walker and DWORD8C is the
 * attack task. Walker DWORD134/13C/140 are reset/target-coordinate overlays.
 * Scalars in search retain the native queue/counter state between real calls.
 * In this combined context, line.water must share search's distance/queue and
 * must point its origin/start/head/tail to search's corresponding scalars.
 * A native binding initializes origin from BCA2D8 and publishes the mutable
 * shared state to EAE004/00C/008 and EADFD4 on return. Origin remains read-only.
 * All terrain/routing arrays require the original sentinel-border backing. */
typedef struct PharaohNativeLegionOrderContext {
    PharaohNativeLegionRecord *formations;
    PharaohWalkerQueryRecord *walkers;
    int16_t *slot_walkers;
    PharaohNativeOrderRouteContext route;
    PharaohNativeOrderLineContext line;
    PharaohCityWarningContext *warnings;
    const volatile uint8_t *command_locked; /* E38E60 */
    const volatile uint32_t *button_parameter; /* D3B198 */
    const volatile uint32_t *current_walker_id; /* D056F8 */
    const volatile uint32_t *selected_formation_id; /* EADCCC */
    volatile uint8_t *redraw_units, *redraw_city; /* E38DDA/E38DCA */
} PharaohNativeLegionOrderContext;

/* Complete native00518400: full EAX0/1; direct real0051AFF0 dependency. */
uint32_t pharaoh_legion_citizen_route_no_ferry(
    const PharaohNativeOrderRouteContext *context,
    int32_t x,int32_t y,int32_t target_x,int32_t target_y);

/* Complete native0040C180: full signed EAX, wrapping squared distance.
 * Output DWORDs preserve native X-then-Y publication, including aliasing.
 * The current hardware x87 control word is inherited, as in the original. */
int32_t pharaoh_legion_water_farthest_on_line(
    const PharaohNativeOrderLineContext *context,
    int32_t x,int32_t y,int32_t target_x,int32_t target_y,
    int32_t *result_x,int32_t *result_y);

/* Native004B6400/66E0/6A40 return full EAX0/1. Only the original quiet
 * parameter's low BYTE is observed; command004B6C20 uses its full signed value
 * for the -1 branch and writes its low BYTE to the standard walker. */
uint32_t pharaoh_formation_legion_move_to_native(
    const PharaohNativeLegionOrderContext *context,
    int32_t x,int32_t y,int32_t formation_id,uint8_t quiet);
uint32_t pharaoh_formation_legion_attack_near_native(
    const PharaohNativeLegionOrderContext *context,
    int32_t x,int32_t y,int32_t formation_id,uint32_t task);
uint32_t pharaoh_formation_legion_return_to_fort_native(
    const PharaohNativeLegionOrderContext *context,
    int32_t x,int32_t y,int32_t formation_id);
void pharaoh_formation_legion_to_standard_native(
    const PharaohNativeLegionOrderContext *context,
    int32_t formation_id,int32_t command);
void pharaoh_legion_return_button_native(
    const PharaohNativeLegionOrderContext *context); /*00406FB0*/

#endif
