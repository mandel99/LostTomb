#include "pharaoh/scenario/map_file_load.h"

int32_t pharaoh_map_file_load(
    const PharaohMapFileLoadContext *context, const char *path, int32_t mode)
{
    const PharaohFileSizeOperations *const files =
        context->files->operations;
    const PharaohMapFileLoadOperations *const operations =
        context->operations;
    int32_t file_handle;

    operations->restore_application_directory(operations->user_data);
    operations->load_scenario_settings(operations->user_data);
    file_handle = files->open_file(context->files->user_data, path,
        PHARAOH_FILE_OPEN_FLAGS_NATIVE_8000);
    if (file_handle == -1) return 1;

    operations->parse_map_file(operations->user_data, file_handle);
    (void)files->close_file(context->files->user_data, file_handle);
    *context->current_map_environment =
        (int32_t)*context->selected_environment;
    if (mode == 0) {
        operations->initialize_map_assets(operations->user_data);
    }
    if (*context->rating_update_enabled == 1) {
        *context->stored_monument_rating = operations->calculate_monument_rating(
            operations->user_data, 1);
    }
    return 0;
}
