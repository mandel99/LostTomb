#include "pharaoh/platform/windows_smacker.h"

#include <ddraw.h>
#include <stdio.h>
#include <string.h>

enum {
    SMACKER_PATH_CAPACITY = 64,
    SMACKER_MOVIE_HEIGHT_OFFSET = 0x08,
    SMACKER_FRAME_COUNT_OFFSET = 0x0c,
    SMACKER_FRAME_INDEX_OFFSET = 0x374
};

_Static_assert(sizeof(DDSURFACEDESC) == 0x6c,
    "Smacker lock description uses the native 108-byte DirectDraw layout");
_Static_assert(sizeof(MEMORYSTATUS) == 0x20,
    "native GlobalMemoryStatus call uses the 32-byte Win32 structure");

static void report_status(PharaohWindowsSmacker *backend,
    const char *message, const char *detail, int32_t value)
{
    pharaoh_status_log_format(message, detail, value, backend->status_state,
        backend->status_operations);
}

static void bind_sound_to_miles(void *user_data, void *audio_driver)
{
    PharaohWindowsSmacker *const backend = user_data;
    (void)backend->sound_use_miles(audio_driver);
}

static void report_movie_start(void *user_data, const char *filename)
{
    report_status(user_data, "OK: SMK starting smack", filename, 0);
}

static void make_game_media_path(void *user_data, const char *filename,
    char *path, uint32_t path_capacity)
{
    (void)user_data;
    if (path_capacity != 0) {
        (void)snprintf(path, path_capacity, "%s", filename);
    }
}

static int32_t probe_media_path(void *user_data, const char *path)
{
    FILE *file = NULL;
    (void)user_data;
    if (fopen_s(&file, path, "rb") != 0 || file == NULL) {
        return 0;
    }
    (void)fclose(file);
    return 1;
}

static void resolve_media_path(void *user_data, char *path,
    uint32_t path_capacity)
{
    PharaohWindowsSmacker *const backend = user_data;
    if (backend->resolve_media_path != NULL) {
        backend->resolve_media_path(backend->media_user_data, path,
            path_capacity);
    }
}

static void *open_movie(void *user_data, const char *path,
    uint32_t sound_flags, uint32_t first_frame)
{
    PharaohWindowsSmacker *const backend = user_data;
    backend->movie_handle = backend->open_movie(path, sound_flags,
        first_frame);
    return backend->movie_handle;
}

static void close_media_resources(void *user_data)
{
    PharaohWindowsSmacker *const backend = user_data;
    if (backend->close_media_resources != NULL) {
        backend->close_media_resources(backend->media_user_data);
    }
}

static void report_movie_open_failure(void *user_data, const char *path)
{
    report_status(user_data, "ERR: SMK Could not open", path, 0);
}

static int32_t available_memory(void *user_data)
{
    MEMORYSTATUS memory = { 0 };
    (void)user_data;
    memory.dwLength = sizeof(memory);
    GlobalMemoryStatus(&memory);
    return (int32_t)memory.dwAvailPageFile;
}

static void report_memory_after_open_failure(void *user_data, int32_t value)
{
    report_status(user_data, "MEM: Max alloc after:", NULL, value);
}

static int32_t wait_for_movie(void *user_data, void *movie)
{
    PharaohWindowsSmacker *const backend = user_data;
    return backend->wait_for_movie(movie);
}

static LPDIRECTDRAWSURFACE smacker_surface(
    PharaohWindowsSmacker *backend, int32_t surface)
{
    return (LPDIRECTDRAWSURFACE)(surface == PHARAOH_SMACKER_FRONT_SURFACE
        ? backend->graphics->primary_surface
        : backend->graphics->back_buffer_surface);
}

static int32_t lock_surface(void *user_data, int32_t surface,
    PharaohSmackerSurfaceDescription *description,
    PharaohSmackerLockedSurface *locked_surface)
{
    PharaohWindowsSmacker *const backend = user_data;
    DDSURFACEDESC *const native_description = (DDSURFACEDESC *)description;
    LPDIRECTDRAWSURFACE draw_surface = smacker_surface(backend, surface);
    const HRESULT result = draw_surface->lpVtbl->Lock(draw_surface, NULL,
        native_description, DDLOCK_WAIT, NULL);
    if (result == DD_OK) {
        locked_surface->destination_pitch = native_description->lPitch;
        locked_surface->surface_pixels = native_description->lpSurface;
        memcpy(&locked_surface->movie_height,
            (const uint8_t *)backend->movie_handle +
                SMACKER_MOVIE_HEIGHT_OFFSET,
            sizeof(locked_surface->movie_height));
    }
    return (int32_t)result;
}

static int32_t wait_for_surface_recovery(void *user_data, int32_t surface)
{
    PharaohWindowsSmacker *const backend = user_data;
    const HRESULT result = smacker_surface(backend, surface)->lpVtbl->Restore(
        smacker_surface(backend, surface));
        return result == DD_OK ? 0 : 1;
}

static int32_t convert_surface_error(void *user_data, int32_t result)
{
    (void)user_data;
    return (int32_t)pharaoh_graphics_hresult_error_character(result);
}

static void report_surface_lock_failure(void *user_data, int32_t value)
{
    report_status(user_data,
        "ERR: SMK releasing write to primary failed:", NULL, value);
}

static void smack_to_buffer(void *user_data, void *movie, int32_t x,
    int32_t y, int32_t destination_pitch, uint32_t movie_height,
    void *surface_pixels, uint32_t surface_buffer_mode)
{
    PharaohWindowsSmacker *const backend = user_data;
    backend->to_buffer(movie, x, y, destination_pitch, movie_height,
        surface_pixels, surface_buffer_mode);
}

static void smack_do_frame(void *user_data, void *movie)
{
    PharaohWindowsSmacker *const backend = user_data;
    backend->do_frame(movie);
}

static void unlock_surface(void *user_data, int32_t surface,
    void *surface_pixels)
{
    LPDIRECTDRAWSURFACE draw_surface = smacker_surface(user_data, surface);
    (void)draw_surface->lpVtbl->Unlock(draw_surface, surface_pixels);
}

static PharaohSmackerFramePosition get_frame_position(void *user_data,
    void *movie)
{
    PharaohSmackerFramePosition position;
    const uint8_t *const movie_bytes = movie;
    (void)user_data;
    memcpy(&position.frame_count, movie_bytes + SMACKER_FRAME_COUNT_OFFSET,
        sizeof(position.frame_count));
    memcpy(&position.frame_index, movie_bytes + SMACKER_FRAME_INDEX_OFFSET,
        sizeof(position.frame_index));
    return position;
}

static void smack_next_frame(void *user_data, void *movie)
{
    PharaohWindowsSmacker *const backend = user_data;
    backend->next_frame(movie);
}

static void close_movie(void *user_data, void *movie)
{
    PharaohWindowsSmacker *const backend = user_data;
    backend->close_movie(movie);
    if (backend->movie_handle == movie) {
        backend->movie_handle = NULL;
    }
}

static void report_natural_end(void *user_data, const char *path)
{
    report_status(user_data, "OK: SMK natural end of", path, 0);
}

static void report_memory_after_natural_end(void *user_data, int32_t value)
{
    report_status(user_data, "MEM: Max alloc after:", NULL, value);
}

static void report_user_stop(void *user_data, const char *path)
{
    report_status(user_data, "OK: SMK user stop:", path, 0);
}

static void report_memory_after_stop(void *user_data, int32_t value)
{
    report_status(user_data, "MEM: Max alloc after:", NULL, value);
}

int32_t pharaoh_windows_smacker_open_library(PharaohWindowsSmacker *backend)
{
    memset(backend, 0, sizeof(*backend));
    backend->module = LoadLibraryA("SMACKW32.DLL");
    if (backend->module == NULL) {
        return 0;
    }

#define BIND_SMACKER(field, export_name) do { \
    FARPROC address = GetProcAddress(backend->module, export_name); \
    if (address == NULL) goto missing_export; \
    memcpy(&backend->field, &address, sizeof(backend->field)); \
} while (0)
    BIND_SMACKER(sound_use_miles, "_SmackSoundUseMSS@4");
    BIND_SMACKER(open_movie, "_SmackOpen@12");
    BIND_SMACKER(wait_for_movie, "_SmackWait@4");
    BIND_SMACKER(to_buffer, "_SmackToBuffer@28");
    BIND_SMACKER(do_frame, "_SmackDoFrame@4");
    BIND_SMACKER(next_frame, "_SmackNextFrame@4");
    BIND_SMACKER(close_movie, "_SmackClose@4");
#undef BIND_SMACKER
    return 1;

missing_export:
    pharaoh_windows_smacker_close_library(backend);
    return 0;
}

void pharaoh_windows_smacker_close_library(PharaohWindowsSmacker *backend)
{
    if (backend->module != NULL) {
        (void)FreeLibrary(backend->module);
    }
    memset(backend, 0, sizeof(*backend));
}

void pharaoh_windows_smacker_bind_operations(
    PharaohWindowsSmacker *backend,
    PharaohSmackerMovieOpenOperations *movie_open,
    PharaohSmackerVideoDrawOperations *video_draw,
    PharaohSmackerWindowVideoStopOperations *video_stop)
{
    memset(movie_open, 0, sizeof(*movie_open));
    movie_open->user_data = backend;
    movie_open->bind_sound_to_miles = bind_sound_to_miles;
    movie_open->report_movie_start = report_movie_start;
    movie_open->make_game_media_path = make_game_media_path;
    movie_open->probe_media_path = probe_media_path;
    movie_open->resolve_media_path = resolve_media_path;
    movie_open->open_movie = open_movie;
    movie_open->close_media_resources = close_media_resources;
    movie_open->report_movie_open_failure = report_movie_open_failure;
    movie_open->available_memory = available_memory;
    movie_open->report_memory_after_open_failure =
        report_memory_after_open_failure;
    movie_open->video_draw_operations = video_draw;

    memset(video_draw, 0, sizeof(*video_draw));
    video_draw->user_data = backend;
    video_draw->wait_for_movie = wait_for_movie;
    video_draw->lock_surface = lock_surface;
    video_draw->wait_for_surface_recovery = wait_for_surface_recovery;
    video_draw->convert_surface_error = convert_surface_error;
    video_draw->report_surface_lock_failure = report_surface_lock_failure;
    video_draw->smack_to_buffer = smack_to_buffer;
    video_draw->smack_do_frame = smack_do_frame;
    video_draw->unlock_surface = unlock_surface;
    video_draw->get_frame_position = get_frame_position;
    video_draw->smack_next_frame = smack_next_frame;
    video_draw->close_movie = close_movie;
    video_draw->close_media_resources = close_media_resources;
    video_draw->report_natural_end = report_natural_end;
    video_draw->available_memory = available_memory;
    video_draw->report_memory_after_natural_end =
        report_memory_after_natural_end;

    memset(video_stop, 0, sizeof(*video_stop));
    video_stop->user_data = backend;
    video_stop->resolve_media_path = resolve_media_path;
    video_stop->close_movie = close_movie;
    video_stop->close_media_resources = close_media_resources;
    video_stop->report_user_stop = report_user_stop;
    video_stop->available_memory = available_memory;
    video_stop->report_memory_after_stop = report_memory_after_stop;
}
