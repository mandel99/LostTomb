#ifndef RESTORE_VIDEO_PLAYER_H
#define RESTORE_VIDEO_PLAYER_H
#include <windows.h>
#include <stdint.h>
#include <stddef.h>
/* Original 28-byte object. The constructor deliberately leaves saved_mode,
   padding and coordinates untouched until a movie is opened. */
typedef struct RestoreVideoPlayer {
    uint32_t saved_mode;
    unsigned char stopped;
    unsigned char padding[3];
    HWND window;
    uintptr_t buffer;
    uintptr_t movie;
    int32_t x,y;
} RestoreVideoPlayer;
typedef char restore_video_size_check[sizeof(RestoreVideoPlayer)==28?1:-1];
typedef char restore_video_movie_offset_check[offsetof(RestoreVideoPlayer,movie)==16?1:-1];
RestoreVideoPlayer *FUN_004133d0(RestoreVideoPlayer *player, HWND window);
void FUN_004133f0(RestoreVideoPlayer *player);
unsigned char FUN_00413420(RestoreVideoPlayer *player);
unsigned char FUN_00413470(RestoreVideoPlayer *player);
unsigned char FUN_00413580(RestoreVideoPlayer *player,const char *filename,int x,int y);
void FUN_00413690(RestoreVideoPlayer *player);
/* Only legacy source wrappers use the recovered data layout. */
#define RESTORE_CURRENT_VIDEO (*(RestoreVideoPlayer **)0x006fdc54u)
#endif
