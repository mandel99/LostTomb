#include "pharaoh/scenario/scenario_state_read.h"

#include <string.h>

enum {
    PHARAOH_SCENARIO_CHUNK_FIRST = 1,
    PHARAOH_SCENARIO_CHUNK_END = 24
};

static void read_descriptor_zero(PharaohScenarioStateReadContext *context,
    int32_t file)
{
    PharaohChunkDescriptor *descriptor = &context->descriptors[0];
    PharaohChunkRead read = descriptor->codec == 0 ?
        context->chunk_io->read_raw : context->chunk_io->read_compressed;
    (void)read(context->chunk_io->user_data, file,
        descriptor->destination,
        (uint32_t)descriptor->element_count *
            (uint32_t)descriptor->element_size);
}

void pharaoh_scenario_route_samples_free(PharaohEditorRoute *routes,
    uint32_t count, void *user_data, PharaohScenarioRouteFree release)
{
    uint32_t index;

    for (index = 0; index < count; ++index) {
        PharaohEditorRoute *route = &routes[index];
        if (route->samples != NULL) {
            release(user_data, route->samples);
            route->samples = NULL;
            route->capacity = 0;
        }
    }
}

void pharaoh_scenario_routes_rebuild(PharaohEditorRoute *routes,
    uint32_t count, void *user_data, PharaohScenarioRouteRebuild rebuild)
{
    uint32_t index;
    PharaohEditorRoute *route;

    for (index = 0; index < count; ++index) {
        route = &routes[index];
        if (route->samples == NULL && route->capacity == 0 &&
            route->point_count < 0x33 && route->kind < 3 &&
            route->spacing != 0 && route->spacing < 0x33) {
            rebuild(user_data, route);
        } else {
            route->sample_count = 0;
            route->samples = NULL;
            route->point_count = 0;
            route->capacity = 0;
            route->spacing = 5;
            route->enabled = 0;
            route->first_city = -1;
            route->last_city = -1;
            route->kind = 1;
        }
    }
}

/* Complete typed translation of Pharaoh.exe 0x004D4AE0. */
void pharaoh_scenario_state_read(PharaohScenarioStateReadContext *context,
    int32_t file)
{
    int32_t index;
    int32_t version;

    read_descriptor_zero(context, file);
    if (context->validate_header(context->user_data, context->header,
            context->descriptor_count) != 0) {
        context->chunk_io->set_message(context->chunk_io->user_data, 5, 0x18);
        context->chunk_io->set_message(context->chunk_io->user_data, 5, 0x19);
        context->chunk_io->show_error(context->chunk_io->user_data,
            context->chunk_io->error_x, context->chunk_io->error_y, 0);
        return;
    }

    context->chunk_io->file_version = 0xa0;
    context->chunk_io->stored_schema_present = 0;
    pharaoh_scenario_route_samples_free(context->routes,
        context->route_count, context->user_data, context->free_route_samples);

    for (index = PHARAOH_SCENARIO_CHUNK_FIRST;
         index < PHARAOH_SCENARIO_CHUNK_END; ++index) {
        if (pharaoh_chunk_read_migrate(context->chunk_io, index,
                context->descriptors, file) == 0)
            return;
    }

    *context->route_state_flag = 0;
    *context->route_state_word = 0;
    pharaoh_scenario_routes_rebuild(context->routes,
        context->route_count, context->user_data, context->rebuild_route);
    version = context->chunk_io->file_version;

    if (version < 0x75) {
        uint32_t word;
        for (word = 0; word < context->version_75_word_count; ++word)
            context->version_75_words[word] =
                (uint16_t)(word < context->version_75_one_count);
    }
    if (version < 0x97) *context->version_97_word = 0;
    if (version < 0x76) context->upgrade_76(context->user_data);
    if (version < 0x7d) context->upgrade_7d(context->user_data);
    if (version < 0x7f) context->upgrade_7f(context->user_data);
    if (version < 0x90) context->upgrade_90(context->user_data);
    if (version < 0x98) context->upgrade_98(context->user_data);
    if (version < 0x99) context->upgrade_99(context->user_data);
    if (version < 0x9b) {
        context->upgrade_9b_first(context->user_data);
        context->upgrade_9b_second(context->user_data);
        memset(context->version_9b_bytes, 0, context->version_9b_byte_count);
    }
    if (version < 0x9c) context->upgrade_9c(context->user_data);
    if (version < 0x9d) {
        *context->late_value_d7b0 = 0;
        *context->late_value_d7a8 = 0;
        *context->late_value_d7a4 = 0;
        context->late_flags[0] = 1;
    }
    if (version < 0x9e) {
        *context->late_value_d7ac = 0;
        context->late_flags[1] = 0;
        *context->late_value_d798 = 10;
        *context->late_value_d7a0 = 0;
    }
}
