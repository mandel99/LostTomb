#ifndef PHARAOH_EDITOR_NUMERIC_DIALOG_H
#define PHARAOH_EDITOR_NUMERIC_DIALOG_H

#include <stdint.h>

enum PharaohEditorNumericWidth {
    PHARAOH_EDITOR_NUMERIC_INT8 = 1,
    PHARAOH_EDITOR_NUMERIC_INT16 = 2,
    PHARAOH_EDITOR_NUMERIC_INT32 = 4
};

enum PharaohEditorNumericMode {
    PHARAOH_EDITOR_NUMERIC_PLAIN = 0,
    PHARAOH_EDITOR_NUMERIC_EMPIRE_MAP = 1,
    PHARAOH_EDITOR_NUMERIC_EMPIRE_CITY = 2
};

typedef void (*PharaohEditorNumericCompletion)(void *user_data);

typedef struct PharaohEditorNumericMapInfo {
    int32_t map_id;
    int32_t camera_x;
    int32_t camera_y;
} PharaohEditorNumericMapInfo;

typedef struct PharaohEditorNumericDialog {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t mode;
    uint8_t *target8;
    int16_t *target16;
    int32_t *target32;
    int32_t value;
    int32_t digits;
    uint8_t return_window;
    uint8_t current_window;
    int32_t redraw_requested;
    PharaohEditorNumericCompletion completion;
    void *completion_data;
    int32_t *empire_map;
    int32_t *empire_city;
    PharaohEditorNumericMapInfo *map_info;
    void (*select_empire_map)(void *user_data, int32_t map_id);
    void (*confirm_empire_city)(void *user_data, int32_t city_id);
    void *user_data;
} PharaohEditorNumericDialog;

void pharaoh_editor_numeric_show(PharaohEditorNumericDialog *dialog,
    int32_t x, int32_t y, int32_t width, uint8_t *target8,
    int16_t *target16, int32_t *target32, int32_t mode);
void pharaoh_editor_numeric_show_int16(PharaohEditorNumericDialog *dialog,
    int32_t x, int32_t y, int16_t *target,
    PharaohEditorNumericCompletion completion, void *completion_data);
void pharaoh_editor_numeric_confirm(PharaohEditorNumericDialog *dialog);

#endif
