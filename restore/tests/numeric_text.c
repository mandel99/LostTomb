#include "standalone_data.h"
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define BASE 0x30000000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define REQUIRE(x) do {if(!(x)){fprintf(stderr,"Numeric text failure at %d\n",__LINE__);ExitProcess(1);}} while(0)
extern int FUN_004cc5c0();extern void FUN_004ccc30();extern void FUN_004cccb0();
extern void FUN_004cce00();extern void FUN_004cce90();extern void FUN_004ccf20();
static uint32_t events[8][11],event_count;
static const char *strings[]={""," People","Prefix"," / 1000"};
static uint32_t hash(const char *s){uint32_t n=2166136261u;while(*s)n=(n^(uint8_t)*s++)*16777619u;return n;}
static void note(uint32_t address,uint32_t count,const uint32_t *args,uint32_t width)
{REQUIRE(event_count<8);memset(events[event_count],0,sizeof(events[0]));events[event_count][0]=address;events[event_count][1]=count;memcpy(events[event_count]+2,args,count*4);events[event_count++][10]=U(0xe20778);U(0xe20778)+=width;}
void FUN_004cc770(const char *text,int x,int y,int font,int color)
{uint32_t p[]={hash(text),x,y,font,color};note(0x4cc770,5,p,(uint32_t)strlen(text)*7);}
void FUN_004cc720(const char *text,int x,int y,int width,int font,int color)
{uint32_t p[]={hash(text),x,y,width,font,color};note(0x4cc720,6,p,(uint32_t)strlen(text)*7);}
void FUN_004ccbb0(int group,int label,int x,int y,int font,int color)
{uint32_t p[]={group,label,x,y,font,color};note(0x4ccbb0,6,p,300+(label&15));}
static uint32_t run(uint32_t p[9])
{
    uint32_t i;REQUIRE(p[0]<6&&p[3]<4&&p[4]<4);event_count=0;
    memset((void *)0xe34238u,0xa5,288);U(0xe20778)=258;U(0xd3b504)=0x12345678;
    for(i=0;i<4;++i)strcpy((char *)(BASE+i*128),strings[i]);
    switch(p[0]) {
    case 0:return FUN_004cc5c0(p[1],p[2],p[3]);
    case 1:FUN_004cccb0(p[2],p[1],p[5],p[6],p[7],p[8]);break;
    case 2:FUN_004cce00(p[1],p[2],BASE+p[4]*128,p[5],p[6],p[7],p[8]);break;
    case 3:FUN_004cce90(p[1],p[2],BASE+p[4]*128,p[5],p[6],314,p[7],p[8]);break;
    case 4:FUN_004ccf20(p[1],BASE+p[3]*128,BASE+p[4]*128,p[5],p[6],314,p[7],p[8]);break;
    case 5:FUN_004ccc30(44,128,p[1],p[5],p[6],p[7],p[8]);break;
    }
    return 0;
}
static void output(uint32_t result)
{
    uint32_t i,j;char hex[577];const uint8_t *bytes=(const uint8_t *)0xe34238u;static const char digits[]="0123456789abcdef";
    for(i=0;i<288;++i){hex[i*2]=digits[bytes[i]>>4];hex[i*2+1]=digits[bytes[i]&15];}hex[576]=0;
    printf("{\"result\":%u,\"advance\":%u,\"flag\":%u,\"events\":[",result,U(0xe20778),U(0xd3b504));
    for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<11;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}printf("],\"state\":\"%s\"}\n",hex);fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[9]={2,1000,32,0,0,258,259,335,0},i,result;
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {puts("{\"fields\":9}");fflush(stdout);for(;;){for(i=0;i<9;++i)if(scanf("%u",p+i)!=1)goto done;result=run(p);output(result);}}
    else {REQUIRE(argc==1);run(p);REQUIRE(!strcmp((const char *)0xe34238u," 1000"));p[0]=1;p[1]=(uint32_t)-35;p[2]=1;run(p);REQUIRE(!strcmp((const char *)0xe34238u," 35 ")&&event_count==2&&events[0][3]==258);puts("Numeric text: complete digits, signed years, suffix/prefix composition and full-width advances.");}
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
