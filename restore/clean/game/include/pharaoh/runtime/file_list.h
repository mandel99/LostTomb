#ifndef PHARAOH_RUNTIME_FILE_LIST_H
#define PHARAOH_RUNTIME_FILE_LIST_H

#include <stddef.h>
#include <stdint.h>

typedef struct PharaohFileList {
    uint32_t vtable;
    int32_t *offsets;
    int32_t count;
    int32_t capacity;
    char *strings;
    int32_t string_capacity;
    int32_t used;
} PharaohFileList;

_Static_assert(sizeof(PharaohFileList) == 28,
    "native file-list record size");
_Static_assert(offsetof(PharaohFileList, offsets) == 4 &&
    offsetof(PharaohFileList, strings) == 16 &&
    offsetof(PharaohFileList, used) == 24,
    "native file-list field offsets");

void pharaoh_file_list_initialize(PharaohFileList *list);
void pharaoh_mission_file_list_initialize(PharaohFileList *list);
void pharaoh_file_list_destroy(PharaohFileList *list);

/* Native entries 0x0041C740/0x0041C8F0 and 0x0041CA10. */
int32_t pharaoh_file_list_load(PharaohFileList *list, const char *pattern,
    int32_t append);
void pharaoh_file_list_sort(PharaohFileList *list);
const char *pharaoh_file_list_name(const PharaohFileList *list,
    int32_t index);

/* Copies an ordered, fixed-width name table into the mission-list layout. */
int32_t pharaoh_mission_file_list_copy_names(PharaohFileList *list,
    const char *names, int32_t count, int32_t name_stride);

#endif
