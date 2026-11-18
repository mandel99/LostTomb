#include "pharaoh/platform/windows_sound.h"
#include "pharaoh/runtime/video_player.h"
#include <stdio.h>
#include <string.h>

static int trace[32], used, bad, playing;
static void event(int value) { trace[used++] = value; }
static uint32_t WINAPI sample_status(uint32_t sample)
{ if (sample != 42) bad = 1; return playing ? 4u : 2u; }
static void WINAPI sample_end(uint32_t sample) { if (sample != 42) bad = 1; event(1); }
static void WINAPI sample_init(uint32_t sample) { if (sample != 42) bad = 1; event(2); }
static int32_t WINAPI sample_file(uint32_t sample, const void *data, int32_t flags)
{ if (sample != 42 || data == NULL || flags != 0) bad = 1; event(3); return 1; }
static void WINAPI sample_loops(uint32_t sample, int32_t value)
{ if (sample != 42 || value != 1) bad = 1; event(4); }
static void WINAPI sample_pan(uint32_t sample, int32_t value)
{ if (sample != 42 || value != 64) bad = 1; event(5); }
static void WINAPI sample_volume(uint32_t sample, int32_t value)
{ if (sample != 42 || value != 100) bad = 1; event(6); }
static void WINAPI sample_start(uint32_t sample) { if (sample != 42) bad = 1; event(7); }
static void close_stream(void *data, uint32_t stream)
{ (void)data; event((int)stream); }
static void movie_device(void *data, uint32_t device)
{ (void)data; if (device != 42) bad = 1; event(4); }
static PharaohMovieProbeFile *probe(void *data, const char *filename, const char *mode)
{ (void)data; (void)filename; (void)mode; return NULL; }
static void *movie_open(void *data, const char *filename, uint32_t flags)
{ (void)data; (void)filename; if (flags != 0) bad = 1; event(5); return NULL; }
static void movie_error(void *data, const char *text)
{ (void)data; if (strcmp(text, "Unable to load BINK!\n") != 0) bad = 1; }

static int music_running, music_open_fails;
static uint32_t music_open(void *data, uint32_t driver, const char *filename, int32_t flags)
{
    (void)data;
    if (driver != 42 || flags != 0 || strcmp(filename, "audio\\music\\setup.mp3")) bad = 1;
    event(11); return music_open_fails ? 0u : 77u;
}
static int32_t music_status(void *data, uint32_t stream)
{ (void)data; if (stream != 77) bad = 1; event(15); return music_running ? 4 : 2; }
static void music_loops(void *data, uint32_t stream, uint32_t loops)
{ (void)data; if (stream != 77 || loops != 0) bad = 1; event(12); }
static void music_volume(void *data, uint32_t stream, int32_t volume)
{ (void)data; if (stream != 77 || volume != 37) bad = 1; event(13); }
static void music_start(void *data, uint32_t stream)
{ (void)data; if (stream != 77) bad = 1; event(14); music_running = 1; }
static void music_close(void *data, uint32_t stream)
{ (void)data; if (stream != 77) bad = 1; event(16); }

static int test_menu_music(void)
{
    PharaohWindowsSound sound = { 0 };
    sound.ready = 1; sound.state.wave_device = 42;
    sound.stream_operations.ail_open_stream = music_open;
    sound.stream_operations.ail_stream_status = music_status;
    sound.stream_operations.ail_set_stream_loop_count = music_loops;
    sound.stream_operations.ail_set_stream_volume = music_volume;
    sound.stream_operations.ail_start_stream = music_start;
    sound.stream_operations.ail_close_stream = music_close;
    used = 0;
    pharaoh_windows_sound_menu_music(&sound, 0, 37);
    if (used != 0) return 10;
    pharaoh_windows_sound_menu_music(&sound, 1, 37);
    const int first[] = { 11, 12, 13, 14 };
    if (bad || used != 4 || memcmp(trace, first, sizeof(first))) return 11;
    used = 0;
    pharaoh_windows_sound_menu_music(&sound, 1, 37);
    if (bad || used != 1 || trace[0] != 15) return 12;
    used = 0;
    pharaoh_windows_sound_menu_music(&sound, 0, 37);
    if (bad || used != 1 || trace[0] != 15) return 13;
    used = 0; sound.ready = 0;
    pharaoh_windows_sound_menu_music(&sound, 1, 37);
    if (used != 0) return 14;
    used = 0; sound.ready = 1; music_running = 0;
    pharaoh_windows_sound_menu_music(&sound, 1, 37);
    const int replay[] = { 15, 16, 11, 12, 13, 14 };
    if (bad || used != 6 || memcmp(trace, replay, sizeof(replay))) return 15;
    used = 0; music_running = 0; music_open_fails = 1;
    pharaoh_windows_sound_menu_music(&sound, 1, 37);
    const int failed[] = { 15, 16, 11 };
    if (bad || used != 3 || memcmp(trace, failed, sizeof(failed)) ||
        sound.state.stream_handles[0] != 0) return 16;
    return 0;
}

int main(void)
{
    PharaohWindowsSound sound = { 0 };
    uint32_t sample = 42;
    char data = 0;
    sound.ready = 1; sound.button_data = &data; sound.state.sample_handles = &sample;
    sound.sample_status = sample_status; sound.sample_end = sample_end;
    sound.sample_init = sample_init; sound.sample_file = sample_file;
    sound.sample_loops = sample_loops; sound.sample_pan = sample_pan;
    sound.sample_volume = sample_volume; sound.sample_start = sample_start;
    pharaoh_windows_sound_button(&sound, 100);
    const int expected_button[] = { 1, 2, 3, 4, 5, 6, 7 };
    if (bad || used != 7 || memcmp(trace, expected_button, sizeof(expected_button))) return 1;
    playing = 1; pharaoh_windows_sound_button(&sound, 100);
    if (used != 7) return 2;

    PharaohVideoPlayer player = { 0 };
    PharaohVideoRuntimeState runtime = { 0 };
    PharaohVideoOpenOperations open = { 0 };
    PharaohDigitalSoundStreamCloseOperations streams = { 0 };
    sound.state.wave_device = 42;
    sound.state.stream_handles[0] = 1; sound.state.stream_handles[1] = 2;
    sound.state.stream_handles[2] = 3;
    streams.ail_close_stream = close_stream;
    open.sound_manager_active = 1; open.sound_manager = &sound.state;
    open.sound_operations = &streams; open.set_movie_sound_device = movie_device;
    open.open_probe_file = probe; open.bink_open = movie_open;
    open.output_debug_string = movie_error;
    used = 0;
    if (pharaoh_video_player_play(&player, &runtime, "intro.bik", 0, 0, &open) != 0) return 3;
    const int expected_bink[] = { 1, 3, 2, 4, 5 };
    if (bad || used != 5 || memcmp(trace, expected_bink, sizeof(expected_bink))) return 4;
    const int music_result = test_menu_music();
    if (music_result != 0) return music_result;
    puts("PASS: button/Bink contracts and 7 menu-music cases: gating, loop=0, volume, continuation, replay, open failure");
    return 0;
}
