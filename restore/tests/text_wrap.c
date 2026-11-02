#include "sprite_libraries.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#define BASE 0x30000000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
extern int FUN_004cc020(uint8_t,int);
extern int FUN_004cd120(const char *,int);
extern void FUN_004cd010(const char *,int,int,int,int,int);
extern void FUN_004ccfd0(int,int,int,int,int,int,int);
static const char *texts[]={"","ABC","ABC DEF GHI JKL"," ABC  DEF","AB$CD EF","one two three four five six seven eight nine ten","A_#B C","ABCDEFGHIJKLMNOPQRSTUVWXYZ", "This deletes the selected dynasty and all of its saved games. Do you really want to do that?"};
static uint32_t event_count,events[100][5],checks,height,advance;
static char lines[100][201];
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Text wrapping failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
void FUN_004cc110(int group,int entry) {REQUIRE(group==5&&entry==1);U(0xe20768)=BASE;}
void FUN_004cc770(const char *text,int x,int y,int font,int color)
{
    REQUIRE(event_count<100&&strlen(text)<201);strcpy_s(lines[event_count],201,text);
    events[event_count][0]=x;events[event_count][1]=y;events[event_count][2]=font;events[event_count][3]=color;events[event_count][4]=U(0xe20780);++event_count;
    U(0xd3b500)=height;U(0xe20778)=(uint32_t)strlen(text)*advance;
}
static uint32_t run(const uint32_t p[8])
{
    unsigned int i;REQUIRE(p[0]<4&&(p[0]==0||p[1]<10));memset((void *)BASE,0,0x10000);
    for(i=0;i<256;++i)((uint8_t *)0x5e047cu)[i]=(i%7)?(uint8_t)(i%19+1):0;
    for(i=0;i<2048;++i){int16_t w=(int16_t)((i*3+17)%19+1);memcpy(RESTORE_SPRITE_LIBRARIES->images[i].bytes+20,&w,2);}
    U(0x126517c)=p[4];U(0xd3b5c0)=0x12345678;U(0xd3b500)=11;U(0xe20760)=0x12345678;U(0xe2076c)=p[6];U(0xe20774)=0x12345678;U(0xe20778)=0x76543210;U(0xe20780)=3;*(uint8_t *)0xe20799u=0;
    U(0xe20768)=0x98765432;memset((void *)0xe3439cu,0xa5,200);event_count=0;height=p[7];advance=7;
    if(p[0]==0)return FUN_004cc020((uint8_t)p[2],(int)p[3]);
    if(p[1]==9)memset((void *)BASE,'A',300);else strcpy((char *)BASE,texts[p[1]]);
    if(p[0]==1)return FUN_004cd120((char *)BASE,(int)p[3]);
    if(p[0]==2)FUN_004cd010((char *)BASE,500,300,(int)p[5],(int)p[3],0xffff);
    else FUN_004ccfd0(5,1,500,300,(int)p[5],(int)p[3],0xffff);
    return 0;
}
static void output(uint32_t result)
{
    static const uint32_t fields[]={0xd3b5c0,0xd3b500,0xe20760,0xe2076c,0xe20774,0xe20778,0xe20780,0xe20768};
    unsigned int i,j;printf("{\"result\":%u,\"state\":[",result);
    for(i=0;i<8;++i)printf("%s%u",i?",":"",U(fields[i]));printf(",%u],\"buffer\":\"",*(uint8_t *)0xe20799u);
    for(i=0;i<200;++i)printf("%02x",*(uint8_t *)(0xe3439cu+i));printf("\",\"events\":[");
    for(i=0;i<event_count;++i){printf("%s[[",i?",":"");for(j=0;j<5;++j)printf("%s%u",j?",":"",events[i][j]);printf("],\"");for(j=0;lines[i][j];++j)printf("%02x",(uint8_t)lines[i][j]);printf("\"]");}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[8]={0,0,'A',335,2048,100,0,11},i,result;
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":8}");fflush(stdout);for(;;){for(i=0;i<8;++i)if(scanf("%u",p+i)!=1)goto done;result=run(p);output(result);}
    } else {
        REQUIRE(run(p)>0&&U(0xd3b5c0)!=0);p[0]=2;p[1]=8;run(p);REQUIRE(event_count>1&&strlen(lines[0])<strlen(texts[8]));
        p[5]=1;run(p);REQUIRE(event_count==99&&lines[0][0]==0);
        printf("Text wrapping: %u checks; byte glyph mapping, 64-byte metrics, word lengths, bounded lines and full-width layout state.\n",checks);
    }
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
