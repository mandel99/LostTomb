#include "campaign_menu.h"
#include "campaign_index.h"
#include "video_player.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#define BASE 0x30000000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern void FUN_0041f650(void);
static uint32_t count,events[96][18],checks,mission_result;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Campaign menu failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t hash(const char *s) {uint32_t v=2166136261u;while(*s)v=(v^(uint8_t)*s++)*16777619u;return v;}
static void event(uint32_t address,uint32_t n,const uint32_t *v)
{
    REQUIRE(count<96&&n<=8);memset(events[count],0,sizeof(events[count]));events[count][0]=address;events[count][1]=n;
    if(n)memcpy(events[count]+2,v,n*4);
    events[count][10]=U(0x605a3c);events[count][11]=B(0x6de515);events[count][12]=U(0xe38ebc);events[count][13]=U(0x5d2a2c);
    events[count][14]=B(0x7027b4);events[count][15]=B(0xe38dca);events[count][16]=U(0x704dac);events[count][17]=U(0x704db0);++count;
}
int FUN_0053e590(int image,int x,int y) {uint32_t v[3]={image,x,y};event(0x53e590,3,v);return 1;}
void FUN_004cc110(int group,int index) {uint32_t v[2]={group,index};event(0x4cc110,2,v);U(0xe20768)=BASE+0x4000;}
void FUN_00448370(const char *text,char *out,int capacity)
{uint32_t v[3]={hash(text),capacity,U(0x784f0c)};event(0x448370,3,v);REQUIRE(capacity==200);strcpy(out,"Restore campaign");}
void FUN_004cc720(const char *text,int x,int y,int w,int font,int color) {uint32_t v[6]={hash(text),x,y,w,font,color};event(0x4cc720,6,v);}
void FUN_004ccb30(int group,int index,int x,int y,int w,int font,int color) {uint32_t v[7]={group,index,x,y,w,font,color};event(0x4ccb30,7,v);}
void FUN_004ccfd0(int group,int index,int x,int y,int w,int font,int color) {uint32_t v[7]={group,index,x,y,w,font,color};event(0x4ccfd0,7,v);}
void FUN_004ccbb0(int group,int index,int x,int y,int font,int color) {uint32_t v[6]={group,index,x,y,font,color};event(0x4ccbb0,6,v);}
void FUN_00536710(int kind,int x,int y,int w,int selected) {uint32_t v[5]={kind,x,y,w,selected};event(0x536710,5,v);}
void FUN_004cd240(int group,int index,const void *rect,int font,int color,int x,int y) {uint32_t v[7]={group,index,(uint32_t)rect,font,color,x,y};event(0x4cd240,7,v);}
void FUN_004cd1a0(int kind,const void *rect,int selected,int x,int y) {uint32_t v[5]={kind,(uint32_t)rect,selected,x,y};event(0x4cd1a0,5,v);}
unsigned char FUN_00413580(RestoreVideoPlayer *self,const char *path,int x,int y)
{uint32_t v[4]={(uint32_t)self,(uint32_t)path,x,y};event(0x413580,4,v);return 0;}
void FUN_00417dd0(int8_t category) {uint32_t v[1]={(uint32_t)(int32_t)category};event(0x417dd0,1,v);B(0xe38ea8)=mission_result;}
void FUN_004181e0(void) {event(0x4181e0,0,NULL);B(0xe38dda)=0x35;}
void FUN_0041f870(void) {event(0x41f870,0,NULL);B(0xe38dda)=61;B(0xe38dca)=0x77;}
void FUN_004cb6a0(void) {event(0x4cb6a0,0,NULL);B(0xe38dda)=48;B(0xe38dca)=1;}
static const uint32_t regions[][2]={{0x6fdc98,4},{0x7027ac,9},{0x605a3c,4},{0x6de515,1},{0xe38ebc,4},{0x5d2a2c,4},{0xe38dc9,2},{0xe38dda,1},{0xe38ea8,1},{0x784f0c,4},{0x704dac,8},{0xd3b568,16},{0xe20768,4},{0xf53f28,4}};
/* op,type,selection,gate,hover,width,height,origin_x,origin_y,
   movie_variant,mission_result,current_screen,pending_load,clicked_category */
static uint32_t run(const uint32_t p[14])
{
    uint32_t i,result=0;REQUIRE(p[0]<8);count=0;mission_result=p[10];
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)(uintptr_t)regions[i][0],0xa5,regions[i][1]);
    U(0x6fdc98)=p[1];U(0x7027ac)=p[2];U(0x7027b0)=p[3];U(0x7056c8)=p[4];U(0xe38db0)=p[5];U(0xe38dac)=p[6];
    U(0xe38da8)=p[7];U(0xe38da4)=p[8];U(0xe38edc)=p[9];B(0xe38dda)=p[11];U(0x605a3c)=p[12];U(0xd3b198)=p[13];
    U(0x6fdc54)=BASE;U(0x784f0c)=258;U(0xf55c94)=35000;U(0xf57314)=46000;U(0xf55cbc)=37000;
    U(0xf56f8c)=1005;U(0xf56f94)=335;U(0xf56fa4)=871;U(0xf56fb4)=18765;U(0xf56fbc)=22345;
    strcpy((char *)(BASE+0x4000),"[player_name] campaign");
    switch(p[0]) {case 0:FUN_0041be10();break;case 1:FUN_0041c2d0();break;case 2:result=FUN_0041bb30();break;case 3:FUN_0041bb10();break;case 4:FUN_0041bb70();break;case 5:FUN_0041c310();break;case 6:FUN_0041c350();break;case 7:FUN_0041f650();break;}
    return result;
}
static void output(uint32_t result)
{
    uint32_t i,j;printf("{\"result\":%u,\"state\":\"",result);
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j)printf("%02x",B(regions[i][0]+j));
    printf("\",\"events\":[");for(i=0;i<count;++i){printf("%s[",i?",":"");for(j=0;j<18;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[14]={0,1,6,0,11,1024,768,192,144,233,0,23,0,258},i,result;
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":14}");fflush(stdout);for(;;){for(i=0;i<14;++i)if(scanf("%u",p+i)!=1)goto done;result=run(p);output(result);}
    } else {
        run(p);REQUIRE(count>25&&B(0xe38dc9)==1&&U(0x784f0c)==258);
        p[0]=2;REQUIRE(run(p)==2);p[0]=3;run(p);REQUIRE(U(0x7027ac)==258);
        p[0]=4;p[2]=0;run(p);REQUIRE(B(0xe38dda)==0x35&&B(0x7027b4)==2&&count==3);
        p[10]=127;run(p);REQUIRE(B(0xe38dda)==0x34&&count==2);
        p[0]=5;run(p);REQUIRE(B(0xe38dda)==61&&U(0x704dac)==2&&U(0x704db0)==1);
        p[0]=7;run(p);REQUIRE(B(0xe38dda)==48);
        printf("Campaign menu: %u checks; selector rendering, predicates, selection, movies, confirmation, tabs and back navigation.\n",checks);
    }
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
