#ifndef PHARAOH_EDITOR_CITY_DETAIL_BUTTONS_H
#define PHARAOH_EDITOR_CITY_DETAIL_BUTTONS_H

#include <stdint.h>

enum {
    PHARAOH_EDITOR_CITY_COUNT = 200,
    PHARAOH_EDITOR_RESOURCE_COUNT = 35
};

typedef struct PharaohEditorCityRecord {
    uint8_t bytes[98];
} PharaohEditorCityRecord;

typedef struct PharaohEditorCityValueRequest {
    int32_t x;
    int32_t y;
    int32_t field_width;
    int32_t text_id;
    uint8_t *target;
} PharaohEditorCityValueRequest;

typedef struct PharaohEditorCityDetailState {
    PharaohEditorCityRecord *cities;
    int32_t selected_city;
    int32_t cursor_y;
    int32_t alternate_text;
    int32_t selection_active;
    uint8_t current_window;
    uint8_t redraw_requested;
    uint32_t resources[PHARAOH_EDITOR_RESOURCE_COUNT];
    void (*open_value)(void *user_data,
        const PharaohEditorCityValueRequest *request);
    void (*open_byte)(void *user_data, int32_t x, int32_t y,
        uint8_t *target);
    void (*open_int16)(void *user_data, int32_t x, int32_t y,
        int16_t *target);
    int32_t (*trade_amount)(void *user_data, int32_t city,
        int32_t resource);
    int32_t (*city_sells)(void *user_data, int32_t city,
        int32_t resource);
    int32_t (*city_buys)(void *user_data, int32_t city,
        int32_t resource);
    void (*permission_set)(void *user_data, int32_t city,
        int32_t resource, int32_t value);
    void *user_data;
} PharaohEditorCityDetailState;

void pharaoh_editor_city_open_name(PharaohEditorCityDetailState *state);
void pharaoh_editor_city_open_text(PharaohEditorCityDetailState *state);
void pharaoh_editor_city_cycle_type(PharaohEditorCityDetailState *state);
void pharaoh_editor_city_open_alt_text(PharaohEditorCityDetailState *state);
void pharaoh_editor_city_open_route(PharaohEditorCityDetailState *state);
void pharaoh_editor_city_open_value(PharaohEditorCityDetailState *state);
void pharaoh_editor_city_normalize_permissions(
    PharaohEditorCityDetailState *state);
void pharaoh_editor_city_show_resources(PharaohEditorCityDetailState *state);

#endif
