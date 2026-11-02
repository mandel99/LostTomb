#include "dropdown_system_actions.h"
#include "dropdown_menu.h"
#include "message_panel_input.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#define CHECK(v) do{if(!(v)){fprintf(stderr,"System dropdown failure line%d\n",__LINE__);ExitProcess(1);}}while(0)
static const uint32_t regions[][2]={{0xd3a6c0,0xb40},{0xe38dc0,0x80},{0xe39120,64},{0xe209a0,0x410},{0xe950d0,32},{0x705700,64},{0xf53f60,32},{0x6f2000,64}};
static const uint32_t tokens[]={0x402b90,0x4033f0,0x403400,0x4034d0};
static void (*const funcs[])(void)={FUN_00402b90,FUN_004033f0,FUN_00403400,FUN_004034d0};
static uint32_t p[24],events[16][13],used,result;
static void event(uint32_t a,uint32_t n,const uint32_t *v)
{
    uint32_t *e;CHECK(used<16);e=events[used++];memset(e,0,52);e[0]=a;e[1]=n;
    if(n)memcpy(e+2,v,n*4);e[8]=DM_B(0xe38dda);e[9]=DM_B(0xe38dca);
    e[10]=DM_U(0xd3b17c);e[11]=DM_U(0xd3b180);e[12]=DM_U(0xe39140);
}
static BOOL WINAPI show(HWND hwnd,int command)
{
    uint32_t a[]={(uint32_t)(uintptr_t)hwnd,(uint32_t)command};event(0x56f178,2,a);
    if(p[16]&1)DM_U(0xe39140)=p[17];if(p[16]&2)DM_B(0xe38dda)=(uint8_t)p[18];
    return (BOOL)p[21];
}
static HINSTANCE WINAPI open_link(HWND hwnd,LPCSTR operation,LPCSTR file,LPCSTR parameters,LPCSTR directory,INT command)
{
    uint32_t a[]={(uint32_t)(uintptr_t)hwnd,(uint32_t)(uintptr_t)operation,(uint32_t)(uintptr_t)file,(uint32_t)(uintptr_t)parameters,(uint32_t)(uintptr_t)directory,(uint32_t)command};event(0x56f16c,6,a);
    if(p[16]&4)DM_B(0xe38dca)=(uint8_t)p[19];return (HINSTANCE)(uintptr_t)p[22];
}
static BOOL WINAPI post(HWND hwnd,UINT message,WPARAM key,LPARAM data)
{
    uint32_t a[]={(uint32_t)(uintptr_t)hwnd,message,(uint32_t)key,(uint32_t)data};event(0x56f174,4,a);
    if(p[16]&8)DM_B(0xe38dda)=(uint8_t)p[18];return (BOOL)p[23];
}
static void unexpected_game(uint32_t address){(void)address;CHECK(0);}
static const RestoreMenuActionApi api={unexpected_game,show,open_link,post};
const RestoreMenuActionApi *restore_menu_action_platform(void){return &api;}
BOOL restore_input_cursor(POINT *point){(void)point;CHECK(0);return FALSE;}
void FUN_00539170(void)
{
    event(0x539170,0,NULL);
    if(p[16]&16){DM_U(0xd3b17c)=p[20];DM_U(0xd3b180)=0xabcdef01;}
}
static void prepare(void)
{
    uint32_t i,j;
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j)DM_B(regions[i][0]+j)=(uint8_t)(p[2]+j*37u+i*11u);
    DM_B(0xe38dda)=(uint8_t)p[3];DM_B(0xe38dca)=(uint8_t)p[4];DM_U(0xe39140)=p[5];
    DM_U(0xe209f4)=p[7];DM_U(0xe20d14)=p[7];DM_U(0xe950dc)=p[8];
    DM_U(0x705728)=p[9];DM_U(0x705724)=0x6f2000;DM_U(0x70572c)=1;DM_U(0x705730)=1;
    DM_U(0x6f2008)=0x6f2020;DM_W(0x6f200c)=1;DM_U(0x6f2024)=tokens[p[0]];DM_U(0x6f202c)=p[10];
    DM_B(0xe38e03)=1;used=result=0;
}
static void run(void)
{
    CHECK(p[0]<4);prepare();
    switch(p[1]){
    case 0:funcs[p[0]]();break;
    case 1:CHECK(restore_dropdown_system_action(tokens[p[0]])==1);break;
    case 2:restore_menu_callback(tokens[p[0]]);break;
    case 3:restore_dropdown_callback(tokens[p[0]]);break;
    case 4:case 5:case 6:result=(uint32_t)FUN_00425f20((RestoreDropdownBar *)0x6f2000,1);break;
    default:CHECK(0);
    }
    if(p[1]>=5){CHECK(p[0]==1||p[0]==2);if(p[1]==6)restore_menu_callback(0x4c7f30);else FUN_004c7f30();}
}
static void output(void)
{
    static const char hex[]="0123456789abcdef";uint32_t i,j;
    printf("{\"result\":%u,\"state\":\"",result);
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j){uint8_t b=DM_B(regions[i][0]+j);putchar(hex[b>>4]);putchar(hex[b&15]);}
    printf("\",\"events\":[");for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<13;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    CHECK(restore_data_open());
    CHECK(!strcmp((const char *)0x5761d8u,"open"));
    CHECK(!strcmp((const char *)0x5761e0u,"mission editor guide.txt"));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        uint32_t i;puts("{\"fields\":24}");fflush(stdout);for(;;){for(i=0;i<24;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}
    }else{
        p[0]=1;p[1]=4;p[2]=73;p[3]=3;p[9]=1;run();CHECK(DM_B(0xe38dda)==5&&DM_U(0xd3b124)==10&&DM_U(0xd3b17c)==1);
        CHECK(DM_U(0xd3aae0)==0&&DM_U(0xd3a6ec)==0&&DM_U(0xd3a6f0)==0&&DM_U(0x705724)==0);
        p[1]=6;run();CHECK(DM_B(0xe38dda)==1&&DM_U(0xd3a6e8)==0);
        p[0]=3;p[1]=0;p[5]=0x12345678;p[16]=1;p[17]=0x87654321;run();CHECK(used==2&&events[1][2]==p[17]);
        p[0]=0;run();CHECK(used==1&&events[0][3]==WM_KEYDOWN&&events[0][4]==VK_ESCAPE);
        CHECK(restore_dropdown_system_action(0xdeadbeef)==0);puts("System dropdown: native quit message, editor guide handle reload and real help open/close passed.");
    }
done:restore_data_close();return 0;
}
