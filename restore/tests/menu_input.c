#include "menu_input.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#define BASE 0x30000000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define W(a) (*(int16_t *)(uintptr_t)(a))
extern int FUN_004d6740(int,int,int,int,int);
extern int FUN_004d5d70(int);
extern void FUN_004d8310(void);
extern void FUN_004cc500(int);
extern void FUN_004cc3c0(void);
extern void FUN_004cc3e0(void);
extern void FUN_004cc370(void);
extern void FUN_004cc390(void);
extern void FUN_004cc560(int,uint32_t,uint32_t,uint32_t,uint8_t,uint32_t);
extern void FUN_004cc540(void);
extern int FUN_00425990(int,int,short *,int);
extern void FUN_004cc400(uint8_t);
extern void FUN_004cc330(void);
extern void FUN_004cc4c0(void);
extern void FUN_004cc250(uint32_t);
extern void FUN_004cc1b0(void);
extern void FUN_004cc320(void);
static const uint32_t regions[][2]={{0xe38dc0,256},{0xd3b190,16},{0xd3b3a8,36},
    {0xe36c88,12},{BASE+0x100,324},{0xe945f8,24},{0xe94d98,16},
    {0x5d2a2c,4},{0x605a3c,12},{0x704dac,8},{0xf53f78,4},{0xe39140,4},
    {0xe38da4,8},{0xe20990,4},{0x7056a8,36},{0xe2079c,512},{0xe20774,40},{BASE+0x500,1280}};
static const uint32_t observed[]={0xd3b198,0xd3b194,0xd3b3c8,0xf53f78,0xe38ebc,
    0x605a3c,0x704dac,0x704db0,0xe38ea8,0x5d2a2c,0xe38dd8,0xe38e40,0xe38e38,0xe94da4};
static uint32_t events[32][24],event_count,checks;
static POINT cursor;
static BOOL cursor_ok=TRUE;
static uint32_t ready_address;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Menu input failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static void event(uint32_t address,int count,const uint32_t *args)
{
    unsigned int i;uint32_t *row;
    REQUIRE(event_count<32&&count<=8);row=events[event_count++];memset(row,0,24*4);
    row[0]=address;row[1]=count;if(count)memcpy(row+2,args,count*4);
    for(i=0;i<14;++i)row[10+i]=U(observed[i]);
}
void restore_menu_callback(uint32_t address) {event(address,0,0);}
BOOL restore_input_cursor(POINT *point) {*point=cursor;return cursor_ok;}
void FUN_0053b090(int sound) {uint32_t a[1]={(uint32_t)sound};event(0x53b090,1,a);}
uint32_t FUN_0047d350(int sound) {uint32_t a[1]={(uint32_t)sound};event(0x47d350,1,a);return 1;}
int FUN_004cbf30(const char *text,int font)
{uint32_t a[2]={(uint32_t)(uintptr_t)text,(uint32_t)font};event(0x4cbf30,2,a);return (int)strlen(text)*8;}
void FUN_00536820(int x,int y,int w,int h) {uint32_t a[4]={x,y,w,h};event(0x536820,4,a);}
void FUN_00536c30(int unused,int x,int y,int w,int h,int color)
{uint32_t a[6]={unused,x,y,w,h,color};event(0x536c30,6,a);}
static void game(uint32_t address) {event(address,0,0);}
static BOOL WINAPI show(HWND window,int command)
{uint32_t a[2]={(uint32_t)(uintptr_t)window,command};event(0x56f178,2,a);return TRUE;}
static HINSTANCE WINAPI open_link(HWND window,LPCSTR verb,LPCSTR file,LPCSTR parameters,LPCSTR directory,INT command)
{uint32_t a[6]={(uint32_t)(uintptr_t)window,(uint32_t)(uintptr_t)verb,(uint32_t)(uintptr_t)file,
    (uint32_t)(uintptr_t)parameters,(uint32_t)(uintptr_t)directory,command};event(0x56f16c,6,a);return (HINSTANCE)33;}
static BOOL WINAPI post(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{uint32_t a[4]={(uint32_t)(uintptr_t)window,message,wparam,lparam};event(0x56f174,4,a);return TRUE;}
const RestoreMenuActionApi *restore_menu_action_platform(void)
{static const RestoreMenuActionApi api={game,show,open_link,post};return &api;}
static void prepare(const uint32_t p[24])
{
    static const uint32_t flag_address[]={0xe38e03,0xe38e02,0xe38df7,0xe38e04,0xe38df8,0xe38e00,0xe38df4,0xe38df6};
    RestoreMenuButton *table=(RestoreMenuButton *)(BASE+0x100);unsigned int i,j;
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)regions[i][0],0xa5,regions[i][1]);
    for(i=0;i<8;++i) {
        RestoreMenuButton *b=table+i;
        b->left=192;b->top=(int16_t)(100+(p[8]==1?40*i:0));b->right=448;b->bottom=b->top+25;
        if(p[8]==2) {b->left=-300;b->right=-100;b->top=-200;b->bottom=-80;}
        b->on_left=BASE+0x1000+i*0x20;b->on_right=b->on_left+0x10;
        b->reserved16=0x36;b->activated=0x5a;b->reserved18=0x73;
        b->trigger=(uint8_t)(i==0?p[6]:i%2?1:3);
        b->primary=0x12345600+i;b->secondary=0x76543200+i;
    }
    for(i=0;i<8;++i)B(flag_address[i])=(p[5]&(1u<<i))?(p[5]&0x200?0x80:1):0;
    U(0xe38e34)=p[3];U(0xe38e30)=p[4];U(0xd3b3ac)=(p[5]&0x100)?0x100:0;
    U(0xd3b3c8)=p[9]+1000;U(0xd3b198)=0x87654321;U(0xd3b194)=0xabcdef01;
    U(0xe39140)=0x12345678;event_count=0;cursor_ok=TRUE;
    B(0xe38dda)=(uint8_t)p[6];U(0x605a44)=p[8];
    ready_address=0xe94624+(B(0xe38dda)==7?100+p[8]:B(0xe38dda))*16;
    U(ready_address)=0xa5a5a5a5;
    if(p[0]==2) {
        cursor.x=(int32_t)p[3];cursor.y=(int32_t)p[4];
        U(0xe38d9c)=p[10];U(0xe38d98)=p[11];U(0xe39130)=11;U(0xe39134)=23;
        U(0xe39138)=p[12]+11;U(0xe3913c)=p[13]+23;U(0xe38dcc)=p[14];
        W(0xe38e0a)=(int16_t)p[15];W(0xe38e08)=(int16_t)p[16];
        U(0xe38e34)=p[17];U(0xe38e30)=p[18];
        B(0xe38e00)=(p[19]&1)!=0;B(0xe38df4)=(p[19]&2)!=0;B(0xe38e07)=(uint8_t)p[5];
        U(0xe38e14)=p[20];U(0xe38e1c)=p[21];U(0xe38e18)=p[22];
        U(0xe92e0c)=p[23];U(0xe36c8c)=p[9]^0x12345678;
    } else if(p[0]==3) {
        U(ready_address)=p[14];U(ready_address-8)=p[7]?BASE+0x400:0;U(0xe94d98)=p[5];
        for(i=0;i<3;++i) {
            uint32_t *node=(uint32_t *)(BASE+0x400+i*28);
            for(j=0;j<7;++j)node[j]=0;
            node[0]=i*2+1;node[2]=p[3]+i*111;node[3]=p[4]-i*83;
            node[4]=p[10]+i*500;node[5]=p[11]+i*600;node[6]=i==2?0:BASE+0x400+(i+1)*28;
        }
    } else if(p[0]==4) U(0xd3b198)=p[1];
    else if(p[0]==5) {
        B(0xe38dda)=0;U(0xe38da8)=p[1];U(0xe38da4)=p[2];U(0xe38e14)=p[20];
        memcpy((void *)0x57bd88,table,5*28);U(0xe94624)=p[14];U(0xe9461c)=0;
        U(0xe94d98)=0;ready_address=0xe94624;
        U(0xe20990)=0x12345678;U(0x7056a8)=0x23456789;U(0x7056b0)=0x34567890;U(0x7056b8)=0x45678901;U(0x7056c8)=0x56789012;
    } else if(p[0]==6) {
        for(i=0;i<20;++i) {
            char *text=(char *)(BASE+0x500+i*64);memset(text,'A'+i,p[3]%32);text[p[3]%32]=0;
            U(0xe2079c+i*4)=(uint32_t)(uintptr_t)text;U(0xe207ec+i*4)=p[17];
            U(0xe208a0+i*4)=p[18];U(0xe208f0+i*4)=p[2];
            U(0xe20850+i*4)=p[10];U(0xe20940+i*4)=p[11];
            B(0xe2083c+i)=p[16]&1;
        }
        U(0xe20990)=p[1];B(0xe2079b)=p[14]&1;
    } else if(p[0]==7) {
        RestoreSmallButton *buttons_small=(RestoreSmallButton *)(BASE+0x100);
        for(i=0;i<9;++i) {
            buttons_small[i].left=192;buttons_small[i].top=100+(p[8]==1?i*40:0);buttons_small[i].width=256;buttons_small[i].height=25;
            buttons_small[i].kind=(int16_t)p[6];buttons_small[i].image=123;buttons_small[i].reserved12=0x5a5a5a5a;
            buttons_small[i].on_left=BASE+0x1000+i*0x20;buttons_small[i].on_right=BASE+0x1010+i*0x20;
            buttons_small[i].enabled=(p[14]&(1u<<i))!=0;buttons_small[i].pressed=(uint8_t)p[15];buttons_small[i].hover=(uint8_t)p[16];buttons_small[i].locked=0x51;
            buttons_small[i].primary=0x12345600+i;buttons_small[i].secondary=0x76543200+i;
        }
    }
}
static int run(const uint32_t p[24])
{
    RestoreMenuButton *table=(RestoreMenuButton *)(BASE+0x100);prepare(p);
    switch(p[0]) {
    case 0:return FUN_004261d0(p[1],p[2],table,p[7]);
    case 1:return FUN_00426450(p[1],p[2],table,p[7]);
    case 2:FUN_004cf210();return 0;
    case 3:return FUN_004d6740(p[1],p[2],(int)table,p[7],p[17]);
    case 4:FUN_00417ff0();return 0;
    case 5:FUN_004d8310();memcpy(table,(void *)0x57bd88,5*28);return 0;
    case 6:
        switch(p[5]) {
        case 0:FUN_004cc500(p[1]);break;case 1:FUN_004cc3c0();break;case 2:FUN_004cc3e0();break;
        case 3:FUN_004cc370();break;case 4:FUN_004cc390();break;
        case 5:FUN_004cc560(p[1],BASE+0x500+p[1]*64,p[2],p[10],(uint8_t)p[7],p[18]);break;
        case 6:FUN_004cc540();break;case 7:FUN_004cc400((uint8_t)p[12]);break;
        case 8:FUN_004cc330();break;case 9:FUN_004cc4c0();break;case 10:FUN_004cc250(p[12]);break;
        case 11:FUN_004cc1b0();break;case 12:FUN_004cc320();break;default:REQUIRE(0);
        }
        return 0;
    case 7:return FUN_00425990(p[1],p[2],(short *)table,p[7]);
    default:REQUIRE(0);return 0;
    }
}
static void output(int result)
{
    unsigned int i,j;printf("{\"result\":%d,\"state\":[",result);
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];j+=4)
        printf("%s%u",i||j?",":"",U(regions[i][0]+j));
    printf(",%u],\"events\":[",U(ready_address));
    for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<24;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}
    puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[24]={0,192,144,400,250,1,1,3,1,7};unsigned int i;
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":24}");fflush(stdout);
        for(;;){for(i=0;i<24;++i)if(scanf("%u",&p[i])!=1)goto done;output(run(p));}
    } else {
        REQUIRE(run(p)==1&&event_count==1&&events[0][0]==BASE+0x1000&&U(0xd3b198)==0x12345600);
        p[0]=1;REQUIRE(run(p)==1&&U(0xe38e2c)==400&&U(0xe38e28)==250);
        p[3]=640;REQUIRE(run(p)==0);
        p[0]=4;for(p[1]=0;p[1]<5;++p[1]){run(p);REQUIRE(event_count==(p[1]==1?2:1));}
        p[0]=2;p[3]=900;p[4]=600;p[10]=1024;p[11]=768;p[12]=1024;p[13]=768;p[14]=1;
        p[5]=1;p[23]=123456;run(p);REQUIRE(U(0xe38e34)==900&&U(0xe38e30)==600&&B(0xe38e03)==1&&event_count==1);
        cursor_ok=FALSE;FUN_004cf210();REQUIRE(U(0xe38e34)==900);
        printf("Menu input: %u checks; full coordinates, button routing, controlled native action endpoints and cursor failure.\n",checks);
    }
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
