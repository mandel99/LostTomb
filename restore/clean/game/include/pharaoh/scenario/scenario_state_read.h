#ifndef PHARAOH_SCENARIO_SCENARIO_STATE_READ_H
#define PHARAOH_SCENARIO_SCENARIO_STATE_READ_H

#include "pharaoh/editor/route_geometry.h"
#include "pharaoh/runtime/chunk_io.h"

#include <stdint.h>

typedef void (*PharaohScenarioRouteFree)(void *user_data, void *samples);
typedef void (*PharaohScenarioRouteRebuild)(void *user_data,
    PharaohEditorRoute *route);

void pharaoh_scenario_route_samples_free(PharaohEditorRoute *routes,
    uint32_t count, void *user_data, PharaohScenarioRouteFree release);
void pharaoh_scenario_routes_rebuild(PharaohEditorRoute *routes,
    uint32_t count, void *user_data, PharaohScenarioRouteRebuild rebuild);

typedef struct PharaohScenarioStateReadContext {
    PharaohChunkIoContext *chunk_io;
    PharaohChunkDescriptor *descriptors;
    const void *header;
    const void *descriptor_count;
    int32_t (*validate_header)(void *user_data, const void *header,
        const void *descriptor_count);
    PharaohEditorRoute *routes;
    uint32_t route_count;
    int32_t *route_state_word;
    uint8_t *route_state_flag;
    void (*free_route_samples)(void *user_data, void *samples);
    void (*rebuild_route)(void *user_data, PharaohEditorRoute *route);
    uint16_t *version_75_words;
    uint32_t version_75_word_count;
    uint32_t version_75_one_count;
    uint16_t *version_97_word;
    void (*upgrade_76)(void *user_data);
    void (*upgrade_7d)(void *user_data);
    void (*upgrade_7f)(void *user_data);
    void (*upgrade_90)(void *user_data);
    void (*upgrade_98)(void *user_data);
    void (*upgrade_99)(void *user_data);
    void (*upgrade_9b_first)(void *user_data);
    void (*upgrade_9b_second)(void *user_data);
    void *version_9b_bytes;
    uint32_t version_9b_byte_count;
    void (*upgrade_9c)(void *user_data);
    int32_t *late_value_d7b0;
    int32_t *late_value_d7a8;
    int32_t *late_value_d7a4;
    uint8_t *late_flags;
    int32_t *late_value_d7ac;
    int32_t *late_value_d798;
    int32_t *late_value_d7a0;
    void *user_data;
} PharaohScenarioStateReadContext;

void pharaoh_scenario_state_read(PharaohScenarioStateReadContext *context,
    int32_t file);

#endif
