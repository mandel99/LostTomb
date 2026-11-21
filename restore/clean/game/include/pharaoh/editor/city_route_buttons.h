#ifndef PHARAOH_EDITOR_CITY_ROUTE_BUTTONS_H
#define PHARAOH_EDITOR_CITY_ROUTE_BUTTONS_H

#include "pharaoh/editor/route_geometry.h"

#include <stdint.h>

enum PharaohEditorRouteNumericHandler {
    PHARAOH_EDITOR_ROUTE_SELECT,
    PHARAOH_EDITOR_ROUTE_REBUILD
};

typedef struct PharaohEditorRouteCityRecord {
    uint8_t route_id;
} PharaohEditorRouteCityRecord;

typedef struct PharaohEditorCityRouteState {
    PharaohEditorRoute *routes;
    PharaohEditorRouteCityRecord *cities;
    int32_t selected_route;
    int32_t selected_city;
    int32_t cursor_y;
    int32_t route_edit_active;
    int32_t editor_mode;
    int32_t current_building_kind;
    int32_t selected_image;
    int32_t mode_images[5];
    const int32_t *permission_kinds;
    uint8_t current_screen;
    uint8_t redraw_requested;
    void (*open_int32)(void *user_data, int32_t x, int32_t y,
        int32_t *target, enum PharaohEditorRouteNumericHandler handler);
    void (*open_uint8)(void *user_data, int32_t x, int32_t y,
        uint8_t *target, enum PharaohEditorRouteNumericHandler handler);
    int32_t (*permission_get)(void *user_data, int32_t city, int32_t kind);
    void (*permission_set)(void *user_data, int32_t city, int32_t kind,
        int32_t value);
    void *user_data;
} PharaohEditorCityRouteState;

void pharaoh_editor_route_open_selection(PharaohEditorCityRouteState *state);
void pharaoh_editor_route_open_spacing(PharaohEditorCityRouteState *state);
void pharaoh_editor_route_remove(PharaohEditorCityRouteState *state);
void pharaoh_editor_route_toggle_kind(PharaohEditorCityRouteState *state);
void pharaoh_editor_route_selection_completed(
    PharaohEditorCityRouteState *state);
void pharaoh_editor_route_select_mode_image(
    PharaohEditorCityRouteState *state);
void pharaoh_editor_route_cycle_permission(
    PharaohEditorCityRouteState *state);
void pharaoh_editor_city_route_clamp(PharaohEditorCityRouteState *state);

#endif
