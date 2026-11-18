#include "pharaoh/runtime/application_window.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void fail_check(const char *expression, int line)
{
    (void)fprintf(stderr, "check failed on line %d: %s\n", line, expression);
    exit(1);
}

#define CHECK(expression) \
    do { \
        if (!(expression)) { \
            fail_check(#expression, __LINE__); \
        } \
    } while (0)

enum WindowTestCall {
    WINDOW_TEST_FIND = 1,
    WINDOW_TEST_ICONIC,
    WINDOW_TEST_OPEN_ICON,
    WINDOW_TEST_FOREGROUND,
    WINDOW_TEST_SEND,
    WINDOW_TEST_LOAD_ICON,
    WINDOW_TEST_STOCK_OBJECT,
    WINDOW_TEST_REGISTER,
    WINDOW_TEST_LOAD_CURSOR,
    WINDOW_TEST_METRIC,
    WINDOW_TEST_CREATE,
    WINDOW_TEST_UPDATE,
    WINDOW_TEST_FOCUS
};

typedef struct WindowTestState {
    void *existing_window;
    void *created_window;
    int32_t register_result;
    uint32_t calls[64];
    uint32_t call_count;
    uint32_t cursor_ids[PHARAOH_APPLICATION_WINDOW_CURSOR_COUNT];
    int32_t metric_ids[2];
    uint32_t cursor_count;
    uint32_t metric_count;
    uint32_t sent_messages[3];
    uint32_t sent_message_count;
    void *instance_token;
    void *icon_token;
    void *brush_token;
} WindowTestState;

static const uint32_t expected_cursor_ids[
    PHARAOH_APPLICATION_WINDOW_CURSOR_COUNT] = {
    0x66, 0x88, 0x82, 0x93, 0x90, 0x91, 0x92, 0x8e, 0x96, 0x95,
    0x94, 0x99, 0x98, 0x9b, 0x9a, 0x9c, 0x9d, 0x9f, 0xa0
};

static void record_call(WindowTestState *state, uint32_t call)
{
    CHECK(state->call_count < sizeof(state->calls) / sizeof(state->calls[0]));
    state->calls[state->call_count++] = call;
}

static void *find_window(void *user_data, const char *class_name,
                         const char *window_title)
{
    WindowTestState *state = (WindowTestState *)user_data;
    record_call(state, WINDOW_TEST_FIND);
    CHECK(strcmp(class_name, "WinSJBClass") == 0);
    CHECK(strcmp(window_title, "localized title") == 0);
    return state->existing_window;
}

static int32_t is_iconic(void *user_data, void *window)
{
    WindowTestState *state = (WindowTestState *)user_data;
    record_call(state, WINDOW_TEST_ICONIC);
    CHECK(window == state->existing_window);
    return 1;
}

static void open_icon(void *user_data, void *window)
{
    WindowTestState *state = (WindowTestState *)user_data;
    record_call(state, WINDOW_TEST_OPEN_ICON);
    CHECK(window == state->existing_window);
}

static void set_foreground_window(void *user_data, void *window)
{
    WindowTestState *state = (WindowTestState *)user_data;
    record_call(state, WINDOW_TEST_FOREGROUND);
    CHECK(window == state->existing_window);
}

static void send_message(void *user_data, void *window, uint32_t message,
                         uintptr_t wparam, intptr_t lparam)
{
    WindowTestState *state = (WindowTestState *)user_data;
    record_call(state, WINDOW_TEST_SEND);
    CHECK(window == state->existing_window);
    CHECK(wparam == (message == PHARAOH_APPLICATION_WINDOW_MESSAGE_SET_FOCUS ?
        0u : 1u));
    CHECK(lparam == 0);
    CHECK(state->sent_message_count < 3);
    state->sent_messages[state->sent_message_count++] = message;
}

static void *load_icon(void *user_data, void *instance, uint32_t resource_id)
{
    WindowTestState *state = (WindowTestState *)user_data;
    record_call(state, WINDOW_TEST_LOAD_ICON);
    CHECK(instance == &state->instance_token);
    CHECK(resource_id == PHARAOH_APPLICATION_WINDOW_CLASS_ICON);
    return &state->icon_token;
}

static void *get_stock_object(void *user_data, uint32_t object_id)
{
    WindowTestState *state = (WindowTestState *)user_data;
    record_call(state, WINDOW_TEST_STOCK_OBJECT);
    CHECK(object_id == PHARAOH_APPLICATION_WINDOW_BLACK_BRUSH);
    return &state->brush_token;
}

static uint16_t register_class(void *user_data,
    const PharaohApplicationWindowClass *window_class)
{
    WindowTestState *state = (WindowTestState *)user_data;
    record_call(state, WINDOW_TEST_REGISTER);
    CHECK(window_class->style == 0);
    CHECK(window_class->window_procedure != NULL);
    CHECK(window_class->class_extra_bytes == 0);
    CHECK(window_class->window_extra_bytes == 0);
    CHECK(window_class->instance == &state->instance_token);
    CHECK(window_class->icon == &state->icon_token);
    CHECK(window_class->cursor == NULL);
    CHECK(window_class->background_brush == &state->brush_token);
    CHECK(window_class->menu_name == NULL);
    CHECK(strcmp(window_class->class_name, "WinSJBClass") == 0);
    return (uint16_t)state->register_result;
}

static void *load_cursor(void *user_data, void *instance, uint32_t resource_id)
{
    WindowTestState *state = (WindowTestState *)user_data;
    const uint32_t index = state->cursor_count++;
    record_call(state, WINDOW_TEST_LOAD_CURSOR);
    CHECK(instance == &state->instance_token);
    CHECK(index < PHARAOH_APPLICATION_WINDOW_CURSOR_COUNT);
    CHECK(resource_id == expected_cursor_ids[index]);
    state->cursor_ids[index] = resource_id;
    return (void *)(uintptr_t)(index + 1);
}

static int32_t get_system_metric(void *user_data, int32_t metric)
{
    WindowTestState *state = (WindowTestState *)user_data;
    const uint32_t index = state->metric_count++;
    record_call(state, WINDOW_TEST_METRIC);
    CHECK(index < 2);
    state->metric_ids[index] = metric;
    return metric == PHARAOH_APPLICATION_WINDOW_METRIC_WIDTH ? 1024 : 768;
}

static void *create_window(void *user_data,
    const PharaohApplicationWindowCreateParameters *parameters)
{
    WindowTestState *state = (WindowTestState *)user_data;
    record_call(state, WINDOW_TEST_CREATE);
    CHECK(parameters->extended_style ==
        PHARAOH_APPLICATION_WINDOW_EXTENDED_STYLE);
    CHECK(strcmp(parameters->class_name, "WinSJBClass") == 0);
    CHECK(strcmp(parameters->window_title, "localized title") == 0);
    CHECK(parameters->style == PHARAOH_APPLICATION_WINDOW_STYLE);
    CHECK(parameters->x == 0 && parameters->y == 0);
    CHECK(parameters->width == 1024 && parameters->height == 768);
    CHECK(parameters->parent_window == NULL && parameters->menu == NULL);
    CHECK(parameters->instance == &state->instance_token);
    CHECK(parameters->creation_parameter == NULL);
    return state->created_window;
}

static int32_t update_window(void *user_data, void *window)
{
    WindowTestState *state = (WindowTestState *)user_data;
    record_call(state, WINDOW_TEST_UPDATE);
    CHECK(window == state->created_window);
    return 1;
}

static void *set_focus(void *user_data, void *window)
{
    WindowTestState *state = (WindowTestState *)user_data;
    record_call(state, WINDOW_TEST_FOCUS);
    CHECK(window == state->created_window);
    return NULL;
}

static intptr_t PHARAOH_APPLICATION_WINDOW_CALLBACK dummy_window_procedure(
    void *window, uint32_t message, uintptr_t wparam, intptr_t lparam)
{
    (void)window;
    (void)message;
    (void)wparam;
    (void)lparam;
    return 0;
}

static const PharaohApplicationWindowOperations test_operations = {
    NULL,
    find_window,
    is_iconic,
    open_icon,
    set_foreground_window,
    send_message,
    load_icon,
    get_stock_object,
    register_class,
    load_cursor,
    get_system_metric,
    create_window,
    update_window,
    set_focus
};

static void test_creates_fullscreen_window_and_loads_cursors(void)
{
    WindowTestState fake = { 0 };
    PharaohApplicationWindowOperations operations = test_operations;
    PharaohApplicationWindowRequest request;
    PharaohApplicationWindowState state = { 0 };

    fake.register_result = 1;
    fake.created_window = &fake;
    operations.user_data = &fake;
    request.instance = &fake.instance_token;
    request.window_title = "localized title";
    request.window_procedure = dummy_window_procedure;

    CHECK(pharaoh_application_window_create(&state, &request, &operations) == 1);
    CHECK(state.window == fake.created_window);
    CHECK(fake.cursor_count == PHARAOH_APPLICATION_WINDOW_CURSOR_COUNT);
    CHECK(fake.metric_count == 2);
    CHECK(fake.metric_ids[0] == PHARAOH_APPLICATION_WINDOW_METRIC_HEIGHT);
    CHECK(fake.metric_ids[1] == PHARAOH_APPLICATION_WINDOW_METRIC_WIDTH);
    CHECK(fake.calls[0] == WINDOW_TEST_FIND);
    CHECK(fake.calls[1] == WINDOW_TEST_LOAD_ICON);
    CHECK(fake.calls[2] == WINDOW_TEST_STOCK_OBJECT);
    CHECK(fake.calls[3] == WINDOW_TEST_REGISTER);
    CHECK(fake.calls[4] == WINDOW_TEST_LOAD_CURSOR);
    CHECK(fake.calls[23] == WINDOW_TEST_METRIC);
    CHECK(fake.calls[24] == WINDOW_TEST_METRIC);
    CHECK(fake.calls[25] == WINDOW_TEST_CREATE);
    CHECK(fake.calls[26] == WINDOW_TEST_UPDATE);
    CHECK(fake.calls[27] == WINDOW_TEST_FOCUS);
}

static void test_existing_window_gets_native_focus_messages(void)
{
    WindowTestState fake = { 0 };
    PharaohApplicationWindowOperations operations = test_operations;
    PharaohApplicationWindowRequest request;
    PharaohApplicationWindowState state = { 0 };

    fake.existing_window = &fake;
    operations.user_data = &fake;
    request.instance = &fake.instance_token;
    request.window_title = "localized title";
    request.window_procedure = dummy_window_procedure;

    CHECK(pharaoh_application_window_create(&state, &request, &operations) == 0);
    CHECK(fake.call_count == 7);
    CHECK(fake.calls[0] == WINDOW_TEST_FIND);
    CHECK(fake.calls[1] == WINDOW_TEST_ICONIC);
    CHECK(fake.calls[2] == WINDOW_TEST_OPEN_ICON);
    CHECK(fake.calls[3] == WINDOW_TEST_FOREGROUND);
    CHECK(fake.sent_message_count == 3);
    CHECK(fake.sent_messages[0] == PHARAOH_APPLICATION_WINDOW_MESSAGE_SET_FOCUS);
    CHECK(fake.sent_messages[1] == PHARAOH_APPLICATION_WINDOW_MESSAGE_SHOW_WINDOW);
    CHECK(fake.sent_messages[2] == PHARAOH_APPLICATION_WINDOW_MESSAGE_ACTIVATE_APP);
    CHECK(fake.cursor_count == 0);
    CHECK(fake.register_result == 0);
    CHECK(state.window == NULL);
}

static void test_registration_failure_stops_before_cursor_loads(void)
{
    WindowTestState fake = { 0 };
    PharaohApplicationWindowOperations operations = test_operations;
    PharaohApplicationWindowRequest request;
    PharaohApplicationWindowState state = { 0 };

    operations.user_data = &fake;
    state.window = &fake;
    request.instance = &fake.instance_token;
    request.window_title = "localized title";
    request.window_procedure = dummy_window_procedure;

    CHECK(pharaoh_application_window_create(&state, &request, &operations) == 0);
    CHECK(state.window == &fake);
    CHECK(fake.call_count == 4);
    CHECK(fake.calls[3] == WINDOW_TEST_REGISTER);
    CHECK(fake.cursor_count == 0);
    CHECK(fake.metric_count == 0);
}

static void test_create_failure_stores_null_window(void)
{
    WindowTestState fake = { 0 };
    PharaohApplicationWindowOperations operations = test_operations;
    PharaohApplicationWindowRequest request;
    PharaohApplicationWindowState state = { 0 };

    fake.register_result = 1;
    operations.user_data = &fake;
    state.window = &fake;
    request.instance = &fake.instance_token;
    request.window_title = "localized title";
    request.window_procedure = dummy_window_procedure;

    CHECK(pharaoh_application_window_create(&state, &request, &operations) == 0);
    CHECK(state.window == NULL);
    CHECK(fake.cursor_count == PHARAOH_APPLICATION_WINDOW_CURSOR_COUNT);
    CHECK(fake.metric_count == 2);
    CHECK(fake.calls[fake.call_count - 1] == WINDOW_TEST_CREATE);
}

int main(void)
{
    test_creates_fullscreen_window_and_loads_cursors();
    test_existing_window_gets_native_focus_messages();
    test_registration_failure_stops_before_cursor_loads();
    test_create_failure_stores_null_window();
    return 0;
}
