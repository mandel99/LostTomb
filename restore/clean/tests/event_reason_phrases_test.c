#include "pharaoh/runtime/event_reason_phrases.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(condition) do { if (!(condition)) abort(); } while (0)

typedef struct FakeReasonPhraseRuntime {
    char file_text[PHARAOH_EVENT_REASON_TEXT_BUFFER_BYTES];
    int32_t load_result;
    int32_t prompt_result;
    uint32_t prompt_calls;
    uint32_t report_calls;
    const char *last_message;
    const char *last_title;
    const char *last_status;
    void *expected_owner;
} FakeReasonPhraseRuntime;

static int32_t fake_load_text_file(void *user_data, const char *filename,
    char *destination, uint32_t capacity, int32_t mode)
{
    FakeReasonPhraseRuntime *runtime = user_data;
    CHECK(strcmp(filename, "auto reason phrases.txt") == 0);
    CHECK(capacity == PHARAOH_EVENT_REASON_TEXT_BUFFER_BYTES);
    CHECK(mode == 0);
    if (runtime->load_result != 0) {
        memcpy(destination, runtime->file_text, strlen(runtime->file_text) + 1);
    }
    return runtime->load_result;
}

static int32_t fake_text_file_size(void *user_data, const char *filename)
{
    FakeReasonPhraseRuntime *runtime = user_data;
    CHECK(strcmp(filename, "auto reason phrases.txt") == 0);
    return (int32_t)strlen(runtime->file_text);
}

static int32_t fake_show_yes_no(void *user_data, void *owner_window,
    const char *message, const char *title)
{
    FakeReasonPhraseRuntime *runtime = user_data;
    CHECK(owner_window == runtime->expected_owner);
    ++runtime->prompt_calls;
    runtime->last_message = message;
    runtime->last_title = title;
    return runtime->prompt_result;
}

static void fake_report_status(void *user_data, const char *message)
{
    FakeReasonPhraseRuntime *runtime = user_data;
    ++runtime->report_calls;
    runtime->last_status = message;
}

static void make_complete_table_file(FakeReasonPhraseRuntime *runtime)
{
    size_t used = 0;
    for (uint32_t row = 0; row < PHARAOH_EVENT_REASON_PHRASE_ROWS; ++row) {
        int written = snprintf(runtime->file_text + used,
            sizeof(runtime->file_text) - used, "{");
        CHECK(written > 0);
        used += (size_t)written;
        for (uint32_t column = 0;
             column < PHARAOH_EVENT_REASON_PHRASE_COLUMNS; ++column) {
            if (row == 0 && column == 0) {
                written = snprintf(runtime->file_text + used,
                    sizeof(runtime->file_text) - used, "-1,");
            } else {
                written = snprintf(runtime->file_text + used,
                    sizeof(runtime->file_text) - used, "%u,", column);
            }
            CHECK(written > 0);
            used += (size_t)written;
        }
        written = snprintf(runtime->file_text + used,
            sizeof(runtime->file_text) - used, "}\n");
        CHECK(written > 0);
        used += (size_t)written;
    }
}

static PharaohEventReasonPhraseOperations make_operations(
    FakeReasonPhraseRuntime *runtime)
{
    const PharaohEventReasonPhraseOperations operations = {
        runtime,
        fake_load_text_file,
        fake_text_file_size,
        fake_show_yes_no,
        fake_report_status
    };
    return operations;
}

static void test_loads_all_native_table_cells(void)
{
    FakeReasonPhraseRuntime runtime = {0};
    runtime.load_result = 1;
    make_complete_table_file(&runtime);
    PharaohEventReasonPhraseOperations operations = make_operations(&runtime);
    char scratch[PHARAOH_EVENT_REASON_TEXT_BUFFER_BYTES];
    PharaohEventReasonPhraseTable table = {0};
    char owner_token;
    PharaohEventReasonPhraseContext context = {
        scratch, PHARAOH_EVENT_REASON_TEXT_BUFFER_BYTES, &owner_token,
        &table, &operations
    };
    runtime.expected_owner = context.owner_window;

    CHECK(pharaoh_event_reason_phrases_load(&context) == 1);
    CHECK(runtime.prompt_calls == 0);
    CHECK(runtime.report_calls == 1);
    CHECK(strcmp(runtime.last_status, " OK: reason table loaded") == 0);
    CHECK(table.value[0][0] == 255);
    CHECK(table.value[0][35] == 35);
    CHECK(table.value[53][0] == 0);
    CHECK(table.value[53][35] == 35);
}

static void test_declined_invalid_row_count(void)
{
    FakeReasonPhraseRuntime runtime = {0};
    runtime.load_result = 1;
    runtime.prompt_result = 7;
    memcpy(runtime.file_text, "{1,}\n", sizeof("{1,}\n"));
    PharaohEventReasonPhraseOperations operations = make_operations(&runtime);
    char scratch[PHARAOH_EVENT_REASON_TEXT_BUFFER_BYTES];
    PharaohEventReasonPhraseTable table = {0};
    char owner_token;
    PharaohEventReasonPhraseContext context = {
        scratch, PHARAOH_EVENT_REASON_TEXT_BUFFER_BYTES, &owner_token,
        &table, &operations
    };
    runtime.expected_owner = context.owner_window;

    CHECK(pharaoh_event_reason_phrases_load(&context) == 0);
    CHECK(runtime.prompt_calls == 1);
    CHECK(strcmp(runtime.last_title, "reason phrase data invalid") == 0);
    CHECK(strstr(runtime.last_message,
        "ERR: reason file has too many lines, expected 54") != NULL);
    CHECK(runtime.report_calls == 1);
}

static void test_missing_file_reports_native_status(void)
{
    FakeReasonPhraseRuntime runtime = {0};
    PharaohEventReasonPhraseOperations operations = make_operations(&runtime);
    char scratch[PHARAOH_EVENT_REASON_TEXT_BUFFER_BYTES];
    PharaohEventReasonPhraseTable table = {0};
    PharaohEventReasonPhraseContext context = {
        scratch, PHARAOH_EVENT_REASON_TEXT_BUFFER_BYTES, NULL, &table,
        &operations
    };

    CHECK(pharaoh_event_reason_phrases_load(&context) == 0);
    CHECK(runtime.prompt_calls == 0);
    CHECK(strcmp(runtime.last_status,
        "ERR:No auto reason phrases file") == 0);
}

static void test_missing_spare_buffer_reports_native_status(void)
{
    FakeReasonPhraseRuntime runtime = {0};
    PharaohEventReasonPhraseOperations operations = make_operations(&runtime);
    PharaohEventReasonPhraseTable table = {0};
    PharaohEventReasonPhraseContext context = {
        NULL, PHARAOH_EVENT_REASON_TEXT_BUFFER_BYTES, NULL, &table,
        &operations
    };

    CHECK(pharaoh_event_reason_phrases_load(&context) == 0);
    CHECK(runtime.report_calls == 1);
    CHECK(strcmp(runtime.last_status,
        "ERR:no spare screen for reason table") == 0);
}

int main(void)
{
    test_loads_all_native_table_cells();
    test_declined_invalid_row_count();
    test_missing_file_reports_native_status();
    test_missing_spare_buffer_reports_native_status();
    return 0;
}
