#include "menu_keyboard_modal.h"
#include "menu_keyboard_modal_case.h"
#include "display_confirmation.h"
#include "window_messages.h"
#include "text_selection.h"
#include "standalone_data.h"
#include "campaign_profile.h"
#include <stdio.h>
#include <string.h>
#define BASE 0x30000000u
#define BODY (BASE+0x102u)
#define TITLE (BASE+0x115u)
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Keyboard modal failure line%d\n",__LINE__);ExitProcess(2);}}while(0)
extern void FUN_004cf210(void);
extern void FUN_004cf1f0(void);
extern uint32_t FUN_0041d110(int message,int key);
static const uint32_t regions[][2]={{0xf535a0,64},{0xf53fe0,0x160},{0xe38d80,0x440},{0xd3b180,0x2d0},{0x7056a0,0x90},{0x5f8900,0x70},{0xe945e0,0x1100},{0xe34c10,0x80},{0xe36c60,64},{0xe20750,48},{0xe92df0,64},{0xf56f70,0x60},{0x6057d0,48},{0x6fdc30,48},{BASE+0xf0,0x200},{BASE+0x800,0x200},{0x5c6890,64}};
static uint32_t p[P_FIELDS],events[256][32],used,frames,peeks,gets,waits,metrics,labels,borders,texts,panels,last_modal,result;
static uint8_t original_buttons[84],original_display_button[28];
static RestoreWindowApi window_api;
static void event(uint32_t a,uint32_t count,const uint32_t *argv)
{
    uint32_t *e;CHECK(used<256&&count<=9);e=events[used++];memset(e,0,128);e[0]=a;e[1]=count;if(count)memcpy(e+2,argv,count*4);
    e[11]=MM_B(0xe38dda);e[12]=MM_B(0xe38dca);e[13]=MM_B(0xf535c0);e[14]=MM_U(0xf535c4);e[15]=MM_U(0xf535c8);e[16]=MM_B(0xd3b43d);e[17]=MM_U(0xe39140);e[18]=MM_U(0xe39128);e[19]=MM_U(0xe3912c);e[20]=MM_U(0x7056c8);e[21]=MM_U(0xd3b198);e[22]=MM_U(0xf54054);e[23]=MM_U(0xf5404c);e[24]=MM_U(0xf54114);e[25]=MM_U(0xf54118);e[26]=MM_U(0xe38e34);e[27]=MM_U(0xe38e30);e[28]=MM_B(0xe38e07);e[29]=MM_U(0xe38da8);e[30]=MM_U(0xe38da4);e[31]=MM_U(0x6057e8);
}
static void message(MSG *msg)
{memset(msg,0,sizeof(*msg));msg->hwnd=(HWND)(uintptr_t)p[P_HWND];msg->message=0x400+gets;msg->wParam=0x12340000u+gets;msg->lParam=(LPARAM)(0x87654321u^gets);msg->time=0x11223344u+gets;msg->pt.x=(LONG)(7u+gets);msg->pt.y=(LONG)(11u+gets);}
static BOOL WINAPI peek(LPMSG msg,HWND window,UINT low,UINT high,UINT remove)
{
    uint32_t a[]={0xabc00001,(uint32_t)(uintptr_t)window,low,high,remove};int ready;
    CHECK(++peeks<=32);event(0x56f184,5,a);
    ready=(p[P_SCRIPT]==1&&peeks==1)||(p[P_SCRIPT]==2&&peeks==1)||(p[P_SCRIPT]==5&&peeks==1)||(p[P_SCRIPT]==8&&gets<2)||((p[P_SCRIPT]==4||p[P_SCRIPT]==6)&&frames>=(p[P_SCRIPT]==4?1u:2u));
    if(ready){message(msg);return (BOOL)p[P_PEEK_RESULT];}return FALSE;
}
static BOOL WINAPI get(LPMSG msg,HWND window,UINT low,UINT high)
{uint32_t a[]={0xabc00001,(uint32_t)(uintptr_t)window,low,high};event(0x56f190,4,a);++gets;message(msg);if(p[P_SCRIPT]==1||p[P_SCRIPT]==4||p[P_SCRIPT]==6)return FALSE;return (BOOL)p[P_GET_RESULT];}
static void msg_event(uint32_t token,const MSG *m)
{uint32_t a[]={0xabc00001,(uint32_t)(uintptr_t)m->hwnd,m->message,(uint32_t)m->wParam,(uint32_t)m->lParam,m->time,(uint32_t)m->pt.x,(uint32_t)m->pt.y};event(token,8,a);}
static BOOL WINAPI translate(const MSG *m){msg_event(0x56f18c,m);return FALSE;}
static LRESULT WINAPI dispatch(const MSG *m)
{msg_event(0x56f188,m);if(p[P_SCRIPT]==5){MM_B(0xf535c0)=1;MM_U(0xf535c4)=p[P_TARGET];}if(p[P_SCRIPT]==8)MM_B(0xd3b43d)=1;if(p[P_MUTATIONS]&1024){MM_B(0xe38dda)=(uint8_t)p[P_MUT_MODE];MM_U(0xe39140)=p[P_EXTRA];}return (LRESULT)0x89abcdefu;}
static BOOL WINAPI wait_message(void)
{event(0x56f198,0,NULL);++waits;MM_U(0xe39128)=0;MM_U(0xe3912c)=1;return (BOOL)p[P_WAIT_RESULT];}
static const RestoreModalEventApi modal_api={peek,get,translate,dispatch,wait_message};
const RestoreModalEventApi *restore_menu_modal_events(void){return &modal_api;}
static BOOL WINAPI post(HWND window,UINT id,WPARAM key,LPARAM value)
{uint32_t a[]={(uint32_t)(uintptr_t)window,id,(uint32_t)key,(uint32_t)value};event(0x56f174,4,a);if(p[P_MUTATIONS]&2048){MM_B(0xd3b43d)=2;MM_U(0xe39140)=p[P_EXTRA];}return (BOOL)p[P_POST_RESULT];}
static const RestoreMenuActionApi action_api={NULL,NULL,NULL,post};
const RestoreMenuActionApi *restore_menu_action_platform(void){return &action_api;}
static LRESULT WINAPI default_window(HWND window,UINT message,WPARAM key,LPARAM value)
{uint32_t a[]={(uint32_t)(uintptr_t)window,message,(uint32_t)key,(uint32_t)value};event(0x56f1f8,4,a);return (LRESULT)p[P_EXTRA];}
SHORT WINAPI restore_modal_async_key(int key){uint32_t a[]={(uint32_t)key};event(0x56f1e4,1,a);return (SHORT)p[P_TRACE];}
void FUN_0041bae0(void){fputs("Unexpected non-Escape cheat branch.\n",stderr);ExitProcess(2);}
void FUN_0041d3d0(void){fputs("Unexpected non-Escape cheat branch.\n",stderr);ExitProcess(2);}
uint32_t FUN_004ceed0(char *name){(void)name;fputs("Unexpected screenshot branch.\n",stderr);ExitProcess(2);}
uint32_t *FUN_00516a20(void){fputs("Unexpected non-Escape profile branch.\n",stderr);ExitProcess(2);}
uint8_t FUN_0041b8a0(const RestoreCampaignProfile *self,const char *name){(void)self;(void)name;fputs("Unexpected non-Escape profile branch.\n",stderr);ExitProcess(2);}
BOOL restore_input_cursor(POINT *point)
{
    uint32_t a[]={0xabc00002};RestoreMenuButton *b=(RestoreMenuButton *)(uintptr_t)(0x5f8910u+p[P_TARGET]*28u);++frames;event(0x56f1cc,1,a);
    point->x=((int)b->left+(int)b->right)/2;point->y=((int)b->top+(int)b->bottom)/2;
    MM_B(0xe38e07)=frames>=p[P_CLICK_AT]?1:0;MM_U(0xe92e0c)=p[P_NOW]+frames*17u;
    return p[P_CURSOR_FAIL]==UINT32_MAX||p[P_CURSOR_FAIL]==frames?FALSE:TRUE;
}
void FUN_0053b090(int id){uint32_t a[]={(uint32_t)id};event(0x53b090,1,a);}
void FUN_004d71c0(int redraw){uint32_t a[]={(uint32_t)redraw};event(0x4d71c0,1,a);}
void FUN_004d8310(void)
{if(MM_B(0xe38dda)==0x54)restore_menu_modal_input();else event(0x4d8310,0,NULL);}
void FUN_004d0280(int flags)
{uint32_t a[]={(uint32_t)flags};event(0x4d0280,1,a);if(p[P_MUTATIONS]&256){MM_U(0xf535c4)=p[P_MUT_INDEX];MM_B(0xf535c0)=0;}if(p[P_MUTATIONS]&512)MM_B(0xd3b43d)=1;}
void FUN_004cf460(uint32_t cursor){uint32_t a[]={cursor};event(0x4cf460,1,a);}
int FUN_004ce100(void *context)
{uint32_t a[]={(uint32_t)(uintptr_t)context};event(0x4ce100,1,a);if(p[P_MUTATIONS]&1){MM_U(0xf54114)=p[P_SMACK_NEW_HANDLE];MM_U(0xf5404c)=1;}return (int)p[P_CONTEXT_RESULT];}
int __cdecl _SmackClose_4(uintptr_t handle)
{uint32_t a[]={(uint32_t)handle};event(0x56f32c,1,a);if(p[P_MUTATIONS]&2)MM_U(0xf54114)=p[P_SMACK_NEW_HANDLE];return (int)p[P_SMACK_RESULT];}
void FUN_004ce2d0(void){event(0x4ce2d0,0,NULL);}
void FUN_004ce6e0(char *format,uint32_t context,uint32_t number)
{uint32_t a[]={(uint32_t)(uintptr_t)format,context,number};event(0x4ce6e0,3,a);if((uintptr_t)format==0x5f8fe4u&&(p[P_MUTATIONS]&4))MM_U(0xf54118)=p[P_SMACK_NEW_KIND];}
uint32_t FUN_004cff20(void){event(0x4cff20,0,NULL);return p[P_MEMORY_RESULT];}
int FUN_004cbf30(const char *text,int font)
{
    uint32_t a[]={(uint32_t)(uintptr_t)text,(uint32_t)font},which=metrics++%3u;event(0x4cbf30,2,a);
    if(which==0&&(p[P_MUTATIONS]&8)){MM_U(0xf56fb4)=p[P_FONT_ALT];MM_U(0xf56f8c)=p[P_FONT_BODY]^0x100u;}
    if(which==1&&(p[P_MUTATIONS]&16)){MM_U(0xf56fb4)=p[P_FONT_ALT]^0x200u;MM_U(0xf56f8c)=p[P_FONT_ALT]^0x400u;}
    if(which==0&&(p[P_MUTATIONS]&8192)){MM_U(0xe38db0)=p[P_WIDTH_ALT];MM_U(0xe38dac)=p[P_HEIGHT_ALT];}
    return (int)p[which==0?P_BODY_WIDTH:which==1?P_TITLE_WIDTH:P_FINAL_WIDTH];
}
void FUN_004d5d90(void){event(0x4d5d90,0,NULL);MM_U(0xe94d9c)=1;}
void FUN_00536820(int x,int y,int w,int h)
{uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)w,(uint32_t)h};event(0x536820,4,a);if(panels++==0&&(p[P_MUTATIONS]&32)){MM_U(0xe38da8)=p[P_OX_ALT];MM_U(0xe38da4)=p[P_OY_ALT];MM_U(0xf56f8c)=p[P_FONT_ALT];}}
void FUN_00536a30(int x,int y,int w,int h){uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)w,(uint32_t)h};event(0x536a30,4,a);}
void FUN_00536c30(int k,int x,int y,int w,int h,int color){uint32_t a[]={(uint32_t)k,(uint32_t)x,(uint32_t)y,(uint32_t)w,(uint32_t)h,(uint32_t)color};event(0x536c30,6,a);}
void FUN_004cc720(uint32_t text,int x,int y,int w,uint32_t font,int flags)
{uint32_t a[]={text,(uint32_t)x,(uint32_t)y,(uint32_t)w,font,(uint32_t)flags};event(0x4cc720,6,a);if(texts++==1&&(p[P_MUTATIONS]&16384)){MM_B(0xf535c0)=(uint8_t)p[P_LATCH];MM_U(0xf535c4)=p[P_MUT_INDEX];}}
void FUN_004d6250(int group,int label,int x,int y,int w,uint32_t font,int flags,int sx,int sy)
{uint32_t a[]={(uint32_t)group,(uint32_t)label,(uint32_t)x,(uint32_t)y,(uint32_t)w,font,(uint32_t)flags,(uint32_t)sx,(uint32_t)sy};event(0x4d6250,9,a);if(labels++==0&&(p[P_MUTATIONS]&64))MM_U(0xf535c8)=p[P_MUT_COUNT];}
void FUN_004d63d0(int k,int x,int y,int w,int h,uint32_t color,int sx,int sy)
{uint32_t a[]={(uint32_t)k,(uint32_t)x,(uint32_t)y,(uint32_t)w,(uint32_t)h,color,(uint32_t)sx,(uint32_t)sy};event(0x4d63d0,8,a);++borders;if(p[P_MUTATIONS]&128){MM_B(0xf535c0)=1;MM_U(0xf535c4)=p[P_MUT_INDEX];}}
void restore_menu_callback(uint32_t token){if(restore_menu_modal_action(token)||restore_display_confirmation_action(token))return;CHECK(token==0x51f780);}
uint32_t restore_display_confirmation_clock(void){event(0x56f22c,0,NULL);return p[P_NOW];}
static uintptr_t game(uint32_t id,uintptr_t a,uintptr_t b,uintptr_t c)
{switch(id){case 0x41d110:return FUN_0041d110((int)a,(int)b);case 0x539170:FUN_00539170();return 0;case 0x4cc110:FUN_004cc110((int)a,(int)b);return 0;case 0x4cc160:FUN_004cc160((int)a,(int)b);return 0;case 0x538cd0:last_modal=(uint32_t)FUN_00538cd0((uint32_t)a,(uint32_t)b,(uint32_t)c);return last_modal;default:CHECK(0);return 0;}}
static void setup(void)
{
    static const unsigned char pool[]="\1\3Exit the city?\0\1\0\3\0Confirm exit\0";
    uint32_t i,j;for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j)MM_B(regions[i][0]+j)=(uint8_t)(p[P_SEED]+j*37u+i*11u);
    memcpy((void *)0x5f8910u,original_buttons,84);memcpy((void *)0x5c68a8u,original_display_button,28);memcpy((void *)(BASE+0x100u),pool,sizeof(pool));
    MM_U(0xe36c70)=BASE+0x100;MM_U(0xe34c38+5*8)=0;MM_U(0xe20768)=BODY;MM_U(0xe20764)=TITLE;
    MM_B(0xe38dda)=(uint8_t)p[P_UI];MM_B(0xe38dca)=0;MM_U(0xf54054)=p[P_ACTIVE];MM_U(0xf5404c)=p[P_READY];MM_U(0xf54114)=p[P_HANDLE];MM_U(0xf54118)=p[P_KIND];MM_B(0xf53ffc)=(uint8_t)p[P_MOVIE_UI];MM_B(0xd3b43d)=(uint8_t)p[P_CLOSE];
    MM_U(0xe39128)=p[P_ICONIC];MM_U(0xe3912c)=p[P_FOREGROUND];MM_U(0xe39140)=p[P_HWND];MM_U(0x6fdc40)=0;MM_U(0x6fdc54)=p[P_TEXT]?BASE+0x900u:0;MM_B(BASE+0x904u)=p[P_TEXT]==2?1:0;
    MM_U(0xf56f8c)=p[P_FONT_BODY];MM_U(0xf56fb4)=p[P_FONT_TITLE];MM_U(0xf56fbc)=p[P_FONT_DRAW];MM_U(0xe38db0)=p[P_WIDTH];MM_U(0xe38dac)=p[P_HEIGHT];MM_U(0xe38da8)=p[P_OX];MM_U(0xe38da4)=p[P_OY];
    MM_U(0xe38d9c)=p[P_WIDTH];MM_U(0xe38d98)=p[P_HEIGHT];MM_U(0xe39130)=MM_U(0xe39134)=0;MM_U(0xe39138)=p[P_WIDTH];MM_U(0xe3913c)=p[P_HEIGHT];MM_U(0xe38dcc)=1;
    for(i=0xe38dec;i<=0xe38e14;++i)MM_B(i)=0;MM_U(0xe38e34)=MM_U(0xe38e30)=0xfffffc18u;MM_U(0xe92e0c)=0;MM_U(0xe36c8c)=0;
    MM_U(0xe94d9c)=p[P_LAYOUT_READY];MM_U(0xe94d98)=p[P_HOVER];MM_U(0x605a44)=0;
    for(i=0;i<256;++i){MM_U(0xe94618+i*16)=0;MM_U(0xe9461c+i*16)=0;MM_U(0xe94620+i*16)=0;MM_U(0xe94624+i*16)=p[P_BUTTON_READY];}
    if(p[P_NODES]&1){uint32_t node=BASE+0x800;int span=(int)((uint32_t)p[P_FINAL_WIDTH]+20u);MM_U(node)=(uint32_t)((int)p[P_WIDTH]/2-span/2+8-(int)p[P_OX]);MM_U(node+4)=(uint32_t)((int)p[P_HEIGHT]/2-45-(int)p[P_OY]);MM_U(node+8)=p[P_NODE_X];MM_U(node+12)=p[P_NODE_Y];MM_U(node+16)=7;MM_U(node+20)=2;MM_U(node+24)=0;MM_U(0xe94620+84*16)=node;}
    if(p[P_NODES]&2){for(i=0;i<3;++i){uint32_t node=BASE+0x840+i*32;MM_U(node)=i+1;MM_U(node+8)=100+i*140;MM_U(node+12)=100;MM_U(node+16)=120;MM_U(node+20)=24;MM_U(node+24)=i<2?node+32:0;}MM_U(0xe9461c+84*16)=BASE+0x840;}
    MM_B(0xf535c0)=(uint8_t)p[P_LATCH];MM_U(0xf535c4)=p[P_INDEX];MM_U(0xf535c8)=p[P_INPUT_COUNT];MM_U(0xd3b198)=p[P_PRIMARY];MM_U(0xd3b194)=p[P_SECONDARY];MM_U(0x7056c8)=0;
    MM_U(0x6057e8)=p[P_STARTED];MM_U(0x70570c)=p[P_API_SAVED];used=frames=peeks=gets=waits=metrics=labels=borders=texts=panels=0;result=last_modal=0;
}
static void run(void)
{
    setup();switch(p[P_OP]){
    case 0:result=(uint32_t)FUN_00538940(TITLE,BODY,p[P_FLAGS]);break;
    case 1:result=(uint32_t)FUN_00538c80(TITLE,BODY,p[P_FLAGS]);break;
    case 2:result=(uint32_t)FUN_00538cd0(TITLE,BODY,p[P_FLAGS]);break;
    case 3:FUN_00539170();break;case 4:FUN_00539220(p[P_IGNORED]);break;
    case 5:result=(uint32_t)restore_window_message(&window_api,(HWND)(uintptr_t)p[P_HWND],WM_KEYDOWN,VK_ESCAPE,0);break;
    case 6:FUN_00538920();break;case 7:restore_menu_modal_input();break;
    default:CHECK(0);
    }
    if(p[P_AFTER_DISPLAY]&&MM_B(0xe38dda)==0x3e&&!MM_B(0xd3b43d)){FUN_004cf210();restore_display_confirmation_input();}
}
static void output(void)
{
    static const char hex[]="0123456789abcdef";static char state[24000];uint32_t i,j,n=0;
    printf("{\"result\":%u,\"modal\":%u,\"state\":\"",result,last_modal);for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j){uint8_t v=MM_B(regions[i][0]+j);CHECK(n+2<=sizeof(state));state[n++]=hex[v>>4];state[n++]=hex[v&15];}fwrite(state,1,n,stdout);
    printf("\",\"events\":[");for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<32;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t i;CHECK(restore_data_open());CHECK(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);memcpy(original_buttons,(void *)0x5f8910u,84);memcpy(original_display_button,(void *)0x5c68a8u,28);memset(&window_api,0,sizeof(window_api));window_api.game=game;window_api.def_window=default_window;
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){printf("{\"fields\":%d,\"message_size\":%u}\n",P_FIELDS,(unsigned)sizeof(MSG));fflush(stdout);for(;;){for(i=0;i<P_FIELDS;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}}
    else{
        memset(p,0,sizeof(p));p[P_OP]=0;p[P_FLAGS]=4;p[P_UI]=84;p[P_LATCH]=1;p[P_INDEX]=1;p[P_WIDTH]=640;p[P_HEIGHT]=480;p[P_BODY_WIDTH]=200;p[P_TITLE_WIDTH]=240;p[P_FINAL_WIDTH]=240;p[P_LAYOUT_READY]=p[P_BUTTON_READY]=1;
        if(argc==2&&!strcmp(argv[1],"--invalid-index"))p[P_INDEX]=0x10000001u;
        run();CHECK(result==7&&MM_U(0xf535c8)==2);CHECK(!restore_menu_modal_action(0x8920));puts("Modal label/result mapping and full callback identity passed.");
    }
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
