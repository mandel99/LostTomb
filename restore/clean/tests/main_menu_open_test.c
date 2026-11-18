#include "pharaoh/ui/main_menu_open.h"

#include <stdlib.h>
#include <string.h>

#define CHECK(condition) do { if (!(condition)) abort(); } while (0)

typedef struct FakeMainMenuFileSystem {
    uint8_t file_token;
    int32_t open_succeeds;
    int32_t matching_file_count;
    uint32_t open_calls;
    uint32_t close_calls;
    uint32_t list_calls;
    uint32_t sequence;
    uint32_t open_order;
    uint32_t close_order;
    uint32_t list_order;
    const char *expected_filename;
    const char *expected_pattern;
} FakeMainMenuFileSystem;

static PharaohReadOnlyFile *fake_open_file(void *user_data,
    const char *filename, const char *mode)
{
    FakeMainMenuFileSystem *fs = user_data;
    CHECK(strcmp(filename, fs->expected_filename) == 0);
    CHECK(strcmp(mode, "rb") == 0);
    ++fs->open_calls;
    fs->open_order = ++fs->sequence;
    return fs->open_succeeds != 0
        ? (PharaohReadOnlyFile *)&fs->file_token : NULL;
}

static int32_t fake_close_file(void *user_data, PharaohReadOnlyFile *file)
{
    FakeMainMenuFileSystem *fs = user_data;
    CHECK(file == (PharaohReadOnlyFile *)&fs->file_token);
    ++fs->close_calls;
    fs->close_order = ++fs->sequence;
    return -1; /* The native caller ignores fclose's result. */
}

static int32_t fake_count_matching_files(void *user_data, const char *pattern,
    int32_t search_mode)
{
    FakeMainMenuFileSystem *fs = user_data;
    CHECK(strcmp(pattern, fs->expected_pattern) == 0);
    CHECK(search_mode == 0);
    ++fs->list_calls;
    fs->list_order = ++fs->sequence;
    return fs->matching_file_count;
}

static void test_opens_menu_with_save_and_campaign_files(void)
{
    FakeMainMenuFileSystem fs = {0};
    fs.open_succeeds = 1;
    fs.matching_file_count = 2;
    fs.expected_filename = "selected.sav";
    fs.expected_pattern = "save\\user\\*.sav";
    const PharaohMainMenuOpenOperations operations = {
        &fs, fake_open_file, fake_close_file, fake_count_matching_files
    };
    const PharaohCampaignIndex campaign_index = {0};
    PharaohMainMenuWindowState state = {1, 0, 1, 0, 0, 0};
    const PharaohMainMenuOpenContext context = {
        &campaign_index, &state, fs.expected_filename, "user", &operations
    };

    pharaoh_main_menu_window_show(&context);

    CHECK(state.menu_active == 1);
    CHECK(state.campaign_period_open == 1);
    CHECK(state.selected_save_available == 1);
    CHECK(state.save_directory_has_files == 1);
    CHECK(state.active_window == 0x30);
    CHECK(state.redraw_requested == 1);
    CHECK(fs.open_calls == 1 && fs.close_calls == 1 && fs.list_calls == 1);
    CHECK(fs.open_order < fs.close_order && fs.close_order < fs.list_order);
}

static void test_missing_selected_save_and_empty_directory(void)
{
    FakeMainMenuFileSystem fs = {0};
    fs.expected_filename = "missing.sav";
    fs.expected_pattern = "save\\user\\*.sav";
    const PharaohMainMenuOpenOperations operations = {
        &fs, fake_open_file, fake_close_file, fake_count_matching_files
    };
    const PharaohCampaignIndex campaign_index = {0};
    PharaohMainMenuWindowState state = {0};
    const PharaohMainMenuOpenContext context = {
        &campaign_index, &state, fs.expected_filename, "user", &operations
    };

    pharaoh_main_menu_window_show(&context);

    CHECK(state.menu_active == 1);
    CHECK(state.selected_save_available == 0);
    CHECK(state.save_directory_has_files == 0);
    CHECK(state.active_window == 0x30);
    CHECK(state.redraw_requested == 1);
    CHECK(fs.open_calls == 1 && fs.close_calls == 0 && fs.list_calls == 1);
}

static void test_empty_selection_skips_file_open(void)
{
    FakeMainMenuFileSystem fs = {0};
    fs.expected_filename = "";
    fs.expected_pattern = "save\\user\\*.sav";
    const PharaohMainMenuOpenOperations operations = {
        &fs, fake_open_file, fake_close_file, fake_count_matching_files
    };
    const PharaohCampaignIndex campaign_index = {0};
    PharaohMainMenuWindowState state = {0};
    const PharaohMainMenuOpenContext context = {
        &campaign_index, &state, fs.expected_filename, "user", &operations
    };

    pharaoh_main_menu_window_show(&context);

    CHECK(fs.open_calls == 0 && fs.close_calls == 0 && fs.list_calls == 1);
    CHECK(state.selected_save_available == 0);
}

int main(void)
{
    test_opens_menu_with_save_and_campaign_files();
    test_missing_selected_save_and_empty_directory();
    test_empty_selection_skips_file_open();
    return 0;
}
