#include "menu_dialog.h"
#include "menu_input.h"
#include "text_selection.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#define BASE 0x30000000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern int FUN_004cc6c0(int,int,uint32_t);
static uint32_t event_count,events[128][10],checks,measured,mutate,return_mode;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Modal failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static void event(uint32_t a,uint32_t n,const uint32_t *v)
{REQUIRE(event_count<128&&n<=8);memset(events[event_count],0,sizeof(events[event_count]));events[event_count][0]=a;events[event_count][1]=n;if(n)memcpy(events[event_count]+2,v,n*4);++event_count;}
void restore_menu_callback(uint32_t address)
{
    if(address==0x426590){FUN_00426590();return;}
    if(address==0x4265b0){FUN_004265b0();return;}
    if(address==0x51f780)return;
    REQUIRE(address==BASE+0xf000);
    {uint32_t v[3]={B(0xd3b439),U(0x70570c),B(0xe38dda)};event(address,3,v);}
    if(mutate) {U(0x70570c)=return_mode;B(0xe38dda)=0x55;B(0xe38dca)=0x7a;}
}
void FUN_00536820(int x,int y,int w,int h) {uint32_t v[4]={x,y,w,h};event(0x536820,4,v);}
void FUN_004ccb30(int g,int i,int x,int y,int w,int f,int c) {uint32_t v[7]={g,i,x,y,w,f,c};event(0x4ccb30,7,v);}
void FUN_004ccfd0(int g,int i,int x,int y,int w,int f,int c) {uint32_t v[7]={g,i,x,y,w,f,c};event(0x4ccfd0,7,v);}
void FUN_00425730(int x,int y,const void *p,int n) {uint32_t v[4]={x,y,(uint32_t)p,n};event(0x425730,4,v);}
uint32_t FUN_0047d350(uint32_t sound) {uint32_t v[1]={sound};event(0x47d350,1,v);return 0;}
int FUN_004cbf30(const char *text,uint32_t font)
{uint32_t v[3]={(uint32_t)text,font,(uint32_t)strlen(text)};event(0x4cbf30,3,v);return (int)measured;}
static uint32_t run(const uint32_t p[14])
{
    unsigned int i;uint32_t result=0;static const uint32_t flags[]={0xe38e00,0xe38e03,0xe38e04,0xe38e28,0xe38df8,0xe38df4,0xe38df7};
    REQUIRE(p[0]<7);measured=p[11];mutate=p[10];return_mode=p[9];event_count=0;
    memset((void *)BASE,0,0x10000);memcpy((void *)(BASE+0x1000),"Alpha\0Beta\0Gamma\0Tail\0",22);
    U(0xe36c70)=BASE;U(0xe34c38+5*8)=0x1000;U(0xe34c3c+5*8)=0xeeeeeeee;U(0xe20768)=0x12345678;
    U(0x705704)=BASE+0xf000;U(0x705708)=p[1];U(0x70570c)=p[2];U(0x705710)=p[3];U(0x705718)=p[4];U(0x705714)=p[5];
    U(0xe38da8)=p[4];U(0xe38da4)=p[5];U(0xe38e34)=p[6];U(0xe38e30)=p[7];U(0xe38e2c)=0;U(0xe38e28)=0;
    for(i=0;i<7;++i)B(flags[i])=(p[8]&(1u<<i))!=0;
    U(0xf56f8c)=p[12];U(0xf56fb4)=18765;B(0xe38dda)=63;B(0xe38dca)=0x31;B(0xe38dc9)=0x77;B(0xd3b439)=0x44;
    U(0xd3b194)=0x11111111;U(0xd3b198)=0x22222222;
    for(i=0;i<2;++i){memset((void *)(0x5c8168+i*36+24),0,4);B(0x5c8168+i*36+24)=1;}
    switch(p[0]) {
    case 0:B(0xe38dda)=(uint8_t)p[2];FUN_004264d0(p[3],BASE+0xf000,p[1]);break;
    case 1:FUN_00426530();break;case 2:FUN_00426590();break;case 3:FUN_004265b0();break;
    case 4:U(0x705710)=0;FUN_00425570();break;case 5:FUN_00425640();break;
    case 6:result=FUN_004cc6c0(5,(int)p[13],p[12]);break;
    }
    return result;
}
static void output(uint32_t result)
{
    static const uint32_t regions[][2]={{0x705704,24},{0xe38dc9,2},{0xe38dda,1},{0xd3b439,1},{0xd3b194,8},{0x5c8168+24,4},{0x5c818c+24,4},{0xe20768,4}};
    unsigned int i,j;printf("{\"result\":%u,\"state\":\"",result);
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j)printf("%02x",B(regions[i][0]+j));
    printf("\",\"events\":[");for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<10;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[14]={2,2,47,90,272,224,0,0,0,258,1,419,1005,1},i,result;
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":14}");fflush(stdout);for(;;){for(i=0;i<14;++i)if(scanf("%u",p+i)!=1)goto done;result=run(p);output(result);}
    } else {
        run(p);REQUIRE(B(0xd3b439)==1&&B(0xe38dda)==2&&B(0xe38dca)==1);
        p[0]=3;run(p);REQUIRE(B(0xd3b439)==0&&B(0xe38dda)==2);
        p[0]=4;run(p);REQUIRE(events[event_count-1][0]==0x4ccb30&&events[event_count-1][8]==0);
        p[11]=420;run(p);REQUIRE(events[event_count-1][0]==0x4ccfd0);
        p[0]=6;REQUIRE(run(p)==420&&U(0xe20768)==BASE+0x1006);
        printf("Modal menus: %u checks; complete callbacks, return-mode order, text widths, label color and geometry.\n",checks);
    }
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
