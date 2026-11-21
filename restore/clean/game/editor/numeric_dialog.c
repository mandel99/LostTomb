#include "pharaoh/editor/numeric_dialog.h"

enum {
    SCREEN_EDITOR_NUMERIC_INPUT = 14,
    REDRAW_ALL = 1,
    EMPIRE_MAP_MAX = 33,
    EMPIRE_MAP_CAMERA_X = 109,
    EMPIRE_MAP_CAMERA_Y = 218
};

static void enter_numeric_window(PharaohEditorNumericDialog *dialog,
    uint8_t previous_window, PharaohEditorNumericCompletion completion,
    void *completion_data)
{
    dialog->completion = completion;
    dialog->completion_data = completion_data;
    dialog->return_window = previous_window;
    dialog->value = 0;
    dialog->digits = 0;
    dialog->current_window = SCREEN_EDITOR_NUMERIC_INPUT;
}

void pharaoh_editor_numeric_show(PharaohEditorNumericDialog *dialog,
    int32_t x, int32_t y, int32_t width, uint8_t *target8,
    int16_t *target16, int32_t *target32, int32_t mode)
{
    const uint8_t previous_window = dialog->current_window;
    dialog->x = x;
    dialog->y = y;
    dialog->mode = mode;
    dialog->target8 = target8;
    dialog->width = width;
    dialog->target32 = target32;
    dialog->target16 = target16;
    enter_numeric_window(dialog, previous_window, 0, 0);
}

void pharaoh_editor_numeric_show_int16(PharaohEditorNumericDialog *dialog,
    int32_t x, int32_t y, int16_t *target,
    PharaohEditorNumericCompletion completion, void *completion_data)
{
    const uint8_t previous_window = dialog->current_window;
    dialog->target16 = target;
    dialog->x = x;
    dialog->y = y;
    dialog->width = PHARAOH_EDITOR_NUMERIC_INT16;
    dialog->mode = PHARAOH_EDITOR_NUMERIC_PLAIN;
    dialog->target8 = 0;
    dialog->target32 = 0;
    enter_numeric_window(dialog, previous_window, completion, completion_data);
}

void pharaoh_editor_numeric_confirm(PharaohEditorNumericDialog *dialog)
{
    int32_t sign = 1;
    if (dialog->width == PHARAOH_EDITOR_NUMERIC_INT16 &&
        *dialog->target16 < 0) {
        *dialog->target16 = (int16_t)dialog->value;
        sign = -1;
    } else if (dialog->width == PHARAOH_EDITOR_NUMERIC_INT32 &&
        *dialog->target32 < 0) {
        *dialog->target32 = dialog->value;
        sign = -1;
    } else if (dialog->width == PHARAOH_EDITOR_NUMERIC_INT8) {
        *dialog->target8 = (uint8_t)dialog->value;
    } else if (dialog->width == PHARAOH_EDITOR_NUMERIC_INT16) {
        *dialog->target16 = (int16_t)dialog->value;
    } else if (dialog->width == PHARAOH_EDITOR_NUMERIC_INT32) {
        *dialog->target32 = dialog->value;
    }

    if (dialog->width == PHARAOH_EDITOR_NUMERIC_INT16 &&
        ((*dialog->target16 < 0 && sign == 1) ||
         (*dialog->target16 > 0 && sign == -1))) {
        *dialog->target16 =
            (int16_t)(0u - (uint16_t)*dialog->target16);
    }
    if (dialog->width == PHARAOH_EDITOR_NUMERIC_INT32 &&
        ((*dialog->target32 < 0 && sign == 1) ||
         (*dialog->target32 > 0 && sign == -1))) {
        *dialog->target32 = (int32_t)(0u - (uint32_t)*dialog->target32);
    }

    if (dialog->mode == PHARAOH_EDITOR_NUMERIC_EMPIRE_MAP) {
        *dialog->empire_map =
            (int32_t)((uint32_t)*dialog->empire_map - 1u);
        if (*dialog->empire_map >= 0 &&
            *dialog->empire_map <= EMPIRE_MAP_MAX) {
            dialog->map_info->map_id = *dialog->empire_map;
            dialog->select_empire_map(dialog->user_data, *dialog->empire_map);
            dialog->map_info->camera_x = EMPIRE_MAP_CAMERA_X;
            dialog->map_info->camera_y = EMPIRE_MAP_CAMERA_Y;
        }
    } else if (dialog->mode == PHARAOH_EDITOR_NUMERIC_EMPIRE_CITY) {
        dialog->confirm_empire_city(dialog->user_data,
            (int32_t)((uint32_t)*dialog->empire_city - 1u));
    }

    if (dialog->completion != 0) {
        dialog->completion(dialog->completion_data);
        dialog->completion = 0;
    }
    dialog->current_window = dialog->return_window;
    dialog->redraw_requested = REDRAW_ALL;
}
