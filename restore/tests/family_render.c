#include "restore_types.h"
#include "standalone_data.h"
#include "sound_streams.h"
#include <stdio.h>
#include <string.h>
#define BASE 0x30000000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern void FUN_00516630(void);
extern void FUN_00516720(void);
extern void FUN_004cb4d0(void);
extern void FUN_004cb5a0(void);
static uint32_t count,events[128][10],checks,playing;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Family render failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t hash(const char *s) {uint32_t v=2166136261u;while(*s)v=(v^(uint8_t)*s++)*16777619u;return v;}
static void event(uint32_t a,uint32_t n,const uint32_t *v)
{REQUIRE(count<128&&n<=8);memset(events[count],0,sizeof(events[count]));events[count][0]=a;events[count][1]=n;memcpy(events[count]+2,v,n*4);++count;}
void FUN_004cf1c0(void) {event(0x4cf1c0,0,NULL);}
int FUN_0053e590(int image,int x,int y) {uint32_t v[3]={image,x,y};event(0x53e590,3,v);return 1;}
void FUN_00536820(int x,int y,int w,int h) {uint32_t v[4]={x,y,w,h};event(0x536820,4,v);}
void FUN_00536a30(int x,int y,int w,int h) {uint32_t v[4]={x,y,w,h};event(0x536a30,4,v);}
void FUN_00536c30(int kind,int x,int y,int w,int h,int selected) {uint32_t v[6]={kind,x,y,w,h,selected};event(0x536c30,6,v);}
void FUN_00425730(int x,int y,const void *buttons,int n) {uint32_t v[4]={x,y,(uint32_t)buttons,n};event(0x425730,4,v);}
void FUN_004ccb30(int group,int index,int x,int y,int w,int font,int color) {uint32_t v[7]={group,index,x,y,w,font,color};event(0x4ccb30,7,v);}
void FUN_004cc770(const char *text,int x,int y,int font,int color) {uint32_t v[5]={hash(text),x,y,font,color};event(0x4cc770,5,v);}
void FUN_004cc720(const char *text,int x,int y,int w,int font,int color) {uint32_t v[6]={hash(text),x,y,w,font,color};event(0x4cc720,6,v);}
void FUN_004cc110(int group,int index) {uint32_t v[2]={group,index};event(0x4cc110,2,v);U(0xe20768)=BASE+0x4000;}
void FUN_00448370(const char *text,char *out,int capacity)
{uint32_t v[3]={hash(text),capacity,U(0x784f0c)};event(0x448370,3,v);REQUIRE(capacity>=15);strcpy(out,"Restore family");}
int FUN_004cde60(int n,int d) {return n*100/d;}
int FUN_004cde40(int value,int percent) {return value*percent/100;}
void FUN_0055f561(RestoreSound *self,int channel) {uint32_t v[2]={(uint32_t)self,channel};event(0x55f561,2,v);}
uint32_t FUN_0055f5a1(RestoreSound *self,int channel) {uint32_t v[2]={(uint32_t)self,channel};event(0x55f5a1,2,v);return playing;}
uint32_t FUN_0055f449(RestoreSound *self,const char *path,int channel,uint32_t loops) {uint32_t v[4]={(uint32_t)self,hash(path),channel,loops};event(0x55f449,4,v);return 1;}
static void run(const uint32_t p[12])
{
    uint32_t i;REQUIRE(p[0]<4&&p[1]<=300);memset((void *)BASE,0,0x10000);
    U(0xeadfac)=BASE+0x1000;U(0xeadfb8)=BASE+0x2000;U(0xeadfb0)=p[1];U(0xeadfc8)=p[2];U(0xeadfcc)=p[3];U(0x7056c8)=p[4];B(0xe38dca)=p[5];B(0xe38dc9)=0;
    U(0xe38da8)=p[6];U(0xe38da4)=p[7];U(0xf55c9c)=35000;U(0xf56494)=46000;U(0xf56f8c)=1005;U(0xf56f94)=335;U(0xf56f9c)=871;U(0xf56fb4)=18765;U(0xf55cac)=37000;
    B(0xd3b188)=(p[8]&1)!=0;B(0xd3b18a)=(p[8]&2)!=0;B(0xd3b18b)=(p[8]&4)!=0;
    playing=p[9];U(0xf55b88)=p[10];B(0xe38e64)=1;U(0x784f0c)=p[11];U(0xe20768)=0x12345678;strcpy((char *)(BASE+0x4000),"[player_name] family");
    for(i=0;i<p[1];++i){U(BASE+0x1000+i*4)=i*32;sprintf_s((char *)(BASE+0x2000+i*32),32,"Family%03u",i);}
    count=0;
    switch(p[0]) {case 0:FUN_00516630();break;case 1:FUN_00516720();break;case 2:FUN_004cb4d0();break;case 3:FUN_004cb5a0();break;}
}
static void output(void)
{
    uint32_t i,j;printf("{\"state\":[%u,%u,%u],\"events\":[",B(0xe38dc9),U(0x784f0c),U(0xe20768));
    for(i=0;i<count;++i){printf("%s[",i?",":"");for(j=0;j<10;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[12]={0,260,248,259,13,1,192,144,7,0,256,258},i;
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":12}");fflush(stdout);for(;;){for(i=0;i<12;++i)if(scanf("%u",p+i)!=1)goto done;run(p);output();}
    } else {
        run(p);REQUIRE(events[1][2]==35000&&events[1][3]==192&&events[1][4]==144&&B(0xe38dc9)==1);
        p[0]=2;run(p);REQUIRE(U(0x784f0c)==258&&events[1][2]==37000);
        printf("Family rendering: %u checks; full IDs/fonts/coordinates, button availability, title and setup audio.\n",checks);
    }
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
