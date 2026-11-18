#ifndef PHARAOH_RUNTIME_SCREEN_BACKGROUND_DISPATCH_H
#define PHARAOH_RUNTIME_SCREEN_BACKGROUND_DISPATCH_H

#include "pharaoh/ui/title_menu_background.h"
#include "pharaoh/ui/menu_screens.h"

#include <stdint.h>

typedef enum PharaohScreenBackgroundAction {
    PHARAOH_SCREEN_BACKGROUND_SIDEBAR,
    PHARAOH_SCREEN_BACKGROUND_INTERMEZZO,
    PHARAOH_SCREEN_BACKGROUND_MESSAGE_DIALOG,
    PHARAOH_SCREEN_BACKGROUND_BUILD_MENU,
    PHARAOH_SCREEN_BACKGROUND_ADVISORS,
    PHARAOH_SCREEN_BACKGROUND_OVERLAY_MENU,
    PHARAOH_SCREEN_BACKGROUND_BUILDING_INFO,
    PHARAOH_SCREEN_BACKGROUND_FILE_DIALOG,
    PHARAOH_SCREEN_BACKGROUND_MAP_FILE_DIALOG,
    PHARAOH_SCREEN_BACKGROUND_MESSAGE_LIST,
    PHARAOH_SCREEN_BACKGROUND_EMPIRE,
    PHARAOH_SCREEN_BACKGROUND_EDITOR_EMPIRE,
    PHARAOH_SCREEN_BACKGROUND_MISSION_SELECTION,
    PHARAOH_SCREEN_BACKGROUND_TOOLTIP,
    PHARAOH_SCREEN_BACKGROUND_CITY_INTRO,
    PHARAOH_SCREEN_BACKGROUND_ADVISOR,
    PHARAOH_SCREEN_BACKGROUND_ADVISOR_TRADE,
    PHARAOH_SCREEN_FOREGROUND_ADVISOR_TRADE,
    PHARAOH_SCREEN_BACKGROUND_ADVISOR_LABOR,
    PHARAOH_SCREEN_FOREGROUND_ADVISOR_LABOR,
    PHARAOH_SCREEN_BACKGROUND_LABOR_PRIORITY,
    PHARAOH_SCREEN_BACKGROUND_FESTIVAL_DIALOG,
    PHARAOH_SCREEN_BACKGROUND_SET_SALARY,
    PHARAOH_SCREEN_BACKGROUND_DONATE_TO_CITY,
    PHARAOH_SCREEN_BACKGROUND_SEND_GIFT,
    PHARAOH_SCREEN_BACKGROUND_PLAYER_SELECTION,
    PHARAOH_SCREEN_BACKGROUND_MAIN_MENU,
    PHARAOH_SCREEN_BACKGROUND_VICTORY,
    PHARAOH_SCREEN_BACKGROUND_MISSION_CHOICE,
    PHARAOH_SCREEN_BACKGROUND_BRIEFING,
    PHARAOH_SCREEN_BACKGROUND_NAME_INPUT,
    PHARAOH_SCREEN_BACKGROUND_END_PICTURE_1,
    PHARAOH_SCREEN_BACKGROUND_END_PICTURE_2,
    PHARAOH_SCREEN_BACKGROUND_END_PICTURE_3,
    PHARAOH_SCREEN_BACKGROUND_END_PICTURE_FINAL,
    PHARAOH_SCREEN_BACKGROUND_LOGO,
    PHARAOH_SCREEN_BACKGROUND_MAP_SELECTION,
    PHARAOH_SCREEN_BACKGROUND_DISPLAY_MODE_CONFIRMATION,
    PHARAOH_SCREEN_BACKGROUND_POPUP_DIALOG,
    PHARAOH_SCREEN_BACKGROUND_VICTORY_DIALOG,
    PHARAOH_SCREEN_BACKGROUND_BURIAL_PROVISIONS,
    PHARAOH_SCREEN_BACKGROUND_ADVISOR_COMMENT,
    PHARAOH_SCREEN_BACKGROUND_ADVISOR_TEXT,
    PHARAOH_SCREEN_BACKGROUND_NAME_LIST,
    PHARAOH_SCREEN_BACKGROUND_HOLD_FESTIVAL,
    PHARAOH_SCREEN_BACKGROUND_HIGH_SCORES,
    PHARAOH_SCREEN_BACKGROUND_LOADING
} PharaohScreenBackgroundAction;

typedef struct PharaohScreenBackgroundDispatchOperations {
    void *user_data;
    void (*draw_action)(void *user_data,
        PharaohScreenBackgroundAction action);
    void (*draw_select_list)(void *user_data, int32_t x, int32_t y,
        int32_t rows);
    void (*set_initial_dispatch_training)(void *user_data,
        int32_t enabled);
} PharaohScreenBackgroundDispatchOperations;

typedef struct PharaohScreenBackgroundDispatchContext {
    uint8_t *redraw_requested;
    int32_t tooltip_window_override_active;
    uint32_t tooltip_window_override_id;
    uint8_t current_window_id;
    int32_t select_list_x;
    int32_t select_list_y;
    int32_t select_list_rows;
    const PharaohTitleMenuBackgroundContext *title_menu_background;
    const PharaohMenuScreenContext *logo_screen;
    const PharaohScreenBackgroundDispatchOperations *operations;
} PharaohScreenBackgroundDispatchContext;

/* Original Pharaoh.exe entry 0x004D71C0. The action callbacks must be bound
   to the corresponding typed draw owners by the runtime host. Window 0 and
   logo window 0x3C call their typed C owners directly. */
void pharaoh_screen_background_dispatch(
    const PharaohScreenBackgroundDispatchContext *context,
    int32_t force_redraw);

#endif
