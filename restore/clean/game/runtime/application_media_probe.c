#include "pharaoh/runtime/application_media_probe.h"

#include <string.h>

enum {
    STARTUP_MEDIA_DRIVE_FIRST = 'A',
    STARTUP_MEDIA_DRIVE_LAST = 'Z',
    STARTUP_MEDIA_DISK_FREE_CLUSTERS_ZERO = 0,
    STARTUP_MEDIA_DISK_FREE_CLUSTERS_INITIAL = 4
};

#define STARTUP_MEDIA_CAPACITY_UPPER_BOUND UINT32_C(0x29dfffff)
#define STARTUP_MEDIA_CAPACITY_LOWER_BOUND UINT32_C(0x2ad00001)

static const char required_filesystem_name[] = "CDFS";
static const char intro_movie_path[] = "binks\\high\\Intro_big.bik";

static const struct PharaohStartupMediaFile {
    const char *path;
    uint32_t size;
} required_media_files[] = {
    { "audio\\wavs\\zoo_r.wav", UINT32_C(0x31962) },
    { "audio\\ambient\\hailstorm.mp3", UINT32_C(0x75680) },
    { "audio\\wavs\\bird_r.wav", UINT32_C(0x20c76) },
    { "binks\\high\\intro_big.bik", UINT32_C(0x2d163ec) },
    { "audio\\wavs\\antelop_r.wav", UINT32_C(0x7ff6) }
};

static int ascii_case_equal(const char *left, const char *right)
{
    while (*left != '\0' && *right != '\0') {
        unsigned char left_character = (unsigned char)*left++;
        unsigned char right_character = (unsigned char)*right++;

        if (left_character >= (unsigned char)'A' &&
            left_character <= (unsigned char)'Z') {
            left_character = (unsigned char)(left_character +
                ((unsigned char)'a' - (unsigned char)'A'));
        }
        if (right_character >= (unsigned char)'A' &&
            right_character <= (unsigned char)'Z') {
            right_character = (unsigned char)(right_character +
                ((unsigned char)'a' - (unsigned char)'A'));
        }
        if (left_character != right_character) {
            return 0;
        }
    }
    return *left == *right;
}

static void set_drive_root(char root[PHARAOH_STARTUP_MEDIA_ROOT_BYTES],
                           char drive_letter)
{
    root[0] = drive_letter;
    root[1] = ':';
    root[2] = '\\';
    root[3] = '\0';
}

static void join_media_path(char path[PHARAOH_STARTUP_MEDIA_PATH_BYTES],
                            const char *drive_root,
                            const char *relative_path)
{
    const size_t root_bytes = strlen(drive_root);

    memcpy(path, drive_root, root_bytes);
    memcpy(path + root_bytes, relative_path, strlen(relative_path) + 1);
}

uint32_t pharaoh_startup_media_check_file(
    const PharaohStartupMediaProbeOperations *operations,
    const char *drive_root, const char *relative_path,
    uint32_t expected_file_size)
{
    char volume_label[PHARAOH_STARTUP_MEDIA_VOLUME_LABEL_BYTES] = { 0 };
    char filesystem_name[PHARAOH_STARTUP_MEDIA_FILESYSTEM_NAME_BYTES] = { 0 };
    uint32_t volume_serial_number = UINT32_MAX;
    uint32_t maximum_component_length = UINT32_MAX;
    uint32_t filesystem_flags = UINT32_MAX;
    PharaohStartupMediaDiskSpace disk_space = {
        UINT32_MAX,
        UINT32_MAX,
        STARTUP_MEDIA_DISK_FREE_CLUSTERS_INITIAL,
        UINT32_MAX
    };
    char path[PHARAOH_STARTUP_MEDIA_PATH_BYTES] = { 0 };
    uintptr_t file_handle;
    uint32_t file_size;
    uint32_t total_capacity_bytes;
    uint32_t result;

    if (operations->get_drive_type(operations->user_data, drive_root) !=
        PHARAOH_STARTUP_MEDIA_DRIVE_TYPE_CDROM) {
        return UINT32_MAX;
    }

    (void)operations->get_volume_information(operations->user_data,
        drive_root, volume_label, PHARAOH_STARTUP_MEDIA_VOLUME_LABEL_BYTES,
        &volume_serial_number, &maximum_component_length, &filesystem_flags,
        filesystem_name, PHARAOH_STARTUP_MEDIA_FILESYSTEM_NAME_ARGUMENT);
    if (ascii_case_equal(required_filesystem_name, filesystem_name) == 0) {
        return 0;
    }

    (void)operations->get_disk_free_space(operations->user_data, drive_root,
        &disk_space);
    if (disk_space.free_clusters == STARTUP_MEDIA_DISK_FREE_CLUSTERS_ZERO) {
        return 1;
    }

    join_media_path(path, drive_root, relative_path);
    file_handle = operations->create_file(operations->user_data, path,
        PHARAOH_STARTUP_MEDIA_GENERIC_READ,
        PHARAOH_STARTUP_MEDIA_SHARE_READ, NULL,
        PHARAOH_STARTUP_MEDIA_OPEN_EXISTING,
        PHARAOH_STARTUP_MEDIA_FILE_FLAGS, (uintptr_t)0);
    if (file_handle == PHARAOH_STARTUP_MEDIA_INVALID_HANDLE) {
        (void)operations->close_handle(operations->user_data, file_handle);
        return 4;
    }

    file_size = operations->get_file_size(operations->user_data, file_handle);
    if (file_size == PHARAOH_STARTUP_MEDIA_FILE_SIZE_ERROR) {
        (void)operations->close_handle(operations->user_data, file_handle);
        return 5;
    }
    if (file_size != expected_file_size) {
        (void)operations->close_handle(operations->user_data, file_handle);
        return 6;
    }
    (void)operations->close_handle(operations->user_data, file_handle);

    total_capacity_bytes = disk_space.total_clusters *
        disk_space.bytes_per_sector * disk_space.sectors_per_cluster;
    if (total_capacity_bytes > STARTUP_MEDIA_CAPACITY_UPPER_BOUND ||
        total_capacity_bytes < STARTUP_MEDIA_CAPACITY_LOWER_BOUND) {
        result = 7;
    } else {
        result = 10;
    }
    return result;
}

void pharaoh_startup_media_find_drive(
    const PharaohStartupMediaDriveState *state,
    const PharaohStartupMediaProbeOperations *operations,
    char drive_root[PHARAOH_STARTUP_MEDIA_ROOT_BYTES])
{
    char candidate_root[PHARAOH_STARTUP_MEDIA_ROOT_BYTES];
    int drive_letter;

    memcpy(drive_root, state->probe_seed,
        PHARAOH_STARTUP_MEDIA_ROOT_BYTES);
    for (drive_letter = STARTUP_MEDIA_DRIVE_FIRST;
         drive_letter <= STARTUP_MEDIA_DRIVE_LAST; ++drive_letter) {
        set_drive_root(candidate_root, (char)drive_letter);
        if (operations->get_drive_type(operations->user_data, candidate_root) ==
                PHARAOH_STARTUP_MEDIA_DRIVE_TYPE_CDROM &&
            pharaoh_startup_media_check_file(operations, candidate_root,
                intro_movie_path, UINT32_C(0x2d163ec)) == 7) {
            memcpy(drive_root, candidate_root,
                PHARAOH_STARTUP_MEDIA_ROOT_BYTES);
            return;
        }
    }
}

uint32_t pharaoh_application_probe_media_drive(
    const PharaohStartupMediaDriveState *state,
    const PharaohStartupMediaProbeOperations *operations,
    uint8_t *detected_drive_letter)
{
    char drive_root[PHARAOH_STARTUP_MEDIA_ROOT_BYTES];
    uint32_t file_results;
    uint32_t i;

    memset(drive_root, 0, sizeof(drive_root));
    pharaoh_startup_media_find_drive(state, operations, drive_root);
    if (strcmp(drive_root, state->probe_seed) == 0) {
        return 0;
    }

    file_results = 7;
    for (i = 0; i < sizeof(required_media_files) /
            sizeof(required_media_files[0]); ++i) {
        const uint32_t file_result = pharaoh_startup_media_check_file(
            operations, drive_root, required_media_files[i].path,
            required_media_files[i].size);
        file_results &= file_result;
    }
    if (file_results != 7) {
        return 0;
    }

    *detected_drive_letter = (uint8_t)drive_root[0];
    return 1;
}
