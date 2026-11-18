#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string.h>

#include "pharaoh/platform/windows_application_window.h"

static void *find_window(void *user_data, const char *class_name,
    const char *window_title)
{
    (void)user_data;
    return FindWindowA(class_name, window_title);
}

static int32_t is_iconic(void *user_data, void *window)
{
    (void)user_data;
    return IsIconic((HWND)window) != 0;
}

static void open_icon(void *user_data, void *window)
{
    (void)user_data;
    (void)OpenIcon((HWND)window);
}

static void set_foreground_window(void *user_data, void *window)
{
    (void)user_data;
    (void)SetForegroundWindow((HWND)window);
}

static void send_message(void *user_data, void *window, uint32_t message,
    uintptr_t wparam, intptr_t lparam)
{
    (void)user_data;
    (void)SendMessageA((HWND)window, message, (WPARAM)wparam,
        (LPARAM)lparam);
}

static void *load_icon(void *user_data, void *instance, uint32_t resource_id)
{
    (void)user_data;
    return LoadIconA((HINSTANCE)instance,
        MAKEINTRESOURCEA((WORD)resource_id));
}

static void *get_stock_object(void *user_data, uint32_t object_id)
{
    (void)user_data;
    return GetStockObject((int)object_id);
}

static uint16_t register_class(void *user_data,
    const PharaohApplicationWindowClass *description)
{
    WNDCLASSA window_class;
    (void)user_data;

    window_class.style = description->style;
    window_class.lpfnWndProc = (WNDPROC)description->window_procedure;
    window_class.cbClsExtra = description->class_extra_bytes;
    window_class.cbWndExtra = description->window_extra_bytes;
    window_class.hInstance = (HINSTANCE)description->instance;
    window_class.hIcon = (HICON)description->icon;
    window_class.hCursor = (HCURSOR)description->cursor;
    window_class.hbrBackground = (HBRUSH)description->background_brush;
    window_class.lpszMenuName = description->menu_name;
    window_class.lpszClassName = description->class_name;
    return (uint16_t)RegisterClassA(&window_class);
}

static void *load_cursor(void *user_data, void *instance,
    uint32_t resource_id)
{
    (void)user_data;
    return LoadCursorA((HINSTANCE)instance,
        MAKEINTRESOURCEA((WORD)resource_id));
}

static int32_t get_system_metric(void *user_data, int32_t metric)
{
    (void)user_data;
    return GetSystemMetrics(metric);
}

static void *create_window(void *user_data,
    const PharaohApplicationWindowCreateParameters *parameters)
{
    (void)user_data;
    return CreateWindowExA(parameters->extended_style,
        parameters->class_name, parameters->window_title, parameters->style,
        parameters->x, parameters->y, parameters->width,
        parameters->height, (HWND)parameters->parent_window,
        (HMENU)parameters->menu, (HINSTANCE)parameters->instance,
        parameters->creation_parameter);
}

static int32_t update_window(void *user_data, void *window)
{
    (void)user_data;
    return UpdateWindow((HWND)window) != 0;
}

static void *set_focus(void *user_data, void *window)
{
    (void)user_data;
    return SetFocus((HWND)window);
}

void pharaoh_windows_application_window_operations_initialize(
    PharaohApplicationWindowOperations *operations)
{
    memset(operations, 0, sizeof(*operations));
    operations->find_window = find_window;
    operations->is_iconic = is_iconic;
    operations->open_icon = open_icon;
    operations->set_foreground_window = set_foreground_window;
    operations->send_message = send_message;
    operations->load_icon = load_icon;
    operations->get_stock_object = get_stock_object;
    operations->register_class = register_class;
    operations->load_cursor = load_cursor;
    operations->get_system_metric = get_system_metric;
    operations->create_window = create_window;
    operations->update_window = update_window;
    operations->set_focus = set_focus;
}
