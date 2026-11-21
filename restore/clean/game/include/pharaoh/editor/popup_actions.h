#ifndef PHARAOH_EDITOR_POPUP_ACTIONS_H
#define PHARAOH_EDITOR_POPUP_ACTIONS_H

#include <stdint.h>

enum PharaohEditorDialogAction {
    PHARAOH_EDITOR_DIALOG_ACKNOWLEDGE,
    PHARAOH_EDITOR_DIALOG_CONFIRM_CITY_PAYMENT
};

typedef struct PharaohSelectionPopupRequest {
    int32_t x;
    int32_t y;
    int32_t count;
    int32_t language_group;
    int32_t first_text;
    uint8_t *byte_destination;
    int16_t *word_destination;
    int32_t *dword_destination;
    int32_t post_action;
    int32_t storage_width;
    int32_t preserve_mapping;
    int32_t completion;
    int32_t alternate_mode;
    int32_t rows;
    int32_t disable_edges;
} PharaohSelectionPopupRequest;

#pragma pack(push, 1)
typedef struct PharaohEditorEmpireCity {
    uint8_t route_id;
    uint8_t state;
    uint8_t reserved_before_cost[72];
    int16_t cost;
    uint8_t reserved_after_cost[30];
} PharaohEditorEmpireCity;
#pragma pack(pop)

typedef struct PharaohEditorPopupState {
    PharaohEditorEmpireCity *cities;
    uint32_t selected_city;
    uint32_t selected_player;
    uint8_t family_exists;
    uint32_t alternate_mode;
    uint32_t selection_active;
    int16_t word_choice;
    int32_t alternate_choice;
    int32_t category_choice;
    uint32_t popup_result;
    uint8_t redraw_requested;
    int32_t (*get_player_funds)(void *user_data, int32_t player);
    uint8_t (*route_is_active)(void *user_data, int32_t route);
    void (*apply_finance)(void *user_data, int32_t player, int32_t amount);
    void (*refresh_finance)(void *user_data);
    void (*refresh_city)(void *user_data);
    void (*show_dialog)(void *user_data, int32_t message,
        enum PharaohEditorDialogAction action, int32_t style);
    void (*open_selection)(void *user_data,
        const PharaohSelectionPopupRequest *request);
    void *user_data;
} PharaohEditorPopupState;

void pharaoh_editor_confirm_city_payment(PharaohEditorPopupState *state);
void pharaoh_editor_request_city_payment(PharaohEditorPopupState *state);
void pharaoh_editor_open_word_choice(PharaohEditorPopupState *state);
void pharaoh_editor_open_alternate_choice(PharaohEditorPopupState *state);
void pharaoh_editor_open_category_choice(PharaohEditorPopupState *state);

#endif
