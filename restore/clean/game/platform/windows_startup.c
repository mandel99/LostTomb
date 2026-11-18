#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <objbase.h>

#include <stdlib.h>
#include <string.h>

#include "pharaoh/platform/windows_startup.h"

static uint32_t get_current_directory(void *user_data,
    uint32_t buffer_capacity, char *buffer)
{
    (void)user_data;
    return GetCurrentDirectoryA(buffer_capacity, buffer);
}

static int32_t co_initialize(void *user_data, void *reserved)
{
    (void)user_data;
    return (int32_t)CoInitialize(reserved);
}

static PharaohVideoPlayer *allocate_video_player(void *user_data,
    uint32_t native_byte_count)
{
    (void)user_data;
    return (PharaohVideoPlayer *)malloc(native_byte_count);
}

static void release_video_player_storage(void *user_data,
    PharaohVideoPlayer *player)
{
    (void)user_data;
    free(player);
}

static void co_uninitialize(void *user_data)
{
    (void)user_data;
    CoUninitialize();
}

void pharaoh_windows_application_directory_operations_initialize(
    PharaohApplicationDirectoryOperations *operations)
{
    memset(operations, 0, sizeof(*operations));
    operations->get_current_directory = get_current_directory;
}

void pharaoh_windows_application_window_startup_operations_initialize(
    PharaohApplicationWindowStartupOperations *operations,
    const PharaohApplicationDirectoryOperations *directory_operations,
    const PharaohGraphicsStartupOperations *graphics_operations,
    const PharaohApplicationWindowOperations *window_operations)
{
    memset(operations, 0, sizeof(*operations));
    operations->directory_operations = directory_operations;
    operations->graphics_operations = graphics_operations;
    operations->window_operations = window_operations;
    operations->co_initialize = co_initialize;
    operations->allocate_video_player = allocate_video_player;
}

void pharaoh_windows_application_shutdown_operations_bind(
    PharaohApplicationShutdownOperations *operations)
{
    operations->release_video_player_storage = release_video_player_storage;
    operations->co_uninitialize = co_uninitialize;
}
