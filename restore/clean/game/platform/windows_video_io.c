#include "pharaoh/platform/windows_video_io.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdio.h>

static uint32_t video_tick(void *user_data)
{
    (void)user_data;
    return GetTickCount();
}

static int32_t peek_remove_message(void *user_data,
                                   PharaohVideoMessage *message)
{
    MSG native_message;
    (void)user_data;
    if (PeekMessageA(&native_message, NULL, 0, 0, PM_REMOVE) == 0) {
        return 0;
    }
    message->window = native_message.hwnd;
    message->message_id = native_message.message;
    message->wparam = native_message.wParam;
    message->lparam = native_message.lParam;
    message->time = native_message.time;
    message->point_x = native_message.pt.x;
    message->point_y = native_message.pt.y;
    return 1;
}

static MSG native_message_from(const PharaohVideoMessage *message)
{
    MSG native_message;
    native_message.hwnd = (HWND)message->window;
    native_message.message = message->message_id;
    native_message.wParam = (WPARAM)message->wparam;
    native_message.lParam = (LPARAM)message->lparam;
    native_message.time = message->time;
    native_message.pt.x = message->point_x;
    native_message.pt.y = message->point_y;
    return native_message;
}

static void translate_message(void *user_data,
                              const PharaohVideoMessage *message)
{
    const MSG native_message = native_message_from(message);
    (void)user_data;
    (void)TranslateMessage(&native_message);
}

static void dispatch_message(void *user_data,
                             const PharaohVideoMessage *message)
{
    const MSG native_message = native_message_from(message);
    (void)user_data;
    (void)DispatchMessageA(&native_message);
}

static void show_cursor(void *user_data, int32_t visible)
{
    (void)user_data;
    (void)ShowCursor(visible != 0);
}

static PharaohMovieProbeFile *open_probe_file(void *user_data,
                                              const char *filename,
                                              const char *mode)
{
    FILE *file = NULL;
    (void)user_data;
    (void)fopen_s(&file, filename, mode);
    return (PharaohMovieProbeFile *)file;
}

static void close_probe_file(void *user_data, PharaohMovieProbeFile *file)
{
    (void)user_data;
    (void)fclose((FILE *)file);
}

static void output_debug_string(void *user_data, const char *message)
{
    (void)user_data;
    OutputDebugStringA(message);
}

void pharaoh_windows_video_io_bind(
    PharaohVideoLoopOperations *loop, PharaohVideoOpenOperations *open)
{
    loop->tick = video_tick;
    loop->peek_remove_message = peek_remove_message;
    loop->translate_message = translate_message;
    loop->dispatch_message = dispatch_message;
    loop->show_cursor = show_cursor;
    open->open_probe_file = open_probe_file;
    open->close_probe_file = close_probe_file;
    open->output_debug_string = output_debug_string;
}
