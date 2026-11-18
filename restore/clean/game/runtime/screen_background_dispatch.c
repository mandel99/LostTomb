#include "pharaoh/runtime/screen_background_dispatch.h"

enum {
    SCREEN_FRONT_MENU = 0x00,
    SCREEN_CITY = 0x01,
    SCREEN_TOP_MENU = 0x03,
    SCREEN_INTERMEZZO = 0x04,
    SCREEN_MESSAGE_DIALOG = 0x05,
    SCREEN_BUILD_MENU = 0x06,
    SCREEN_ADVISORS = 0x07,
    SCREEN_OVERLAY_MENU = 0x08,
    SCREEN_BUILDING_INFO = 0x09,
    SCREEN_FILE_DIALOG = 0x0d,
    SCREEN_SELECT_LIST = 0x0f,
    SCREEN_MAP_FILE_DIALOG = 0x10,
    SCREEN_MESSAGE_LIST = 0x11,
    SCREEN_EMPIRE = 0x14,
    SCREEN_EDITOR_EMPIRE = 0x15,
    SCREEN_EDITOR_START_YEAR = 0x16,
    SCREEN_MISSION_SELECTION = 0x17,
    SCREEN_EDITOR_EVENT = 0x18,
    SCREEN_EDITOR_FLOOD = 0x19,
    SCREEN_EDITOR_ALLOWED_BUILDINGS = 0x1b,
    SCREEN_EDITOR_WIN_CRITERIA = 0x1c,
    SCREEN_CITY_LEGION_SELECTED = 0x1e,
    SCREEN_CITY_TRANSPORT_SELECTED = 0x1f,
    SCREEN_CITY_WARSHIP_SELECTED = 0x20,
    SCREEN_EDITOR_SETTINGS = 0x25,
    SCREEN_EDITOR_EVENT_LIST = 0x27,
    SCREEN_RESOURCE_SETTINGS = 0x28,
    SCREEN_LABOR_PRIORITY = 0x2a,
    SCREEN_FESTIVAL_DIALOG = 0x2b,
    SCREEN_SET_SALARY = 0x2c,
    SCREEN_DONATE_TO_CITY = 0x2d,
    SCREEN_SEND_GIFT = 0x2e,
    SCREEN_PLAYER_SELECTION = 0x2f,
    SCREEN_MAIN_MENU = 0x30,
    SCREEN_MISSION_END = 0x33,
    SCREEN_MISSION_CHOICE = 0x34,
    SCREEN_BRIEFING = 0x35,
    SCREEN_NAME_INPUT = 0x36,
    SCREEN_END_PICTURE_1 = 0x37,
    SCREEN_END_PICTURE_2 = 0x38,
    SCREEN_END_PICTURE_3 = 0x39,
    SCREEN_END_PICTURE_FINAL = 0x3a,
    SCREEN_BRIEFING_2 = 0x3b,
    SCREEN_LOGO = 0x3c,
    SCREEN_MAP_SELECTION = 0x3d,
    SCREEN_DISPLAY_MODE_CONFIRMATION = 0x3e,
    SCREEN_POPUP_DIALOG = 0x3f,
    SCREEN_VICTORY = 0x43,
    SCREEN_EDITOR_GODS = 0x46,
    SCREEN_CITY_LEGION_ORDER = 0x49,
    SCREEN_EDITOR_MONUMENTS = 0x4b,
    SCREEN_BURIAL_PROVISIONS = 0x4c,
    SCREEN_ADVISOR_COMMENT_ALIAS = 0x4d,
    SCREEN_ADVISOR_COMMENT = 0x4f,
    SCREEN_ADVISOR_TEXT = 0x50,
    SCREEN_NAME_LIST = 0x51,
    SCREEN_HOLD_FESTIVAL = 0x52,
    SCREEN_HIGH_SCORES = 0x53,
    SCREEN_LOADING = 0x57
};

static void draw_action(
    const PharaohScreenBackgroundDispatchContext *context,
    PharaohScreenBackgroundAction action)
{
    context->operations->draw_action(context->operations->user_data, action);
}

void pharaoh_screen_background_dispatch(
    const PharaohScreenBackgroundDispatchContext *context,
    int32_t force_redraw)
{
    uint32_t window_id;

    if (force_redraw == 0) {
        if (*context->redraw_requested == 0) {
            return;
        }
    } else {
        *context->redraw_requested = 1;
    }

    window_id = context->tooltip_window_override_id;
    if (context->tooltip_window_override_active == 0) {
        window_id = context->current_window_id;
    }

    switch (window_id) {
    case SCREEN_FRONT_MENU:
        pharaoh_title_menu_background_draw(context->title_menu_background, 0);
        break;
    case SCREEN_CITY:
    case SCREEN_TOP_MENU:
    case SCREEN_EDITOR_START_YEAR:
    case SCREEN_EDITOR_EVENT:
    case SCREEN_EDITOR_FLOOD:
    case SCREEN_EDITOR_ALLOWED_BUILDINGS:
    case SCREEN_EDITOR_WIN_CRITERIA:
    case SCREEN_CITY_LEGION_SELECTED:
    case SCREEN_CITY_TRANSPORT_SELECTED:
    case SCREEN_CITY_WARSHIP_SELECTED:
    case SCREEN_EDITOR_SETTINGS:
    case SCREEN_EDITOR_EVENT_LIST:
    case SCREEN_EDITOR_GODS:
    case SCREEN_CITY_LEGION_ORDER:
    case SCREEN_EDITOR_MONUMENTS:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_SIDEBAR);
        break;
    case SCREEN_INTERMEZZO:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_INTERMEZZO);
        break;
    case SCREEN_MESSAGE_DIALOG:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_MESSAGE_DIALOG);
        break;
    case SCREEN_BUILD_MENU:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_BUILD_MENU);
        break;
    case SCREEN_ADVISORS:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_ADVISORS);
        break;
    case SCREEN_OVERLAY_MENU:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_OVERLAY_MENU);
        break;
    case SCREEN_BUILDING_INFO:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_SIDEBAR);
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_BUILDING_INFO);
        break;
    case SCREEN_FILE_DIALOG:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_FILE_DIALOG);
        break;
    case SCREEN_SELECT_LIST:
        context->operations->draw_select_list(
            context->operations->user_data, context->select_list_x,
            context->select_list_y, context->select_list_rows);
        break;
    case SCREEN_MAP_FILE_DIALOG:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_MAP_FILE_DIALOG);
        break;
    case SCREEN_MESSAGE_LIST:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_SIDEBAR);
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_MESSAGE_LIST);
        break;
    case SCREEN_EMPIRE:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_EMPIRE);
        break;
    case SCREEN_EDITOR_EMPIRE:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_EDITOR_EMPIRE);
        break;
    case SCREEN_MISSION_SELECTION:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_MISSION_SELECTION);
        break;
    case 0x23:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_TOOLTIP);
        break;
    case 0x26:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_CITY_INTRO);
        break;
    case SCREEN_RESOURCE_SETTINGS:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_ADVISOR);
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_ADVISOR_TRADE);
        draw_action(context, PHARAOH_SCREEN_FOREGROUND_ADVISOR_TRADE);
        context->operations->set_initial_dispatch_training(
            context->operations->user_data, 1);
        break;
    case SCREEN_LABOR_PRIORITY:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_ADVISOR);
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_ADVISOR_LABOR);
        draw_action(context, PHARAOH_SCREEN_FOREGROUND_ADVISOR_LABOR);
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_LABOR_PRIORITY);
        break;
    case SCREEN_FESTIVAL_DIALOG:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_ADVISOR);
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_FESTIVAL_DIALOG);
        break;
    case SCREEN_SET_SALARY:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_ADVISOR);
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_SET_SALARY);
        break;
    case SCREEN_DONATE_TO_CITY:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_ADVISOR);
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_DONATE_TO_CITY);
        break;
    case SCREEN_SEND_GIFT:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_ADVISOR);
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_SEND_GIFT);
        break;
    case SCREEN_PLAYER_SELECTION:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_PLAYER_SELECTION);
        break;
    case SCREEN_MAIN_MENU:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_MAIN_MENU);
        break;
    case SCREEN_MISSION_END:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_VICTORY);
        break;
    case SCREEN_MISSION_CHOICE:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_MISSION_CHOICE);
        break;
    case SCREEN_BRIEFING:
    case SCREEN_BRIEFING_2:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_BRIEFING);
        break;
    case SCREEN_NAME_INPUT:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_NAME_INPUT);
        break;
    case SCREEN_END_PICTURE_1:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_END_PICTURE_1);
        break;
    case SCREEN_END_PICTURE_2:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_END_PICTURE_2);
        break;
    case SCREEN_END_PICTURE_3:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_END_PICTURE_3);
        break;
    case SCREEN_END_PICTURE_FINAL:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_END_PICTURE_FINAL);
        break;
    case SCREEN_LOGO:
        pharaoh_logo_window_draw(context->logo_screen);
        break;
    case SCREEN_MAP_SELECTION:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_MAP_SELECTION);
        break;
    case SCREEN_DISPLAY_MODE_CONFIRMATION:
        draw_action(context,
            PHARAOH_SCREEN_BACKGROUND_DISPLAY_MODE_CONFIRMATION);
        break;
    case SCREEN_POPUP_DIALOG:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_POPUP_DIALOG);
        break;
    case SCREEN_VICTORY:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_VICTORY_DIALOG);
        break;
    case SCREEN_BURIAL_PROVISIONS:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_ADVISOR);
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_BURIAL_PROVISIONS);
        break;
    case SCREEN_ADVISOR_COMMENT:
    case SCREEN_ADVISOR_COMMENT_ALIAS:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_ADVISOR_COMMENT);
        break;
    case SCREEN_ADVISOR_TEXT:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_ADVISOR_TEXT);
        break;
    case SCREEN_NAME_LIST:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_NAME_LIST);
        break;
    case SCREEN_HOLD_FESTIVAL:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_HOLD_FESTIVAL);
        break;
    case SCREEN_HIGH_SCORES:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_HIGH_SCORES);
        break;
    case SCREEN_LOADING:
        draw_action(context, PHARAOH_SCREEN_BACKGROUND_LOADING);
        break;
    default:
        break;
    }

    *context->redraw_requested = 0;
}
