#include "pharaoh/scenario/scenario_preview.h"

#include <limits.h>
#include <stdio.h>
#include <string.h>

enum {
    DESCRIPTOR_CAPACITY = 300,
    MAP_METADATA_CHUNK = 14,
    MAP_EVENTS_CHUNK = 16,
    SAVE_METADATA_CHUNK = 75,
    SAVE_EVENTS_CHUNK = 136
};

typedef struct PharaohStoredChunkDescriptor {
    uint32_t codec;
    uint32_t stored_destination;
    int32_t size;
    int32_t count;
    uint32_t stored_migration;
} PharaohStoredChunkDescriptor;

_Static_assert(sizeof(PharaohStoredChunkDescriptor) == 20,
    "native chunk descriptor size");

static int32_t read_exact(FILE *file, void *destination, size_t bytes)
{
    return bytes == 0 || fread(destination, 1, bytes, file) == bytes;
}

static int32_t skip_bytes(FILE *file, int64_t bytes)
{
    if (bytes < 0 || bytes > LONG_MAX) return 0;
    return fseek(file, (long)bytes, SEEK_CUR) == 0;
}

static int32_t chunk_bytes(const PharaohStoredChunkDescriptor *descriptor,
    int64_t *bytes)
{
    if (descriptor->size < 0 || descriptor->count < 0) return 0;
    *bytes = (int64_t)descriptor->size * descriptor->count;
    return *bytes <= LONG_MAX;
}

static int32_t read_or_skip_chunk(FILE *file,
    const PharaohStoredChunkDescriptor *descriptor, void *destination,
    size_t destination_bytes)
{
    int64_t expanded_bytes;
    int32_t stored_bytes;
    if (chunk_bytes(descriptor, &expanded_bytes) == 0) return 0;

    if (descriptor->codec == 0) {
        if (destination != NULL) {
            return expanded_bytes == (int64_t)destination_bytes &&
                read_exact(file, destination, destination_bytes);
        }
        return skip_bytes(file, expanded_bytes);
    }

    if (read_exact(file, &stored_bytes, sizeof(stored_bytes)) == 0) return 0;
    if (stored_bytes == INT_MIN) {
        if (destination != NULL) {
            return expanded_bytes == (int64_t)destination_bytes &&
                read_exact(file, destination, destination_bytes);
        }
        return skip_bytes(file, expanded_bytes);
    }
    if (stored_bytes == 0) {
        if (destination != NULL) memset(destination, 0, destination_bytes);
        return 1;
    }
    if (stored_bytes < 0 || destination != NULL) return 0;
    return skip_bytes(file, stored_bytes);
}

static int32_t read_preview_chunks(FILE *file,
    const PharaohStoredChunkDescriptor descriptors[DESCRIPTOR_CAPACITY],
    int32_t first_chunk, int32_t metadata_chunk, int32_t events_chunk,
    PharaohScenarioPreview *preview)
{
    for (int32_t index = first_chunk; index <= events_chunk; ++index) {
        void *destination = NULL;
        size_t bytes = 0;
        if (index == metadata_chunk) {
            destination = preview->metadata;
            bytes = sizeof(preview->metadata);
        } else if (index == events_chunk) {
            destination = preview->events;
            bytes = sizeof(preview->events);
        }
        if (read_or_skip_chunk(file, &descriptors[index], destination,
                bytes) == 0) return 0;
    }
    preview->valid = 1;
    return 1;
}

int32_t pharaoh_scenario_preview_load_map(
    PharaohScenarioPreview *preview, const char *path)
{
    PharaohStoredChunkDescriptor descriptors[DESCRIPTOR_CAPACITY];
    FILE *file = NULL;
    uint32_t magic;
    int32_t descriptor_count;
    int32_t success = 0;

    memset(preview, 0, sizeof(*preview));
    if (fopen_s(&file, path, "rb") != 0 || file == NULL) return 1;
    if (read_exact(file, &magic, sizeof(magic)) != 0 &&
        read_exact(file, &preview->version, sizeof(preview->version)) != 0 &&
        read_exact(file, &descriptor_count, sizeof(descriptor_count)) != 0 &&
        magic == 0x5350414du && descriptor_count > MAP_EVENTS_CHUNK &&
        descriptor_count <= DESCRIPTOR_CAPACITY &&
        read_exact(file, descriptors, sizeof(descriptors)) != 0) {
        success = read_preview_chunks(file, descriptors, 4,
            MAP_METADATA_CHUNK, MAP_EVENTS_CHUNK, preview);
    }
    (void)fclose(file);
    return success != 0 ? 0 : 1;
}

int32_t pharaoh_scenario_preview_load_campaign_mission(
    PharaohScenarioPreview *preview, const char *package_path,
    int32_t mission_id)
{
    PharaohStoredChunkDescriptor descriptors[DESCRIPTOR_CAPACITY];
    FILE *file = NULL;
    int32_t mission_offset;
    int32_t descriptor_count;
    uint8_t save_flags[4];
    int32_t success = 0;

    memset(preview, 0, sizeof(*preview));
    if (mission_id < 0 || fopen_s(&file, package_path, "rb") != 0 ||
        file == NULL) return 0;
    if (fseek(file, mission_id * (long)sizeof(int32_t), SEEK_SET) == 0 &&
        read_exact(file, &mission_offset, sizeof(mission_offset)) != 0 &&
        mission_offset > 0 && fseek(file, mission_offset, SEEK_SET) == 0 &&
        read_exact(file, save_flags, sizeof(save_flags)) != 0 &&
        read_exact(file, &preview->version, sizeof(preview->version)) != 0 &&
        read_exact(file, &descriptor_count, sizeof(descriptor_count)) != 0 &&
        descriptor_count > SAVE_EVENTS_CHUNK &&
        descriptor_count <= DESCRIPTOR_CAPACITY &&
        read_exact(file, descriptors, sizeof(descriptors)) != 0) {
        success = read_preview_chunks(file, descriptors, 7,
            SAVE_METADATA_CHUNK, SAVE_EVENTS_CHUNK, preview);
    }
    (void)fclose(file);
    return success;
}
