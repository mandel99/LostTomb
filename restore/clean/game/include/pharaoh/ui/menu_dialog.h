#ifndef PHARAOH_UI_MENU_DIALOG_H
#define PHARAOH_UI_MENU_DIALOG_H

#include <stdint.h>

typedef struct PharaohMenuDialogState {
    uint8_t *accepted;
    uint8_t *redraw_requested;
    uint8_t *current_window_id;
    const uint8_t *return_window_id;
} PharaohMenuDialogState;

typedef struct PharaohMenuDialogOperations {
    void *user_data;
    void (*respond)(void *user_data);
} PharaohMenuDialogOperations;

/* Complete typed translations of native 0x00426590 and 0x004265B0. */
void pharaoh_menu_dialog_accept(const PharaohMenuDialogState *state,
    const PharaohMenuDialogOperations *operations);
void pharaoh_menu_dialog_reject(const PharaohMenuDialogState *state,
    const PharaohMenuDialogOperations *operations);

#endif
