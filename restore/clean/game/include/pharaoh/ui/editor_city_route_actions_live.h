#ifndef PHARAOH_UI_EDITOR_CITY_ROUTE_ACTIONS_LIVE_H
#define PHARAOH_UI_EDITOR_CITY_ROUTE_ACTIONS_LIVE_H

#include "pharaoh/commerce/empire_queries.h"
#include "pharaoh/runtime/city_warning.h"
#include "pharaoh/scenario/tutorial_complete.h"
#include "pharaoh/ui/message_box.h"
#include "pharaoh/ui/small_button_callbacks.h"

/* All fields borrow the actual scene/settings/UI owners. The two text
 * selections are distinct live pointer cells (E20768/E20764), sharing the
 * same loaded catalogue. Child contexts and their platform/drawing operations
 * must already be genuine, fully bound providers for the reached Native arm.
 * This module does not supply a modal loop, tutorial sidebar, or warning font.
 *
 * current_window/redraw alias the modal and tutorial message UI. In particular
 * tutorial->flags is tutorial_flags, tutorial->mission_id is selected_mission,
 * tutorial->rules is tutorial_rules, and tutorial->advisor_override is
 * custom_scenario. The actual14th flag is
 * tutorial_flags[14]; it is not a private copy. warnings->editor_active and
 * warnings->selection share editor_active and primary_text respectively.
 *
 * scroll fields retain442800's actual DWORD owners. When editor_active==1,
 * selected_map_point is a valid index in the original 32-byte map-point bank;
 * the selected index is narrowed only at the real committed WORD store.
 * All data remain stable on the game thread except the documented Native
 * child stores. There are no substitute operations or private world/pool copies.
 */
typedef struct PharaohEditorCityRouteLiveContext {
    const uint32_t *editor_active;       /*605A3C, compare exactly1*/
    const int32_t *custom_scenario;      /*E38EBC, full DWORD*/
    const uint32_t *native_78d828;       /*78D828, full DWORD zero gate*/
    const int32_t *display_mode;         /*E38E68*/
    uint8_t *current_window,*redraw;     /*E38DDA/E38DCA*/
    int32_t *native_5d2e88;              /*5D2E88, store-1*/
    uint32_t *native_f53fdc;             /*F53FDC, store0*/
    uint32_t *city_map_kind,*city_map_active; /*775928/775920*/
    uint8_t *tutorial_flags;             /*7027B8,14th flag7027C6*/
    const int8_t *selected_mission;      /*E38EA8, signed BYTE*/
    const PharaohTutorialRule *tutorial_rules; /*5794B0,28-byte records;
                                               rule14 DWORD579638*/
    uint32_t *next_window;               /*D3B17C, store20*/
    PharaohTextSelection *primary_text,*secondary_text;
    PharaohMessageBoxModalState *message_box;
    const PharaohMessageBoxModalOperations *message_box_operations;
    PharaohCityWarningContext *warnings;
    const PharaohTutorialCompletionContext *tutorial;
    PharaohEmpireScrollBorrowedBounds *scroll;
    const uint32_t *selected_map_point;  /*784F10*/
    int16_t *committed_map_point;        /*7848DC*/
    PharaohMapViewPoint *map_points;     /*7843D8, Native stride32*/
} PharaohEditorCityRouteLiveContext;

/* Whole409FA0 and40B590. Required reached child providers are borrowed above;
 * the application must establish their contracts before calling either entry.
 * Native40B590's JMP409FA0 and JMP40A090 are complete calls to the genuine
 * shared C algorithms. Incidental EAX/flags are not logical API results. */
void pharaoh_editor_city_enter(const PharaohEditorCityRouteLiveContext *);
void pharaoh_editor_city_return(const PharaohEditorCityRouteLiveContext *);

#endif
