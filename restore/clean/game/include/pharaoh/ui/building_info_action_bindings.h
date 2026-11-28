#ifndef PHARAOH_UI_BUILDING_INFO_ACTION_BINDINGS_H
#define PHARAOH_UI_BUILDING_INFO_ACTION_BINDINGS_H

#include "pharaoh/ui/building_info_program_data.h"
#include "pharaoh/ui/building_info_actions_live.h"
#include "pharaoh/ui/building_info_init_bindings.h"
#include "pharaoh/ui/selection_button_actions.h"
#include "pharaoh/ui/unit_command_ui.h"
#include "pharaoh/ui/advisor_open.h"

/* Actual EA5EE4 DWORD view, one49D8-byte player bank apart. */
typedef struct PharaohInfoSelectedBuildingPlayer {
    uint32_t selected_building;
    uint8_t reserved_04[0x49d8-4];
} PharaohInfoSelectedBuildingPlayer;

typedef struct PharaohBuildingInfoActionProviders {
    PharaohBuildingInfoLiveContext *ui;
    const PharaohInfoProgramBorrowed *live;
    const PharaohBuildingInfoActionsLiveContext *actions;
    const PharaohUnitCommandUiContext *units;
    const PharaohAdvisorOpenContext *advisors;
    PharaohInfoSelectedBuildingPlayer *selected_buildings;
    const uint32_t *current_player;
    /* Same synchronous non-returning #DE endpoint contract as init bindings.
     * All diagnostic percentage statuses must leave permanently, preserving
     * Native stores already made. A returned endpoint violates the contract. */
    PharaohBuildingInfoInitDivideFaultEndpoint divide_fault;
    void *divide_fault_user_data;
} PharaohBuildingInfoActionProviders;

enum PharaohBuildingInfoActionSupport {
    PHARAOH_INFO_ACTION_READY,
    PHARAOH_INFO_ACTION_MISSING_CONTEXT,
    PHARAOH_INFO_ACTION_MISSING_DATA,
    PHARAOH_INFO_ACTION_STATE_MISMATCH,
    PHARAOH_INFO_ACTION_UNKNOWN_TOKEN,
    PHARAOH_INFO_ACTION_MISSING_FAULT_ENDPOINT
};

typedef struct PharaohBuildingInfoActionResult {
    enum PharaohBuildingInfoActionSupport support;
    uint32_t native_token;
    const char *field;
} PharaohBuildingInfoActionResult;

typedef struct PharaohBuildingInfoActionBindings {
    PharaohBuildingInfoActionProviders providers;
    PharaohSquareButtonClock clock;
    void *clock_user_data;
    PharaohBuildingInfoActionResult last_result;
} PharaohBuildingInfoActionBindings;

/* Borrow all real contexts/pools and keep bindings at a stable address.
 * Every supplied child context must satisfy its own existing provider contract,
 * including allocated table/sentinel backing and initialized platform state.
 * Support checks provider availability, required function endpoints and shared
 * aliases; it does not establish array bounds or producer lifecycle/history.
 * bind() installs genuine adapters on the SAME existing generic/image/square
 * input objects; it creates no input owner. The real clock/user-data pair is
 * retained behind a forwarding clock adapter. Scope these input objects to
 * info-window use, or restore the other catalogue's binding before using it.
 * No pool or224-byte info record is copied.
 *
 * Bind first, then use resolve with program_data.initialize_once. The latter
 * rejects any missing one of23 providers. Generic callbacks publish selected
 * primary/secondary to actual D3B198/D3B194 before invoking their C provider.
 * Image/square dispatch publish their own selected parameters instead. */
PharaohBuildingInfoActionResult pharaoh_building_info_action_bindings_bind(
    PharaohBuildingInfoActionBindings *,const PharaohBuildingInfoActionProviders *);
PharaohBuildingInfoActionResult pharaoh_building_info_action_bindings_support(
    const PharaohBuildingInfoActionBindings *,uint32_t native_token);
PharaohGenericButtonAction pharaoh_building_info_action_bindings_resolve(
    void *bindings,uint32_t native_token);
void pharaoh_building_info_action_bindings_image_dispatch(void *,uint32_t native_token);
void pharaoh_building_info_action_bindings_square_dispatch(void *,uint32_t native_token);

#endif
