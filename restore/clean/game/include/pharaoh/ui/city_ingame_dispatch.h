#ifndef PHARAOH_UI_CITY_INGAME_DISPATCH_H
#define PHARAOH_UI_CITY_INGAME_DISPATCH_H

#include "pharaoh/ui/build_menu.h"
#include "pharaoh/ui/city_input_actions.h"
#include "pharaoh/ui/city_map_input.h"
#include "pharaoh/ui/construction_finish.h"
#include "pharaoh/ui/menu_dropdown_input.h"
#include "pharaoh/ui/advisor_open.h"
#include "pharaoh/building/construction_preview.h"
#include "pharaoh/monuments/image_updates.h"
#include "pharaoh/runtime/camera_scroll.h"
#include "pharaoh/runtime/sound_effect_request.h"
#include "pharaoh/scenario/tutorial_messages.h"

typedef struct PharaohCityBuildPopupSize {
    int32_t y_offset, width;
} PharaohCityBuildPopupSize;

/* Every pointer borrows its actual scene owner. In particular menu's selected
   category, construction's tool/window/drag fields, and the table's button
   parameter must be shared with the image/generic action adapters. */
typedef struct PharaohCityBuildSelectionContext {
    PharaohBuildMenuContext *menu;
    const PharaohConstructionInputContext *construction;
    PharaohConstructionRequirements *requirements;
    PharaohConstructionTerrainRule *terrain_rule;
    uint32_t *button_parameter; /* D3B198 */
    const PharaohCityBuildPopupSize *popup_sizes; /* 5CF63C, indexed by count */
    int32_t *popup_count, *popup_y_offset, *popup_width; /* 7056F0/E8/E4 */
    const int32_t *scenario_mode, *current_tick, *orientation; /* 978D70/E92E0C/BCA2E0 */
    uint32_t *selection_tick, *variation; /* BC9710/14 */
    uint32_t *selected_cell, *anchor_cell; /* BCA300/BCA2C4 */
    uint8_t *selection_locked; /* 605A5C */
    int32_t *editor_category, *editor_brush_size, *editor_entry_point; /* 605A30/34/2C */
    int32_t *editor_invasion_point, *editor_special_point; /* 605A28/24 */
    int32_t *editor_river_point, *editor_other_point; /* 605A1C/20 */
    int16_t *editor_animal; /* 784B94 */
    int32_t *obelisk_size, *pyramid_choice; /* 5E6970/576144 */
    const int16_t *goal_definition_ids; /* 784DE0; choice is one based */
    PharaohConstructionCommitCity *cities;
    const PharaohSpriteLibraryLoadContext *sprite_libraries;
    const char *const (*sphinx_stage_library_names)[3];
    const char *sphinx_fixed_library_name;
} PharaohCityBuildSelectionContext;

/* Complete ordinary C owners of native 403D70,403F10,405200. The two scalar
   dimensions retain the native table order; no geometry is invented here.
   Native producers supply categories 0..37 and previous rows -1..29. */
void pharaoh_city_sidebar_build_click(const PharaohCityBuildSelectionContext *);
int32_t pharaoh_city_build_menu_next_item(PharaohBuildMenuContext *,
    int32_t category, int32_t previous);
void pharaoh_city_build_menu_set_type(const PharaohCityBuildSelectionContext *);

typedef struct PharaohCityBuildMenuInputContext {
    const PharaohCityBuildSelectionContext *selection;
    const int32_t *expanded_x, *collapsed_x, *sidebar_collapsed;
    const int32_t *label_width, *mouse_x, *mouse_y; /* E94DAC/E38E34/30 */
    const uint8_t *left_pressed;
    /* Geometry projected from the actual thirty native 28-byte rows. */
    const PharaohMenuRect *rectangles;
    PharaohGenericButtonFocusState *focus;
    int32_t *focused_button; /* 7056C8 */
} PharaohCityBuildMenuInputContext;

/* Complete native 403E00. Selection and return value are one based, while
   next_item returns the native zero-based enabled row. */
uint32_t pharaoh_city_build_menu_handle_input(
    const PharaohCityBuildMenuInputContext *);

typedef struct PharaohCitySidebarActionContext {
    uint8_t *current_window, *redraw;
    int32_t *slide_step; /* F53F7C */
    PharaohCityViewContext *view;
    PharaohSoundEffectRequestState *sound;
    const PharaohDigitalSoundDeviceContext *sound_device;
    int32_t *popup_count, *popup_y_offset, *popup_width; /* 7056F0/E8/E4 */
    int32_t *native_7056e0;
    const int32_t **overlay_submenu_open; /* relocated native7056DC pointer */
    int32_t *overlay_submenu_count; /* 7056D8 */
    int32_t *native_7056d4, *overlay_submenu_index, *native_7056cc;
} PharaohCitySidebarActionContext;

/* Complete 409D10/409D40. Collapse uses actual viewport, clamp and digital
   sound C children; the original extra47D350 argument is unused. */
void pharaoh_city_sidebar_collapse(const PharaohCitySidebarActionContext *);
void pharaoh_city_overlay_menu_open(const PharaohCitySidebarActionContext *);

/* Complete 407320: the real image/generic adapter must publish the selected
   row's secondary DWORD D3B194 before calling this concrete help action. */
void pharaoh_city_button_message_help(const PharaohCityMessageQueueContext *,
    const uint32_t *secondary_parameter, uint8_t *redraw);

typedef struct PharaohCityOverlayRefreshContext {
    const int32_t *highest_building_id;
    const PharaohBuildingTypeRecord *buildings;
    /* Native receivers 6F9320,6F5648,6DE9E0,6E9188 in this order. */
    PharaohMonumentImageContext *tombs[4];
} PharaohCityOverlayRefreshContext;

/* Complete native 547A60. It intentionally tests type without a state gate. */
void pharaoh_city_refresh_tomb_overlay(const PharaohCityOverlayRefreshContext *);

typedef struct PharaohCityHotkeyContext {
    uint8_t *current_window, *redraw;
    const uint32_t *message_return_window; /* D3A6C4 */
    int32_t *overlay, *previous_overlay; /* E94E0C/10 */
    const PharaohCityOverlayRefreshContext *overlay_refresh;
    uint32_t *button_parameter;
    const PharaohAdvisorOpenContext *advisors;
    const int32_t *bookmarks; /* ten DWORDs, five pairs; E94DDC */
    PharaohCityViewContext *view;
    int32_t *focus_x, *focus_y;
} PharaohCityHotkeyContext;

void pharaoh_city_hotkey_toggle_overlay(const PharaohCityHotkeyContext *,
    int32_t overlay); /* 4168E0 */
PharaohPercentageScaleStatus pharaoh_city_hotkey_show_advisor(
    const PharaohCityHotkeyContext *, uint32_t advisor); /* 416990 */
void pharaoh_city_hotkey_go_to_bookmark(const PharaohCityHotkeyContext *,
    int32_t bookmark); /* 416AC0 */

typedef enum PharaohCityIngameDisposition {
    PHARAOH_CITY_INPUT_HANDLED,
    PHARAOH_CITY_INPUT_OTHER_WINDOW,
    PHARAOH_CITY_INPUT_OPEN_BUILDING_INFO,
    PHARAOH_CITY_INPUT_EDITOR_DISPATCH_REQUIRED,
    PHARAOH_CITY_INPUT_EDITOR_PAINT_REQUIRED,
    PHARAOH_CITY_INPUT_ARITHMETIC_FAULT
} PharaohCityIngameDisposition;

typedef struct PharaohCityIngameResult {
    PharaohCityIngameDisposition disposition;
    PharaohPercentageScaleStatus arithmetic_status;
} PharaohCityIngameResult;

typedef struct PharaohCityConstructionDragContext {
    const PharaohConstructionInputContext *input;
    const uint32_t *selected_cell; /* BCA300, distinct from input->anchor_tile */
    const int32_t *map_x, *map_y, *editor_active;
    uint32_t *fallback_cell; /* BCA2E4 */
    const PharaohConstructionPreviewContext *preview;
} PharaohCityConstructionDragContext;

/* Native 405D90 in-game path and editor tools 5,88,89,93. The remaining
   editor path requires unmigrated native 478440 and returns that exact gap;
   it is not a complete owner of the entire native entrypoint. */
PharaohCityIngameResult pharaoh_city_construction_drag(
    const PharaohCityConstructionDragContext *);

typedef struct PharaohCityIngameFrame {
    const int32_t *mouse_x, *mouse_y; /* E38E34/30 */
    const uint8_t *left_pressed, *left_down, *left_released; /* E38E03/00/04 */
    const uint8_t *right_pressed, *right_released; /* E38DF7/E38DF8 */
} PharaohCityIngameFrame;

typedef struct PharaohCityInputImageTable {
    PharaohImageButtonRecord *records;
    /* Actual trigger after the counted records; never the hovered trigger. */
    const int16_t *trailing_trigger;
} PharaohCityInputImageTable;

typedef struct PharaohCityIngameButtonTables {
    PharaohCityInputImageTable expanded_extra, expanded_build, expanded_top;
    PharaohCityInputImageTable collapsed_expand, collapsed_build;
    PharaohCityInputImageTable editor_full, editor_preview;
    PharaohGenericButton *problem;
} PharaohCityIngameButtonTables;

typedef struct PharaohCityIngameDispatchContext {
    const PharaohCityIngameFrame *frame;
    uint8_t *current_window, *redraw, *ui_redraw;
    int32_t *keyboard_field, *image_focus, *arrow_focus, *menu_focus; /* E20990/7056B8/B0/A8 */
    int32_t *ui_mouse_x, *ui_mouse_y; /* E38E2C/28; shared hit-coordinate owners */
    int32_t *mouse_drag_state, *city_transient; /* E38E14/E94E14 */
    const int32_t *editor_active, *sidebar_collapsed;
    const int32_t *expanded_x, *collapsed_x, *editor_map_preview;
    const PharaohMenuBarFocusRecord *game_headers, *editor_headers;
    const PharaohMenuBarState *menu_bar;
    const PharaohMenuDropdownInputContext *dropdown;
    const PharaohTutorialMissionMessagesContext *tutorial;
    const PharaohCameraScrollInput *scroll_input;
    const PharaohCameraScrollContext *scroll;
    const PharaohMinimapClickContext *minimap;
    int32_t *minimap_hover; /* 7056A0; same storage as minimap->hovered */
    const PharaohCityRotateInputContext *rotate;
    const int32_t *top_info_left_edges;
    int32_t *top_info_focus; /* 70569C */
    const PharaohCityMessageQueueContext *messages;
    PharaohImageButtonInputState *image_input;
    PharaohGenericButtonInputState *generic_input;
    PharaohButtonHitState *tooltip;
    const PharaohCityIngameButtonTables *buttons;
    const PharaohCityBuildMenuInputContext *build_menu;
    const PharaohCityMapInputContext *map;
    const PharaohCitySelectionContext *selection;
    const PharaohConstructionInputContext *construction;
    const PharaohCityConstructionDragContext *drag;
    const PharaohConstructionFinishContext *finish;
    uint32_t *construction_place_ok; /* 605994 */
} PharaohCityIngameDispatchContext;

/* Native 4D8310 common prefix and windows 1,3,6,18 only. Other windows are
   returned untouched. The City editor subbranch is explicitly unresolved.
   OPEN_BUILDING_INFO is the real final 4F7E50 call boundary, to be continued by
   the host's complete building-info owner, not an invented click behavior.

   The caller supplies full, native frame flags. Image input's misleadingly
   named right_pressed slot consumes native RIGHT RELEASE at E38DF8; the
   minimap's misleading left_down slot consumes LEFT PRESS at E38E03 and its
   right_went_down consumes the separate E38DF7 press slot. These mappings
   are performed here from frame pointers. Image
   input's redraw_requested aliases ui_redraw (E38DC9), not redraw (E38DCA).
   Image/generic actions share the real parameters/window fields, and the
   host adapters publish their selected parameters before invoking an action.
   Their callbacks must bind real C actions before this dispatcher is used;
   callback DWORD tokens are never invoked as process addresses. */
PharaohCityIngameResult pharaoh_city_ingame_dispatch(
    const PharaohCityIngameDispatchContext *);

#endif
