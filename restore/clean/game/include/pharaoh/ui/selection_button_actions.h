#ifndef PHARAOH_UI_SELECTION_BUTTON_ACTIONS_H
#define PHARAOH_UI_SELECTION_BUTTON_ACTIONS_H

#include <stdint.h>

typedef struct PharaohSelectionButtonState {
    uint32_t toggle;
    uint32_t selection_mode;
    uint32_t selected_building;
    uint32_t button_parameter;
    uint32_t current_player;
    int32_t primary_count;
    int32_t secondary_count;
    const uint32_t *offsets;
    const uint8_t *building_owner;
    uint8_t *player_records;
    uint32_t *player_selected_building;
    uint8_t redraw_requested;
} PharaohSelectionButtonState;

/* Concrete borrowed fields for the same four algorithms. resource_states is
 * the selected196-byte storage record's actual36-byte mode field; the caller
 * resolves storage_id from its real building view, not a copied owner array.
 * player_selected_building is the actual selected player's EA5EE4 DWORD.
 * Unused branch fields may be NULL; a reached branch requires its real field. */
typedef struct PharaohSelectionButtonBorrowedState {
    volatile uint32_t *toggle,*selection_mode;
    const volatile uint32_t *selected_building,*button_parameter;
    const volatile int32_t *primary_count,*secondary_count;
    const uint32_t *offsets;
    volatile uint8_t *resource_states;
    volatile uint32_t *player_selected_building;
    volatile uint8_t *redraw_requested;
} PharaohSelectionButtonBorrowedState;

void pharaoh_selection_toggle_borrowed(const PharaohSelectionButtonBorrowedState *);
void pharaoh_selection_enable_mode_borrowed(const PharaohSelectionButtonBorrowedState *);
void pharaoh_selection_mark_primary_borrowed(const PharaohSelectionButtonBorrowedState *);
void pharaoh_selection_apply_borrowed(const PharaohSelectionButtonBorrowedState *);

void pharaoh_selection_toggle(PharaohSelectionButtonState *state);
void pharaoh_selection_enable_mode(PharaohSelectionButtonState *state);
void pharaoh_selection_mark_primary(PharaohSelectionButtonState *state);
void pharaoh_selection_apply(PharaohSelectionButtonState *state);

#endif
