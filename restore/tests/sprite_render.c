#include "sprite_render.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define BASE 0x30000000u
static uint32_t events[64][8],event_count,checks,scenarios,io_result;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Sprite dispatch failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static void event(uint32_t address,unsigned int n,uint32_t a,uint32_t b,uint32_t c,uint32_t d,uint32_t e)
{
    uint32_t *row;REQUIRE(event_count<64);row=events[event_count++];
    row[0]=address;row[1]=n;row[2]=a;row[3]=b;row[4]=c;row[5]=d;row[6]=e;row[7]=0;
}
static uint32_t hash(const char *text) {uint32_t value=2166136261u;while(*text)value=(value^(uint8_t)*text++)*16777619u;return value;}
static void put32(void *p,uint32_t value) {memcpy(p,&value,4);}
static void prepare(const uint32_t p[21])
{
    RestoreSpriteLibraries *self=(RestoreSpriteLibraries *)(uintptr_t)BASE;
    unsigned int i;uint16_t width=(uint16_t)p[1],height=(uint16_t)p[2],tint=(uint16_t)p[10];
    memset(self,0,sizeof(*self));self->next_image=p[16];event_count=0;io_result=p[18];
    for(i=0;i<1024;++i) {
        uint8_t *image=self->images[i].bytes;
        put32(image,p[17]);put32(image+4,64);memcpy(image+20,&width,2);memcpy(image+22,&height,2);
        image[0x34]=(uint8_t)p[5];image[0x33]=(uint8_t)p[6];image[0x32]=(uint8_t)p[7];image[0x38]=(uint8_t)(i&1);
        put32(image+16,p[9]);
    }
    if(p[0]<1024)self->images[p[0]].bytes[0x3e]=(uint8_t)p[8];
    strcpy(self->libraries[0].files[0],"picture0.bmp");strcpy(self->libraries[0].files[1],"picture1.bmp");
    memset((void *)0xd3b530u,0x5a,180);memset((void *)0xe38aa8u,0x5a,260);memcpy((void *)0x70e120u,&tint,2);
    U(0xe38d9c)=p[11];U(0xe38d98)=p[12];U(0xe38db0)=p[13];U(0xe38dac)=p[14];
    U(0xe38d94)=p[11]*2u;U(0xe92e1c)=0x31200000u;U(0xe92e18)=0x31100000u;
    U(0xd3b558)=p[11];U(0xd3b550)=p[12];U(0xd3b55c)=p[19];U(0xd3b554)=p[20];
    B(0xe38dda)=(uint8_t)p[15];U(0xf56db4)=p[0];
}
void FUN_004ce310(char *path,const char *extension)
{
    char *dot=strrchr(path,'.');(void)extension;REQUIRE(dot);strcpy(dot,".555");
    event(0x4ce310,1,hash(path),0,0,0,0);
}
int FUN_004ce480(const char *path,uint32_t dest,uint32_t bytes,uint32_t offset)
{event(0x4ce480,4,hash(path),dest,bytes,offset,0);return (int)io_result;}
int FUN_0053f7d0(const char *path,uint32_t dest,uint32_t bytes,uint32_t rows,uint32_t offset)
{event(0x53f7d0,5,hash(path),dest,bytes,rows,offset);return (int)io_result;}
void FUN_0053f790(uint32_t *slot,char force)
{event(0x53f790,2,(uint32_t)(uintptr_t)slot,(uint8_t)force,0,0,0);if(*slot!=U(0xe92e1c)||force)*slot=0;}
void FUN_004cdf20(uint32_t pointer,int bytes) {event(0x4cdf20,2,pointer,(uint32_t)bytes,0,0,0);}
void FUN_004cdeb0(uint32_t pointer,int bytes) {event(0x4cdeb0,2,pointer,(uint32_t)bytes,0,0,0);}
void FUN_0040fb60(const void *pointer) {event(0x0040fb60,1,(uint32_t)(uintptr_t)pointer,0,0,0,0);}
int FUN_0040fc40(uint32_t pointer) {event(0x0040fc40,1,pointer,0,0,0,0);return 77;}
int FUN_0040fcf0(uint32_t pointer) {event(0x0040fcf0,1,pointer,0,0,0,0);return 77;}
int FUN_0040fdd0(uint32_t pointer) {event(0x0040fdd0,1,pointer,0,0,0,0);return 77;}
int FUN_0040fed0(uint32_t pointer) {event(0x0040fed0,1,pointer,0,0,0,0);return 77;}
int FUN_0040ff50(uint32_t pointer) {event(0x0040ff50,1,pointer,0,0,0,0);return 77;}
void FUN_0040ffd0(const void *pointer) {event(0x0040ffd0,1,(uint32_t)(uintptr_t)pointer,0,0,0,0);}
void FUN_00410090(const void *pointer) {event(0x00410090,1,(uint32_t)(uintptr_t)pointer,0,0,0,0);}
void FUN_00410230(const void *pointer) {event(0x00410230,1,(uint32_t)(uintptr_t)pointer,0,0,0,0);}
int FUN_004105c0(uint32_t pointer) {event(0x004105c0,1,pointer,0,0,0,0);return 77;}
int FUN_00410660(uint32_t pointer) {event(0x00410660,1,pointer,0,0,0,0);return 77;}
void FUN_00410b10(const void *pointer) {event(0x00410b10,1,(uint32_t)(uintptr_t)pointer,0,0,0,0);}
int FUN_00410de0(uint32_t pointer) {event(0x00410de0,1,pointer,0,0,0,0);return 77;}
int FUN_00410e80(uint32_t pointer) {event(0x00410e80,1,pointer,0,0,0,0);return 77;}
int FUN_00410f70(uint32_t pointer) {event(0x00410f70,1,pointer,0,0,0,0);return 77;}
int FUN_00410fb0(uint32_t pointer) {event(0x00410fb0,1,pointer,0,0,0,0);return 77;}
int FUN_004114a0(uint32_t pointer) {event(0x004114a0,1,pointer,0,0,0,0);return 77;}
int FUN_00411fd0(uint32_t pointer) {event(0x00411fd0,1,pointer,0,0,0,0);return 77;}
int FUN_004100f0(uint32_t pointer,uint32_t value) {event(0x004100f0,2,pointer,value&65535u,0,0,0);return 77;}
int FUN_004101c0(uint32_t pointer,uint32_t value) {event(0x004101c0,2,pointer,value&65535u,0,0,0);return 77;}
int FUN_00410700(uint32_t pointer,uint32_t value) {event(0x00410700,2,pointer,value&65535u,0,0,0);return 77;}
int FUN_004109d0(uint32_t pointer,uint32_t value) {event(0x004109d0,2,pointer,value&65535u,0,0,0);return 77;}
int FUN_00410a70(uint32_t pointer,uint32_t value) {event(0x00410a70,2,pointer,value&65535u,0,0,0);return 77;}
int FUN_00427a20(uint32_t pointer,uint32_t value) {event(0x00427a20,2,pointer,value,0,0,0);return 77;}
int FUN_0042a160(uint32_t pointer,uint32_t value) {event(0x0042a160,2,pointer,value,0,0,0);return 77;}
int FUN_0042b440(uint32_t pointer,uint32_t value) {event(0x0042b440,2,pointer,value,0,0,0);return 77;}
int FUN_0051f780(uint32_t pointer,uint32_t value) {event(0x0051f780,2,pointer,value,0,0,0);return 77;}
int FUN_0042c740(void) {event(0x0042c740,0,0,0,0,0,0);return 77;}
int FUN_0042c9a0(void) {event(0x0042c9a0,0,0,0,0,0,0);return 77;}
int FUN_0042cee0(void) {event(0x0042cee0,0,0,0,0,0,0);return 77;}
int FUN_0042d810(void) {event(0x0042d810,0,0,0,0,0,0);return 77;}
int FUN_0042e680(void) {event(0x0042e680,0,0,0,0,0,0);return 77;}

static int run(const uint32_t p[21]) {prepare(p);++scenarios;return restore_sprite_draw((RestoreSpriteLibraries *)(uintptr_t)BASE,(int32_t)p[0],(int32_t)p[3],(int32_t)p[4]);}
static void hex(const void *memory,size_t size) {size_t i;putchar('"');for(i=0;i<size;++i)printf("%02x",((const uint8_t *)memory)[i]);putchar('"');}
static void oracle(void)
{
    uint32_t p[21];unsigned int i,j;int result;
    puts("{\"base\":805306368}");fflush(stdout);
    while(1) {
        for(i=0;i<21;++i)if(scanf("%u",&p[i])!=1)return;
        result=run(p);printf("{\"result\":%d,\"globals\":",result);hex((void *)0xd3b530u,180);
        printf(",\"filename\":");hex((void *)0xe38aa8u,260);
        printf(",\"scratch\":%u,\"events\":[",(unsigned int)(uintptr_t)((RestoreSpriteLibraries *)(uintptr_t)BASE)->scratch);
        for(i=0;i<event_count;++i) {printf("%s[",i?",":"");for(j=0;j<8;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
    }
}
int main(int argc,char **argv)
{
    uint32_t p[21]={5,8,7,2,3,0,0,1,0,1,0,640,480,640,480,0,16,0x31000000u,1,0,0};
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)(uintptr_t)BASE,0x400000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)(uintptr_t)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch"))oracle();
    else {
        REQUIRE(run(p)==8&&event_count==1&&events[0][0]==0x40fb60);
        p[5]=1;REQUIRE(run(p)==8&&event_count>=4&&events[0][0]==0x4ce310);
        p[18]=0;REQUIRE(run(p)==0&&events[event_count-1][0]==0x4ce480);
        p[5]=0;p[18]=1;p[0]=0;REQUIRE(run(p)==0&&event_count==0);
        p[0]=25500;REQUIRE(run(p)==0&&event_count==0);
        p[0]=5;p[1]=8;p[3]=0xfffffffdu;REQUIRE(run(p)==5&&B(0xd3b531)==1&&I(0xd3b54c)==3);
        p[0]=509;p[16]=683;p[3]=2;REQUIRE(run(p)==8&&I(0xd3b580)==509&&event_count==1);
        printf("Sprite dispatch: %u checks, %u boundary scenarios; explicit receiver, image IDs above one byte, file paths, complete clip state and endpoint arguments verified with controlled rendering endpoints.\n",checks,scenarios);
    }
    VirtualFree((void *)(uintptr_t)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
