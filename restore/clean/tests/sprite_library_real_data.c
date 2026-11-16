#include "pharaoh/assets/sprite_libraries.h"
#include "pharaoh/assets/sprite_pixel_conversion.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct PharaohReadOnlyFile {
    FILE *stream;
};

typedef struct SpriteDataFiles {
    char root[512];
} SpriteDataFiles;

static PharaohReadOnlyFile *open_data(void *user_data, const char *filename)
{
    SpriteDataFiles *files = user_data;
    static PharaohReadOnlyFile file;
    char path[1024];
    size_t root_length = strlen(files->root);
    size_t name_length;

    if (strncmp(filename, "data\\", 5) != 0) {
        return NULL;
    }
    name_length = strlen(filename + 5);
    if (root_length + name_length + 2 > sizeof(path)) {
        return NULL;
    }
    memcpy(path, files->root, root_length);
    path[root_length] = '\\';
    memcpy(path + root_length + 1, filename + 5, name_length + 1);
    for (char *cursor = path + root_length + 1; *cursor != '\0'; ++cursor) {
        if (*cursor == '/') {
            *cursor = '\\';
        }
    }
    file.stream = fopen(path, "rb");
    return file.stream != NULL ? &file : NULL;
}

static size_t file_size(void *user_data, PharaohReadOnlyFile *file)
{
    long end;
    (void)user_data;
    if (fseek(file->stream, 0, SEEK_END) != 0) {
        return 0;
    }
    end = ftell(file->stream);
    (void)fseek(file->stream, 0, SEEK_SET);
    return end < 0 ? 0 : (size_t)end;
}

static size_t read_file(void *user_data, void *destination, size_t element_size,
    size_t element_count, PharaohReadOnlyFile *file)
{
    (void)user_data;
    return fread(destination, element_size, element_count, file->stream);
}

static int32_t seek_file(void *user_data, PharaohReadOnlyFile *file,
    int32_t offset, int32_t origin)
{
    (void)user_data;
    return fseek(file->stream, offset, origin);
}

static void close_file(void *user_data, PharaohReadOnlyFile *file)
{
    (void)user_data;
    (void)fclose(file->stream);
    file->stream = NULL;
}

static void *allocate_memory(void *user_data, uint32_t byte_count)
{
    (void)user_data;
    return malloc(byte_count);
}

static void release_memory(void *user_data, void *allocation)
{
    (void)user_data;
    free(allocation);
}

static uint32_t read_u32(const uint8_t *bytes)
{
    uint32_t value;
    memcpy(&value, bytes, sizeof(value));
    return value;
}

static void record_tag_warning(void *user_data, const char *library_name,
    uint32_t actual_tags, uint32_t expected_tags)
{
    uint32_t *warning_count = user_data;
    (void)library_name;
    (void)actual_tags;
    (void)expected_tags;
    ++*warning_count;
}

int main(int argc, char **argv)
{
    static const PharaohFileReadOperations file_operations = {
        open_data, file_size, read_file, seek_file, close_file
    };
    static const PharaohSpritePixelMemoryOperations memory_operations = {
        allocate_memory, release_memory
    };
    SpriteDataFiles files;
    PharaohFileReadContext file_context;
    PharaohGameDataLoadState load_state = PHARAOH_GAME_DATA_LOAD_STATE_INITIALIZER;
    PharaohGameDataLoadContext data_context;
    PharaohSpritePixelLoadContext pixel_context;
    PharaohSpriteLibraryReadContext reader_context;
    PharaohSpriteLibrary library = { 0 };
    PharaohSpriteLibraryResetState *libraries;
    PharaohSpriteLibraryLoadContext load_context;
    FILE *metadata_file;
    uint8_t fixed_header[0x9f28];
    PharaohSpriteImageRecord *images;
    PharaohSpriteTagRecord tags[300];
    uint32_t pixel_cursor = 0;
    uint32_t expected_image_count;
    uint32_t expected_tag_count = 0;
    uint32_t expected_pixel_cursor = sizeof(uint32_t);
    uint32_t tag_warnings = 0;
    int32_t library_index = 0;
    uint32_t system_image_skip = 0;
    uint8_t result;
    char metadata_path[1024];

    if (argc < 3 || argc > 4) {
        fprintf(stderr, "usage: %s <Data directory> <library name> [slot]\n", argv[0]);
        return 2;
    }
    if (argc == 4) {
        library_index = (int32_t)strtol(argv[3], NULL, 10);
    }
    if (strlen(argv[1]) >= sizeof(files.root)) {
        return 2;
    }
    memcpy(files.root, argv[1], strlen(argv[1]) + 1);
    (void)snprintf(metadata_path, sizeof(metadata_path), "%s\\%s.sg3",
        files.root, argv[2]);
    metadata_file = fopen(metadata_path, "rb");
    if (metadata_file == NULL ||
        fread(fixed_header, 1, sizeof(fixed_header), metadata_file) !=
            sizeof(fixed_header)) {
        fprintf(stderr, "could not read SG3 header: %s\n", metadata_path);
        if (metadata_file != NULL) {
            (void)fclose(metadata_file);
        }
        return 1;
    }
    (void)fclose(metadata_file);

    expected_image_count = read_u32(fixed_header + 16);
    if (library_index != 0 &&
        strcmp((const char *)(fixed_header + 0x2a8), "system.bmp") == 0) {
        system_image_skip = 200;
    }
    expected_image_count -= system_image_skip;
    if (expected_image_count == 0 || expected_image_count > 25500) {
        fprintf(stderr, "unexpected SG3 image count: %u\n", expected_image_count);
        return 1;
    }
    images = calloc((size_t)expected_image_count + 1, sizeof(*images));
    if (images == NULL) {
        return 1;
    }
    for (uint32_t group = 0; group < 300; ++group) {
        int16_t image_number;
        memcpy(&image_number, fixed_header + 0x50 + group * 2, sizeof(image_number));
        if ((int32_t)image_number - (int32_t)system_image_skip > 0) {
            ++expected_tag_count;
        }
    }

    file_context.operations = &file_operations;
    file_context.user_data = &files;
    data_context.files = &file_context;
    data_context.settings = NULL;
    data_context.text_selection = NULL;
    data_context.state = &load_state;
    pixel_context.game_data = &data_context;
    pixel_context.memory_operations = &memory_operations;
    pixel_context.memory_user_data = NULL;
    pixel_context.shared_pixel_buffer = NULL;
    pixel_context.pixel_mode = PHARAOH_SPRITE_PIXEL_MODE_RGB565;
    reader_context.game_data = &data_context;
    reader_context.pixels = &pixel_context;
    reader_context.pixel_cursor = &pixel_cursor;

    result = pharaoh_sprite_library_read(&reader_context, &library, argv[2],
        library_index, images, 1, tags, 1);
    if (result == 0 || library.image_count != expected_image_count ||
        library.tag_count != expected_tag_count || library.pixels == NULL) {
        fprintf(stderr, "SG3 load mismatch: result=%u images=%u/%u tags=%u/%u\n",
            result, library.image_count, expected_image_count,
            library.tag_count, expected_tag_count);
        free(images);
        free(library.pixels);
        return 1;
    }
    for (uint32_t index = 0; index < library.image_count; ++index) {
        if (images[index].is_external == 0) {
            expected_pixel_cursor += images[index].pixel_bytes;
        }
    }
    if (pixel_cursor != expected_pixel_cursor ||
        pixel_cursor > library.pixel_bytes + sizeof(uint32_t)) {
        fprintf(stderr, "pixel cursor mismatch: %u expected=%u bytes=%u\n",
            pixel_cursor, expected_pixel_cursor, library.pixel_bytes);
        free(images);
        free(library.pixels);
        return 1;
    }

    libraries = calloc(1, sizeof(*libraries));
    if (libraries == NULL) {
        free(images);
        free(library.pixels);
        return 1;
    }
    load_context.libraries = libraries;
    load_context.reader = &reader_context;
    load_context.images = images;
    load_context.tags = tags;
    load_context.show_tag_sync_warning = record_tag_warning;
    load_context.warning_user_data = &tag_warnings;
    pharaoh_sprite_libraries_reset(libraries, &pixel_context);
    result = pharaoh_sprite_library_load_file(&load_context, argv[2],
        library_index);
    if (result == 0 ||
        libraries->items[library_index].image_count != expected_image_count ||
        libraries->items[library_index].tag_count != expected_tag_count ||
        libraries->loaded_count != 1 ||
        libraries->next_image != expected_image_count + 1 ||
        libraries->next_tag != expected_tag_count + 1 ||
        libraries->pixel_bytes != libraries->items[library_index].pixel_bytes) {
        fprintf(stderr, "managed load mismatch for %s at slot %d\n",
            argv[2], library_index);
        free(libraries->items[library_index].pixels);
        free(libraries);
        free(images);
        free(library.pixels);
        return 1;
    }

    printf("Loaded %s.sg3: %u images, %u tags, %u pixel bytes, cursor %u; manager and %u tag warnings verified.\n",
        argv[2], library.image_count, library.tag_count,
        library.pixel_bytes, pixel_cursor, tag_warnings);
    free(libraries->items[library_index].pixels);
    free(libraries);
    free(images);
    free(library.pixels);
    return 0;
}
