#include "pharaoh/platform/windows_bink.h"

#include <string.h>
#include <ddraw.h>

int32_t pharaoh_windows_bink_open_library(PharaohWindowsBink *backend)
{
    memset(backend, 0, sizeof(*backend));
    backend->module = LoadLibraryA("binkw32.dll");
    if (backend->module == NULL) {
        return 0;
    }

#define BIND_BINK(field, export_name) do { \
    FARPROC address = GetProcAddress(backend->module, export_name); \
    if (address == NULL) goto missing_export; \
    memcpy(&backend->field, &address, sizeof(backend->field)); \
} while (0)
    BIND_BINK(open, "_BinkOpen@8");
    BIND_BINK(close, "_BinkClose@4");
    BIND_BINK(buffer_close, "_BinkBufferClose@4");
    BIND_BINK(do_frame, "_BinkDoFrame@4");
    BIND_BINK(next_frame, "_BinkNextFrame@4");
    BIND_BINK(service, "_BinkService@4");
    BIND_BINK(wait, "_BinkWait@4");
    BIND_BINK(get_error, "_BinkGetError@0");
    BIND_BINK(set_sound_system, "_BinkSetSoundSystem@8");
    BIND_BINK(surface_type, "_BinkDDSurfaceType@4");
    BIND_BINK(copy_to_buffer, "_BinkCopyToBuffer@28");
    BIND_BINK(open_miles, "_BinkOpenMiles@4");
#undef BIND_BINK
    return 1;

missing_export:
    pharaoh_windows_bink_close_library(backend);
    return 0;
}

void pharaoh_windows_bink_close_library(PharaohWindowsBink *backend)
{
    if (backend->module != NULL) {
        (void)FreeLibrary(backend->module);
    }
    memset(backend, 0, sizeof(*backend));
}

static void close_movie(void *user_data, void *movie)
{
    ((PharaohWindowsBink *)user_data)->close(movie);
}

static void close_buffer(void *user_data, void *buffer)
{
    ((PharaohWindowsBink *)user_data)->buffer_close(buffer);
}

static void *open_movie(void *user_data, const char *filename, uint32_t flags)
{
    return ((PharaohWindowsBink *)user_data)->open(filename, flags);
}

static void set_sound_device(void *user_data, uint32_t sound_device)
{
    PharaohWindowsBink *const backend = (PharaohWindowsBink *)user_data;
    (void)backend->set_sound_system(backend->open_miles, sound_device);
}

static void do_frame(void *user_data, void *movie)
{
    ((PharaohWindowsBink *)user_data)->do_frame(movie);
}

static void next_frame(void *user_data, void *movie)
{
    ((PharaohWindowsBink *)user_data)->next_frame(movie);
}

static void service(void *user_data, void *movie)
{
    ((PharaohWindowsBink *)user_data)->service(movie);
}

static int32_t wait_frame(void *user_data, void *movie)
{
    return ((PharaohWindowsBink *)user_data)->wait(movie);
}

static const char *last_error(void *user_data)
{
    return ((PharaohWindowsBink *)user_data)->get_error();
}

static int32_t lock_primary_surface(void *user_data, void *surface,
                                    void **pixels, int32_t *pitch)
{
    LPDIRECTDRAWSURFACE draw_surface = (LPDIRECTDRAWSURFACE)surface;
    DDSURFACEDESC description;
    (void)user_data;
    memset(&description, 0, sizeof(description));
    description.dwSize = sizeof(description);
    const HRESULT result = draw_surface->lpVtbl->Lock(draw_surface, NULL,
        &description, DDLOCK_WAIT, NULL);
    if (result == DD_OK) {
        *pixels = description.lpSurface;
        *pitch = description.lPitch;
    }
    return (int32_t)result;
}

static int32_t restore_primary_surface(void *user_data, void *surface)
{
    LPDIRECTDRAWSURFACE draw_surface = (LPDIRECTDRAWSURFACE)surface;
    (void)user_data;
    return (int32_t)draw_surface->lpVtbl->Restore(draw_surface);
}

static void unlock_primary_surface(void *user_data, void *surface, void *pixels)
{
    LPDIRECTDRAWSURFACE draw_surface = (LPDIRECTDRAWSURFACE)surface;
    (void)user_data;
    (void)draw_surface->lpVtbl->Unlock(draw_surface, pixels);
}

static int32_t present_video_surface(void *user_data, void *primary_surface,
    void *video_surface, const PharaohGraphicsWindowRect *destination,
    int32_t source_x, int32_t source_y,
    uint32_t source_width, uint32_t source_height)
{
    PharaohWindowsBink *const backend = user_data;
    LPDIRECTDRAWSURFACE const display = primary_surface;
    LPDIRECTDRAWSURFACE const source = video_surface;
    LPDIRECTDRAWSURFACE const frame = backend->presentation_surface;
    const int32_t frame_left =
        ((int32_t)backend->presentation_width - (int32_t)source_width) / 2;
    const int32_t frame_top =
        ((int32_t)backend->presentation_height - (int32_t)source_height) / 2;
    RECT movie_rectangle = { frame_left, frame_top,
        frame_left + (LONG)source_width, frame_top + (LONG)source_height };
    RECT source_rectangle = { source_x, source_y,
        source_x + (LONG)source_width, source_y + (LONG)source_height };
    RECT frame_rectangle = { 0, 0,
        (LONG)backend->presentation_width, (LONG)backend->presentation_height };
    RECT client_rectangle = { destination->left, destination->top,
        destination->right, destination->bottom };
    DDBLTFX clear = { 0 };
    clear.dwSize = sizeof(clear);
    HRESULT result = frame->lpVtbl->IsLost(frame);
    if (result == DDERR_SURFACELOST) result = frame->lpVtbl->Restore(frame);
    if (result != DD_OK) return (int32_t)result;
    result = frame->lpVtbl->Blt(frame, &frame_rectangle,
        NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &clear);
    if (result != DD_OK) return (int32_t)result;
    result = frame->lpVtbl->Blt(frame, &movie_rectangle,
        source, &source_rectangle, DDBLT_WAIT, NULL);
    if (result != DD_OK) return (int32_t)result;
    /* Native 00426F60 leaves black around the centered movie. Present the
       complete composed frame in one operation, as other windowed scenes do. */
    result=display->lpVtbl->Blt(display,&client_rectangle,frame,&frame_rectangle,DDBLT_WAIT,NULL);
    if(result==DD_OK && backend->diagnostic_frame_limit!=0){
        ++backend->diagnostic_presented;
        DDSURFACEDESC d={0};d.dwSize=sizeof(d);
        if(frame->lpVtbl->Lock(frame,NULL,&d,DDLOCK_WAIT|DDLOCK_READONLY,NULL)==DD_OK){
            if(d.ddpfPixelFormat.dwRGBBitCount==32){
                ++backend->diagnostic_pixels_checked;
                for(uint32_t y=0;y<backend->presentation_height;++y){
                    const uint8_t *row=(const uint8_t *)d.lpSurface+(ptrdiff_t)y*d.lPitch;
                    for(uint32_t x=0;x<backend->presentation_width;++x){
                        uint32_t color;memcpy(&color,row+x*4u,4);
                        if((color&0x00ffffffu)!=0){
                            if((int32_t)x<frame_left||(int32_t)x>=movie_rectangle.right||
                               (int32_t)y<frame_top||(int32_t)y>=movie_rectangle.bottom)
                                ++backend->diagnostic_margin_nonblack;
                            else ++backend->diagnostic_movie_nonblack;
                        }
                    }
                }
            }
            (void)frame->lpVtbl->Unlock(frame,NULL);
        }
    }
    return (int32_t)result;
}

static uint32_t bink_surface_type(void *user_data, void *surface)
{
    return ((PharaohWindowsBink *)user_data)->surface_type(surface);
}

static void bink_copy_to_buffer(void *user_data, void *movie,
                                void *destination, int32_t pitch,
                                uint32_t height, uint32_t surface_type)
{
    PharaohWindowsBink *const backend = (PharaohWindowsBink *)user_data;
    (void)backend->copy_to_buffer(movie, destination, pitch, height,
        0, 0, surface_type);
}

static void blit_movie(void *user_data, PharaohVideoPlayer *player)
{
    (void)pharaoh_video_player_blit((const PharaohVideoBlitContext *)user_data,
        player);
}

void pharaoh_windows_bink_bind_video_operations(
    PharaohWindowsBink *backend, PharaohVideoPlayerBackend *player,
    PharaohVideoLoopOperations *loop, PharaohVideoOpenOperations *open,
    PharaohVideoBlitContext *blit)
{
    backend->blit_operations.surface_user_data = backend;
    backend->blit_operations.bink_user_data = backend;
    backend->blit_operations.lock_video_surface = lock_primary_surface;
    backend->blit_operations.restore_surface = restore_primary_surface;
    backend->blit_operations.unlock_video_surface = unlock_primary_surface;
    backend->blit_operations.bink_surface_type = bink_surface_type;
    backend->blit_operations.bink_copy_to_buffer = bink_copy_to_buffer;
    backend->blit_operations.present_video_surface = present_video_surface;
    blit->operations = &backend->blit_operations;
    player->user_data = backend;
    player->close_movie = close_movie;
    player->close_buffer = close_buffer;
    loop->player_backend = player;
    loop->bink_user_data = backend;
    loop->blit_user_data = blit;
    loop->blit_movie = blit_movie;
    loop->bink_do_frame = do_frame;
    loop->bink_next_frame = next_frame;
    loop->bink_service = service;
    loop->bink_wait = wait_frame;
    loop->bink_last_error = last_error;
    open->loop_operations = loop;
    open->bink_user_data = backend;
    open->bink_open = open_movie;
    open->set_movie_sound_device = set_sound_device;
}
