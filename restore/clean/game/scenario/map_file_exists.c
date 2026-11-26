#include "pharaoh/scenario/map_file_exists.h"

/* Complete typed translation of Pharaoh.exe 0x005409D0. */
bool pharaoh_scenario_map_exists(const PharaohMapFileExistsContext *context,
    char *path)
{
    int result;
    context->normalize_name(context->user_data, path);
    context->prepare_extension(context->user_data);
    context->finalize_name(context->user_data, path);
    result = context->file_exists(context->user_data, path);
    context->normalize_name(context->user_data, path);
    return result != 0;
}
