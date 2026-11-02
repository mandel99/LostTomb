#include "menu_layout.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define BASE 0x30000000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern void FUN_004d6160(int,int,int,int,int,int,int,int,int);
extern void FUN_004d6250(int,int,int,int,int,int,int,int,int);
extern void FUN_004d62f0(int,int,int,int,int,int,int,int);
extern void FUN_004d63d0(int,int,int,int,int,int,int,int);
extern void FUN_004d6480(int,int,int,int,int,int,int,int);
extern void FUN_004d65b0(int,int,int,int,int,int);
extern void FUN_004d6860(int,int,int,int,int,int,int,int,int);
extern void FUN_004d6090(int,int,int,int,int,int,int,int,int);
extern void FUN_00537140(int,int,uint32_t);
extern void FUN_004d5dd0(void);
extern void FUN_004d5e70(void);
extern int FUN_004d5ef0(int);
extern int FUN_004d5ed0(void);
extern int FUN_004d5ee0(void);
extern int FUN_004d5f80(int);
static uint32_t events[128][11],count,checks,result;
static uint32_t mode_index;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Menu layout failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static void event(uint32_t address,unsigned int n,const uint32_t *args)
{REQUIRE(count<128&&n<=9);memset(events[count],0,sizeof(events[count]));events[count][0]=address;events[count][1]=n;if(n)memcpy(events[count]+2,args,n*4);++count;}
void FUN_004d69b0(void) {event(0x4d69b0,0,0);}
void FUN_004cc110(int group,int entry)
{uint32_t p[]={group,entry};U(0xe20768)=BASE+0x1000+group*256+(entry<3?entry*16:256);event(0x4cc110,2,p);}
int FUN_004cbf30(uint32_t text,int font)
{uint32_t p[]={text,font};event(0x4cbf30,2,p);return (font&0xffff)+(int)(((text-BASE)>>4)%23);}
void FUN_004ccfd0(int a,int b,int x,int y,int w,int f,int c) {uint32_t p[]={a,b,x,y,w,f,c};event(0x4ccfd0,7,p);}
void FUN_004ccb30(int a,int b,int x,int y,int w,int f,int c) {uint32_t p[]={a,b,x,y,w,f,c};event(0x4ccb30,7,p);}
void FUN_004ccbb0(int a,int b,int x,int y,int f,int c) {uint32_t p[]={a,b,x,y,f,c};event(0x4ccbb0,6,p);}
void FUN_00536c30(int a,int x,int y,int w,int h,int c) {uint32_t p[]={a,x,y,w,h,c};event(0x536c30,6,p);}
void FUN_00537000(int a,int x,int y,int w,int h,int c) {uint32_t p[]={a,x,y,w,h,c};event(0x537000,6,p);}
void FUN_00536a30(int x,int y,int w,int h) {uint32_t p[]={x,y,w,h};event(0x536a30,4,p);}
void FUN_004cce00(int a,int b,int c,int x,int y,int f,int color) {uint32_t p[]={a,b,c,x,y,f,color};event(0x4cce00,7,p);}
void FUN_00536820(int x,int y,int w,int h) {uint32_t p[]={x,y,w,h};event(0x536820,4,p);}
void FUN_004cd9d0(int x,int y,int w,int h,uint32_t color) {uint32_t p[]={x,y,w,h,color};event(0x4cd9d0,5,p);}
void FUN_004cd510(int x,int y,int right,int bottom,uint32_t color) {uint32_t p[]={x,y,right,bottom,color};event(0x4cd510,5,p);}
static void run(const uint32_t p[16])
{
    unsigned int i;int x=p[2],y=p[3],ox=p[4],oy=p[5],w=p[6],h=p[7],color=p[15];
    B(0xe38dda)=(uint8_t)p[1];U(0x605a44)=p[14];U(0xe94d98)=p[10];U(0xe94d9c)=p[12];
    U(0xe38e34)=p[8];U(0xe38e30)=p[9];memset((void *)0xe945f8,0xa5,24);
    mode_index=p[1]==7?100+p[14]:p[1];count=0;result=0;
    U(0xf53fe4)=0xa5a5a5a5;U(0xf53fe8)=0xa5a5a5a5;
    for(i=0;i<6;++i)U(0xe945d0+i*4)=0x10000+p[13]+i;
    U(0xe94dac)=p[6];U(0xe94db0)=p[7];U(0xf56f8c)=p[6];memset((void *)0x5c28ac,0xa5,840);
    for(i=0;i<3;++i) {
        uint32_t *node=(uint32_t *)(BASE+i*32);node[0]=x-ox+(p[11]==2?1:0);node[1]=y-oy;
        node[2]=7+p[13]+i*100;node[3]=9+p[13]+i*100;node[4]=15+p[13]+i;node[5]=17+p[13]+i;node[6]=0;
        U(0xe94618+mode_index*16+i*4)=p[11]?BASE+i*32:0;
    }
    switch(p[0]) {
    case 0:FUN_004d6160(31,2,x,y,w,h,color,ox,oy);break;
    case 1:FUN_004d6250(31,2,x,y,w,h,color,ox,oy);break;
    case 2:FUN_004d62f0(31,2,x,y,w,color,ox,oy);break;
    case 3:FUN_004d63d0(0,x,y,w,h,color,ox,oy);break;
    case 4:FUN_004d6480(7,x,y,w,h,color,ox,oy);break;
    case 5:FUN_004d65b0(x,y,w,h,ox,oy);break;
    case 6:FUN_004d6860(31,2,11,x,y,w,color,ox,oy);break;
    case 7:FUN_004d6090(p[10],x,y,w,h,x-ox,y-oy,ox,oy);break;
    case 8:
        U(0xe2078c)=p[6];U(0xe20790)=p[4];U(0xe20794)=p[5];B(0xe2079b)=(uint8_t)(p[14]&1);
        U(0xe92e0c)=p[8];U(0xf53fe4)=p[9];U(0xf53fe8)=p[10];FUN_00537140(x,y,p[15]);break;
    case 9:FUN_004d5dd0();break;case 10:FUN_004d5e70();break;
    case 11:result=FUN_004d5ef0(p[10]);break;case 12:result=FUN_004d5ed0();break;case 13:result=FUN_004d5ee0();break;
    case 14:result=FUN_004d5f80(p[10]);break;
    default:REQUIRE(0);
    }
}
static void output(void)
{
    unsigned int i,j;printf("{\"result\":%u,\"state\":[%u",result,U(0xe94d9c));for(i=0;i<6;++i)printf(",%u",U(0xe945f8+i*4));printf(",%u,%u",U(0xf53fe4),U(0xf53fe8));
    for(i=0;i<6;++i)printf(",%u",U(0xe945d0+i*4));printf(",%u,%u",U(0xe94dac),U(0xe94db0));
    for(i=0;i<840;i+=4)printf(",%u",U(0x5c28ac+i));printf("],\"events\":[");
    for(i=0;i<count;++i){printf("%s[",i?",":"");for(j=0;j<11;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}
    puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[16]={1,54,800,600,192,144,335,0,0,0,0,1,1,17,3,0xffff};unsigned int i;
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,4096,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":16}");fflush(stdout);for(;;){for(i=0;i<16;++i)if(scanf("%u",p+i)!=1)goto done;run(p);output();}
    } else {
        run(p);REQUIRE(count==1&&events[0][0]==0x4ccb30&&events[0][4]==216&&events[0][5]==170&&events[0][6]==32);
        p[11]=2;run(p);REQUIRE(count==1&&events[0][4]==800&&events[0][5]==600&&events[0][6]==335);
        p[0]=5;p[11]=1;run(p);REQUIRE(count==1&&events[0][0]==0x536a30&&events[0][2]==416&&events[0][3]==370);
        printf("Menu layout: %u checks; all four lookup arguments, full coordinates and cached rectangle overrides.\n",checks);
    }
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
