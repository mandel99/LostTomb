#include "pharaoh/ui/menu_screens.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "line %d: %s\n", __LINE__, #x); exit(1); } } while (0)
static char trace[64];
static size_t used;
static int playing;
static PharaohMenuScreenState screen;
static void event(char c) { trace[used++] = c; trace[used] = 0; }
void pharaoh_text_select(PharaohTextSelection *s, int32_t group, int32_t entry)
{ CHECK(group == 293 && entry == 5); s->current = "title"; event('T'); }
void pharaoh_main_menu_draw_foreground(const PharaohMenuRenderer *r,
    const PharaohMainMenuState *s, const PharaohMainMenuSound *a, const PharaohMenuRect *rects)
{ (void)r; (void)s; (void)a; (void)rects; CHECK(screen.text_expansion_control == 17); event('F'); }
static void sprite(void *c, int32_t image, int32_t x, int32_t y)
{ (void)c; CHECK(image == 11 || image == 12); CHECK(x == 8 && y == 10); CHECK(screen.screen_pixels[0] == 0); event('S'); }
static void panel(void *c, int32_t x, int32_t y, int32_t w, int32_t h)
{ (void)c; CHECK(x == 136 && y == 50 && w == 24 && h == 21); event('P'); }
static void expand(void *c, const char *s, char *d, int32_t n)
{ (void)c; CHECK(!strcmp(s,"title") && n == 200); CHECK(screen.text_expansion_control == 0); memcpy(d,"expanded",sizeof("expanded")); event('E'); }
static void title(void *c, const char *s, int32_t x, int32_t y, int32_t w, int32_t f, int32_t color)
{ (void)c; CHECK(!strcmp(s,"expanded")); CHECK(x == 148 && y == 70 && w == 368 && f == 13 && color == 0); CHECK(screen.text_expansion_control == 17); event('D'); }
static void prompt(void *c, int32_t g, int32_t e, int32_t x, int32_t y,
    int32_t w, int32_t f, int32_t color, int32_t ox, int32_t oy)
{ (void)c; CHECK(g == 13 && e == 7 && x == 240 && y == 472 && w == 320 && f == 14 && color == 65535 && ox == 8 && oy == 10); event('L'); }
static int32_t is_playing(void *c, int32_t channel)
{ (void)c; CHECK(channel == 0); event('Q'); return playing; }
static void play(void *c, const char *path, int32_t channel, int32_t flags)
{ (void)c; CHECK(!strcmp(path,"audio\\music\\setup.mp3") && channel == 0 && flags == 0); event('M'); }
static void update(void *c, int32_t mode)
{ (void)c; (void)mode; CHECK(0); }

int main(void)
{
    uint16_t pixels[8];
    PharaohLabelRenderer labels = {0};
    PharaohMenuRenderer renderer = {0};
    PharaohMainMenuState menu = {0};
    PharaohTextSelection selection = {0};
    PharaohMainMenuSound sound = {NULL, update, is_playing, play};
    PharaohMenuScreenOperations ops = {NULL, sprite, panel, expand, title, prompt};
    PharaohMenuScreenContext c = {&screen, &selection, &renderer, &menu, &sound, &ops, NULL};
    int redraw, digital, music, busy, i;
    labels.origin_x = 8; labels.origin_y = 10; renderer.labels = &labels;
    screen.screen_pixels = pixels; screen.screen_width = 3; screen.screen_height = 2;
    screen.window_width = 800; screen.background_image = 11; screen.logo_image = 12;
    screen.title_font = 13; screen.prompt_font = 14; screen.text_expansion_control = 17;
    for (redraw = 0; redraw < 3; ++redraw) {
        for (i = 0; i < 8; ++i) pixels[i] = 123;
        screen.redraw_request = (uint8_t)redraw; used = 0;
        pharaoh_main_menu_draw_background(&c);
        CHECK(!strcmp(trace, redraw == 1 ? "SPTEDF" : "PTEDF"));
        for (i = 0; i < 8; ++i) CHECK(pixels[i] == (redraw == 1 && i < 6 ? 0 : 123));
    }
    for (digital = 0; digital < 2; ++digital) for (music = 0; music < 2; ++music)
        for (busy = 0; busy < 2; ++busy) {
            menu.digital_sound_ok = digital; menu.music_enabled = (uint8_t)music;
            playing = busy; used = 0;
            pharaoh_logo_window_draw(&c);
            CHECK(!strcmp(trace, !digital ? "SL" : music && !busy ? "QMSL" : "QSL"));
        }
    puts("menu screens: 3 redraw cases and 8 music cases passed (controlled C endpoints)");
    return 0;
}
