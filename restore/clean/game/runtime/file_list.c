#include "pharaoh/runtime/file_list.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdlib.h>
#include <string.h>

enum {
    FILE_LIST_VTABLE = 0x0056f548,
    MISSION_FILE_LIST_VTABLE = 0x0056f54c
};

static void replace_string_storage(PharaohFileList *list, int32_t capacity)
{
    char *replacement = malloc((size_t)capacity);
    if (replacement == NULL) return;
    if (list->strings != NULL && list->used > 0) {
        memcpy(replacement, list->strings, (size_t)list->used);
    }
    free(list->strings);
    list->strings = replacement;
    list->string_capacity = capacity;
}

static void replace_offset_storage(PharaohFileList *list, int32_t capacity)
{
    int32_t *replacement = malloc((size_t)capacity * sizeof(*replacement));
    if (replacement == NULL) return;
    if (list->offsets != NULL && list->count > 0) {
        memcpy(replacement, list->offsets,
            (size_t)list->count * sizeof(*replacement));
    }
    free(list->offsets);
    list->offsets = replacement;
    list->capacity = capacity;
}

void pharaoh_file_list_initialize(PharaohFileList *list)
{
    memset(list, 0, sizeof(*list));
    list->vtable = FILE_LIST_VTABLE;
}

void pharaoh_mission_file_list_initialize(PharaohFileList *list)
{
    pharaoh_file_list_initialize(list);
    list->vtable = MISSION_FILE_LIST_VTABLE;
}

void pharaoh_file_list_destroy(PharaohFileList *list)
{
    list->vtable = FILE_LIST_VTABLE;
    free(list->strings);
    free(list->offsets);
    list->strings = NULL;
    list->offsets = NULL;
    list->count = 0;
    list->capacity = 0;
    list->string_capacity = 0;
    list->used = 0;
}

int32_t pharaoh_file_list_load(PharaohFileList *list, const char *pattern,
    int32_t append)
{
    int32_t string_capacity_was_reset = 0;
    int32_t offset_capacity_was_reset = 0;
    const int32_t initial_count = append == 0 ? 0 : list->count;
    const int32_t initial_used = append == 0 ? 0 : list->used;

    if (append == 0) {
        list->count = 0;
        list->used = 0;
    }

    for (;;) {
        WIN32_FIND_DATAA entry;
        HANDLE search = FindFirstFileA(pattern, &entry);
        int32_t scan_count = 0;
        int32_t scan_bytes = 0;
        int32_t complete = 1;

        list->count = initial_count;
        list->used = initial_used;
        if (search == INVALID_HANDLE_VALUE) return 0;
        do {
            const size_t length = strlen(entry.cFileName) + 1u;
            if (length > (size_t)INT32_MAX - (size_t)scan_bytes ||
                scan_count == INT32_MAX) {
                complete = 0;
            } else {
                scan_bytes += (int32_t)length;
                ++scan_count;
                if (list->count < list->capacity &&
                    (size_t)list->used + length <
                        (size_t)list->string_capacity) {
                    list->offsets[list->count] = list->used;
                    memcpy(list->strings + list->used, entry.cFileName, length);
                    ++list->count;
                    list->used += (int32_t)length;
                } else {
                    complete = 0;
                }
            }
        } while (FindNextFileA(search, &entry) != 0);
        (void)FindClose(search);

        if (complete != 0) return 1;
        if ((int64_t)initial_used + scan_bytes <= list->string_capacity &&
            (int64_t)initial_count + scan_count <= list->capacity) {
            list->count = initial_count;
            list->used = initial_used;
            return 0;
        }
        list->count = initial_count;
        list->used = initial_used;

        if (scan_bytes > list->string_capacity) {
            int32_t capacity;
            if (append == 0 && string_capacity_was_reset == 0) {
                free(list->strings);
                list->strings = NULL;
                list->string_capacity = 0;
                string_capacity_was_reset = 1;
            }
            if (scan_bytes > INT32_MAX - 1000 - list->string_capacity) {
                return 0;
            }
            capacity = scan_bytes + 1000 + list->string_capacity;
            replace_string_storage(list, capacity);
            if (list->string_capacity < capacity) return 0;
        }

        if (scan_count > list->capacity) {
            int32_t capacity;
            if (append == 0 && offset_capacity_was_reset == 0) {
                free(list->offsets);
                list->offsets = NULL;
                list->capacity = 0;
                offset_capacity_was_reset = 1;
            }
            if (scan_count > INT32_MAX - 100 - list->capacity) return 0;
            capacity = scan_count + 100 + list->capacity;
            replace_offset_storage(list, capacity);
            if (list->capacity < capacity) return 0;
        }
    }
}

static const char *sort_strings;

static int compare_names(const void *left, const void *right)
{
    const int32_t left_offset = *(const int32_t *)left;
    const int32_t right_offset = *(const int32_t *)right;
    const char *const left_name = sort_strings + left_offset;
    const char *const right_name = sort_strings + right_offset;
    return _stricmp(left_name, right_name);
}

void pharaoh_file_list_sort(PharaohFileList *list)
{
    if (list->offsets != NULL && list->strings != NULL && list->count > 1) {
        sort_strings = list->strings;
        qsort(list->offsets, (size_t)list->count, sizeof(*list->offsets),
            compare_names);
        sort_strings = NULL;
    }
}

const char *pharaoh_file_list_name(const PharaohFileList *list, int32_t index)
{
    if (index < 0 || index >= list->count) return NULL;
    return list->strings + list->offsets[index];
}
