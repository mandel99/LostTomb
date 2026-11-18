#include "pharaoh/runtime/application_window.h"

#include <stddef.h>

enum PharaohApplicationCursorResourceId {
    APPLICATION_CURSOR_RESOURCE_DEFAULT = 0x66,
    APPLICATION_CURSOR_RESOURCE_SWORD = 0x88,
    APPLICATION_CURSOR_RESOURCE_CLEAR_LAND = 0x82,
    APPLICATION_CURSOR_RESOURCE_BOAT_DOWN_B = 0x93,
    APPLICATION_CURSOR_RESOURCE_BOAT_FIGURE_A = 0x90,
    APPLICATION_CURSOR_RESOURCE_BOAT_FIGURE_B = 0x91,
    APPLICATION_CURSOR_RESOURCE_BOAT_DOWN_A = 0x92,
    APPLICATION_CURSOR_RESOURCE_BOAT = 0x8e,
    APPLICATION_CURSOR_RESOURCE_PERSON_DIRECTION = 0x96,
    APPLICATION_CURSOR_RESOURCE_CART = 0x95,
    APPLICATION_CURSOR_RESOURCE_BOW = 0x94,
    APPLICATION_CURSOR_RESOURCE_PARTICLES_B = 0x99,
    APPLICATION_CURSOR_RESOURCE_PARTICLES_A = 0x98,
    APPLICATION_CURSOR_RESOURCE_PARTICLES_D = 0x9b,
    APPLICATION_CURSOR_RESOURCE_PARTICLES_C = 0x9a,
    APPLICATION_CURSOR_RESOURCE_COMBAT = 0x9c,
    APPLICATION_CURSOR_RESOURCE_BOAT_HORIZONTAL = 0x9d,
    APPLICATION_CURSOR_RESOURCE_BOAT_RIGHT = 0x9f,
    APPLICATION_CURSOR_RESOURCE_BOAT_BRUSH = 0xa0
};

static const char native_window_class_name[] = "WinSJBClass";

typedef struct PharaohApplicationCursorResource {
    PharaohApplicationCursor cursor;
    uint32_t resource_id;
} PharaohApplicationCursorResource;

static const PharaohApplicationCursorResource cursor_resources[
    PHARAOH_APPLICATION_WINDOW_CURSOR_COUNT] = {
    { PHARAOH_APPLICATION_CURSOR_DEFAULT, APPLICATION_CURSOR_RESOURCE_DEFAULT },
    { PHARAOH_APPLICATION_CURSOR_SWORD, APPLICATION_CURSOR_RESOURCE_SWORD },
    { PHARAOH_APPLICATION_CURSOR_CLEAR_LAND, APPLICATION_CURSOR_RESOURCE_CLEAR_LAND },
    { PHARAOH_APPLICATION_CURSOR_BOAT_DOWN_B, APPLICATION_CURSOR_RESOURCE_BOAT_DOWN_B },
    { PHARAOH_APPLICATION_CURSOR_BOAT_FIGURE_A, APPLICATION_CURSOR_RESOURCE_BOAT_FIGURE_A },
    { PHARAOH_APPLICATION_CURSOR_BOAT_FIGURE_B, APPLICATION_CURSOR_RESOURCE_BOAT_FIGURE_B },
    { PHARAOH_APPLICATION_CURSOR_BOAT_DOWN_A, APPLICATION_CURSOR_RESOURCE_BOAT_DOWN_A },
    { PHARAOH_APPLICATION_CURSOR_BOAT, APPLICATION_CURSOR_RESOURCE_BOAT },
    { PHARAOH_APPLICATION_CURSOR_PERSON_DIRECTION, APPLICATION_CURSOR_RESOURCE_PERSON_DIRECTION },
    { PHARAOH_APPLICATION_CURSOR_CART, APPLICATION_CURSOR_RESOURCE_CART },
    { PHARAOH_APPLICATION_CURSOR_BOW, APPLICATION_CURSOR_RESOURCE_BOW },
    { PHARAOH_APPLICATION_CURSOR_PARTICLES_B, APPLICATION_CURSOR_RESOURCE_PARTICLES_B },
    { PHARAOH_APPLICATION_CURSOR_PARTICLES_A, APPLICATION_CURSOR_RESOURCE_PARTICLES_A },
    { PHARAOH_APPLICATION_CURSOR_PARTICLES_D, APPLICATION_CURSOR_RESOURCE_PARTICLES_D },
    { PHARAOH_APPLICATION_CURSOR_PARTICLES_C, APPLICATION_CURSOR_RESOURCE_PARTICLES_C },
    { PHARAOH_APPLICATION_CURSOR_COMBAT, APPLICATION_CURSOR_RESOURCE_COMBAT },
    { PHARAOH_APPLICATION_CURSOR_BOAT_HORIZONTAL, APPLICATION_CURSOR_RESOURCE_BOAT_HORIZONTAL },
    { PHARAOH_APPLICATION_CURSOR_BOAT_RIGHT, APPLICATION_CURSOR_RESOURCE_BOAT_RIGHT },
    { PHARAOH_APPLICATION_CURSOR_BOAT_BRUSH, APPLICATION_CURSOR_RESOURCE_BOAT_BRUSH }
};

_Static_assert(sizeof(cursor_resources) / sizeof(cursor_resources[0]) ==
    PHARAOH_APPLICATION_WINDOW_CURSOR_COUNT, "native cursor resource count");

static int32_t focus_existing_window(
    const PharaohApplicationWindowOperations *operations, void *window)
{
    if (operations->is_iconic(operations->user_data, window) != 0) {
        operations->open_icon(operations->user_data, window);
    }
    operations->set_foreground_window(operations->user_data, window);
    operations->send_message(operations->user_data, window,
        PHARAOH_APPLICATION_WINDOW_MESSAGE_SET_FOCUS, 0, 0);
    operations->send_message(operations->user_data, window,
        PHARAOH_APPLICATION_WINDOW_MESSAGE_SHOW_WINDOW, 1, 0);
    operations->send_message(operations->user_data, window,
        PHARAOH_APPLICATION_WINDOW_MESSAGE_ACTIVATE_APP, 1, 0);
    return 0;
}

static int32_t register_native_window_class(
    const PharaohApplicationWindowRequest *request,
    const PharaohApplicationWindowOperations *operations)
{
    PharaohApplicationWindowClass window_class;
    window_class.style = 0;
    window_class.window_procedure = request->window_procedure;
    window_class.class_extra_bytes = 0;
    window_class.window_extra_bytes = 0;
    window_class.instance = request->instance;
    window_class.icon = operations->load_icon(operations->user_data,
        request->instance, PHARAOH_APPLICATION_WINDOW_CLASS_ICON);
    window_class.cursor = NULL;
    window_class.background_brush = operations->get_stock_object(
        operations->user_data, PHARAOH_APPLICATION_WINDOW_BLACK_BRUSH);
    window_class.menu_name = NULL;
    window_class.class_name = native_window_class_name;
    return operations->register_class(operations->user_data,
        &window_class) != 0;
}

static void load_native_cursors(PharaohApplicationWindowState *state,
    const PharaohApplicationWindowRequest *request,
    const PharaohApplicationWindowOperations *operations)
{
    uint32_t cursor_index;
    for (cursor_index = 0;
         cursor_index < PHARAOH_APPLICATION_WINDOW_CURSOR_COUNT;
         ++cursor_index) {
        const PharaohApplicationCursorResource *const resource =
            &cursor_resources[cursor_index];
        state->cursors[resource->cursor] = operations->load_cursor(
            operations->user_data, request->instance,
            resource->resource_id);
    }
}

int32_t pharaoh_application_window_create(
    PharaohApplicationWindowState *state,
    const PharaohApplicationWindowRequest *request,
    const PharaohApplicationWindowOperations *operations)
{
    void *existing_window = operations->find_window(operations->user_data,
        native_window_class_name, request->window_title);
    if (existing_window != NULL) {
        return focus_existing_window(operations, existing_window);
    }

    if (register_native_window_class(request, operations) == 0) {
        return 0;
    }

    load_native_cursors(state, request, operations);

    int32_t window_height = operations->get_system_metric(
        operations->user_data, PHARAOH_APPLICATION_WINDOW_METRIC_HEIGHT);
    int32_t window_width = operations->get_system_metric(
        operations->user_data, PHARAOH_APPLICATION_WINDOW_METRIC_WIDTH);
    const PharaohApplicationWindowCreateParameters parameters = {
        PHARAOH_APPLICATION_WINDOW_EXTENDED_STYLE,
        native_window_class_name,
        request->window_title,
        PHARAOH_APPLICATION_WINDOW_STYLE,
        0,
        0,
        window_width,
        window_height,
        NULL,
        NULL,
        request->instance,
        NULL
    };
    state->window = operations->create_window(operations->user_data,
        &parameters);
    if (state->window == NULL) {
        return 0;
    }

    (void)operations->update_window(operations->user_data, state->window);
    (void)operations->set_focus(operations->user_data, state->window);
    return 1;
}
