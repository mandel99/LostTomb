#include "window_messages.h"
#include "video_player.h"
#include "standalone_data.h"
#include "display_configuration.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "window_oracle_fields.h"
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define H(a) (*(HWND *)(uintptr_t)(a))
typedef struct {uint32_t id; uintptr_t a,b,c;} Call;
static Call calls[128];
static unsigned int count,defaults,quit_count,paint_count,paint_end_count,positions,active_count,foreground_sets,shows,post_count,metric_count;
static int iconic,ready,reinit,population,dialog;
static HWND foreground_window,active_values[4];
static int metric_order[4],position_values[5];
static RECT adjusted_input;
static MSG traced,default_message,posted;
static RestoreVideoPlayer video;
static const HWND window=(HWND)0xf1234567u;
static uint32_t message_count;
static unsigned int invalidations;
extern void FUN_004cff90(void);
extern void FUN_004cffc0(void);
static void require_at(int result,int line)
{if(!result) {fprintf(stderr,"Window reconstruction failure at line %d\n",line); abort();}}
#define REQUIRE(c) require_at(!!(c),__LINE__)
static LRESULT WINAPI default_proc(HWND h,UINT m,WPARAM w,LPARAM l)
{++defaults; default_message.hwnd=h; default_message.message=m; default_message.wParam=w; default_message.lParam=l; return (LRESULT)0x89abcdefu;}
static BOOL WINAPI is_iconic(HWND h) {REQUIRE(h==window); return iconic;}
static HWND WINAPI foreground(void) {return foreground_window;}
static HWND WINAPI set_active(HWND h) {REQUIRE(active_count<4); active_values[active_count++]=h; return 0;}
static BOOL WINAPI set_foreground(HWND h) {REQUIRE(h==window); ++foreground_sets; return TRUE;}
static BOOL WINAPI show(HWND h,int how) {REQUIRE(h==window&&how==3); ++shows; return TRUE;}
static int WINAPI metrics(int which)
{REQUIRE(metric_count<4); metric_order[metric_count++]=which; return which==SM_CXSCREEN?1920:1080;}
static BOOL WINAPI client_rect(HWND h,LPRECT r)
{REQUIRE(h==window); r->left=r->top=0; r->right=640; r->bottom=480; return TRUE;}
static BOOL WINAPI client_to_screen(HWND h,LPPOINT p) {REQUIRE(h==window); p->x+=17; p->y+=19; return TRUE;}
static LONG WINAPI window_long(HWND h,int index)
{REQUIRE(h==window&&(index==GWL_STYLE||index==GWL_EXSTYLE)); return index==GWL_STYLE?0x81234567u:0x92345678u;}
static HMENU WINAPI menu(HWND h) {REQUIRE(h==window); return (HMENU)0xfedcba98u;}
static BOOL WINAPI adjust(LPRECT r,DWORD style,BOOL has_menu,DWORD extra)
{
    REQUIRE(style==0x81234567u&&has_menu==TRUE&&extra==0x92345678u);
    adjusted_input=*r; r->left-=4; r->top-=5; r->right+=6; r->bottom+=7; return TRUE;
}
static BOOL WINAPI position(HWND h,HWND after,int x,int y,int width,int height,UINT flags)
{REQUIRE(h==window&&after==0); ++positions; position_values[0]=x; position_values[1]=y; position_values[2]=width; position_values[3]=height; position_values[4]=(int)flags; return TRUE;}
static HDC WINAPI begin_paint(HWND h,LPPAINTSTRUCT p) {REQUIRE(h==window); memset(p,0,sizeof(*p)); ++paint_count; return (HDC)0x87654321u;}
static BOOL WINAPI end_paint(HWND h,const PAINTSTRUCT *p) {REQUIRE(h==window&&p!=0); ++paint_end_count; return TRUE;}
static void WINAPI quit(int code) {REQUIRE(code==0); ++quit_count;}
static BOOL WINAPI post(HWND h,UINT m,WPARAM w,LPARAM l)
{++post_count; posted.hwnd=h; posted.message=m; posted.wParam=w; posted.lParam=l; return TRUE;}
static LONG WINAPI message_time(void) {return (LONG)0xfedcba98u;}
static DWORD WINAPI message_pos(void) {return 0xffff8000u;}
static BOOL WINAPI invalidate(HWND h,const RECT *rect,BOOL erase)
{REQUIRE(h==window&&rect==0&&erase==TRUE); ++invalidations; return TRUE;}
void FUN_004ce6e0(char *text,int detail,int number)
{REQUIRE(text==(char *)0x005e0764u&&detail==0&&number==0);}
static uintptr_t game(uint32_t id,uintptr_t a,uintptr_t b,uintptr_t c)
{
    REQUIRE(count<128); calls[count++]=(Call){id,a,b,c};
    switch(id) {
    case 0x560330: traced=*(const MSG *)a; break;
    case 0x4cff90: FUN_004cff90(); break;
    case 0x4cffc0: FUN_004cffc0(); break;
    case 0x4cffe0: return ready;
    case 0x4d0220: return reinit;
    case 0x502780: return population;
    case 0x538cd0: return dialog;
    case 0x516a20: return 0xfedcba98u;
    case 0x4cfdc0: restore_display_configure((uint32_t)a); break;
    }
    return 0;
}
static const RestoreWindowApi api={default_proc,is_iconic,foreground,set_active,set_foreground,show,metrics,
    client_rect,client_to_screen,window_long,menu,adjust,position,begin_paint,end_paint,quit,post,message_time,message_pos,game,invalidate};
const RestoreWindowApi *restore_window_platform(void) {return &api;}
static void reset(void)
{
    count=defaults=quit_count=paint_count=paint_end_count=positions=active_count=foreground_sets=shows=post_count=metric_count=0;
    invalidations=0;
    iconic=0; ready=reinit=1; population=3; dialog=6; foreground_window=window;
    memset(&video,0,sizeof(video)); memset(&traced,0,sizeof(traced));
    RESTORE_CURRENT_VIDEO=0;
    I(0x6fdc40)=I(0x6fdc48)=I(0x6fdc50)=0; H(0x6fdc44)=0; H(0xe39140)=window;
    B(0xe38dda)=1; B(0xe38dca)=B(0xe38dc8)=0;
    B(0xd3b43d)=0;
    B(0xe38e07)=0xf0; B(0xe38e02)=B(0xe38e4e)=B(0xe38e60)=B(0x605a5c)=0;
    W(0xe38e08)=0x1111; W(0xe38e0a)=0x2222;
    I(0xe39128)=I(0xe3912c)=I(0xe38e5c)=0;
    U(0xe92e20)=U(0xe92e24)=U(0xe92e2c)=U(0xe92e30)=U(0xe92e34)=U(0xe92e38)=0;
    U(0xe20764)=0x81234567u; U(0xe20768)=0xfedcba98u;
    I(0xd3b198)=0x12345678; I(0x605a44)=6; I(0xeadd6c)=7; I(0xeadcc0)=0;
    W(0x7999e8+7*264)=0x48;
    B(0x578c68)=0; B(0xe38e74)=1; I(0x605a3c)=0;
    I(0x605a08)=I(0x605a00)=0; I(0x605a04)=1; U(0x605a0c)=0;
    I(0xe38e94)=0; B(0x775680)=B(0x6de515)=0;
    I(0xbcb9c0)=4; I(0xeadd50)=0; I(0xd05708)=I(0xd0570c)=1;
    U(0xd05700)=12; U(0xd05704)=13; U(0xeadcc4)=U(0xeadcc8)=U(0xeadccc)=0;
    U(0xd3b194)=0; U(0xe94e0c)=0x98765432u; U(0xe94e10)=0xfedcba98u;
    W(0xbcb9cc+4*144)=1; W(0xbcba46+4*144)=0; B(0xbcba3f+4*144)=0;
    W(0xbcb9f2+4*144)=0x7654; W(0xbcba2a+4*144)=0;
    I(0xe38db8)=1920; I(0xe38db4)=1080;
    restore_display_configure(3);
}
static LRESULT dispatch_test_message(UINT m,WPARAM w,LPARAM l) {++message_count; return restore_window_proc(window,m,w,l);}
static unsigned int occurrences(uint32_t id) {unsigned int i,n=0; for(i=0;i<count;++i) n+=calls[i].id==id; return n;}
static Call last(uint32_t id) {unsigned int i=count; while(i) {--i; if(calls[i].id==id) return calls[i];} REQUIRE(0); return calls[0];}
static void lifecycle(void)
{
    unsigned int i; RECT *r=(RECT *)0xe39130u;
    reset(); REQUIRE(dispatch_test_message(WM_CREATE,0,0)==(LRESULT)0x89abcdefu&&H(0x6fdc44)==window);
    REQUIRE(calls[0].id==0x41d110&&defaults==1);
    reset(); dispatch_test_message(WM_CLOSE,0,0); REQUIRE(B(0xd3b43d)==3&&quit_count==0&&defaults==1);
    reset(); dispatch_test_message(WM_DESTROY,0,0); REQUIRE(H(0xe39140)==0&&quit_count==1&&last(0x4d01e0).a==1);
    reset(); dispatch_test_message(WM_PAINT,0,0); REQUIRE(paint_count==1&&paint_end_count==1&&last(0x4d0280).a==1&&B(0xe38dc8)&&B(0xe38dca));
    reset(); dispatch_test_message(WM_MOVE,0,0); REQUIRE(r->left==17&&r->top==19&&r->right==657&&r->bottom==499&&I(0xe3912c)==1);
    reset(); iconic=1; dispatch_test_message(WM_SIZE,0,0); REQUIRE(I(0xe39128)==1&&occurrences(0x4cff90)==1&&invalidations==1);
    reset(); I(0xe38e5c)=1; dispatch_test_message(WM_SIZE,0,0);
    REQUIRE(r->left==0&&r->top==0&&r->right==1920&&r->bottom==1080&&metric_count==2&&metric_order[0]==SM_CYSCREEN&&metric_order[1]==SM_CXSCREEN);
    reset(); dispatch_test_message(WM_ACTIVATE,0x10001,0);
    REQUIRE(I(0x6fdc48)==1&&occurrences(0x55f561)==3&&calls[1].a==0&&calls[2].a==2&&calls[3].a==1);
    count=0; I(0xe38e5c)=1; dispatch_test_message(WM_ACTIVATE,1,0);
    REQUIRE(I(0x6fdc48)==0&&active_count==2&&active_values[0]==0&&active_values[1]==window&&foreground_sets==1&&shows==1);
    reset(); I(0xe39128)=1; ready=0; dispatch_test_message(WM_ACTIVATEAPP,1,0);
    REQUIRE(I(0xe3912c)==1&&I(0xe39128)==0&&occurrences(0x4cffe0)==1&&occurrences(0x4d0220)==1&&occurrences(0x4ce6e0)==2);
    reset(); foreground_window=0; dispatch_test_message(WM_ACTIVATEAPP,1,0); REQUIRE(I(0xe3912c)==0&&last(0x4ce6e0).a==0x5792d4u);
    reset(); dispatch_test_message(WM_DISPLAYCHANGE,32,(LPARAM)0x04380780u);
    REQUIRE(U(0xe38dbc)==32&&I(0xe38db8)==1920&&I(0xe38db4)==1080);
    reset(); video.stopped=0; RESTORE_CURRENT_VIDEO=&video;
    dispatch_test_message(WM_KEYDOWN,VK_ESCAPE,0); REQUIRE(count==0&&defaults==1&&B(0xe38e4e)==0);
    reset(); B(0xe38dda)='T'; dispatch_test_message(WM_KEYDOWN,VK_ESCAPE,0); REQUIRE(count==0&&defaults==1);
    reset(); B(0xe38dda)='T'; dispatch_test_message(WM_PAINT,0,0); REQUIRE(paint_count==1&&occurrences(0x41d110)==1);
    reset(); I(0x6fdc40)=1; dispatch_test_message(WM_NULL,0x87654321u,(LPARAM)0xfedcba98u);
    REQUIRE(calls[0].id==0x560330&&traced.hwnd==window&&traced.wParam==0x87654321u&&traced.lParam==(LPARAM)0xfedcba98u);
    REQUIRE(traced.time==0xfedcba98u&&traced.pt.x==32768&&traced.pt.y==65535);
    reset(); for(i=0;i<10000;++i) {count=0; REQUIRE(dispatch_test_message(0x7fff,i,(LPARAM)~i)==(LRESULT)0x89abcdefu);}
    REQUIRE(defaults==10000&&default_message.wParam==9999);
}
static void mouse_and_modifiers(void)
{
    reset(); dispatch_test_message(WM_MOUSEMOVE,0,(LPARAM)0xffff8001u); REQUIRE(W(0xe38e0a)==0x8001&&W(0xe38e08)==0xffff&&B(0xe38e07)==0xf0);
    dispatch_test_message(WM_LBUTTONDOWN,0,0x12345678); REQUIRE(B(0xe38e07)==0xf1&&B(0xe38e02)==1&&W(0xe38e0a)==0x5678&&W(0xe38e08)==0x1234);
    dispatch_test_message(WM_RBUTTONDOWN,0,0); REQUIRE(B(0xe38e07)==0xf3);
    dispatch_test_message(WM_LBUTTONUP,0,0); REQUIRE(B(0xe38e07)==0xf2);
    dispatch_test_message(WM_RBUTTONUP,0,0); REQUIRE(B(0xe38e07)==0xf0);
    W(0xe38e0a)=0xabcd; dispatch_test_message(WM_LBUTTONDBLCLK,0,0); REQUIRE(W(0xe38e0a)==0xabcd);
    reset(); dispatch_test_message(WM_KEYDOWN,VK_CONTROL,0); REQUIRE(U(0xe92e20)==1&&B(0xe38e4e)==1);
    dispatch_test_message(WM_KEYDOWN,VK_SHIFT,0); REQUIRE(U(0xe92e24)==1);
    U(0xe92e20)=U(0xe92e24)=0xffffffffu;
    dispatch_test_message(WM_KEYUP,VK_CONTROL,0); dispatch_test_message(WM_KEYUP,VK_SHIFT,0); REQUIRE(U(0xe92e20)==0&&U(0xe92e24)==0);
    reset(); REQUIRE(dispatch_test_message(WM_KEYDOWN,VK_ESCAPE,0)==0&&defaults==0&&B(0xd3b43d)==1&&B(0xe38e07)==0);
    REQUIRE(last(0x538cd0).a==0x81234567u&&last(0x538cd0).b==0xfedcba98u&&last(0x538cd0).c==4);
    reset(); dialog=7; dispatch_test_message(WM_KEYDOWN,VK_ESCAPE,0); REQUIRE(B(0xd3b43d)!=1&&B(0xe38e07)==0&&defaults==0);
}
static void keyboard_actions(void)
{
    unsigned int i,down;
    static const struct {unsigned char mode; uint32_t entry; unsigned int repeats;} pages[]={
        {7,0x40c760,12},{9,0x40c6b0,3},{13,0x402980,12},{16,0x402980,12},{'/',0x516890,12},{'=',0x41f460,15}};
    reset(); dispatch_test_message(WM_KEYDOWN,VK_RETURN,0); REQUIRE(occurrences(0x4cc350)==1&&occurrences(0x4cc320)==1);
    reset(); B(0xe38dda)=14; dispatch_test_message(WM_KEYDOWN,VK_RETURN,0); REQUIRE(occurrences(0x4035f0)==1&&occurrences(0x4cc320)==1);
    for(i=0;i<sizeof(pages)/sizeof(pages[0]);++i) for(down=0;down<2;++down) {
        reset(); B(0xe38dda)=pages[i].mode; dispatch_test_message(WM_KEYDOWN,down?VK_NEXT:VK_PRIOR,0);
        REQUIRE(occurrences(pages[i].entry)==pages[i].repeats);
        REQUIRE(I(0xd3b198)==(int)down);
    }
    reset(); B(0xe38dda)=1; dispatch_test_message(WM_KEYDOWN,VK_PRIOR,0); REQUIRE(I(0xd3b198)==2&&occurrences(0x409eb0)==1);
    reset(); B(0xe38dda)=9; W(0x7999e8+7*264)=7; dispatch_test_message(WM_KEYDOWN,VK_PRIOR,0); REQUIRE(occurrences(0x502780)==0);
    reset(); dispatch_test_message(WM_KEYDOWN,VK_LEFT,0); REQUIRE(last(0x407c10).a==6&&last(0x407c10).b==1&&U(0xe92e30)==1);
    reset(); I(0xe92e20)=1; dispatch_test_message(WM_KEYDOWN,VK_RIGHT,0); REQUIRE(last(0x407c10).a==2&&last(0x407c10).b==8&&U(0xe92e2c)==1);
    reset(); B(0xe38dda)=9; I(0xeadcc0)=0; dispatch_test_message(WM_KEYDOWN,VK_UP,0); REQUIRE(occurrences(0x40c630)==1&&U(0xe92e38)==1);
    reset(); B(0xe38dda)=9; I(0xeadcc0)=1; dispatch_test_message(WM_KEYDOWN,VK_DOWN,0); REQUIRE(occurrences(0x40c6b0)==1&&U(0xe92e34)==1);
    reset(); dispatch_test_message(WM_KEYDOWN,'b',0); REQUIRE(I(0xe38e94)==27);
    reset(); I(0xe92e20)=1; dispatch_test_message(WM_KEYDOWN,'A',0);
    REQUIRE(U(0x605a0c)==1&&occurrences(0x404350)==1&&occurrences(0x4051a0)==1&&last(0x41b8a0).a==0xfedcba98u);
    reset(); I(0xe92e20)=1; I(0x605a08)=4; I(0x605a04)=3; I(0x605a00)=4;
    dispatch_test_message(WM_KEYDOWN,'G',0); dispatch_test_message(WM_KEYDOWN,'H',0); dispatch_test_message(WM_KEYDOWN,'J',0);
    REQUIRE(I(0x605a08)==0&&I(0x605a04)==1&&I(0x605a00)==0);
    for(i=0;i<4;++i) {reset(); I(0xe92e20)=1; dispatch_test_message(WM_KEYDOWN,VK_F1+i,0); REQUIRE(last(0x416b10).a==i);}
    for(i=0;i<4;++i) {reset(); dispatch_test_message(WM_KEYDOWN,VK_F1+i,0); REQUIRE(last(0x416ac0).a==i);}
}
static void characters(void)
{
    unsigned int i,lower;
    static const struct {char key; int action;} hotkeys[]={{'F',8},{'D',9},{'C',10},{'T',31},{'W',45},{'H',51},{'X',25},{'Y',48},{'Z',43}};
    for(i=0;i<sizeof(hotkeys)/sizeof(hotkeys[0]);++i) for(lower=0;lower<2;++lower) {
        reset(); dispatch_test_message(WM_CHAR,hotkeys[i].key+lower*32,0); REQUIRE(last(0x4168e0).a==(uintptr_t)hotkeys[i].action);
    }
    reset(); B(0xe38dda)=30; dispatch_test_message(WM_CHAR,'P',0); REQUIRE(B(0xe38e60)==1&&B(0xe38dda)==30&&occurrences(0x4e2ab0)==1);
    reset(); dispatch_test_message(WM_CHAR,'M',0); REQUIRE(B(0x605a5c)==1&&I(0x6fdc50)==0);
    reset(); B(0xe38dda)=30; dispatch_test_message(WM_CHAR,'M',0); REQUIRE(W(0xbcba2a+4*144)==0x7654&&W(0xbcb9f2+4*144)==6);
    reset(); B(0xe38dda)=30; W(0xbcb9cc+4*144)=13; dispatch_test_message(WM_CHAR,'C',0); REQUIRE(B(0xe38dda)=='I');
    reset(); B(0xe38dda)='I'; dispatch_test_message(WM_CHAR,'T',0); REQUIRE(B(0xe38dda)==30&&W(0xbcb9f2+4*144)==1);
    reset(); B(0xe38dda)=31; dispatch_test_message(WM_CHAR,'K',0);
    REQUIRE(U(0xeadcc8)==12&&occurrences(0x406b40)==1&&I(0xd3b198)==0x12345678);
    reset(); B(0xe38dda)=32; dispatch_test_message(WM_CHAR,'N',0);
    REQUIRE(U(0xeadcc4)==13&&occurrences(0x406c90)==1&&I(0xd3b198)==0x12345678);
    reset(); B(0xe38dda)=9; I(0xeadd50)=6; dispatch_test_message(WM_CHAR,'W',0);
    REQUIRE(U(0xeadcc4)==12&&occurrences(0x406e80)==1&&I(0xd3b198)==0x12345678);
    reset(); B(0xe38dda)=14; dispatch_test_message(WM_CHAR,'9',0); REQUIRE(I(0xd3b198)==9&&occurrences(0x4035a0)==1&&occurrences(0x416990)==0);
    reset(); dispatch_test_message(WM_CHAR,' ',0); REQUIRE(U(0xe94e0c)==0xfedcba98u&&U(0xe94e10)==0x98765432u&&occurrences(0x547a60)==1);
    for(i='2';i<='9';++i) {reset(); dispatch_test_message(WM_CHAR,i,0); REQUIRE(last(0x416990).a==i-'2'+4);}
    reset(); I(0x605a3c)=1; dispatch_test_message(WM_CHAR,'H',0); REQUIRE(B(0x6de515)==1&&occurrences(0x4168e0)==0);
    reset(); dispatch_test_message(WM_SYSKEYDOWN,'X',0); REQUIRE(post_count==1&&posted.hwnd==window&&posted.message==WM_KEYDOWN&&posted.wParam==VK_ESCAPE);
    reset(); dispatch_test_message(WM_SYSKEYDOWN,'D',0); REQUIRE(I(0x6fdc50)==1);
}
static void modes_and_matrix(void)
{
    unsigned int key,mode;
    reset(); dispatch_test_message(WM_KEYDOWN,VK_F5,0);
    REQUIRE(positions==1&&adjusted_input.right==1024&&adjusted_input.bottom==768);
    REQUIRE(position_values[0]==448&&position_values[1]==156&&position_values[2]==1034&&position_values[3]==780&&position_values[4]==0x14);
    reset(); dispatch_test_message(WM_KEYDOWN,VK_F6,0); REQUIRE(I(0xe38e5c)==1&&I(0x5e06b0)==0&&active_count==2&&shows==1);
    for(key=VK_F7;key<=VK_F9;++key) {
        reset(); dispatch_test_message(WM_KEYDOWN,key,0);
        REQUIRE(last(0x4cfdc0).a==key-VK_F7+1&&I(0x5e06b0)==0&&B(0xe38dca)==1&&occurrences(0x414bd0)==1);
    }
    reset(); B(0xe38dda)=21; dispatch_test_message(WM_KEYDOWN,VK_F7,0); REQUIRE(occurrences(0x4cfdc0)==0);
    reset(); B(0xe38dda)=21; dispatch_test_message(WM_KEYDOWN,VK_F8,0); REQUIRE(last(0x4cfdc0).a==2);
    reset(); B(0xe38dda)=14; dispatch_test_message(WM_KEYDOWN,VK_F9,0); REQUIRE(occurrences(0x4cfdc0)==0);
    /* Exhaust every byte-sized game mode and character/key selector. Endpoints
       are controlled; no OS window/input or legacy game code is exercised. */
    for(mode=0;mode<256;++mode) for(key=0;key<256;++key) {
        reset(); B(0xe38dda)=(uint8_t)mode; dispatch_test_message(WM_KEYDOWN,key,0);
        reset(); B(0xe38dda)=(uint8_t)mode; dispatch_test_message(WM_CHAR,key,0);
    }
}
static void oracle_values(const uint32_t *values)
{
    size_t i;
    putchar('[');
    for(i=0;i<sizeof(oracle_fields)/sizeof(oracle_fields[0]);++i)
        printf("%s%u",i?",":"",values[i]);
    putchar(']');
}
static void oracle_read(uint32_t *values)
{
    size_t i;
    for(i=0;i<sizeof(oracle_fields)/sizeof(oracle_fields[0]);++i) {
        values[i]=0;
        memcpy(&values[i],(void *)oracle_fields[i].address,oracle_fields[i].width);
    }
}
static void oracle_batch(void)
{
    unsigned int message,key,mode,value,control,category,debug,variant,i;
    uint32_t before[sizeof(oracle_fields)/sizeof(oracle_fields[0])],after[sizeof(oracle_fields)/sizeof(oracle_fields[0])];
    printf("{\"fields\":[");
    for(i=0;i<sizeof(oracle_fields)/sizeof(oracle_fields[0]);++i)
        printf("%s[%u,%u]",i?",":"",(unsigned int)oracle_fields[i].address,oracle_fields[i].width);
    puts("]}"); fflush(stdout);
    while(scanf("%u %u %u %u %u %u %u %u",&message,&key,&mode,&value,&control,&category,&debug,&variant)==8) {
        LRESULT result;
        reset();
        I(0xe39130)=I(0xe39134)=I(0xe39138)=I(0xe3913c)=0;
        B(0xe38dda)=(uint8_t)mode; I(0xe92e20)=control; I(0xeadd50)=category; B(0x578c68)=(uint8_t)debug;
        if(variant&1) W(0xbcb9cc+4*144)=13;
        if(variant&2) I(0x605a44)=0;
        if(variant&4) I(0xe38e5c)=1;
        iconic=(variant&8)!=0; if(variant&16) ready=0; if(variant&32) reinit=0;
        if(variant&64) foreground_window=0; if(variant&128) dialog=7;
        oracle_read(before); result=dispatch_test_message(message,key,(LPARAM)value); oracle_read(after);
        printf("{\"before\":"); oracle_values(before); printf(",\"after\":"); oracle_values(after);
        printf(",\"return\":%u,\"calls\":[",(unsigned int)result);
        for(i=0;i<count;++i) printf("%s[%u,%u,%u,%u]",i?",":"",calls[i].id,(unsigned int)calls[i].a,(unsigned int)calls[i].b,(unsigned int)calls[i].c);
        printf("],\"platform\":[%u,%u,%u,%u,%u,%u,%u,%u,%u],\"position\":[%d,%d,%d,%d,%d]}\n",
            defaults,quit_count,paint_count,paint_end_count,positions,active_count,foreground_sets,shows,post_count,
            positions?position_values[0]:0,positions?position_values[1]:0,positions?position_values[2]:0,positions?position_values[3]:0,positions?position_values[4]:0);
        fflush(stdout);
    }
}
int main(int argc,char **argv)
{
    REQUIRE(restore_data_open());
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {oracle_batch(); restore_data_close(); return 0;}
    lifecycle(); mouse_and_modifiers(); keyboard_actions(); characters(); modes_and_matrix();
    restore_data_close();
    printf("Window callback C: %u synthetic messages, stdcall return, filters, mouse/key state, actions, activation and mode changes verified; no game/window/input used.\n",message_count);
    return 0;
}
