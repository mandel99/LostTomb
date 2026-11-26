#ifndef PHARAOH_SCENARIO_MAP_FILE_EXISTS_H
#define PHARAOH_SCENARIO_MAP_FILE_EXISTS_H

#include <stdbool.h>

typedef struct PharaohMapFileExistsContext {
    void *user_data;
    void (*normalize_name)(void *user_data, char *path);
    void (*prepare_extension)(void *user_data);
    void (*finalize_name)(void *user_data, char *path);
    int (*file_exists)(void *user_data, const char *path);
} PharaohMapFileExistsContext;

bool pharaoh_scenario_map_exists(const PharaohMapFileExistsContext *context,
    char *path);

#endif
