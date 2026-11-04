#include "sound_initialization.h"
#include "standalone_data.h"
#include "media_bindings.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define OBJECT 0x30000000u
#define HEAP 0x31000000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
static uint32_t events[256][6],event_count,cursor,allocations,handles,free_count,live_count,checks,scenarios;
static uint32_t current[16],live[64];
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Sound initialization failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
void FUN_005637ee(void){REQUIRE(0);}
static void event(uint32_t id,unsigned int n,uint32_t a,uint32_t b,uint32_t c,uint32_t d)
{uint32_t *p;REQUIRE(event_count<256);p=events[event_count++];p[0]=id;p[1]=n;p[2]=a;p[3]=b;p[4]=c;p[5]=d;}
static uint32_t hash(const char *s) {uint32_t v=2166136261u;while(*s)v=(v^(uint8_t)*s++)*16777619u;return v;}
static void track(uint32_t address) {REQUIRE(live_count<64);live[live_count++]=address;}
void *FUN_00561a1d(uint32_t size)
{
    uint32_t value;event(0x561a1d,1,size,0,0,0);++allocations;if(allocations==current[11])return NULL;
    value=cursor;cursor+=(size+15u)&~15u;REQUIRE(cursor<HEAP+0x200000);memset((void *)(uintptr_t)value,0xcd,size);track(value);return (void *)(uintptr_t)value;
}
void FUN_00561a2b(void *pointer)
{
    unsigned int i;event(0x561a2b,1,(uint32_t)(uintptr_t)pointer,0,0,0);++free_count;
    for(i=0;i<live_count;++i)if(live[i]==(uint32_t)(uintptr_t)pointer) {live[i]=live[--live_count];return;}
    REQUIRE(0);
}
void FUN_00561bdc(void *pointer) {event(0x561bdc,1,(uint32_t)(uintptr_t)pointer,0,0,0);}
int restore_sound_version(uint32_t *platform,uint32_t *major) {event(0x56f124,0,0,0,0,0);*platform=current[2];*major=current[3];return (int)current[4];}
void restore_sound_debug(const char *s) {event(0x56f0f8,1,hash(s),0,0,0);}
int _AIL_startup_0(void) {event(0x56f2dc,0,0,0,0,0);return 1;}
int _AIL_set_preference_8(uintptr_t a,uintptr_t b) {event(0x56f2e0,2,(uint32_t)a,(uint32_t)b,0,0);return 0;}
int _AIL_waveOutOpen_16(uintptr_t a,uintptr_t b,uintptr_t c,uintptr_t d) {event(0x56f2e4,4,(uint32_t)a,(uint32_t)b,(uint32_t)c,(uint32_t)d);U(a)=0x70123456;return (int)current[5];}
int _AIL_allocate_sample_handle_4(uintptr_t a) {event(0x56f2f4,1,(uint32_t)a,0,0,0);++handles;return handles==current[12]?0:(int)(0x70000000u+handles);}
int _AIL_release_sample_handle_4(uintptr_t a) {event(0x56f2f0,1,(uint32_t)a,0,0,0);return 0;}
int _AIL_end_sample_4(uintptr_t a) {event(0x56f2cc,1,(uint32_t)a,0,0,0);return 0;}
int _AIL_end_3D_sample_4(uintptr_t a) {event(0x56f2c8,1,(uint32_t)a,0,0,0);return 0;}
int _AIL_mem_free_lock_4(uintptr_t a) {event(0x56f308,1,(uint32_t)a,0,0,0);return 0;}
static void prepare(const uint32_t p[16])
{
    RestoreSound *s=(RestoreSound *)(uintptr_t)OBJECT;unsigned int i;memcpy(current,p,sizeof(current));
    event_count=allocations=handles=free_count=live_count=0;cursor=HEAP+0x10000;
    memset(s,(uint8_t)p[1],sizeof(*s));memset((void *)(uintptr_t)HEAP,0x5a,0x200000);U(0x12663b0)=0x10203040;
    FUN_0055e38b(s);
    if(p[0]==0)return;
    s->rate=(int32_t)p[8];s->bits=(int32_t)p[9];s->channels=(int32_t)p[10];s->disabled=p[13];s->digital=0x70123456;s->sample_count=0;s->count3d=0;
    if(p[7]&&p[0]!=3) {
        s->sample_count=2;s->handles=HEAP+0x1000;s->auxiliary=HEAP+0x1100;s->samples=HEAP+0x1200;s->cache=HEAP+0x1400;
        track(s->handles);track(s->auxiliary);track(s->samples);track(s->cache);
        for(i=0;i<2;++i) {U(s->handles+i*4)=0x71000000u+i;U(s->cache+i*12)=20+i;U(s->cache+i*12+4)=0x32000000u+i;U(s->cache+i*12+8)=1;}
        s->count3d=2;s->handles3d=HEAP+0x1700;U(s->handles3d)=0x72000000;U(s->handles3d+4)=0x72000001;
    }
    if(p[7]&&p[0]==3) {
        s->files.capacity=3;s->files.count=2;s->files.entries=HEAP+0x1804;track(HEAP+0x1800);
        U(HEAP+0x1800)=3;U(HEAP+0x1804)=0x32000080;U(HEAP+0x1808)=1;
        U(HEAP+0x180c)=0x32000100;U(HEAP+0x1810)=0;U(HEAP+0x1814)=0;U(HEAP+0x1818)=1;
    }
}
static int run(const uint32_t p[16])
{
    RestoreSound *s=(RestoreSound *)(uintptr_t)OBJECT;prepare(p);++scenarios;
    switch(p[0]) {
    case 0:return 1;
    case 1:return FUN_0055e6c4(s);
    case 2:return FUN_0055dea2(s,(int32_t)p[6]);
    case 3:return FUN_0055dc63(&s->files,(int32_t)p[6]);
    case 4:FUN_0055ec7c(s);return 0;
    case 5:FUN_0055e5d2(s,(int)p[15]);return 0;
    default:REQUIRE(0);return 0;
    }
}
static void hex(const void *data,size_t n) {size_t i;putchar('"');for(i=0;i<n;++i)printf("%02x",((const uint8_t *)data)[i]);putchar('"');}
static void oracle(void)
{
    uint32_t p[16];unsigned int i,j;int result;puts("{\"object\":805306368,\"heap\":822083584}");fflush(stdout);
    for(;;) {
        for(i=0;i<16;++i)if(scanf("%u",&p[i])!=1)return;
        result=run(p);printf("{\"result\":%d,\"object\":",result);hex((void *)OBJECT,sizeof(RestoreSound));
        printf(",\"counter\":%u,\"owner\":%u,\"heap\":",U(0x12663b0),U(0x12663ac));hex((void *)(uintptr_t)(HEAP+0x10000),cursor-HEAP-0x10000);
        printf(",\"old_heap\":");hex((void *)(uintptr_t)(HEAP+0x1000),0x1000);printf(",\"events\":[");
        for(i=0;i<event_count;++i) {printf("%s[",i?",":"");for(j=0;j<6;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
    }
}
int main(int argc,char **argv)
{
    uint32_t p[16]={1,0,2,10,1,0,5,0,22050,16,2,0,0,0,0,0};RestoreSound *s=(RestoreSound *)(uintptr_t)OBJECT;unsigned int i;
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)OBJECT,0x1000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)OBJECT);
    REQUIRE(VirtualAlloc((void *)HEAP,0x200000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)HEAP);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch"))oracle();
    else {
        REQUIRE(run(p)==1&&s->sample_count==5&&s->format.bytes_per_second==88200&&s->format.block==4&&live_count==4);
        REQUIRE(U(s->samples)==0xffffffffu&&U(s->samples+8)==100&&U(s->cache)==0xffffffffu&&U(s->cache+4)==0);
        p[3]=4;REQUIRE(run(p)==0&&s->disabled==1&&event_count==1);p[3]=10;
        p[4]=0;REQUIRE(run(p)==0&&event_count==1);p[4]=1;
        p[5]=1;REQUIRE(run(p)==0&&event_count==6&&live_count==0);p[5]=0;
        for(i=1;i<=4;++i) {
            p[11]=i;REQUIRE(run(p)==0&&live_count==i-1&&s->sample_count==0);
            REQUIRE((s->handles!=0)==(i>1)&&(s->auxiliary!=0)==(i>2)&&(s->samples!=0)==(i>3)&&!s->cache);
        }p[11]=0;
        for(i=1;i<=5;++i) {
            p[12]=i;REQUIRE(run(p)==1&&live_count==4&&s->sample_count==5&&s->samples);
            REQUIRE(U(s->handles+(i-1)*4)==0);
        }p[12]=0;
        p[0]=2;p[7]=1;REQUIRE(run(p)==1&&live_count==4&&free_count==4&&s->sample_count==5);
        p[6]=0x7fffffff;p[11]=1;
        REQUIRE(run(p)==0&&live_count==0&&free_count==4&&s->sample_count==2&&allocations==1);
        REQUIRE(events[event_count-1][0]==0x561a1d&&events[event_count-1][2]==0xfffffffcu);p[11]=0;
        p[0]=3;p[6]=500;REQUIRE(run(p)==1&&s->files.capacity==500&&s->files.count==0&&live_count==1&&U(s->files.entries)==0);
        p[11]=1;REQUIRE(run(p)==0&&live_count==0);p[11]=0;
        p[0]=0;p[1]=0xa5;REQUIRE(run(p)==1&&s->rate==22050&&s->bits==16&&s->channels==2&&s->settings[9]==-1&&U(0x12663b0)==0x1020304c);
        REQUIRE(!memcmp(s->providers,(void *)0x604628u,sizeof(s->providers)));
        printf("Sound initialization: %u checks across %u scenarios; format, receiver, channels, cache defaults, reallocation, owned files and retained partial allocation/handle failure state verified without audio output.\n",checks,scenarios);
    }
    VirtualFree((void *)OBJECT,0,MEM_RELEASE);VirtualFree((void *)HEAP,0,MEM_RELEASE);restore_data_close();return 0;
}
