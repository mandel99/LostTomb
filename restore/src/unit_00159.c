#include "video_player.h"
#include "video_platform.h"
#include "media_bindings.h"
#include "startup_trace.h"
extern unsigned int FUN_00426f60(void);
extern void FUN_004d01e0(int value);
extern void FUN_004cfdc0(unsigned int mode);
extern void FUN_00414bd0(void);
extern void FUN_004e2ab0(void);
extern unsigned int FUN_004d0220(void);
extern void FUN_00402da0(void);
extern void FUN_004ce6e0(char *message,int detail,int number);
static void change_video_mode(unsigned int mode)
{
    *(uint32_t *)0x005e06b0u=1;
    FUN_004d01e0(1);
    FUN_004cfdc0(mode);
    FUN_00414bd0();
    FUN_004e2ab0();
    FUN_004d0220();
    *(uint32_t *)0x005e06b0u=0;
    FUN_00402da0();
    *(unsigned char *)0x00e38dcau=1;
}
void FUN_00413690(RestoreVideoPlayer *player)
{
    unsigned int old_mode;
    int switch_mode;
    uint32_t *movie;
    MSG message;
    unsigned int decoded=0;
    char test_skip[8];
    if(!RESTORE_CURRENT_VIDEO) return;
    FUN_00426f60();
    old_mode=*(uint32_t *)0x00e38e68u;
    switch_mode=*(uint32_t *)0x00e38e5cu!=0;
    if(switch_mode) change_video_mode(1);
    movie=(uint32_t *)player->movie;
    restore_startup_trace("STARTUP Bink header width=%lu height=%lu fields8=%lu,12=%lu,16=%lu,20=%lu\n",movie[0],movie[1],movie[2],movie[3],movie[4],movie[5]);
    player->x=(int32_t)((*(uint32_t *)0x00e38db0u-movie[0])>>1);
    player->y=(int32_t)((*(uint32_t *)0x00e38dacu-movie[1])>>1);
    restore_video_show_cursor(FALSE);
    while(movie[5]<movie[4]) {
        restore_video_tick();
        _BinkDoFrame_4(player->movie);
        _BinkNextFrame_4(player->movie);
        FUN_00413470(player);
        ++decoded;
        if(decoded==1||decoded%300==0) restore_startup_trace("STARTUP Bink decoded=%u frame=%lu bound=%lu\n",decoded,movie[5],movie[4]);
        if(decoded==1&&GetEnvironmentVariableA("RESTORE_STARTUP_SKIP_VIDEO_TEST",test_skip,sizeof(test_skip))==1&&test_skip[0]=='1') {
            FUN_00413420(player);
            restore_startup_trace("STARTUP diagnostic Bink cancellation after one real decoded frame\n");
            goto finished;
        }
        do {
            const char *error=(const char *)(uintptr_t)_BinkGetError_0();
            if(*error) FUN_004ce6e0((char *)0x005763c4u,(int)(uintptr_t)error,0);
            while(restore_video_peek_message(&message)) {
                if(message.message==WM_KEYDOWN || message.message==WM_SYSKEYDOWN ||
                   message.message==WM_LBUTTONDOWN) {
                    FUN_00413420(player);
                    restore_startup_trace("STARTUP Bink cancelled decoded=%u mode=%u\n",decoded,*(unsigned char *)0x00e38ddau);
                    goto finished;
                }
                restore_video_translate_message(&message);
                restore_video_dispatch_message(&message);
                _BinkService_4(player->movie);
            }
            _BinkService_4(player->movie);
        } while(_BinkWait_4(player->movie));
    }
    FUN_00413420(player);
    restore_startup_trace("STARTUP Bink completed decoded=%u mode=%u\n",decoded,*(unsigned char *)0x00e38ddau);
finished:
    if(switch_mode) change_video_mode(old_mode);
    restore_video_show_cursor(TRUE);
}
