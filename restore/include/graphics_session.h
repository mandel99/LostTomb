#ifndef RESTORE_GRAPHICS_SESSION_H
#define RESTORE_GRAPHICS_SESSION_H
#include <windows.h>
#include <ddraw.h>
#include <stdint.h>
#include <stddef.h>
/* Original 004d0480 receiver: 44 bytes, not an inferred stack frame. */
typedef struct RestoreSurfaceContext {
    int32_t steps,brightness,width,height,format,effect;
    uint8_t randomize,padding[3];
    LPDIRECTDRAWSURFACE front,back;
    void *front_pixels,*back_pixels;
} RestoreSurfaceContext;
typedef char restore_surface_context_size[(sizeof(RestoreSurfaceContext)==44)?1:-1];
typedef char restore_surface_context_pixels[(offsetof(RestoreSurfaceContext,front_pixels)==36)?1:-1];
typedef struct RestoreGraphicsApi {
    HDC (WINAPI *get_dc)(HWND);
    int (WINAPI *device_caps)(HDC,int);
    int (WINAPI *release_dc)(HWND,HDC);
    int (WINAPI *metrics)(int);
    UINT (WINAPI *palette_entries)(HPALETTE,UINT,UINT,LPPALETTEENTRY);
    HRESULT (WINAPI *create_draw)(GUID *,LPDIRECTDRAW *,IUnknown *);
    LONG (WINAPI *window_long)(HWND,int);
    LONG (WINAPI *set_window_long)(HWND,int,LONG);
    HMENU (WINAPI *menu)(HWND);
    BOOL (WINAPI *adjust)(LPRECT,DWORD,BOOL,DWORD);
    BOOL (WINAPI *position)(HWND,HWND,int,int,int,int,UINT);
    BOOL (WINAPI *parameters)(UINT,UINT,PVOID,UINT);
    BOOL (WINAPI *window_rect)(HWND,LPRECT);
    BOOL (WINAPI *update)(HWND);
    void (WINAPI *debug)(LPCSTR);
    DWORD (WINAPI *ticks)(void);
} RestoreGraphicsApi;
const RestoreGraphicsApi *restore_graphics_platform(void);
int FUN_00426b40(void);
int FUN_00426ee0(int release_device);
int FUN_00426f60(void);
int FUN_00426ff0(void);
LPDIRECTDRAWSURFACE FUN_00427480(uint32_t width,uint32_t height,int system_memory,uint32_t unused);
int FUN_004d0040(void);
int FUN_004d0060(void);
void FUN_004d01e0(uint32_t release_device);
int FUN_004d0220(void);
void FUN_004d03f0(void);
int FUN_004d0480(RestoreSurfaceContext *self,LPDIRECTDRAWSURFACE front,LPDIRECTDRAWSURFACE back);
void FUN_004d0280(int force);
void FUN_004d05f0(RestoreSurfaceContext *self,int buffer,int steps,int brightness,uint8_t randomize,int effect);
int FUN_004d0630(int value,int minimum,int maximum);
void FUN_004d0650(RestoreSurfaceContext *self,int buffer);
#define RESTORE_SURFACE_CONTEXT ((RestoreSurfaceContext *)0x006de518u)
/* These are full x86 data slots. Never cast their numeric contents to code. */
#define GFX_U(address) (*(uint32_t *)(uintptr_t)(address))
#define GFX_I(address) (*(int32_t *)(uintptr_t)(address))
#define GFX_B(address) (*(uint8_t *)(uintptr_t)(address))
#define GFX_P(type,address) (*(type *)(uintptr_t)(address))
#endif
