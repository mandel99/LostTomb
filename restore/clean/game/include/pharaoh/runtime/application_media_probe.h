#ifndef PHARAOH_RUNTIME_APPLICATION_MEDIA_PROBE_H
#define PHARAOH_RUNTIME_APPLICATION_MEDIA_PROBE_H

#include <stdint.h>

enum {
    PHARAOH_STARTUP_MEDIA_ROOT_BYTES = 4,
    PHARAOH_STARTUP_MEDIA_PATH_BYTES = 256,
    PHARAOH_STARTUP_MEDIA_VOLUME_LABEL_BYTES = 200,
    PHARAOH_STARTUP_MEDIA_FILESYSTEM_NAME_BYTES = 52,
    PHARAOH_STARTUP_MEDIA_FILESYSTEM_NAME_ARGUMENT = 50,
    PHARAOH_STARTUP_MEDIA_DRIVE_TYPE_CDROM = 5,
    PHARAOH_STARTUP_MEDIA_OPEN_EXISTING = 3,
    PHARAOH_STARTUP_MEDIA_SHARE_READ = 1
};

#define PHARAOH_STARTUP_MEDIA_GENERIC_READ UINT32_C(0x80000000)
#define PHARAOH_STARTUP_MEDIA_FILE_FLAGS UINT32_C(0x60000001)
#define PHARAOH_STARTUP_MEDIA_INVALID_HANDLE UINTPTR_MAX
#define PHARAOH_STARTUP_MEDIA_FILE_SIZE_ERROR UINT32_MAX

typedef struct PharaohStartupMediaDiskSpace {
    uint32_t sectors_per_cluster;
    uint32_t bytes_per_sector;
    uint32_t free_clusters;
    uint32_t total_clusters;
} PharaohStartupMediaDiskSpace;

typedef struct PharaohStartupMediaDriveState {
    /* Original DAT_007027D4: the pre-scan string copied to detect whether
       0x00426810 replaced it. Its wider application meaning is unresolved. */
    char probe_seed[PHARAOH_STARTUP_MEDIA_ROOT_BYTES];
} PharaohStartupMediaDriveState;

typedef struct PharaohStartupMediaProbeOperations {
    void *user_data;
    uint32_t (*get_drive_type)(void *user_data, const char *root_path);
    int32_t (*get_volume_information)(
        void *user_data, const char *root_path, char *volume_label,
        uint32_t volume_label_capacity, uint32_t *volume_serial_number,
        uint32_t *maximum_component_length, uint32_t *filesystem_flags,
        char *filesystem_name, uint32_t filesystem_name_capacity);
    int32_t (*get_disk_free_space)(
        void *user_data, const char *root_path,
        PharaohStartupMediaDiskSpace *disk_space);
    uintptr_t (*create_file)(
        void *user_data, const char *path, uint32_t desired_access,
        uint32_t share_mode, void *security_attributes,
        uint32_t creation_disposition, uint32_t flags_and_attributes,
        uintptr_t template_file);
    uint32_t (*get_file_size)(void *user_data, uintptr_t file_handle);
    int32_t (*close_handle)(void *user_data, uintptr_t handle);
} PharaohStartupMediaProbeOperations;

/* Original 0x004265D0. Returns the native status code for one media file. */
uint32_t pharaoh_startup_media_check_file(
    const PharaohStartupMediaProbeOperations *operations,
    const char *drive_root, const char *relative_path,
    uint32_t expected_file_size);

/* Original 0x00426810. Starts with the previously selected drive root and
   replaces it with the first CD drive containing the expected intro movie. */
void pharaoh_startup_media_find_drive(
    const PharaohStartupMediaDriveState *state,
    const PharaohStartupMediaProbeOperations *operations,
    char drive_root[PHARAOH_STARTUP_MEDIA_ROOT_BYTES]);

/* Original 0x004268C0. Detects the optional media drive. A missing CD or
   mismatched file leaves detected_drive_letter unchanged and is not fatal. */
uint32_t pharaoh_application_probe_media_drive(
    const PharaohStartupMediaDriveState *state,
    const PharaohStartupMediaProbeOperations *operations,
    uint8_t *detected_drive_letter);

#endif
