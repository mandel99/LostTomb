#ifndef PHARAOH_UI_UNIT_PANEL_LIVE_H
#define PHARAOH_UI_UNIT_PANEL_LIVE_H

#include "pharaoh/ui/building_info_draw_live.h"
#include "pharaoh/core/percentage_scale.h"

typedef struct PharaohUnitPanelFont {
    const PharaohBitmapFont *font;
    const volatile int32_t *id;
} PharaohUnitPanelFont;

/* Actual EADCB8 info window,264-byte buildings,388-byte walkers and144-byte
 * formations. Text, layout, image and pixel providers share their native live
 * scratch and the already locked drawing surface. pixels' backing/pitch must
 * match images' surface/pitch; layout.outer_panel's tile_x/y alias images.clip draw_x/y.
 * No replacement draw hooks. */
typedef struct PharaohUnitPanelLiveContext {
    volatile PharaohBuildingInfoLiveState *window;
    const PharaohBuildingTypeRecord *buildings;
    const PharaohWalkerQueryRecord *walkers;
    const PharaohNativeLegionRecord *formations;
    const PharaohInfoLayoutDrawContext *layout;
    const PharaohUiTextLayoutContext *text;
    const PharaohBitmapTextCatalogWrappedNativeContext *wrapped;
    PharaohImageDrawContext *images;
    PharaohGraphicsPrimitives *pixels;
    const volatile uint32_t *pixel_format; /* actual entire DWORD E3914C */
    uint32_t *pixel_read_scratch; /*605B70*/
    volatile int32_t *text_width; /*E20778; aliases text.drawing.cursor_x*/
    const volatile int32_t *focus_primary,*focus_secondary; /*7056C8/C4*/
    const volatile int32_t *transport_button_base,*warship_button_base; /*F55CE4/CDC*/
    const volatile int32_t *experience_icon_base; /*F5642C*/
    PharaohUnitPanelFont normal,title,heading,description; /*F56F8C/FB4/F9C/F94*/
    const volatile int32_t *legion_badge_base,*legion_type_base; /*F56814/80C*/
    const volatile int32_t *legion_morale_base,*legion_experience_base; /*F565CC/6B1C*/
    const volatile int32_t *charioteer_button_base,*archer_button_base,*infantry_button_base; /*F55CD4/CCC/CC4*/
    const volatile int32_t *map_orientation; /*BCA2E0, entire DWORD*/
    const volatile int32_t *native_7058b8; /*entire DWORD: no-soldiers text selector*/
} PharaohUnitPanelLiveContext;

/* Native50FC10 returns complete EAX3..8, not a partial-register char.
 * Its real percentage child can raise#DE; status propagates that exception.
 * Native50FC70 returns complete EAX28..30 and reads only walker BYTE154. */
PharaohPercentageScaleResult pharaoh_unit_panel_hull_text(
    const PharaohUnitPanelLiveContext *,int32_t walker_id);
int32_t pharaoh_unit_panel_crew_text(const PharaohUnitPanelLiveContext *,int32_t walker_id);

/* Complete4CF090 over the real read/AND/OR pixel providers. The target must
 * have the original native backing; this routine performs no added clipping. */
void pharaoh_unit_panel_grey_rect(const PharaohUnitPanelLiveContext *,
    int32_t x,int32_t y,int32_t width,int32_t height,uint32_t shift);

/* Normal return is VALUE. Other status is the actual004CDE60#DE boundary;
 * host bindings must raise it immediately, retaining prior graphics/state. */
PharaohPercentageScaleStatus pharaoh_unit_panel_draw_transport(const PharaohUnitPanelLiveContext *); /*50F320*/
PharaohPercentageScaleStatus pharaoh_unit_panel_draw_transport_orders(
    const PharaohUnitPanelLiveContext *,int32_t walker_id); /*50F890*/
PharaohPercentageScaleStatus pharaoh_unit_panel_draw_warship(const PharaohUnitPanelLiveContext *); /*50FCA0*/
PharaohPercentageScaleStatus pharaoh_unit_panel_draw_warship_orders(
    const PharaohUnitPanelLiveContext *,int32_t walker_id); /*50FE50*/
void pharaoh_unit_panel_draw_legion_foreground(const PharaohUnitPanelLiveContext *,
    int32_t formation_id); /*510B00*/
PharaohPercentageScaleStatus pharaoh_unit_panel_draw_legion(const PharaohUnitPanelLiveContext *,
    int32_t formation_id); /*5101B0*/

/* The actual row input is already the complete4F96E0 implementation in
 * building_info_live.c, including modes4/5/6 and the legion return row. Bind
 * its real GenericButton rows to unit_command_ui; do not add another owner.
 * Legion background5101B0 calls the distinct53FA40 mirrored renderer with
 * the same actual F55BB0 image/pixel/clip/state view as ordinary53E590. */

#endif
