#include "pharaoh/runtime/application_window_startup.h"

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

enum StartupTestEvent {
    STARTUP_TEST_DIRECTORY = 1,
    STARTUP_TEST_METRIC_WIDTH,
    STARTUP_TEST_METRIC_HEIGHT,
    STARTUP_TEST_STATUS,
    STARTUP_TEST_COM_INITIALIZE,
    STARTUP_TEST_FIND_WINDOW,
    STARTUP_TEST_IS_ICONIC,
    STARTUP_TEST_FOREGROUND,
    STARTUP_TEST_SEND_MESSAGE
};

typedef struct StartupTestState {
    uint32_t events[16];
    uint32_t event_count;
    void *existing_window;
    uint32_t message_ids[3];
    uint32_t message_count;
    char current_directory[32];
    char localized_title[32];
    uint32_t allocation_count;
} StartupTestState;

static void record_event(StartupTestState *state, uint32_t event)
{
    CHECK(state->event_count < sizeof(state->events) / sizeof(state->events[0]));
    state->events[state->event_count++] = event;
}

static uint32_t get_current_directory(void *user_data,
    uint32_t buffer_capacity, char *buffer)
{
    StartupTestState *state = (StartupTestState *)user_data;
    const uint32_t bytes = (uint32_t)strlen(state->current_directory) + 1;
    record_event(state, STARTUP_TEST_DIRECTORY);
    CHECK(buffer_capacity == PHARAOH_APPLICATION_DIRECTORY_QUERY_BYTES);
    CHECK(bytes <= buffer_capacity);
    (void)memcpy(buffer, state->current_directory, bytes);
    return bytes - 1;
}

static int32_t get_system_metric(void *user_data, int32_t metric)
{
    StartupTestState *state = (StartupTestState *)user_data;
    if (metric == 0) {
        record_event(state, STARTUP_TEST_METRIC_WIDTH);
        return 1920;
    }
    CHECK(metric == 1);
    record_event(state, STARTUP_TEST_METRIC_HEIGHT);
    return 1080;
}

static void report_status(void *user_data, const char *message,
    int32_t argument_1, int32_t argument_2)
{
    StartupTestState *state = (StartupTestState *)user_data;
    record_event(state, STARTUP_TEST_STATUS);
    CHECK(strcmp(message, "Debug and status report on ") == 0);
    CHECK(argument_1 == 0 && argument_2 == 0);
}

static int32_t co_initialize(void *user_data, void *reserved)
{
    StartupTestState *state = (StartupTestState *)user_data;
    record_event(state, STARTUP_TEST_COM_INITIALIZE);
    CHECK(reserved == NULL);
    return 0;
}

static void *find_window(void *user_data, const char *class_name,
                         const char *window_title)
{
    StartupTestState *state = (StartupTestState *)user_data;
    record_event(state, STARTUP_TEST_FIND_WINDOW);
    CHECK(strcmp(class_name, "WinSJBClass") == 0);
    CHECK(strcmp(window_title, "Main menu") == 0);
    return state->existing_window;
}

static int32_t is_iconic(void *user_data, void *window)
{
    StartupTestState *state = (StartupTestState *)user_data;
    record_event(state, STARTUP_TEST_IS_ICONIC);
    CHECK(window == state->existing_window);
    return 0;
}

static void set_foreground_window(void *user_data, void *window)
{
    StartupTestState *state = (StartupTestState *)user_data;
    record_event(state, STARTUP_TEST_FOREGROUND);
    CHECK(window == state->existing_window);
}

static void send_message(void *user_data, void *window, uint32_t message,
    uintptr_t wparam, intptr_t lparam)
{
    StartupTestState *state = (StartupTestState *)user_data;
    record_event(state, STARTUP_TEST_SEND_MESSAGE);
    CHECK(window == state->existing_window);
    CHECK(state->message_count < 3);
    CHECK(lparam == 0);
    CHECK(wparam == (state->message_count == 0 ? 0u : 1u));
    state->message_ids[state->message_count++] = message;
}

static void *load_icon(void *user_data, void *instance, uint32_t resource_id)
{
    (void)user_data;
    (void)instance;
    (void)resource_id;
    return NULL;
}

static void *get_stock_object(void *user_data, uint32_t object_id)
{
    (void)user_data;
    (void)object_id;
    return NULL;
}

static uint16_t register_class(void *user_data,
    const PharaohApplicationWindowClass *window_class)
{
    (void)user_data;
    (void)window_class;
    return 0;
}

static void *load_cursor(void *user_data, void *instance, uint32_t resource_id)
{
    (void)user_data;
    (void)instance;
    (void)resource_id;
    return NULL;
}

static void *create_window(void *user_data,
    const PharaohApplicationWindowCreateParameters *parameters)
{
    (void)user_data;
    (void)parameters;
    return NULL;
}

static int32_t update_window(void *user_data, void *window)
{
    (void)user_data;
    (void)window;
    return 0;
}

static void *set_focus(void *user_data, void *window)
{
    (void)user_data;
    (void)window;
    return NULL;
}

static PharaohVideoPlayer *allocate_video_player(void *user_data,
    uint32_t native_byte_count)
{
    StartupTestState *state = (StartupTestState *)user_data;
    ++state->allocation_count;
    CHECK(native_byte_count == PHARAOH_APPLICATION_VIDEO_PLAYER_NATIVE_BYTES);
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

static void test_order_and_duplicate_instance_early_return(void)
{
    StartupTestState fake = { 0 };
    PharaohTextCatalog catalog = { 0 };
    PharaohTextSelection selection = { &catalog, NULL };
    PharaohGraphicsStartupState graphics = { 0 };
    PharaohApplicationWindowStartupState startup = { 0 };
    PharaohApplicationWindowStartupRequest request = {
        &fake, 2, &selection, dummy_window_procedure
    };
    PharaohApplicationDirectoryOperations directory_operations = {
        &fake, get_current_directory
    };
    PharaohGraphicsStartupOperations graphics_operations = { 0 };
    PharaohApplicationWindowOperations window_operations = { 0 };
    PharaohApplicationWindowStartupOperations operations = { 0 };
    const char title[] = "Main menu";
    uint32_t index;

    (void)memcpy(fake.current_directory, "C:\\Pharaoh\\",
        sizeof("C:\\Pharaoh\\"));
    fake.existing_window = &fake;
    catalog.primary_text = (void *)title;
    catalog.primary_index[9].text_offset = 0;
    selection.current = "stale title";
    graphics_operations.user_data = &fake;
    graphics_operations.get_system_metric = get_system_metric;
    graphics_operations.report_status = report_status;
    window_operations.user_data = &fake;
    window_operations.find_window = find_window;
    window_operations.is_iconic = is_iconic;
    window_operations.set_foreground_window = set_foreground_window;
    window_operations.send_message = send_message;
    window_operations.load_icon = load_icon;
    window_operations.get_stock_object = get_stock_object;
    window_operations.register_class = register_class;
    window_operations.load_cursor = load_cursor;
    window_operations.get_system_metric = get_system_metric;
    window_operations.create_window = create_window;
    window_operations.update_window = update_window;
    window_operations.set_focus = set_focus;
    operations.user_data = &fake;
    operations.directory_operations = &directory_operations;
    operations.graphics_operations = &graphics_operations;
    operations.window_operations = &window_operations;
    operations.co_initialize = co_initialize;
    operations.allocate_video_player = allocate_video_player;

    memset(startup.reset_state.pre_window_flags, 0xff,
        sizeof(startup.reset_state.pre_window_flags));
    startup.reset_state.pre_window_status = UINT8_C(0xff);
    for (index = 0; index < PHARAOH_APPLICATION_WINDOW_RESET_WORD_COUNT;
         ++index) {
        startup.reset_state.pre_window_words[index] = -1;
    }

    CHECK(pharaoh_application_window_initialize(&startup, &graphics,
        &request, &operations) == 0);
    CHECK(strcmp(startup.directory.path, fake.current_directory) == 0);
    CHECK(startup.directory.runtime_errno == 0);
    CHECK(startup.random_generator.seed_1 == UINT32_C(0x54657687));
    CHECK(startup.random_generator.seed_2 == UINT32_C(0x72641663));
    CHECK(startup.random_generator.seed_3 == UINT32_C(0x34518632));
    for (index = 0; index < PHARAOH_APPLICATION_WINDOW_RESET_FLAG_COUNT;
         ++index) {
        CHECK(startup.reset_state.pre_window_flags[index] == 0);
    }
    CHECK(startup.reset_state.pre_window_status == 0);
    for (index = 0; index < PHARAOH_APPLICATION_WINDOW_RESET_WORD_COUNT;
         ++index) {
        CHECK(startup.reset_state.pre_window_words[index] == 0);
    }
    CHECK(selection.current == title);
    CHECK(startup.geometry.window_width == 800);
    CHECK(startup.geometry.window_height == 600);
    CHECK(graphics.screen_width == 800 && graphics.screen_height == 600);
    CHECK(graphics.desktop_width == 1920 && graphics.desktop_height == 1080);
    CHECK(graphics.mode_status_byte == 1);
    CHECK(fake.message_count == 3);
    CHECK(fake.message_ids[0] == PHARAOH_APPLICATION_WINDOW_MESSAGE_SET_FOCUS);
    CHECK(fake.message_ids[1] == PHARAOH_APPLICATION_WINDOW_MESSAGE_SHOW_WINDOW);
    CHECK(fake.message_ids[2] == PHARAOH_APPLICATION_WINDOW_MESSAGE_ACTIVATE_APP);
    CHECK(fake.allocation_count == 0);
    CHECK(fake.events[0] == STARTUP_TEST_DIRECTORY);
    CHECK(fake.events[1] == STARTUP_TEST_METRIC_WIDTH);
    CHECK(fake.events[2] == STARTUP_TEST_METRIC_HEIGHT);
    CHECK(fake.events[3] == STARTUP_TEST_STATUS);
    CHECK(fake.events[4] == STARTUP_TEST_COM_INITIALIZE);
    CHECK(fake.events[5] == STARTUP_TEST_FIND_WINDOW);
    CHECK(fake.events[6] == STARTUP_TEST_IS_ICONIC);
    CHECK(fake.events[7] == STARTUP_TEST_FOREGROUND);
}

int main(void)
{
    test_order_and_duplicate_instance_early_return();
    return 0;
}
