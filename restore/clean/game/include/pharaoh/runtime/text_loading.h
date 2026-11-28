#ifndef PHARAOH_RUNTIME_TEXT_LOADING_H
#define PHARAOH_RUNTIME_TEXT_LOADING_H

#include <stdint.h>

enum {
    PHARAOH_TEXT_FILE_OPEN_FLAGS = 0x8000,
    PHARAOH_TEXT_PRIMARY_HEADER_BYTES = 0x1c,
    PHARAOH_TEXT_PRIMARY_INDEX_BYTES = 8000,
    PHARAOH_TEXT_PRIMARY_GROUP_COUNT = 1000,
    PHARAOH_TEXT_PRIMARY_FIXED_BYTES = 0x1f5c,
    PHARAOH_TEXT_MEDIA_INDEX_BYTES = 0x13898,
    PHARAOH_TEXT_ERROR_GROUP = 5,
    PHARAOH_TEXT_ERROR_CAPTION_ENTRY = 0x89,
    PHARAOH_TEXT_ERROR_BODY_ENTRY = 0x8a,
    PHARAOH_TEXT_ERROR_MESSAGE_BOX_FLAGS = 0x40001
};

typedef struct PharaohTextGroupIndex {
    int32_t text_offset;
    uint32_t unobserved_word;
} PharaohTextGroupIndex;

_Static_assert(sizeof(PharaohTextGroupIndex) == 8,
               "language text group index record size");
_Static_assert(sizeof(PharaohTextGroupIndex) *
               PHARAOH_TEXT_PRIMARY_GROUP_COUNT ==
               PHARAOH_TEXT_PRIMARY_INDEX_BYTES,
               "language text group index region size");

typedef struct PharaohTextFile PharaohTextFile;

typedef struct PharaohTextFileOperations {
    PharaohTextFile *(*open)(void *user_data, const char *filename,
                             uint32_t open_flags);
    uint32_t (*size)(void *user_data, PharaohTextFile *file);
    int32_t (*read_text)(void *user_data, PharaohTextFile *file,
                         void *destination, uint32_t byte_count);
    void (*close)(void *user_data, PharaohTextFile *file);
    void *(*allocate)(void *user_data, uint32_t byte_count);
    void (*release)(void *user_data, void *allocation);
} PharaohTextFileOperations;

typedef struct PharaohTextCatalog {
    uint8_t primary_header[PHARAOH_TEXT_PRIMARY_HEADER_BYTES];
    PharaohTextGroupIndex primary_index[PHARAOH_TEXT_PRIMARY_GROUP_COUNT];
    void *primary_text;
    uint8_t media_index[PHARAOH_TEXT_MEDIA_INDEX_BYTES];
    void *media_text;
} PharaohTextCatalog;

typedef struct PharaohTextLoadContext {
    const PharaohTextFileOperations *operations;
    void *user_data;
    PharaohTextCatalog *catalog;
} PharaohTextLoadContext;

typedef struct PharaohTextMediaMessage {
    const char *title, *subtitle, *body;
} PharaohTextMediaMessage;

/* Briefing0041A180 reads the final three DWORDs of the selected80-byte
 * media record, after its24-byte header. Native-valid message IDs are0..999. */
PharaohTextMediaMessage pharaoh_text_media_message(
    const PharaohTextCatalog *catalog,int32_t message_id);

typedef struct PharaohTextSelection {
    const PharaohTextCatalog *catalog;
    const char *current;
} PharaohTextSelection;

int pharaoh_load_text_catalogs(const PharaohTextLoadContext *context,
                               const char *primary_filename,
                               const char *media_filename);
void pharaoh_text_select(PharaohTextSelection *selection, int32_t group,
                         int32_t entry);
int32_t pharaoh_file_load_retry_prompt(PharaohTextSelection *selection);

typedef struct PharaohTextPointerTable {
    const char *const *entries;
    int32_t count;
} PharaohTextPointerTable;

const char *pharaoh_text_pointer_table_get(
    const PharaohTextPointerTable *table, int32_t index);

void pharaoh_text_select_secondary(
    PharaohTextSelection *selection, int32_t group, int32_t entry);

#endif
