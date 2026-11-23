#ifndef PHARAOH_SCENARIO_SAVED_SCENE_READER_H
#define PHARAOH_SCENARIO_SAVED_SCENE_READER_H

#include "pharaoh/scenario/saved_scene_storage.h"
#include "pharaoh/scenario/saved_routes.h"

typedef struct PharaohSavedSceneReaderInputs {
    PharaohChunkIoContext io;
    PharaohCompressedChunkState compressed;
    /* Live graphics/style providers, established before scene initialization. */
    const PharaohRouteImage *route_images;
    int32_t route_image_count;
    const int32_t *pyramid_style, *pyramid_parts, *mastaba_parts;
    const int32_t (*default_resource_prices)[2]; /* five pairs */
    PharaohCrtRandom *random;
} PharaohSavedSceneReaderInputs;

/* One owner for the real 004D41E0 child graph. Scene payloads and external
 * file/heap/graphics providers remain borrowed. No original process pointers
 * or executable image are needed by this owner. */
typedef struct PharaohSavedSceneReader {
    PharaohSavedSceneStorage *scene;
    PharaohSavedGameIo io;
    PharaohSavedGameReadContext read;
    PharaohSavedGameReadOperations operations;
    PharaohSavedGameUpgradeBindings upgrades;
    PharaohSavedGameSpotBindings spots;
    PharaohSavedRouteBindings routes;
    PharaohHistoryMemory history_memory;
    PharaohDynamicQueueSlot histories[10];
} PharaohSavedSceneReader;

/* Bind once to a freshly opened scene. Close this reader's child allocations
 * before closing/reusing that scene. Scratch and all input pointers must stay
 * live. The native decoder permits chunks up to 1,200,000 bytes; its scratch
 * buffer must accommodate the input supplied by the file provider. */
void pharaoh_saved_scene_reader_bind(PharaohSavedSceneReader *reader,
    PharaohSavedSceneStorage *scene,
    const PharaohSavedSceneReaderInputs *inputs);

/* Release route samples and histories through the same allocator that created
 * them. Does not release the borrowed scene, IO or codec scratch. */
void pharaoh_saved_scene_reader_close(PharaohSavedSceneReader *reader);

#endif
