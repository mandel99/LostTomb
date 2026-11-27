#ifndef PHARAOH_UI_LOAD_SAVE_ACTIONS_H
#define PHARAOH_UI_LOAD_SAVE_ACTIONS_H

#include "pharaoh/ui/file_selector.h"
#include "pharaoh/runtime/text_input_state.h"

typedef struct PharaohLoadSaveActionsState {
    uint8_t *current_screen,*redraw_requested; /* E38DDA/E38DCA */
    uint8_t *dialog_mode; /* D3B1C0: save0, load1, delete2 */
    uint32_t *return_screen; /* D3B1BC */
    uint32_t *load_latch; /* 6059E8, cleared only by00402450 */
    const uint32_t *button_parameter; /* D3B198 */
    /* Native aligned64-byte buffers, with character views for text input. */
    uint32_t *load_name,*alternate_name; /* E37100/E37078 */
    const char *default_name; /* E370BC */
    PharaohFileSelectorState *selector;
    /* Actual shared return_pressed(E20998), active field and cursor state.
       Fields2/4 must refer to the corresponding actual name buffers. */
    PharaohTextInputState *text_input;
} PharaohLoadSaveActionsState;

void pharaoh_load_save_enter_load(PharaohLoadSaveActionsState *state);
void pharaoh_load_save_enter_save(PharaohLoadSaveActionsState *state);
void pharaoh_load_save_enter_delete(PharaohLoadSaveActionsState *state);
void pharaoh_load_save_select_row(PharaohLoadSaveActionsState *state);

#endif
