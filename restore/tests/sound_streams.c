#include "sound_streams.h"
#include "media_bindings.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>
#define BASE 0x30000000u
#define BYTES (sizeof(RestoreSound)+16)
static uint32_t parameters[6],events[8][4],event_count,checks,scenarios;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Sound stream failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static void event(uint32_t op,uintptr_t a,uintptr_t b,uintptr_t c)
{
    REQUIRE(event_count<8);events[event_count][0]=op;events[event_count][1]=(uint32_t)a;
    events[event_count][2]=(uint32_t)b;events[event_count++][3]=(uint32_t)c;
}
int _AIL_close_stream_4(uintptr_t a) {event(1,a,0,0);return 0;}
int _AIL_open_stream_12(uintptr_t a,uintptr_t b,uintptr_t c) {event(2,a,b,c);return (int)parameters[5];}
int _AIL_set_stream_loop_count_8(uintptr_t a,uintptr_t b) {event(3,a,b,0);return 0;}
int _AIL_set_stream_volume_8(uintptr_t a,uintptr_t b) {event(4,a,b,0);return 0;}
int _AIL_start_stream_4(uintptr_t a) {event(5,a,0,0);return 0;}
int _AIL_stream_status_4(uintptr_t a) {event(6,a,0,0);return (int)parameters[4];}
int _AIL_stream_volume_4(uintptr_t a) {event(7,a,0,0);return (int)0xf1234567u;}
static uint32_t run(void)
{
    RestoreSound *sound=(RestoreSound *)BASE;int channel=(int32_t)parameters[1];
    event_count=0;memset((void *)BASE,0xa5,BYTES);++scenarios;
    sound->digital=0xfedcba98u;sound->disabled=parameters[2];
    sound->volumes[0]=0x123;sound->volumes[2]=0x234;sound->volumes[3]=0x345;
    sound->streams[0]=0xabc00000u;sound->streams[1]=0xabc00001u;sound->streams[2]=0xabc00002u;
    if(channel>=0&&channel<3)sound->streams[channel]=parameters[3];
    switch(parameters[0]) {
    case 0:return FUN_0055f449(sound,(const char *)(BASE+0x1000),channel,0xffffffffu);
    case 1:FUN_0055f561(sound,channel);return 0;
    case 2:return FUN_0055f5a1(sound,channel);
    case 3:return FUN_0055f5f4(sound,channel);
    case 4:FUN_0055f639(sound,channel,-5);return 0;
    default:REQUIRE(0);return 0;
    }
}
static void oracle(void)
{
    uint32_t result;unsigned int i,j;
    REQUIRE(_setmode(_fileno(stdout),_O_BINARY)!=-1);printf("{\"bytes\":%u}\n",(unsigned int)BYTES);fflush(stdout);
    for(;;) {
        for(i=0;i<6;++i)if(scanf("%u",&parameters[i])!=1)return;
        result=run();printf("{\"result\":%u,\"events\":[",result);
        for(i=0;i<event_count;++i){if(i)putchar(',');putchar('[');for(j=0;j<4;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}
        puts("]}");fwrite((void *)BASE,1,BYTES,stdout);fflush(stdout);
    }
}
int main(int argc,char **argv)
{
    uint32_t op,channel,disabled,old,opened,result;unsigned int i;
    REQUIRE(VirtualAlloc((void *)BASE,0x2000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch"))oracle();
    else {
        for(op=0;op<5;++op)for(channel=0;channel<3;++channel)for(disabled=0;disabled<2;++disabled)
        for(old=0;old<2;++old)for(opened=0;opened<2;++opened) {
            RestoreSound *sound=(RestoreSound *)BASE;
            parameters[0]=op;parameters[1]=channel;parameters[2]=disabled?0x100:0;
            parameters[3]=old?0x87654321u:0;parameters[4]=4;parameters[5]=opened?0xf7654321u:0;
            result=run();
            REQUIRE(result==(op==0?1:op==2?(!disabled&&old):op==3&&(!disabled&&old)?0xf1234567u:0));
            REQUIRE(sound->streams[channel]==(op==0&&!disabled?parameters[5]:op==1&&!disabled?0:parameters[3]));
            REQUIRE(event_count==(disabled?0:op==0?old+1+3*opened:op==1?1:old?1:0));
            for(i=sizeof(RestoreSound);i<BYTES;++i)REQUIRE(((unsigned char *)BASE)[i]==0xa5);
            for(i=0;i<3;++i)if(i!=channel)REQUIRE(sound->streams[i]==0xabc00000u+i);
        }
        for(op=0;op<5;++op) {
            parameters[0]=op;parameters[1]=0xffffffffu;parameters[2]=0;
            result=run();REQUIRE(result==(op==0?UINT32_MAX:0)&&event_count==0);
        }
        printf("Sound streams: %u checks, %u scenarios; explicit receiver, three channels, disabled state, failed opens, full pointers, ordered media calls and guard bytes.\n",checks,scenarios);
    }
    VirtualFree((void *)BASE,0,MEM_RELEASE);return 0;
}
