#ifndef PHARAOH_RUNTIME_SETTINGS_LOAD_H
#define PHARAOH_RUNTIME_SETTINGS_LOAD_H

#include "pharaoh/runtime/settings_ini.h"

enum {
    PHARAOH_SCREEN_METRIC_PRIMARY_WIDTH = 0,
    PHARAOH_SETTINGS_MINIMUM_RAM_FOR_DEFAULT_SPEED = 24,
    PHARAOH_SETTINGS_SCREEN_WIDTH_LOW = 800,
    PHARAOH_SETTINGS_SCREEN_WIDTH_MEDIUM = 1024
};

typedef struct PharaohSettingsMenuIndexes {
    int32_t tooltips_primary;
    int32_t tooltips_secondary;
    int32_t warnings;
    int32_t monthly_autosave;
    int32_t pyramid_speedup;
    int32_t classical_names;
} PharaohSettingsMenuIndexes;

typedef struct PharaohSettingsLoadOperations {
    int32_t (*get_system_metric)(void *user_data, int32_t metric);
    void (*start_campaign)(void *user_data, PharaohSettingsFile *settings,
                           int32_t campaign_id);
    void (*set_startup_finalization_flag)(void *user_data, uint8_t value);
} PharaohSettingsLoadOperations;

typedef struct PharaohSettingsLoadContext {
    const PharaohSettingsIniOperations *ini_operations;
    const PharaohFileReadContext *files;
    const PharaohSettingsLoadOperations *operations;
    void *user_data;
    void *ini_user_data;
    PharaohSettingsFile *settings;
    PharaohSettingsMenuIndexes *menu_indexes;
} PharaohSettingsLoadContext;

int32_t pharaoh_settings_load(const PharaohSettingsLoadContext *context);

#endif
