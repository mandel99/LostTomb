#ifndef PHARAOH_UI_UNIT_COMMAND_UI_H
#define PHARAOH_UI_UNIT_COMMAND_UI_H

#include "pharaoh/military/native_orders.h"
#include "pharaoh/map/ferry_routing.h"
#include "pharaoh/ui/transport_cursor.h"
#include "pharaoh/core/percentage_scale.h"

/* All mutable game records are borrowed actual saved-scene pools. Cursor
 * coordinates/IDs/flags retain their native DWORD/BYTE widths. The tile walker
 * grid must view orders->walkers at its real388-byte stride, not a copied list.
 * The routing aliases documented by native_orders.h also apply here. */
typedef struct PharaohUnitCommandUiContext {
    const PharaohNativeLegionOrderContext *orders;
    const PharaohBuildingTypeRecord *buildings;
    const int16_t *building_grid;
    const PharaohShipwrightOrderRecord *shipwright_orders;
    PharaohFerryMapContext *shore;
    const PharaohTransportCursorGrid *tile_walkers;
    const volatile int32_t *cursor_x,*cursor_y,*grid_start;
    const volatile uint32_t *cursor_enabled; /* BCA2C4 */
    const volatile uint32_t *panel_warship_id; /* EADCC4 */
    volatile uint32_t *selected_warship_id,*selected_transport_id; /* D05704/00 */
    volatile uint32_t *warship_active,*transport_active; /* D0570C/08 */
    volatile uint32_t *active_formation_id; /* BCB9C0 */
    volatile uint32_t *button_parameter; /* D3B198, aliases orders' pointer */
    volatile uint32_t *orders_open; /* EADCC0 */
    const volatile uint32_t *panel_mode; /* EADD50 */
    const volatile uint32_t *cursor_image,*embark_image,*land_image; /* E390DC/C8/C4 */
} PharaohUnitCommandUiContext;

typedef struct PharaohUnitDirectionResult {
    PharaohPercentageScaleStatus status;
    uint8_t direction;
} PharaohUnitDirectionResult;

/* Actual-layout adapter for the existing single00406720 shared algorithm.
 * It is an ABI/layout reuse, not a second Native owner or a copied game state. */
void pharaoh_unit_legion_order_button(const PharaohUnitCommandUiContext *context);

/* Real bridge to existing004A45D0 C and its actual004806C0 shore query.
 * It is also suitable as the canonical transport-button assign callback. */
void pharaoh_unit_assign_dock(void *context,int32_t walker_id);

/* Native00406C90/00406DE0/0040BA40 normally return VALUE. An overflow status
 * denotes the original x86#DE at the real004CDE60 child. The host must raise
 * that exception and stop this action; prior native stores already occurred. */
PharaohPercentageScaleStatus pharaoh_unit_warship_button(const PharaohUnitCommandUiContext *context);
PharaohPercentageScaleStatus pharaoh_unit_warship_return_button(const PharaohUnitCommandUiContext *context);
PharaohPercentageScaleStatus pharaoh_unit_transport_target_input(const PharaohUnitCommandUiContext *context);

void pharaoh_unit_legion_exit(const PharaohUnitCommandUiContext *context); /*004071C0*/
void pharaoh_unit_warship_target_input(const PharaohUnitCommandUiContext *context); /*0040B830*/
void pharaoh_unit_legion_target_input(const PharaohUnitCommandUiContext *context); /*0040C440*/
void pharaoh_unit_legion_fort_input(const PharaohUnitCommandUiContext *context); /*0040C570*/
void pharaoh_unit_warship_return_to_dock(const PharaohUnitCommandUiContext *context,
    int32_t walker_id); /*004BC670*/

/* Native0051CD80's observed contract is AL (the real caller stores BYTE17).
 * Do not expose unobserved partial-register upper bits as a logical result. */
PharaohUnitDirectionResult pharaoh_unit_target_direction(
    int32_t x,int32_t y,int32_t target_x,int32_t target_y);

/* Complete0040BDA0: signed wrapped squared distance in EAX and DWORD outputs.
 * The native X/Y publication and subsequent X reload preserve output aliases. */
int32_t pharaoh_unit_land_farthest_on_line(const PharaohNativeOrderLineContext *context,
    int32_t x,int32_t y,int32_t target_x,int32_t target_y,
    int32_t *result_x,int32_t *result_y);

#endif
