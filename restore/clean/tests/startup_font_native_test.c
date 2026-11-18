/* Controlled DirectDraw endpoint around the actual production renderer. */
#include "../game/platform/windows_startup_host.c"
struct FontCase { int32_t group; uint32_t first; const uint8_t *text; int32_t width; };
struct GlyphCase { uint32_t image, offset; };
#include "font_native_cases.h"
static uint8_t test_pixels[16384];
static uint32_t test_pixel_bits;
static HRESULT WINAPI test_lock(LPDIRECTDRAWSURFACE surface, LPRECT rect,
    LPDDSURFACEDESC description, DWORD flags, HANDLE event)
{
    (void)surface; (void)rect; (void)flags; (void)event;
    description->lpSurface = test_pixels;
    description->lPitch = 64 * (LONG)(test_pixel_bits / 8);
    description->ddpfPixelFormat.dwRGBBitCount = test_pixel_bits;
    description->ddpfPixelFormat.dwRBitMask = 0xff0000;
    description->ddpfPixelFormat.dwGBitMask = 0xff00;
    description->ddpfPixelFormat.dwBBitMask = 0xff;
    return DD_OK;
}
static HRESULT WINAPI test_unlock(LPDIRECTDRAWSURFACE surface, LPVOID pixels)
{ (void)surface; (void)pixels; return DD_OK; }
static void *read_fixture(const char *name, size_t bytes)
{
    FILE *file = NULL;
    if (fopen_s(&file, name, "rb") != 0 || file == NULL) return NULL;
    void *data = malloc(bytes);
    if (data != NULL && fread(data, 1, bytes, file) != bytes) { free(data); data = NULL; }
    fclose(file);
    return data;
}
int main(void)
{
    static PharaohWindowsStartupHost host;
    static PharaohSpriteLibraryResetState libraries;
    IDirectDrawSurfaceVtbl vtable = { 0 };
    IDirectDrawSurface surface = { &vtable };
    vtable.Lock = test_lock; vtable.Unlock = test_unlock;
    host.sprite_images = read_fixture("font_images.bin", FONT_IMAGE_COUNT * 64u);
    if (host.sprite_images == NULL) return 1;
    for (size_t i = 0; i < sizeof(font_cases)/sizeof(font_cases[0]); ++i) {
        const struct FontCase *c = &font_cases[i];
        PharaohBitmapFont font = { 0 };
        font.images = host.sprite_images; font.first_image = c->first;
        font.image_count = FONT_IMAGE_COUNT;
        font.measure_space = c->group == 634 ? 4 : c->group >= 640 ? 10 : 6;
        font.measure_gap = c->group == 634 || c->group >= 640 ? 1 : 0;
        if (pharaoh_bitmap_text_width(&font, c->text) != c->width) return 2;
    }
    FILE *file = NULL;
    if (fopen_s(&file, "font_pixels.bin", "rb") != 0 || file == NULL) return 3;
    fseek(file, 0, SEEK_END); const long bytes = ftell(file); fclose(file);
    libraries.items[0].pixels = read_fixture("font_pixels.bin", (size_t)bytes);
    libraries.items[0].pixel_bytes = (uint32_t)bytes;
    host.loaded_libraries = &libraries;
    host.sprite_memory_operations.allocate = allocate_sprite_memory;
    host.sprite_memory_operations.release = release_sprite_memory;
    host.graphics.back_buffer_surface = &surface;
    host.graphics.render_width = 64; host.graphics.render_height = 64;
    uint8_t *expected = read_fixture("font_expected.bin", sizeof(glyph_cases)/sizeof(glyph_cases[0])*8192u);
    if (libraries.items[0].pixels == NULL || expected == NULL) return 4;
    for (test_pixel_bits = 16; test_pixel_bits <= 32; test_pixel_bits += 16) {
        for (size_t i = 0; i < sizeof(glyph_cases)/sizeof(glyph_cases[0]); ++i) {
            const uint16_t background = 0x4210;
            uint32_t background32 = 0;
            for (int32_t component = 0; component < 3; ++component) {
                const uint32_t value = (background >> (component * 5)) & 31u;
                background32 |= ((value << 3) | (value >> 2)) << (component * 8);
            }
            for (size_t p = 0; p < 4096; ++p) {
                if (test_pixel_bits == 16) memcpy(test_pixels + p*2, &background, 2);
                else memcpy(test_pixels + p*4, &background32, 4);
            }
            draw_sprite_pixels(&host, glyph_cases[i].image, 0, 0, 0xffff);
            const uint8_t *native = expected + glyph_cases[i].offset;
            for (size_t p = 0; p < 4096; ++p) {
                uint16_t native_pixel;
                memcpy(&native_pixel, native + p*2, 2);
                if (test_pixel_bits == 16) {
                    if (memcmp(test_pixels + p*2, &native_pixel, 2) != 0) return 5;
                } else {
                    uint32_t converted = 0;
                    for (int32_t component = 0; component < 3; ++component) {
                        const uint32_t value = (native_pixel >> (component * 5)) & 31u;
                        converted |= ((value << 3) | (value >> 2)) << (component * 8);
                    }
                    if (memcmp(test_pixels + p*4, &converted, 4) != 0) {
                        printf("32-bit glyph mismatch: image %u pixel %zu\n", glyph_cases[i].image, p);
                        return 6;
                    }
                }
            }
        }
    }
    puts("PASS: 40 native/C widths, 24 native glyph cases in 16/32-bit targets, 8 font variants");
    free(expected); free(libraries.items[0].pixels); free(host.sprite_images);
    return 0;
}
