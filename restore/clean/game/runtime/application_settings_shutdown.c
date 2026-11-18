#include "pharaoh/runtime/application_settings_shutdown.h"

static const char settings_filename[] = "Pharaoh.inf";
static const char map_info_filename[] = "Pharaohmap.inf";
static const char removed_manmades_status[] = "OK :removed manmades";

static void release_allocation(
    const PharaohApplicationSettingsShutdownContext *context,
    void *allocation)
{
    context->memory_operations->release_allocation(
        context->memory_operations->user_data, allocation);
}

void pharaoh_application_settings_shutdown(
    const PharaohApplicationSettingsShutdownContext *context)
{
    (void)pharaoh_file_write_all(context->file_writer, settings_filename,
        context->settings_file, (uint32_t)sizeof *context->settings_file);
    (void)pharaoh_file_write_all(context->file_writer, map_info_filename,
        context->map_info_file_buffer, PHARAOH_MAP_INFO_SETTINGS_BYTES);
    pharaoh_status_log_format(removed_manmades_status, NULL,
        (int32_t)((uint32_t)context->removed_manmades_count + UINT32_C(1)),
        context->status_log_state,
        context->status_log_operations);

    if (context->owned_allocation_1 != NULL) {
        release_allocation(context, context->owned_allocation_1);
    }
    if (context->owned_allocation_2 != NULL) {
        release_allocation(context, context->owned_allocation_2);
    }
    if (*context->owned_allocation_3 != NULL) {
        release_allocation(context, *context->owned_allocation_3);
        *context->owned_allocation_3 = NULL;
    }
    if (*context->owned_allocation_4 != NULL) {
        release_allocation(context, *context->owned_allocation_4);
        *context->owned_allocation_4 = NULL;
    }
}
