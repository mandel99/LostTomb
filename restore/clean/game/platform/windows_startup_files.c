#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pharaoh/platform/windows_startup_files.h"

struct PharaohTextFile {
    int descriptor;
};

static PharaohReadOnlyFile *open_read_file(void *user_data,
    const char *filename)
{
    FILE *file = NULL;
    (void)user_data;
    (void)fopen_s(&file, filename, "rb");
    return (PharaohReadOnlyFile *)file;
}

static size_t read_file_size(void *user_data, PharaohReadOnlyFile *file)
{
    (void)user_data;
    return (size_t)_filelength(_fileno((FILE *)file));
}

static size_t read_file_elements(void *user_data, void *destination,
    size_t element_size, size_t element_count, PharaohReadOnlyFile *file)
{
    (void)user_data;
    return fread(destination, element_size, element_count, (FILE *)file);
}

static int32_t seek_read_file(void *user_data, PharaohReadOnlyFile *file,
    int32_t offset, int32_t origin)
{
    (void)user_data;
    return fseek((FILE *)file, offset, origin);
}

static void close_read_file(void *user_data, PharaohReadOnlyFile *file)
{
    (void)user_data;
    (void)fclose((FILE *)file);
}

static int32_t open_file_size_handle(void *user_data, const char *filename,
    uint32_t native_flags)
{
    HANDLE file;
    SECURITY_ATTRIBUTES security;
    (void)user_data;
    if (native_flags != PHARAOH_FILE_OPEN_FLAGS_NATIVE_8000) {
        return -1;
    }
    security.nLength = sizeof(security);
    security.lpSecurityDescriptor = NULL;
    security.bInheritHandle = TRUE;
    file = CreateFileA(filename, GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE, &security, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        return -1;
    }
    return (int32_t)(uintptr_t)file;
}

static int32_t get_file_size_handle(void *user_data, int32_t file_handle)
{
    const DWORD size = GetFileSize((HANDLE)(uintptr_t)(uint32_t)file_handle,
        NULL);
    (void)user_data;
    return size == INVALID_FILE_SIZE ? -1 : (int32_t)size;
}

static int32_t close_file_size_handle(void *user_data, int32_t file_handle)
{
    (void)user_data;
    return CloseHandle((HANDLE)(uintptr_t)(uint32_t)file_handle) != 0;
}

_Static_assert(FILE_BEGIN == SEEK_SET && FILE_CURRENT == SEEK_CUR &&
    FILE_END == SEEK_END, "native seek origins must match Win32 file origins");

int32_t pharaoh_windows_startup_handle_read(int32_t handle,
    void *destination, uint32_t byte_count)
{
    DWORD bytes_read = 0;
    if (ReadFile((HANDLE)(uintptr_t)(uint32_t)handle, destination,
            byte_count, &bytes_read, NULL) == 0) return -1;
    return (int32_t)bytes_read;
}

int32_t pharaoh_windows_startup_handle_seek(int32_t handle,
    int32_t offset, int32_t origin)
{
    return (int32_t)SetFilePointer((HANDLE)(uintptr_t)(uint32_t)handle,
        offset, NULL, (DWORD)origin);
}

static PharaohTextFile *open_text_file(void *user_data,
    const char *filename, uint32_t flags)
{
    (void)user_data;
    const int descriptor = _open(filename, (int)flags);
    if (descriptor == -1) {
        return NULL;
    }
    PharaohTextFile *const file = (PharaohTextFile *)malloc(sizeof(*file));
    if (file == NULL) {
        (void)_close(descriptor);
        return NULL;
    }
    file->descriptor = descriptor;
    return file;
}

static uint32_t text_file_size(void *user_data, PharaohTextFile *file)
{
    (void)user_data;
    return (uint32_t)_filelength(file->descriptor);
}

static int32_t read_text_file(void *user_data, PharaohTextFile *file,
    void *destination, uint32_t byte_count)
{
    (void)user_data;
    return _read(file->descriptor, destination, byte_count);
}

static void close_text_file(void *user_data, PharaohTextFile *file)
{
    (void)user_data;
    (void)_close(file->descriptor);
    free(file);
}

static void *allocate_bytes(void *user_data, uint32_t byte_count)
{
    (void)user_data;
    return malloc(byte_count);
}

static void release_bytes(void *user_data, void *allocation)
{
    (void)user_data;
    free(allocation);
}

static void report_ini_error(void *user_data, const char *message)
{
    const PharaohWindowsStartupFileStatus *const status =
        (const PharaohWindowsStartupFileStatus *)user_data;
    pharaoh_status_log_format(message, NULL, 0, status->state,
        status->operations);
}

static uint32_t drive_type(void *user_data, const char *root_path)
{
    (void)user_data;
    return GetDriveTypeA(root_path);
}

static int32_t volume_information(void *user_data, const char *root_path,
    char *label, uint32_t label_capacity, uint32_t *serial,
    uint32_t *maximum_component_length, uint32_t *filesystem_flags,
    char *filesystem_name, uint32_t filesystem_name_capacity)
{
    DWORD native_serial = 0;
    DWORD native_maximum_length = 0;
    DWORD native_flags = 0;
    (void)user_data;
    const BOOL result = GetVolumeInformationA(root_path, label,
        label_capacity, &native_serial, &native_maximum_length,
        &native_flags, filesystem_name, filesystem_name_capacity);
    if (result != 0) {
        *serial = native_serial;
        *maximum_component_length = native_maximum_length;
        *filesystem_flags = native_flags;
    }
    return result != 0;
}

static int32_t disk_free_space(void *user_data, const char *root_path,
    PharaohStartupMediaDiskSpace *space)
{
    DWORD sectors = 0;
    DWORD bytes = 0;
    DWORD free_clusters = 0;
    DWORD total_clusters = 0;
    (void)user_data;
    const BOOL result = GetDiskFreeSpaceA(root_path, &sectors, &bytes,
        &free_clusters, &total_clusters);
    if (result != 0) {
        space->sectors_per_cluster = sectors;
        space->bytes_per_sector = bytes;
        space->free_clusters = free_clusters;
        space->total_clusters = total_clusters;
    }
    return result != 0;
}

static uintptr_t create_file(void *user_data, const char *path,
    uint32_t access, uint32_t share_mode, void *security_attributes,
    uint32_t disposition, uint32_t flags, uintptr_t template_file)
{
    (void)user_data;
    return (uintptr_t)CreateFileA(path, access, share_mode,
        (LPSECURITY_ATTRIBUTES)security_attributes, disposition, flags,
        (HANDLE)template_file);
}

static uint32_t file_size(void *user_data, uintptr_t handle)
{
    (void)user_data;
    return GetFileSize((HANDLE)handle, NULL);
}

static int32_t close_handle(void *user_data, uintptr_t handle)
{
    (void)user_data;
    return CloseHandle((HANDLE)handle) != 0;
}

void pharaoh_windows_startup_file_operations_bind(
    PharaohFileReadOperations *files,
    PharaohFileSizeOperations *file_sizes,
    PharaohTextFileOperations *texts,
    PharaohSettingsIniOperations *ini,
    PharaohStartupMediaProbeOperations *media)
{
    memset(files, 0, sizeof(*files));
    files->open = open_read_file;
    files->size = read_file_size;
    files->read = read_file_elements;
    files->seek = seek_read_file;
    files->close = close_read_file;

    memset(file_sizes, 0, sizeof(*file_sizes));
    file_sizes->open_file = open_file_size_handle;
    file_sizes->get_file_size = get_file_size_handle;
    file_sizes->close_file = close_file_size_handle;

    memset(texts, 0, sizeof(*texts));
    texts->open = open_text_file;
    texts->size = text_file_size;
    texts->read_text = read_text_file;
    texts->close = close_text_file;
    texts->allocate = allocate_bytes;
    texts->release = release_bytes;

    memset(ini, 0, sizeof(*ini));
    ini->allocate = allocate_bytes;
    ini->release = release_bytes;
    ini->show_error_status = report_ini_error;

    memset(media, 0, sizeof(*media));
    media->get_drive_type = drive_type;
    media->get_volume_information = volume_information;
    media->get_disk_free_space = disk_free_space;
    media->create_file = create_file;
    media->get_file_size = file_size;
    media->close_handle = close_handle;
}
