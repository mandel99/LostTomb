#ifndef PHARAOH_RUNTIME_SETTINGS_INI_H
#define PHARAOH_RUNTIME_SETTINGS_INI_H

#include "pharaoh/runtime/file_loading.h"

#include <stddef.h>
#include <stdint.h>

enum {
    PHARAOH_SETTINGS_PLAYER_NAME_BYTES = 32,
    PHARAOH_SETTINGS_PERSONAL_SAVINGS_MISSIONS = 98,
    PHARAOH_SETTINGS_INI_BUFFER_BYTES = 20000
};

typedef struct PharaohSettingsSavedWithGame {
    int32_t personal_savings[PHARAOH_SETTINGS_PERSONAL_SAVINGS_MISSIONS];
    int32_t victory_video_senate;
} PharaohSettingsSavedWithGame;

typedef struct PharaohSettingsFile {
    int32_t desktop_color_depth;
    int32_t fullscreen;
    uint8_t paused;
    uint8_t unused_a;
    uint8_t cd_drive_letter;
    uint8_t sound_effects_enabled;
    uint8_t sound_music_enabled;
    uint8_t sound_speech_enabled;
    uint8_t reserved_1[2];
    int32_t display_mode;
    int32_t game_speed;
    int32_t scroll_speed;
    char player_name[PHARAOH_SETTINGS_PLAYER_NAME_BYTES];
    int32_t debug_grid_overlay;
    uint8_t reserved_2[12];
    int32_t last_advisor;
    int8_t mission_id;
    uint8_t campaign;
    uint8_t previous_campaign_link;
    int8_t campaign_link;
    int32_t tooltips;
    int32_t starting_favor;
    int32_t personal_savings_last_mission;
    int32_t current_mission_id;
    int32_t is_custom_scenario;
    uint8_t sound_city_enabled;
    uint8_t warnings;
    uint8_t monthly_autosave;
    uint8_t pyramid_speedup;
    int32_t sound_effects_volume;
    int32_t sound_music_volume;
    int32_t sound_speech_volume;
    int32_t sound_city_volume;
    int32_t unused_b;
    int32_t ram;
    int32_t cpu;
    int32_t unused_c;
    int32_t display_mode_start;
    uint8_t reserved_3[4];
    int32_t autosave;
    uint32_t message_filter;
    PharaohSettingsSavedWithGame saved_with_game;
    int32_t difficulty;
    int32_t gods_enabled;
    uint8_t classical_names;
    uint8_t check_cd;
    uint8_t reserved_4[2];
} PharaohSettingsFile;

_Static_assert(sizeof(PharaohSettingsFile) == 0x234,
               "Pharaoh.inf settings record size");
_Static_assert(offsetof(PharaohSettingsFile, player_name) == 0x1c &&
               offsetof(PharaohSettingsFile, ram) == 0x80 &&
               offsetof(PharaohSettingsFile, cpu) == 0x84 &&
               offsetof(PharaohSettingsFile, check_cd) == 0x231,
               "Pharaoh.inf settings field offsets");

typedef struct PharaohSettingsIniOperations {
    void *(*allocate)(void *user_data, uint32_t byte_count);
    void (*release)(void *user_data, void *allocation);
    void (*show_error_status)(void *user_data, const char *message);
} PharaohSettingsIniOperations;

typedef struct PharaohSettingsIniContext {
    const PharaohFileReadContext *files;
    const PharaohSettingsIniOperations *operations;
    void *user_data;
    PharaohSettingsFile *settings;
} PharaohSettingsIniContext;

int32_t pharaoh_ini_find_key(const char *buffer, const char *key,
                             int32_t search_bytes);
int32_t pharaoh_ini_find_byte(const char *buffer, char value,
                              int32_t search_bytes);
int32_t pharaoh_ini_compare_bytes(const char *left, const char *right,
                                  int32_t byte_count);
int32_t pharaoh_ini_parse_integer(const char *text);
int32_t pharaoh_ini_to_integer(const char *text);
int32_t pharaoh_settings_read_ini(const PharaohSettingsIniContext *context);

#endif
