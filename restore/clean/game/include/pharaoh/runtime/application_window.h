#ifndef PHARAOH_RUNTIME_APPLICATION_WINDOW_H
#define PHARAOH_RUNTIME_APPLICATION_WINDOW_H

#include <stdint.h>

enum {
    PHARAOH_APPLICATION_WINDOW_CLASS_ICON = 141,
    PHARAOH_APPLICATION_WINDOW_BLACK_BRUSH = 4,
    PHARAOH_APPLICATION_WINDOW_METRIC_WIDTH = 0,
    PHARAOH_APPLICATION_WINDOW_METRIC_HEIGHT = 1,
    PHARAOH_APPLICATION_WINDOW_CURSOR_COUNT = 19,
    PHARAOH_APPLICATION_WINDOW_MESSAGE_SET_FOCUS = 0x0007,
    PHARAOH_APPLICATION_WINDOW_MESSAGE_SHOW_WINDOW = 0x0018,
    PHARAOH_APPLICATION_WINDOW_MESSAGE_ACTIVATE_APP = 0x001c
};

#define PHARAOH_APPLICATION_WINDOW_STYLE UINT32_C(0x90080000)
#define PHARAOH_APPLICATION_WINDOW_EXTENDED_STYLE UINT32_C(0x00040000)

#if defined(_MSC_VER)
#define PHARAOH_APPLICATION_WINDOW_CALLBACK __stdcall
#elif defined(__i386__) && defined(__GNUC__)
#define PHARAOH_APPLICATION_WINDOW_CALLBACK __attribute__((stdcall))
#else
#define PHARAOH_APPLICATION_WINDOW_CALLBACK
#endif

typedef intptr_t (PHARAOH_APPLICATION_WINDOW_CALLBACK *
    PharaohApplicationWindowProcedure)(void *window, uint32_t message,
                                       uintptr_t wparam, intptr_t lparam);

typedef enum PharaohApplicationCursor {
    PHARAOH_APPLICATION_CURSOR_DEFAULT,
    PHARAOH_APPLICATION_CURSOR_SWORD,
    PHARAOH_APPLICATION_CURSOR_CLEAR_LAND,
    PHARAOH_APPLICATION_CURSOR_BOAT_DOWN_B,
    PHARAOH_APPLICATION_CURSOR_BOAT_FIGURE_A,
    PHARAOH_APPLICATION_CURSOR_BOAT_FIGURE_B,
    PHARAOH_APPLICATION_CURSOR_BOAT_DOWN_A,
    PHARAOH_APPLICATION_CURSOR_BOAT,
    PHARAOH_APPLICATION_CURSOR_PERSON_DIRECTION,
    PHARAOH_APPLICATION_CURSOR_CART,
    PHARAOH_APPLICATION_CURSOR_BOW,
    PHARAOH_APPLICATION_CURSOR_PARTICLES_B,
    PHARAOH_APPLICATION_CURSOR_PARTICLES_A,
    PHARAOH_APPLICATION_CURSOR_PARTICLES_D,
    PHARAOH_APPLICATION_CURSOR_PARTICLES_C,
    PHARAOH_APPLICATION_CURSOR_COMBAT,
    PHARAOH_APPLICATION_CURSOR_BOAT_HORIZONTAL,
    PHARAOH_APPLICATION_CURSOR_BOAT_RIGHT,
    PHARAOH_APPLICATION_CURSOR_BOAT_BRUSH
} PharaohApplicationCursor;

typedef struct PharaohApplicationWindowClass {
    uint32_t style;
    PharaohApplicationWindowProcedure window_procedure;
    int32_t class_extra_bytes;
    int32_t window_extra_bytes;
    void *instance;
    void *icon;
    void *cursor;
    void *background_brush;
    const char *menu_name;
    const char *class_name;
} PharaohApplicationWindowClass;

typedef struct PharaohApplicationWindowRequest {
    void *instance;
    const char *window_title;
    PharaohApplicationWindowProcedure window_procedure;
} PharaohApplicationWindowRequest;

typedef struct PharaohApplicationWindowCreateParameters {
    uint32_t extended_style;
    const char *class_name;
    const char *window_title;
    uint32_t style;
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
    void *parent_window;
    void *menu;
    void *instance;
    void *creation_parameter;
} PharaohApplicationWindowCreateParameters;

typedef struct PharaohApplicationWindowState {
    void *window;
    void *cursors[PHARAOH_APPLICATION_WINDOW_CURSOR_COUNT];
    void *current_cursor; /* Cached selected handle from native 0x004CF460. */
} PharaohApplicationWindowState;

typedef struct PharaohApplicationWindowOperations {
    void *user_data;
    void *(*find_window)(void *user_data, const char *class_name,
                         const char *window_title);
    int32_t (*is_iconic)(void *user_data, void *window);
    void (*open_icon)(void *user_data, void *window);
    void (*set_foreground_window)(void *user_data, void *window);
    void (*send_message)(void *user_data, void *window, uint32_t message,
                         uintptr_t wparam, intptr_t lparam);
    void *(*load_icon)(void *user_data, void *instance, uint32_t resource_id);
    void *(*get_stock_object)(void *user_data, uint32_t object_id);
    uint16_t (*register_class)(void *user_data,
                              const PharaohApplicationWindowClass *window_class);
    void *(*load_cursor)(void *user_data, void *instance, uint32_t resource_id);
    int32_t (*get_system_metric)(void *user_data, int32_t metric);
    void *(*create_window)(
        void *user_data,
        const PharaohApplicationWindowCreateParameters *parameters);
    int32_t (*update_window)(void *user_data, void *window);
    void *(*set_focus)(void *user_data, void *window);
} PharaohApplicationWindowOperations;

/* Exact Win32 window-creation sub-block inside native 0x004CF960. Duplicate
   instance, registration-failure and creation-failure paths all return zero. */
int32_t pharaoh_application_window_create(
    PharaohApplicationWindowState *state,
    const PharaohApplicationWindowRequest *request,
    const PharaohApplicationWindowOperations *operations);

#endif
