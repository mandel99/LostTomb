#include "display_confirmation.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#define CHECK(v) do{if(!(v)){fprintf(stderr,"Display confirmation failure line%d\n",__LINE__);ExitProcess(1);}}while(0)
extern void FUN_004cfd60(int mode);
extern void FUN_0040f8f0(uint32_t *destination,uint32_t count,uint8_t value);
#define FRAME 0x30000000u
#define FRAME_CAP 0x200000u
static const uint32_t regions[][2]={{0x6057d0,48},{0x7056a0,0x90},{0x5c6890,64},{0xe38d90,0x400},{0xe945e0,0x1100},{0xd3b180,0x280},{0x6f2000,0x100},{0xf56f70,64},{0xe92e00,64}};
static uint32_t p[52],events[32][26],used,clocks,tiles,pixel_count,frame_count,frame_pixels;
static uint8_t original_button[28];
static void event(uint32_t address,uint32_t count,const uint32_t *argv)
{
    uint32_t *e;CHECK(used<32&&count<=9);e=events[used++];memset(e,0,104);e[0]=address;e[1]=count;if(count)memcpy(e+2,argv,count*4);
    e[11]=DC_B(0xe38dda);e[12]=DC_B(0xe38dca);e[13]=DC_U(0xe38e68);e[14]=DC_U(0xe38ee4);e[15]=DC_U(0x70570c);e[16]=DC_U(0x6057e8);e[17]=DC_U(0xe39140);e[18]=DC_U(0xd3b198);e[19]=DC_U(0x7056c8);e[20]=DC_U(0xe38e34);e[21]=DC_U(0xe38e30);e[22]=DC_B(0x5c68b9);e[23]=DC_U(0xf56f8c);
    e[24]=pixel_count?DC_B(frame_pixels):0;e[25]=pixel_count?DC_B(frame_pixels+pixel_count-1):0;
}
uint32_t restore_display_confirmation_clock(void)
{
    uint32_t index=clocks++;event(0x56f22c,0,NULL);
    if(!index){if(p[12]&1)DC_B(0xe38dda)=(uint8_t)p[13];if(p[12]&2)DC_U(0x6057e8)=p[14];if(p[12]&4)DC_U(0x70570c)=p[15];if(p[12]&8)DC_U(0xe38e68)=p[16];if(p[12]&16)DC_U(0xe38ee4)=p[17];}
    else{if(p[12]&2048)DC_U(0x6057e8)=p[28];if(p[12]&4096)DC_B(0xe38dda)=(uint8_t)p[29];}
    return index?p[7]:p[6];
}
static BOOL WINAPI post(HWND window,UINT message,WPARAM key,LPARAM value)
{
    uint32_t a[]={(uint32_t)(uintptr_t)window,message,(uint32_t)key,(uint32_t)value};event(0x56f174,4,a);
    if(p[12]&32)DC_U(0x70570c)=p[18];if(p[12]&64)DC_B(0xe38dda)=(uint8_t)p[19];if(p[12]&128)DC_U(0xe39140)=p[20];
    return (BOOL)p[11];
}
static const RestoreMenuActionApi api={NULL,NULL,NULL,post};
const RestoreMenuActionApi *restore_menu_action_platform(void){return &api;}
void restore_menu_callback(uint32_t token)
{if(restore_display_confirmation_action(token))return;CHECK(token==0x51f780);}
void FUN_00536820(int x,int y,int w,int h)
{
    uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)w,(uint32_t)h};event(0x536820,4,a);
    if(tiles++==0){
        if(p[12]&256){DC_U(0xe38da8)=p[21];DC_U(0xe38da4)=p[22];}
        if(p[12]&512){RestoreMenuButton *b=(RestoreMenuButton *)0x5c68a8u;b->left=(int16_t)p[23];b->top=(int16_t)p[24];b->right=(int16_t)p[25];b->bottom=(int16_t)p[26];}
        if(p[12]&1024)DC_B(0xe38dda)=(uint8_t)p[27];
        if(p[12]&16384)DC_U(0xf56f8c)=p[46];
    }
}
void FUN_00536c30(int k,int x,int y,int w,int h,uint32_t color)
{uint32_t a[]={(uint32_t)k,(uint32_t)x,(uint32_t)y,(uint32_t)w,(uint32_t)h,color};event(0x536c30,6,a);}
void FUN_004d6160(int group,int entry,int x,int y,int width,uint32_t font,int flags,int sx,int sy)
{uint32_t a[]={(uint32_t)group,(uint32_t)entry,(uint32_t)x,(uint32_t)y,(uint32_t)width,font,(uint32_t)flags,(uint32_t)sx,(uint32_t)sy};event(0x4d6160,9,a);}
static void setup(void)
{
    uint32_t i,j;RestoreMenuButton *b=(RestoreMenuButton *)0x5c68a8u;
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j)DC_B(regions[i][0]+j)=(uint8_t)(p[1]+j*37u+i*11u);
    memcpy(b,original_button,28);b->primary=(int32_t)p[39];b->secondary=(int32_t)p[40];
    DC_U(0xe38e68)=p[2];DC_U(0xe38ee4)=p[3];DC_B(0xe38dda)=(uint8_t)p[4];DC_B(0xe38dca)=(uint8_t)p[5];DC_U(0x70570c)=p[8];DC_U(0x6057e8)=p[9];DC_U(0xe39140)=p[10];DC_U(0xe38da8)=p[30];DC_U(0xe38da4)=p[31];DC_U(0xe94d98)=p[37];DC_U(0x605a44)=0;
    DC_U(0xf56f8c)=p[43];
    pixel_count=p[0]==9?p[9]:p[49]*p[50]*2u;CHECK(pixel_count<=0x100000u&&p[51]<4);
    frame_pixels=FRAME+16u+p[51];frame_count=pixel_count+32u+p[51];
    for(i=0;i<frame_count;++i)DC_B(FRAME+i)=(uint8_t)(p[1]+i*37u+0x63u);
    DC_U(0xe38db0)=p[49];DC_U(0xe38dac)=p[50];DC_U(0xe92e18)=p[48]?frame_pixels:0;
    for(i=0;i<256;++i){DC_U(0xe94624+i*16)=p[36];DC_U(0xe9461c+i*16)=0;}
    for(i=0;i<2;++i){uint32_t node=0x6f2080+i*32;DC_U(node)=1;DC_U(node+8)=0x12348001u+i;DC_U(node+12)=0xabcd7fffu+i;DC_U(node+16)=0x1234ffffu;DC_U(node+20)=3;DC_U(node+24)=i==0&&p[38]==2?node+32:0;}
    if(p[38]){DC_U(0xe9461c+(p[4]&255)*16)=0x6f2080;DC_U(0xe9461c+62*16)=0x6f2080;}
    used=clocks=tiles=0;
}
static void mouse(uint32_t flags,uint32_t position)
{
    RestoreMenuButton *b=(RestoreMenuButton *)0x5c68a8u;uint32_t x=DC_U(0xe38da8)+80u,y=DC_U(0xe38da4)+64u,i;
    for(i=0xe38df4;i<=0xe38e04;++i)DC_B(i)=0;
    DC_U(0xe38e34)=x+(uint32_t)(int32_t)b->left+1u;DC_U(0xe38e30)=y+(uint32_t)(int32_t)b->top+1u;
    if(position==1)DC_U(0xe38e34)=x+(uint32_t)(int32_t)b->left-1u;
    if(position==2)DC_U(0xe38e34)=x+(uint32_t)(int32_t)b->right;
    if(position==3)DC_U(0xe38e30)=y+(uint32_t)(int32_t)b->top-1u;
    if(position==4)DC_U(0xe38e30)=y+(uint32_t)(int32_t)b->bottom;
    if(position==5){DC_U(0xe38e34)=0x80000000;DC_U(0xe38e30)=0x80000000;}
    if(position==6){DC_U(0xe38e34)=p[34];DC_U(0xe38e30)=p[35];}
    if(flags==1)DC_B(0xe38e03)=1;if(flags==2)DC_B(0xe38e02)=1;if(flags==3)DC_B(0xe38df7)=1;if(flags==4)DC_B(0xe38df8)=1;
}
static void run(void)
{
    setup();mouse(p[32],p[33]);
    switch(p[0]){
    case 0:FUN_00402da0();break;case 1:FUN_00402de0();break;case 2:FUN_00402e10();break;
    case 3:restore_display_confirmation_input();break;
    case 4:FUN_00402da0();if(DC_B(0xe38dda)==62)restore_display_confirmation_input();break;
    case 5:restore_display_confirmation_input();if(DC_B(0xe38dda)==62){mouse(p[41],p[42]);restore_display_confirmation_input();}break;
    case 6:FUN_004cfd60((int)p[2]);break;case 7:FUN_00534540();break;
    case 8:FUN_004cf1c0();break;case 9:FUN_0040f8f0((uint32_t *)(uintptr_t)frame_pixels,p[9],(uint8_t)p[43]);break;default:CHECK(0);
    }
}
static void output(void)
{
    static const char hex[]="0123456789abcdef";static char state[2200000];uint32_t i,j,n=0;
    printf("{\"state\":\"");for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j){uint8_t b=DC_B(regions[i][0]+j);CHECK(n+2<=sizeof(state));state[n++]=hex[b>>4];state[n++]=hex[b&15];}fwrite(state,1,n,stdout);
    for(i=0;i<frame_count;++i){uint8_t b=DC_B(FRAME+i);CHECK(n+2<=sizeof(state));state[n++]=hex[b>>4];state[n++]=hex[b&15];}fwrite(state+n-frame_count*2u,1,frame_count*2u,stdout);
    printf("\",\"events\":[");for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<26;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t i;CHECK(restore_data_open());CHECK(VirtualAlloc((void *)FRAME,FRAME_CAP,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE)==(void *)FRAME);memcpy(original_button,(void *)0x5c68a8u,28);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":52}");fflush(stdout);for(;;){for(i=0;i<52;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}}
    else{memset(p,0,sizeof(p));p[2]=3;p[3]=1;p[4]=12;p[6]=0x12345678;run();CHECK(DC_U(0x6057e8)==p[6]&&DC_B(0xe38dda)==62&&DC_U(0x70570c)==12&&DC_B(0xe38dca)==1);p[0]=2;p[8]=0xaabbccdd;run();CHECK(DC_U(0xe38ee4)==3&&DC_B(0xe38dda)==0xdd);CHECK(!restore_display_confirmation_action(0xdeadbeef));puts("Display confirmation setup/accept and typed routing passed.");}
done:VirtualFree((void *)FRAME,0,MEM_RELEASE);restore_data_close();return 0;
}
