#include "video_player.h"
#include "video_platform.h"
#include "standalone_data.h"
#include "media_bindings.h"
#include <ddraw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int FUN_00427510(int error);
extern uint32_t FUN_004cffe0(void);
extern uint32_t FUN_004143c0(void);
#define U32(address) (*(uint32_t *)(address##u))
#define U8(address) (*(unsigned char *)(address##u))
static RestoreVideoPlayer player;
static uint32_t movie[8],audio[128];
static IDirectDrawSurface surface;
static IDirectDrawSurface back_surface;
static IDirectDrawSurfaceVtbl surface_methods;
static unsigned char pixels[64];
static int locks,restores,unlocks,copies,closes,buffer_closes,opens,services,waits,frames;
static int sound_stops,sound_set,logs,log_number,shows,translations,dispatches,ticks,prepares;
static int requested_x,requested_y,lock_failures,restore_failure,open_failure,busy_waits;
static HRESULT lock_error;
static uintptr_t destination,copy_pitch,copy_height;
static uint32_t sound_values[3],modes[4];
static unsigned int mode_count,messages[8],message_count,message_cursor;
static char path_seen[100],trace[128];
static const char *error_text="";
static uintptr_t last_error_message;
static int back_failure,remove_back_during_restore;
static HRESULT surface_status,back_status;
static int status_calls,back_status_calls,paused,rendered;
static void require_at(int condition,int line)
{
    if(!condition) {fprintf(stderr,"Video reconstruction failure at line %d\n",line); abort();}
}
#define REQUIRE(condition) require_at(!!(condition),__LINE__)
void FUN_004cff90(void) {++paused;}
void FUN_004d6b60(void) {++rendered;}
static HRESULT WINAPI fake_is_lost(LPDIRECTDRAWSURFACE self)
{
    REQUIRE(self==&surface||self==&back_surface);
    if(self==&surface) {++status_calls;return surface_status;}
    ++back_status_calls;return back_status;
}
static void event(char value) {size_t n=strlen(trace); REQUIRE(n+1<sizeof(trace)); trace[n]=value; trace[n+1]=0;}
static HRESULT WINAPI fake_lock(LPDIRECTDRAWSURFACE self,LPRECT rect,LPDDSURFACEDESC description,DWORD flags,HANDLE handle)
{
    size_t i;
    REQUIRE(self==&surface&&rect==0&&flags==DDLOCK_WAIT&&handle==0);
    REQUIRE(description==(DDSURFACEDESC *)0x00605b78u&&description->dwSize==108);
    if(!locks) for(i=4;i<108;++i) REQUIRE(((unsigned char *)description)[i]==0);
    ++locks; event('L');
    if(lock_failures) {--lock_failures; return DDERR_SURFACELOST;}
    if(lock_error) return lock_error;
    description->lPitch=(LONG)copy_pitch;
    description->lpSurface=pixels;
    return DD_OK;
}
static HRESULT WINAPI fake_restore(LPDIRECTDRAWSURFACE self)
{
    REQUIRE(self==&surface||self==&back_surface); ++restores; event('R');
    if(self==&surface&&remove_back_during_restore) *(LPDIRECTDRAWSURFACE *)0x00e39174u=0;
    return (self==&surface?restore_failure:back_failure)?DDERR_GENERIC:DD_OK;
}
static HRESULT WINAPI fake_unlock(LPDIRECTDRAWSURFACE self,LPVOID memory)
{REQUIRE(self==&surface&&memory==pixels); ++unlocks; event('U'); return DD_OK;}
int _BinkClose_4(uintptr_t value) {REQUIRE(value==(uintptr_t)movie); ++closes; event('C'); return 0;}
int _BinkBufferClose_4(uintptr_t value) {REQUIRE(value==0xfedcba98u); ++buffer_closes; event('B'); return 0;}
int _BinkDDSurfaceType_4(uintptr_t value) {REQUIRE(value==(uintptr_t)&surface); event('T'); return 0x89abcdefu;}
int _BinkCopyToBuffer_28(uintptr_t a,uintptr_t b,uintptr_t c,uintptr_t d,uintptr_t e,uintptr_t f,uintptr_t g)
{
    REQUIRE(a==(uintptr_t)movie&&e==0&&f==0&&g==0x89abcdefu);
    ++copies; destination=b; REQUIRE(c==copy_pitch&&d==copy_height); event('X'); return 0;
}
int _BinkOpen_8(uintptr_t name,uintptr_t flags)
{
    REQUIRE(flags==0); ++opens; strcpy_s(path_seen,sizeof(path_seen),(const char *)name);
    return open_failure?0:(int)(uintptr_t)movie;
}
int __stdcall _BinkOpenMiles_4(uintptr_t value) {(void)value; abort();}
int _BinkSetSoundSystem_8(uintptr_t callback,uintptr_t handle)
{REQUIRE(callback==(uintptr_t)_BinkOpenMiles_4&&handle==0xf1234567u); ++sound_set; return 0;}
int _AIL_close_stream_4(uintptr_t stream)
{REQUIRE(sound_stops<3); sound_values[sound_stops++]=(uint32_t)stream; return 0;}
/* The linked sound provider also owns volume control. Playback cancellation
   must not enter that separate path in these scenarios. */
int _AIL_stream_status_4(uintptr_t stream) {(void)stream; abort();}
int _AIL_set_stream_volume_8(uintptr_t stream,uintptr_t volume)
{(void)stream; (void)volume; abort();}
int _BinkDoFrame_4(uintptr_t value) {REQUIRE(value==(uintptr_t)movie); ++frames; return 0;}
int _BinkNextFrame_4(uintptr_t value) {REQUIRE(value==(uintptr_t)movie); ++movie[5]; return 0;}
int _BinkGetError_0(void) {return (int)(uintptr_t)error_text;}
int _BinkService_4(uintptr_t value) {REQUIRE(value==(uintptr_t)movie); ++services; return 0;}
int _BinkWait_4(uintptr_t value)
{REQUIRE(value==(uintptr_t)movie); ++waits; if(busy_waits) {--busy_waits; return 1;} return 0;}
void FUN_004ce6e0(char *message,int detail,int number)
{
    ++logs; log_number=number;
    last_error_message=(uintptr_t)message;
    if(message==(char *)0x005763c4u) REQUIRE(detail==(int)(uintptr_t)error_text&&number==0);
    else REQUIRE((message==(char *)0x00576364u||message==(char *)0x005e0774u||message==(char *)0x005e0798u)&&detail==0);
    event('E');
}
unsigned int FUN_00426f60(void)
{++prepares; requested_x=player.x; requested_y=player.y; return 1;}
void FUN_004d01e0(int value) {REQUIRE(value==1&&U32(0x005e06b0)==1);}
void FUN_004cfdc0(unsigned int mode) {REQUIRE(mode_count<4); modes[mode_count++]=mode;}
void FUN_00414bd0(void) {REQUIRE(U32(0x005e06b0)==1);}
void FUN_004e2ab0(void) {REQUIRE(U32(0x005e06b0)==1);}
unsigned int FUN_004d0220(void) {REQUIRE(U32(0x005e06b0)==1); return 1;}
void FUN_00402da0(void) {REQUIRE(U32(0x005e06b0)==0);}
BOOL restore_video_peek_message(MSG *message)
{
    if(message_cursor==message_count) return FALSE;
    memset(message,0,sizeof(*message)); message->message=messages[message_cursor++]; return TRUE;
}
void restore_video_translate_message(const MSG *message) {REQUIRE(message->message==WM_TIMER); ++translations;}
void restore_video_dispatch_message(const MSG *message) {REQUIRE(message->message==WM_TIMER); ++dispatches;}
void restore_video_show_cursor(BOOL show) {REQUIRE(show==(shows%2?TRUE:FALSE)); ++shows;}
void restore_video_tick(void) {++ticks;}
static void reset(void)
{
    memset(&player,0,sizeof(player)); memset(movie,0,sizeof(movie)); memset(audio,0,sizeof(audio));
    movie[0]=320; movie[1]=200; movie[4]=3; copy_height=200; copy_pitch=2048;
    locks=restores=unlocks=copies=closes=buffer_closes=opens=services=waits=frames=0;
    sound_stops=sound_set=logs=log_number=shows=translations=dispatches=ticks=prepares=0;
    lock_failures=restore_failure=open_failure=busy_waits=0; lock_error=DD_OK;
    back_failure=0; last_error_message=0;
    mode_count=message_count=message_cursor=0; trace[0]=path_seen[0]=0; error_text="";
    RESTORE_CURRENT_VIDEO=&player;
    *(LPDIRECTDRAWSURFACE *)0x00e39178u=&surface;
    *(LPDIRECTDRAWSURFACE *)0x00e39174u=&back_surface;
    U32(0x00e39130)=10; U32(0x00e39134)=20;
    U32(0x00e38db0)=640; U32(0x00e38dac)=480;
    U32(0x00e38e68)=3; U32(0x00e38e5c)=0;
    U32(0x012663ac)=0; U8(0x00e38dda)=0x77; U8(0x00e38dca)=1; U8(0x00e38e62)='D';
    U32(0x00e92bf8+0x200)=0;
}
static void lifecycle(void)
{
    RestoreVideoPlayer before;
    reset(); memset(&player,0xa5,sizeof(player)); before=player;
    REQUIRE(FUN_004133d0(&player,(HWND)0xf1234567u)==&player);
    REQUIRE(player.window==(HWND)0xf1234567u&&player.stopped==1&&!player.movie&&!player.buffer);
    REQUIRE(player.saved_mode==before.saved_mode&&player.x==before.x&&player.y==before.y);
    REQUIRE(!memcmp(player.padding,before.padding,3));
    player.movie=(uintptr_t)movie; player.buffer=0xfedcba98u;
    FUN_004133f0(&player); REQUIRE(!strcmp(trace,"CB")&&!player.movie&&!player.buffer);
    FUN_004133f0(&player); REQUIRE(closes==1&&buffer_closes==1);
    player.movie=(uintptr_t)movie; player.saved_mode=0xabcdef42u; player.stopped=0;
    U8(0x00e38ddb)=0x9d;
    REQUIRE(FUN_00413420(&player)==1&&player.stopped==1&&U8(0x00e38dda)==0x42&&U8(0x00e38dca)==1);
    REQUIRE(U8(0x00e38ddb)==0x9d&&!player.movie);
}
static void drawing(void)
{
    reset(); REQUIRE(FUN_00413470(&player)==0&&locks==0);
    player.movie=(uintptr_t)movie; player.stopped=1;
    REQUIRE(FUN_00413470(&player)==0&&locks==0); player.stopped=0;
    player.x=300; player.y=260;
    REQUIRE(FUN_00413470(&player)==1&&!strcmp(trace,"LTXU"));
    REQUIRE(destination==(uintptr_t)pixels+2048u*280u+2u*310u&&copies==1&&unlocks==1);
    reset(); player.movie=(uintptr_t)movie; lock_failures=2;
    REQUIRE(FUN_00413470(&player)==1&&!strcmp(trace,"LRLRLTXU")&&restores==2);
    reset(); player.movie=(uintptr_t)movie; lock_failures=1; restore_failure=1;
    REQUIRE(FUN_00413470(&player)==1&&!strcmp(trace,"LR")&&copies==0&&unlocks==0);
    reset(); player.movie=(uintptr_t)movie; lock_error=DDERR_GENERIC; U32(0x00e92e04)=0xfedcba98u;
    REQUIRE(FUN_00413470(&player)==1&&!strcmp(trace,"LE")&&log_number==7&&U32(0x00e92e04)==0);
    reset(); player.movie=(uintptr_t)movie; player.x=-31; player.y=300; copy_pitch=(uint32_t)-2048;
    REQUIRE(FUN_00413470(&player)==1);
    REQUIRE(destination==(uintptr_t)pixels+(uint32_t)-2048*320u+2u*(uint32_t)-21);
    REQUIRE(FUN_00427510((int)0x88760005u)==1&&FUN_00427510((int)0x80004005u)==6&&FUN_00427510(0)==100);
}
static void playback(void)
{
    char directory[MAX_PATH],filename[MAX_PATH],missing[80],long_name[130];
    REQUIRE(GetTempPathA(sizeof(directory),directory));
    REQUIRE(GetTempFileNameA(directory,"rsv",0,filename));
    REQUIRE(strlen(filename)<100);
    reset();
    REQUIRE(FUN_00413580(&player,filename,-321,987)==1&&!strcmp(path_seen,filename));
    REQUIRE(requested_x==-321&&requested_y==987&&player.x==160&&player.y==140);
    REQUIRE(frames==3&&copies==3&&closes==1&&shows==2&&ticks==3&&!player.movie&&player.stopped);
    REQUIRE(player.saved_mode==0x77&&U8(0x00e38dda)==0x77&&U8(0x00e38dca)==1);
    REQUIRE(DeleteFileA(filename));
    snprintf(missing,sizeof(missing),"restore-video-missing-%lu.bik",GetCurrentProcessId());
    reset(); open_failure=1;
    REQUIRE(FUN_00413580(&player,missing,1,2)==0&&opens==1&&shows==0&&frames==0);
    snprintf(filename,sizeof(filename),"D:\\%s",missing); REQUIRE(!strcmp(path_seen,filename));
    REQUIRE(U8(0x00e38dda)==0x77&&U8(0x00e38dca)==1);
    reset(); player.movie=(uintptr_t)movie; player.buffer=0xfedcba98u; open_failure=1;
    REQUIRE(FUN_00413580(&player,missing,1,2)==0&&closes==1&&!player.movie&&player.buffer==0xfedcba98u&&buffer_closes==0);
    reset(); U32(0x012663ac)=(uintptr_t)audio; audio[0x178/4]=0xf1234567u;
    U32(0x00e92bf8+0x1d8)=0x12345678; U32(0x00e92bf8+0x1dc)=0x23456789; U32(0x00e92bf8+0x1e0)=0x3456789a;
    U32(0x00e38e5c)=1; messages[0]=WM_TIMER; messages[1]=WM_KEYDOWN; message_count=2;
    REQUIRE(FUN_00413580(&player,missing,4,5)==1);
    REQUIRE(sound_stops==3&&sound_values[0]==0x12345678&&sound_values[1]==0x3456789a&&sound_values[2]==0x23456789&&sound_set==1);
    REQUIRE(U32(0x00e92bf8+0x1d8)==0&&U32(0x00e92bf8+0x1dc)==0&&U32(0x00e92bf8+0x1e0)==0);
    REQUIRE(frames==1&&translations==1&&dispatches==1&&waits==0&&shows==2&&closes==1);
    REQUIRE(mode_count==2&&modes[0]==1&&modes[1]==3&&U32(0x005e06b0)==0);
    reset(); U32(0x012663ac)=(uintptr_t)audio; audio[0x178/4]=0xf1234567u;
    U32(0x00e92bf8+0x200)=1;
    REQUIRE(FUN_00413580(&player,missing,0,0)==1&&sound_stops==0&&sound_set==1);
    reset(); movie[4]=0;
    REQUIRE(FUN_00413580(&player,missing,0,0)==1&&frames==0&&closes==1&&shows==2);
    reset(); movie[0]=800; movie[1]=700; copy_height=700;
    REQUIRE(FUN_00413580(&player,missing,0,0)==1&&player.x==2147483568&&player.y==2147483538);
    reset(); messages[0]=WM_SYSKEYDOWN; message_count=1;
    REQUIRE(FUN_00413580(&player,missing,0,0)==1&&frames==1&&closes==1&&shows==2);
    reset(); messages[0]=WM_LBUTTONDOWN; message_count=1;
    REQUIRE(FUN_00413580(&player,missing,0,0)==1&&frames==1&&closes==1&&shows==2);
    reset(); busy_waits=2; error_text="fixture codec error";
    REQUIRE(FUN_00413580(&player,missing,0,0)==1&&frames==3&&waits==5&&logs==5&&shows==2);
    reset(); memset(long_name,'A',sizeof(long_name)-1); long_name[sizeof(long_name)-1]=0;
    REQUIRE(FUN_00413580(&player,long_name,0,0)==0&&opens==0&&shows==0);
    reset(); RESTORE_CURRENT_VIDEO=0; FUN_00413690(&player); REQUIRE(prepares==0&&shows==0);
}
static void surface_recovery(void)
{
    reset(); REQUIRE(FUN_004cffe0()==1&&restores==2&&logs==0);
    reset(); *(LPDIRECTDRAWSURFACE *)0x00e39178u=0;
    REQUIRE(FUN_004cffe0()==0&&restores==0&&last_error_message==0x005e0774u);
    reset(); restore_failure=1;
    REQUIRE(FUN_004cffe0()==0&&restores==1&&last_error_message==0x005e0774u);
    reset(); *(LPDIRECTDRAWSURFACE *)0x00e39174u=0;
    REQUIRE(FUN_004cffe0()==0&&restores==1&&last_error_message==0x005e0798u);
    reset(); back_failure=1;
    REQUIRE(FUN_004cffe0()==0&&restores==2&&last_error_message==0x005e0798u);
    reset(); back_failure=0; remove_back_during_restore=1;
    REQUIRE(FUN_004cffe0()==0&&restores==1&&last_error_message==0x005e0798u);
    remove_back_during_restore=0;
}
static void frame_entry(void)
{
    int front,back,quit,failed;
    for(front=0;front<4;++front)for(back=0;back<4;++back)
    for(quit=0;quit<3;++quit)for(failed=0;failed<2;++failed) {
        int lost=front==2||back==2;
        int recover_failure=lost&&(failed||front==0||back==0);
        reset();
        *(LPDIRECTDRAWSURFACE *)0x00e39178u=front?&surface:0;
        *(LPDIRECTDRAWSURFACE *)0x00e39174u=back?&back_surface:0;
        surface_status=front==2?DDERR_SURFACELOST:front==3?DDERR_GENERIC:DD_OK;
        back_status=back==2?DDERR_SURFACELOST:back==3?DDERR_GENERIC:DD_OK;
        restore_failure=failed;U8(0x00d3b43d)=quit;
        status_calls=back_status_calls=paused=rendered=0;
        REQUIRE(FUN_004143c0()==(uint32_t)(!recover_failure&&quit!=1));
        REQUIRE(status_calls==(front!=0)&&back_status_calls==(front!=2&&back!=0));
        REQUIRE(paused==recover_failure&&rendered==(!recover_failure&&quit!=1));
        REQUIRE(restores==(lost?(front==0?0:failed||back==0?1:2):0));
    }
    U8(0x00d3b43d)=0;
}
int main(void)
{
    REQUIRE(restore_data_open());
    memset(&surface_methods,0,sizeof(surface_methods));
    surface_methods.Lock=fake_lock; surface_methods.Restore=fake_restore; surface_methods.Unlock=fake_unlock;
    surface_methods.IsLost=fake_is_lost;
    surface.lpVtbl=&surface_methods;
    back_surface.lpVtbl=&surface_methods;
    lifecycle(); drawing(); playback(); surface_recovery(); frame_entry();
    restore_data_close();
    puts("Video C reconstruction: object lifetime, pointers, signed pitch, lost surfaces, paths, audio handoff, frames, input cancellation and mode restoration verified; no DLL, window or game used.");
    return 0;
}
