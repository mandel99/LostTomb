#include "pharaoh/runtime/startup_asset_loading.h"

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <string.h>

typedef struct StartupLoadFile {
    uint8_t bytes[64];
    size_t cursor;
} StartupLoadFile;

typedef struct StartupLoadProbe {
    StartupLoadFile file;
    uint8_t allocation[32];
    void *allocation_result;
    size_t allocation_size;
    unsigned int open_calls;
    unsigned int read_calls;
    unsigned int close_calls;
    unsigned int directory_calls;
    unsigned int clear_calls;
    unsigned int sprite_calls;
    unsigned int text_calls;
    unsigned int loading_update_calls;
    unsigned int status_calls;
    const char *directories[2];
    const char *texts[10];
    uint32_t image_ids[6];
} StartupLoadProbe;

static PharaohSpriteLibraryResetState sprite_state;

static PharaohReadOnlyFile *startup_file_open(void *user_data,
                                              const char *filename)
{
    StartupLoadProbe *probe = (StartupLoadProbe *)user_data;
    assert(strcmp(filename, "Pharaohmap.inf") == 0);
    ++probe->open_calls;
    return (PharaohReadOnlyFile *)&probe->file;
}

static size_t startup_file_size(void *user_data, PharaohReadOnlyFile *file)
{
    (void)user_data;
    (void)file;
    return 64;
}

static size_t startup_file_read(void *user_data, void *destination,
                                size_t element_size, size_t element_count,
                                PharaohReadOnlyFile *file)
{
    StartupLoadProbe *probe = (StartupLoadProbe *)user_data;
    StartupLoadFile *source = (StartupLoadFile *)file;
    const size_t byte_count = element_size * element_count;
    assert(byte_count == sizeof(source->bytes));
    assert(source->cursor == 0);
    memcpy(destination, source->bytes, byte_count);
    source->cursor += byte_count;
    ++probe->read_calls;
    return element_count;
}

static int32_t startup_file_seek(void *user_data, PharaohReadOnlyFile *file,
                                 int32_t offset, int32_t origin)
{
    (void)user_data;
    (void)file;
    assert(offset == 0);
    assert(origin == 0);
    return 0;
}

static void startup_file_close(void *user_data, PharaohReadOnlyFile *file)
{
    StartupLoadProbe *probe = (StartupLoadProbe *)user_data;
    (void)file;
    ++probe->close_calls;
}

static void startup_create_directory(void *user_data, const char *path)
{
    StartupLoadProbe *probe = (StartupLoadProbe *)user_data;
    assert(probe->directory_calls < 2);
    probe->directories[probe->directory_calls++] = path;
}

static void *startup_allocate(void *user_data, size_t byte_count)
{
    StartupLoadProbe *probe = (StartupLoadProbe *)user_data;
    probe->allocation_size = byte_count;
    return probe->allocation_result;
}

static void startup_clear_surface(void *user_data)
{
    StartupLoadProbe *probe = (StartupLoadProbe *)user_data;
    ++probe->clear_calls;
}

static void startup_draw_sprite(void *user_data, uint32_t image_id,
                                int32_t x, int32_t y)
{
    StartupLoadProbe *probe = (StartupLoadProbe *)user_data;
    assert(probe->sprite_calls < 6);
    assert(x == 17);
    assert(y == 29);
    probe->image_ids[probe->sprite_calls++] = image_id;
}

static void startup_draw_text(void *user_data, const char *text)
{
    StartupLoadProbe *probe = (StartupLoadProbe *)user_data;
    assert(probe->text_calls < 10);
    probe->texts[probe->text_calls++] = text;
}

static void startup_process_loading_updates(void *user_data,
                                            int32_t force_update)
{
    StartupLoadProbe *probe = (StartupLoadProbe *)user_data;
    assert(force_update == 1);
    ++probe->loading_update_calls;
}

static void startup_write_status(void *user_data, const char *text)
{
    StartupLoadProbe *probe = (StartupLoadProbe *)user_data;
    assert(strcmp(text, "OK: Setup loaded DAT files") == 0);
    ++probe->status_calls;
}

static void initialize_startup_context(
    PharaohStartupAssetLoadContext *context,
    PharaohStartupAssetState *state,
    PharaohGameDataLoadContext *game_data,
    PharaohFileReadContext *files,
    PharaohTextSelection *selection,
    PharaohTextCatalog *catalog,
    PharaohSpriteLibraryLoadContext *sprite_load,
    const PharaohFileReadOperations *file_operations,
    const PharaohStartupAssetOperations *startup_operations,
    uint8_t *map_info_buffer, uint32_t screen_width,
    uint32_t screen_height)
{
    static char text_entries[] =
        "entry0\0entry1\0entry2\0entry3\0entry4\0entry5\0entry6\0"
        "entry7\0entry8\0entry9\0entry10\0entry11\0";
    static PharaohTextGroupIndex text_index[PHARAOH_TEXT_PRIMARY_GROUP_COUNT];
    static PharaohSettingsFile settings;
    static PharaohGameDataLoadState load_state =
        PHARAOH_GAME_DATA_LOAD_STATE_INITIALIZER;

    text_index[PHARAOH_STARTUP_TEXT_GROUP].text_offset = 0;
    catalog->primary_index[PHARAOH_STARTUP_TEXT_GROUP] =
        text_index[PHARAOH_STARTUP_TEXT_GROUP];
    catalog->primary_text = text_entries;
    selection->catalog = catalog;
    selection->current = text_entries;
    files->operations = file_operations;
    files->user_data = startup_operations->user_data;
    game_data->files = files;
    game_data->settings = &settings;
    game_data->text_selection = selection;
    game_data->state = &load_state;
    sprite_load->libraries = &sprite_state;

    state->map_info_file_buffer = map_info_buffer;
    context->game_data = game_data;
    context->sprite_libraries = sprite_load;
    context->text_selection = selection;
    context->state = state;
    context->operations = startup_operations;
    context->screen_width = screen_width;
    context->screen_height = screen_height;
    context->loading_image_x = 17;
    context->loading_image_y = 29;
    context->loading_stage_image_base = 100;
    context->loading_background_image = 55;
}

static void run_startup_setup(int allocation_succeeds, uint32_t screen_width,
                              uint32_t screen_height)
{
    static const PharaohFileReadOperations file_operations = {
        startup_file_open,
        startup_file_size,
        startup_file_read,
        startup_file_seek,
        startup_file_close
    };
    StartupLoadProbe probe = {0};
    PharaohStartupAssetOperations startup_operations = {
        &probe,
        startup_create_directory,
        startup_allocate,
        startup_clear_surface,
        startup_draw_sprite,
        startup_draw_text,
        startup_process_loading_updates,
        startup_write_status
    };
    PharaohStartupAssetState state = {0};
    PharaohGameDataLoadContext game_data = {0};
    PharaohFileReadContext files = {0};
    PharaohTextSelection selection = {0};
    PharaohTextCatalog catalog = {0};
    PharaohSpriteLibraryLoadContext sprite_load = {0};
    PharaohStartupAssetLoadContext context = {0};
    uint8_t map_info[64] = {0};

    memset(&sprite_state, 0, sizeof(sprite_state));
    sprite_state.loaded_count = PHARAOH_STARTUP_SPRITE_LIBRARY_COUNT;
    probe.allocation_result = allocation_succeeds ? probe.allocation : NULL;
    initialize_startup_context(&context, &state, &game_data, &files,
        &selection, &catalog, &sprite_load, &file_operations,
        &startup_operations, map_info, screen_width, screen_height);

    const int32_t result = pharaoh_startup_asset_setup(&context);
    assert(probe.open_calls == 1);
    assert(probe.read_calls == 1);
    assert(probe.close_calls == 1);
    assert(probe.directory_calls == 2);
    assert(strcmp(probe.directories[0], "Save") == 0);
    assert(strcmp(probe.directories[1], "Maps") == 0);
    assert(probe.allocation_size == (allocation_succeeds ?
        1572864u : PHARAOH_STARTUP_PIXEL_BUFFER_MINIMUM_BYTES));
    assert(state.selected_campaign_id == 16);
    assert(state.startup_asset_setup_flag == 1);

    if (allocation_succeeds) {
        assert(result == 0);
        assert(state.screen_pixel_buffer == probe.allocation);
        assert(probe.clear_calls == 1);
        assert(probe.sprite_calls == 6);
        assert(probe.image_ids[0] == 100u);
        assert(probe.image_ids[1] == 101u);
        assert(probe.image_ids[2] == 102u);
        assert(probe.image_ids[3] == 103u);
        assert(probe.image_ids[4] == 103u);
        assert(probe.image_ids[5] == 55u);
        assert(probe.text_calls == 10);
        assert(probe.loading_update_calls == 6);
        assert(strcmp(probe.texts[0], "entry7") == 0);
        assert(strcmp(probe.texts[1], "entry7") == 0);
        assert(strcmp(probe.texts[2], "entry8") == 0);
        assert(strcmp(probe.texts[3], "entry8") == 0);
        assert(strcmp(probe.texts[4], "entry9") == 0);
        assert(strcmp(probe.texts[5], "entry9") == 0);
        assert(strcmp(probe.texts[6], "entry10") == 0);
        assert(strcmp(probe.texts[7], "entry10") == 0);
        assert(strcmp(probe.texts[8], "entry11") == 0);
        assert(strcmp(probe.texts[9], "entry11") == 0);
        assert(probe.status_calls == 1);
    } else {
        assert(result == 1);
        assert(state.screen_pixel_buffer == NULL);
        assert(probe.clear_calls == 1);
        assert(probe.sprite_calls == 1);
        assert(probe.image_ids[0] == 100u);
        assert(probe.text_calls == 2);
        assert(probe.loading_update_calls == 1);
        assert(probe.status_calls == 0);
    }
    assert(state.loading_progress_percent == 100u);
}

int main(void)
{
    run_startup_setup(1, 1024, 768);
    run_startup_setup(0, 800, 600);
    return 0;
}
