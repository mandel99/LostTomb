#include "pharaoh/runtime/application_media_probe.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void fail_check(const char *expression, int line)
{
    (void)fprintf(stderr, "check failed on line %d: %s\n", line, expression);
    exit(1);
}

#define CHECK(expression) \
    do { \
        if (!(expression)) { \
            fail_check(#expression, __LINE__); \
        } \
    } while (0)

typedef struct FakeMediaApi {
    char cd_root[PHARAOH_STARTUP_MEDIA_ROOT_BYTES];
    uint32_t create_file_count;
    uint32_t close_count;
    uint32_t last_expected_size;
    uint32_t mismatch_expected_size;
    uint32_t mismatch_actual_size;
    int32_t fail_create_file;
    uintptr_t last_closed_handle;
} FakeMediaApi;

static uint32_t fake_get_drive_type(void *user_data, const char *root_path)
{
    const FakeMediaApi *fake = (const FakeMediaApi *)user_data;
    return strcmp(root_path, fake->cd_root) == 0 ?
        PHARAOH_STARTUP_MEDIA_DRIVE_TYPE_CDROM : 3;
}

static int32_t fake_get_volume_information(
    void *user_data, const char *root_path, char *volume_label,
    uint32_t volume_label_capacity, uint32_t *volume_serial_number,
    uint32_t *maximum_component_length, uint32_t *filesystem_flags,
    char *filesystem_name, uint32_t filesystem_name_capacity)
{
    (void)user_data;
    (void)root_path;
    CHECK(volume_label_capacity == PHARAOH_STARTUP_MEDIA_VOLUME_LABEL_BYTES);
    CHECK(filesystem_name_capacity ==
        PHARAOH_STARTUP_MEDIA_FILESYSTEM_NAME_ARGUMENT);
    (void)memcpy(volume_label, "Pharaoh", sizeof("Pharaoh"));
    *volume_serial_number = 1;
    *maximum_component_length = 255;
    *filesystem_flags = 0;
    (void)memcpy(filesystem_name, "CDFS", sizeof("CDFS"));
    return 1;
}

static int32_t fake_get_disk_free_space(
    void *user_data, const char *root_path,
    PharaohStartupMediaDiskSpace *disk_space)
{
    (void)user_data;
    (void)root_path;
    disk_space->sectors_per_cluster = 4;
    disk_space->bytes_per_sector = 512;
    disk_space->free_clusters = 100;
    disk_space->total_clusters = 1000;
    return 1;
}

static uint32_t size_for_path(const char *path)
{
    static const struct {
        const char *suffix;
        uint32_t size;
    } files[] = {
        { "binks\\high\\Intro_big.bik", UINT32_C(0x2d163ec) },
        { "audio\\wavs\\zoo_r.wav", UINT32_C(0x31962) },
        { "audio\\ambient\\hailstorm.mp3", UINT32_C(0x75680) },
        { "audio\\wavs\\bird_r.wav", UINT32_C(0x20c76) },
        { "binks\\high\\intro_big.bik", UINT32_C(0x2d163ec) },
        { "audio\\wavs\\antelop_r.wav", UINT32_C(0x7ff6) }
    };
    uint32_t i;

    for (i = 0; i < sizeof(files) / sizeof(files[0]); ++i) {
        const size_t path_length = strlen(path);
        const size_t suffix_length = strlen(files[i].suffix);
        if (path_length >= suffix_length &&
            strcmp(path + path_length - suffix_length, files[i].suffix) == 0) {
            return files[i].size;
        }
    }
    return 0;
}

static uintptr_t fake_create_file(
    void *user_data, const char *path, uint32_t desired_access,
    uint32_t share_mode, void *security_attributes,
    uint32_t creation_disposition, uint32_t flags_and_attributes,
    uintptr_t template_file)
{
    FakeMediaApi *fake = (FakeMediaApi *)user_data;
    CHECK(desired_access == PHARAOH_STARTUP_MEDIA_GENERIC_READ);
    CHECK(share_mode == PHARAOH_STARTUP_MEDIA_SHARE_READ);
    CHECK(security_attributes == NULL);
    CHECK(creation_disposition == PHARAOH_STARTUP_MEDIA_OPEN_EXISTING);
    CHECK(flags_and_attributes == PHARAOH_STARTUP_MEDIA_FILE_FLAGS);
    CHECK(template_file == 0);
    ++fake->create_file_count;
    fake->last_expected_size = size_for_path(path);
    return fake->fail_create_file != 0 ?
        PHARAOH_STARTUP_MEDIA_INVALID_HANDLE : (uintptr_t)123;
}

static uint32_t fake_get_file_size(void *user_data, uintptr_t file_handle)
{
    FakeMediaApi *fake = (FakeMediaApi *)user_data;
    CHECK(file_handle == (uintptr_t)123);
    if (fake->last_expected_size == fake->mismatch_expected_size) {
        return fake->mismatch_actual_size;
    }
    return fake->last_expected_size;
}

static int32_t fake_close_handle(void *user_data, uintptr_t handle)
{
    FakeMediaApi *fake = (FakeMediaApi *)user_data;
    ++fake->close_count;
    fake->last_closed_handle = handle;
    return handle != PHARAOH_STARTUP_MEDIA_INVALID_HANDLE;
}

static const PharaohStartupMediaProbeOperations fake_operations = {
    NULL,
    fake_get_drive_type,
    fake_get_volume_information,
    fake_get_disk_free_space,
    fake_create_file,
    fake_get_file_size,
    fake_close_handle
};

static void bind_fake(FakeMediaApi *fake,
                      PharaohStartupMediaProbeOperations *operations)
{
    *operations = fake_operations;
    operations->user_data = fake;
}

static void test_optional_cd_probe(void)
{
    FakeMediaApi fake = { "G:\\", 0, 0, 0, UINT32_MAX, 0, 0, 0 };
    PharaohStartupMediaProbeOperations operations;
    PharaohStartupMediaDriveState state = { { 'C', ':', '\\', '\0' } };
    uint8_t drive_letter = UINT8_C(0x5a);

    bind_fake(&fake, &operations);
    CHECK(pharaoh_application_probe_media_drive(&state, &operations,
        &drive_letter) == 1);
    CHECK(drive_letter == (uint8_t)'G');
    CHECK(fake.create_file_count == 6);
    CHECK(fake.close_count == 6);
}

static void test_same_drive_and_no_cd_are_nonfatal(void)
{
    FakeMediaApi fake = { "G:\\", 0, 0, 0, UINT32_MAX, 0, 0, 0 };
    PharaohStartupMediaProbeOperations operations;
    PharaohStartupMediaDriveState state = { { 'G', ':', '\\', '\0' } };
    uint8_t drive_letter = UINT8_C(0x5a);

    bind_fake(&fake, &operations);
    CHECK(pharaoh_application_probe_media_drive(&state, &operations,
        &drive_letter) == 0);
    CHECK(drive_letter == UINT8_C(0x5a));
    CHECK(fake.create_file_count == 1);

    fake.cd_root[0] = '\0';
    fake.create_file_count = 0;
    CHECK(pharaoh_application_probe_media_drive(&state, &operations,
        &drive_letter) == 0);
    CHECK(drive_letter == UINT8_C(0x5a));
    CHECK(fake.create_file_count == 0);
}

static void test_mismatched_file_size_fails_detection(void)
{
    FakeMediaApi fake = {
        "G:\\", 0, 0, 0, UINT32_C(0x31962), UINT32_C(0x31961), 0, 0
    };
    PharaohStartupMediaProbeOperations operations;
    PharaohStartupMediaDriveState state = { { 'C', ':', '\\', '\0' } };
    uint8_t drive_letter = UINT8_C(0x5a);

    bind_fake(&fake, &operations);
    CHECK(pharaoh_application_probe_media_drive(&state, &operations,
        &drive_letter) == 0);
    CHECK(drive_letter == UINT8_C(0x5a));
    CHECK(fake.create_file_count == 6);
}

static void test_file_open_failure_closes_invalid_handle(void)
{
    FakeMediaApi fake = { "G:\\", 0, 0, 0, 0, 0, 1, 0 };
    PharaohStartupMediaProbeOperations operations;

    bind_fake(&fake, &operations);
    CHECK(pharaoh_startup_media_check_file(&operations, "G:\\",
        "binks\\high\\Intro_big.bik", UINT32_C(0x2d163ec)) == 4);
    CHECK(fake.create_file_count == 1);
    CHECK(fake.close_count == 1);
    CHECK(fake.last_closed_handle == PHARAOH_STARTUP_MEDIA_INVALID_HANDLE);
}

int main(void)
{
    test_optional_cd_probe();
    test_same_drive_and_no_cd_are_nonfatal();
    test_mismatched_file_size_fails_detection();
    test_file_open_failure_closes_invalid_handle();
    return 0;
}
