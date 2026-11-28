#ifndef PHARAOH_UI_BUILDING_INFO_ACTIONS_LIVE_H
#define PHARAOH_UI_BUILDING_INFO_ACTIONS_LIVE_H

#include "pharaoh/ui/building_info_init_support.h"
#include "pharaoh/ui/city_message_queue.h"
#include "pharaoh/ui/message_dialog_live.h"
#include "pharaoh/ui/window_state_actions.h"
#include "pharaoh/runtime/input_transient_reset.h"
#include <stddef.h>

/* Read-only fields in the actual EA1870 player bank, not player snapshots. */
typedef struct PharaohBuildingInfoActionPlayer {
    uint8_t native_0000[0x0c];
    int32_t treasury;
    uint8_t native_0010[0x44f8 - 0x10];
    uint32_t native_44f8; /* EA5D68, nonzero gate in40AA50 */
    uint8_t native_44fc[0x4814 - 0x44fc];
    uint16_t festival_placed; /* EA6084 */
    uint8_t native_4816[0x499c - 0x4816];
    uint32_t mansion_placed; /* EA620C, nonzero gate in40AD30 */
    uint8_t native_49a0[0x49d8 - 0x49a0];
} PharaohBuildingInfoActionPlayer;

/* The signed old phrase clears BYTE weights[-1..8] in Native53D730.
   Those two neighboring bytes belong to these actual DWORD fields. */
typedef struct PharaohBuildingInfoPhraseWeightBank {
    uint32_t was_in_int; /* F55BA0 */
    uint8_t weights[8]; /* F55BA4, shared with info/phrase readers */
    uint32_t extra_script_monument; /* F55BAC, shared scene extra_monument */
} PharaohBuildingInfoPhraseWeightBank;

_Static_assert(sizeof(PharaohBuildingInfoActionPlayer) == 0x49d8 &&
    offsetof(PharaohBuildingInfoActionPlayer, treasury) == 0x0c &&
    offsetof(PharaohBuildingInfoActionPlayer, native_44f8) == 0x44f8 &&
    offsetof(PharaohBuildingInfoActionPlayer, festival_placed) == 0x4814 &&
    offsetof(PharaohBuildingInfoActionPlayer, mansion_placed) == 0x499c,
    "native info-action player fields");
_Static_assert(sizeof(PharaohBuildingInfoPhraseWeightBank) == 16 &&
    offsetof(PharaohBuildingInfoPhraseWeightBank, weights) == 4 &&
    offsetof(PharaohBuildingInfoPhraseWeightBank, extra_script_monument) == 12,
    "native F55BA0 phrase bank and neighboring DWORDs");

typedef struct PharaohBuildingInfoActionsLiveContext {
    PharaohBuildingInfoLiveState *window; /* actual EADCB8 info state */
    PharaohWalkerQueryRecord *walkers; /* actual BCD5F8,388-byte rows */
    const PharaohBuildingInfoActionPlayer *players; /* actual EA1870 */
    const uint32_t *current_player; /* EAAC28 */
    const uint32_t *button_parameter; /* actual D3B198 */
    uint8_t *current_window, *redraw; /* E38DDA/E38DCA */
    const PharaohInfoWalkerSelectionVoiceContext *selection_voice;
    PharaohPopupWindowState *popup; /* persistent real4264D0 state */
    const int32_t *mouse_x, *mouse_y; /* E38E34/E38E30, popup inputs */
    const uint32_t *native_e38da8, *native_e38da4;
    uint32_t *native_f53f0c, *native_f53f08;
    PharaohInputTransientResetState *input_transient; /* actual4CF1F0 state */
    const PharaohMessageDialogLiveCloseContext *dialog_close;
    const PharaohCityMessageQueueContext *messages; /* real catalogue/dialog */
    volatile PharaohBuildingInfoPhraseWeightBank *phrase_bank;
} PharaohBuildingInfoActionsLiveContext;

void pharaoh_building_info_actions_select_walker(
    const PharaohBuildingInfoActionsLiveContext *); /*00406700*/
void pharaoh_building_info_actions_modal_40aa50(
    const PharaohBuildingInfoActionsLiveContext *); /*0040AA50*/
void pharaoh_building_info_actions_modal_40ad30(
    const PharaohBuildingInfoActionsLiveContext *); /*0040AD30*/
void pharaoh_building_info_actions_show_message_help(
    const PharaohBuildingInfoActionsLiveContext *); /*004E2360*/
void pharaoh_building_info_actions_next_phrase(
    const PharaohBuildingInfoActionsLiveContext *); /*0053D730*/

/* Call only with the actual providers selected by the Native branch.
   selection_voice->ui shares window/walkers/redraw. The popup record owns
   its persistent popup fields; its shared window/mouse/redraw scalars are
   published by the concrete adapter. The transient reset record is the
   shared E38DFC DWORD and E38E02/E38DF6 click-latch owner.
   Dialog close/show contexts share the live window/redraw and catalogue.
   info.phrase_counts and phrase.weights both point to phrase_bank->weights;
   scene extra_monument must alias phrase_bank->extra_script_monument.
   No separate8-byte scratch or pool copy is valid. The original signed
   category producer domain is-1..9; old9 skips clearing. Malformed indices
   and incidental GPR/flags/fault behavior are not generalized by this API. */

#endif
