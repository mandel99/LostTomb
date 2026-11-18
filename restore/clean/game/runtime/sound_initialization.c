#include "pharaoh/runtime/sound_initialization.h"

void pharaoh_sound_initialize(
    const PharaohSoundInitializationContext *context)
{
    PharaohSoundInitializationState *state = context->state;
    const PharaohSoundInitializationOperations *operations =
        context->operations;

    state->digital_output_result =
        pharaoh_digital_sound_device_initialize(context->digital_sound);
    (void)pharaoh_sound_file_table_resize(context->sound_file_table,
        PHARAOH_SOUND_INITIALIZATION_FILE_CAPACITY, context->file_memory);
    pharaoh_music_manager_set_enabled(context->music_manager, 0);
    operations->load_music_configuration(operations->user_data,
        context->music_manager);
    pharaoh_music_manager_select_population_range(context->music_manager,
        context->music_category, context->city_population,
        PHARAOH_SOUND_INITIALIZATION_FORCE_TRACK_SELECTION);
    state->post_configuration_sentinel = -1;
    operations->register_object_sound_files(operations->user_data);
    operations->register_environment_sound_files(operations->user_data);
    pharaoh_sound_effect_files_initialize(context->interface_sound_file_ids,
        context->interface_sound_file_load_context);
    operations->register_figure_sound_files(operations->user_data);
}
