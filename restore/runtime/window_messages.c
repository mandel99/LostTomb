#include "window_messages.h"
#include "video_player.h"
#define B(address) (*(uint8_t *)(uintptr_t)(address))
#define W(address) (*(uint16_t *)(uintptr_t)(address))
#define S(address) (*(int16_t *)(uintptr_t)(address))
#define I(address) (*(int32_t *)(uintptr_t)(address))
#define U(address) (*(uint32_t *)(uintptr_t)(address))
#define H(address) (*(HWND *)(uintptr_t)(address))
#define C0(id) api->game(id,0,0,0)
#define C1(id,a) api->game(id,(uintptr_t)(a),0,0)
#define C2(id,a,b) api->game(id,(uintptr_t)(a),(uintptr_t)(b),0)
#define C3(id,a,b,c) api->game(id,(uintptr_t)(a),(uintptr_t)(b),(uintptr_t)(c))
static void dirty(void) {B(0xe38dca)=1; B(0xe38dc8)=1;}
static int is_case(uint32_t key,uint32_t upper) {return key==upper||key==upper+32;}
static int building_kind(void)
{
    int kind=S(0x7999e8u+U(0xeadd6c)*264u);
    return kind==0x48||kind==0xc1||kind==0x49;
}
static void repeat(const RestoreWindowApi *api,uint32_t entry,int count)
{while(count-->0) C0(entry);}
static void change_resolution(const RestoreWindowApi *api,uint32_t mode)
{
    U(0x5e06b0)=1; C1(0x4d01e0,1); C1(0x4cfdc0,mode);
    C0(0x414bd0); C0(0x4e2ab0); C0(0x4d0220);
    U(0x5e06b0)=0; C0(0x402da0); B(0xe38dca)=1;
}
static void page_key(const RestoreWindowApi *api,int down)
{
    switch(B(0xe38dda)) {
    case 1: I(0xd3b198)=down?1:2; C0(0x409eb0); break;
    case 7:
        if(I(0x605a44)==6) {I(0xd3b198)=down; repeat(api,0x40c760,12); B(0xe38dca)=1;}
        break;
    case 9:
        if(building_kind()) {
            int index=0; I(0xd3b198)=0;
            /* PageDown assigns one; PageUp assigns zero before querying. */
            I(0xd3b198)=down;
            while(index<(int)C0(0x502780)) {C0(0x40c6b0); ++index;}
            B(0xe38dca)=1;
        }
        break;
    case 13: case 16:
        I(0xd3b198)=down; repeat(api,0x402980,12); B(0xe38dca)=1; break;
    case '/':
        I(0xd3b198)=down; repeat(api,0x516890,12); B(0xe38dca)=1; break;
    case '=':
        I(0xd3b198)=down; repeat(api,0x41f460,15); B(0xe38dca)=1;
        break;
    }
}
static void arrow_key(const RestoreWindowApi *api,UINT key)
{
    uint32_t flag=key==VK_UP?0xe92e38u:0xe92e34u;
    int down=key==VK_DOWN, special=1;
    switch(B(0xe38dda)) {
    case 7:
        if(I(0x605a44)==6) {I(0xd3b198)=down; C0(0x40c760);}
        break;
    case 9:
        if(building_kind()) {I(0xd3b198)=down; C0(I(0xeadcc0)==0?0x40c630:0x40c6b0);}
        break;
    case 13: case 16: I(0xd3b198)=down; C0(0x402980); break;
    case '/': I(0xd3b198)=down; C0(0x516890); break;
    case '=': I(0xd3b198)=down; C0(0x41f460); break;
    default: special=0; break;
    }
    if(!special) {C0(down?0x4cc390:0x4cc370); C2(0x407c10,down?4:0,I(0xe92e20)?8:1);}
    U(flag)=1;
}
static void formation_key(const RestoreWindowApi *api,uint32_t key,int from_building)
{
    uint32_t record=0xbcb9c0u; /* Selection index is loaded before the action. */
    uint32_t index=U(record),offset=index*144u;
    if(is_case(key,'C')) {
        if(S(0xbcb9ccu+offset)==13&&S(0xbcba46u+offset)==0) {
            if(from_building) {I(0xd3b198)=1; C0(0x406720);}
            else B(0xe38dda)='I';
        }
    } else if(is_case(key,'F')) {
        if(!from_building) U(0xeadccc)=index;
        C0(0x406fb0);
    } else if(is_case(key,'L')) {
        if(S(0xbcb9ccu+offset)!=13) {
            if(from_building) {I(0xd3b198)=1; C0(0x406720);}
            else W(0xbcb9f2u+offset)=B(0xbcba3fu+offset)?4:3;
        }
    } else if(is_case(key,'M')) {
        if(from_building) {
            I(0xd3b198)=3; C0(0x406720);
            if(S(0xbcb9f2u+offset)==6) B(0xe38dda)=1;
        } else {W(0xbcba2au+offset)=W(0xbcb9f2u+offset); W(0xbcb9f2u+offset)=6;}
    } else if(is_case(key,'N')) {
        if(from_building) {I(0xd3b198)=2; C0(0x406720);}
        else W(0xbcb9f2u+offset)=15;
    } else if(is_case(key,'R')) {
        if(!from_building) U(0xd3b194)=1;
        C0(0x407110);
    } else if(is_case(key,'T')) {
        if(from_building) {I(0xd3b198)=0; C0(0x406720);}
        else W(0xbcb9f2u+offset)=B(0xbcba3fu+offset)?2:1;
    }
}
static void special_selection(const RestoreWindowApi *api,uint32_t key,int category,int saved)
{
    int action=-1;
    if(category==5) {
        if(is_case(key,'H')) action=0; else if(is_case(key,'N')) action=1;
        else if(is_case(key,'A')) action=2; else if(is_case(key,'R')) action=3;
        else if(is_case(key,'W')) action=4;
    } else {
        if(is_case(key,'H')) action=0; else if(is_case(key,'E')) action=1;
        else if(is_case(key,'K')) action=2; else if(is_case(key,'R')) action=3;
        else if(is_case(key,'W')) action=4;
    }
    if(action>=0) {
        I(0xd3b198)=action;
        C0(category==5?(action==4?0x406de0:0x406c90):(action==4?0x406e80:0x406b40));
        I(0xd3b198)=saved;
    }
}
static void character_key(const RestoreWindowApi *api,uint32_t key)
{
    uint8_t old_mode;
    uint32_t old_panel;
    int saved,mode;
    C1(0x4cc250,key);
    old_mode=B(0xe38dda);
    if(key=='[') C1(0x416ba0,1);
    else if(key==']') C1(0x416ba0,0);
    else {
        if(is_case(key,'P')) {
            if(old_mode==1||old_mode==30||old_mode=='I'||old_mode==31||old_mode==32) {
                B(0xe38dda)=1;
                if(I(0x605a3c)==0) {B(0xe38e60)=!B(0xe38e60); C0(0x4e2ab0);}
            }
        }
        B(0xe38dda)=old_mode;
        if(is_case(key,'M')&&I(0x605a3c)==0) {I(0x6fdc50)=0; B(0x605a5c)=!B(0x605a5c);}
    }
    old_panel=U(0xe94e10); saved=I(0xd3b198); mode=B(0xe38dda);
    if(mode==30) {formation_key(api,key,0); return;}
    if(mode=='I') {
        if(I(0xbcb9c0)!=0) {B(0xe38dda)=30; formation_key(api,key,0); return;}
    } else if(mode==31) {
        if(I(0xd05708)!=0) {U(0xeadcc8)=U(0xd05700); special_selection(api,key,6,saved); return;}
    } else if(mode==32) {
        if(I(0xd0570c)!=0) {U(0xeadcc4)=U(0xd05704); special_selection(api,key,5,saved); return;}
    } else if(mode==9) {
        if(I(0xeadd50)==4) {formation_key(api,key,1); return;}
        if(I(0xeadd50)==5) {U(0xeadcc4)=U(0xd05704); special_selection(api,key,5,saved); return;}
        if(I(0xeadd50)==6) {U(0xeadcc4)=U(0xd05700); special_selection(api,key,6,saved); return;}
    } else if(mode==14) {
        if(key>='0'&&key<='9') {I(0xd3b198)=(int32_t)key-'0'; C0(0x4035a0);}
        return;
    }
    if(key==' '&&(mode==1||mode=='I'||mode==31||mode==32)) {
        B(0xe38dda)=1; U(0xe94e10)=U(0xe94e0c); U(0xe94e0c)=old_panel;
        B(0xe38dca)=1; C0(0x547a60);
    }
    if(I(0x605a3c)==0) {
        static const struct {uint8_t key,action;} hotkeys[]={
            {'F',8},{'D',9},{'C',10},{'T',31},{'W',45},{'H',51},{'X',25},{'Y',48},{'Z',43}};
        size_t i;
        for(i=0;i<sizeof(hotkeys)/sizeof(hotkeys[0]);++i)
            if(is_case(key,hotkeys[i].key)) C1(0x4168e0,hotkeys[i].action);
        if(key=='`') C1(0x416990,1);
        else if(key=='1') C1(0x416990,2);
        else if(key>='2'&&key<='9') C1(0x416990,key-'2'+4);
        else if(key=='0') C1(0x416990,12);
        else if(key=='-') C1(0x416990,13);
        else if(key=='=') C1(0x416990,14);
        else if(is_case(key,'L')) C0(0x416a20);
    } else if(is_case(key,'H')) B(0x6de515)=!B(0x6de515);
}
/* Only Escape returns zero instead of the default procedure result. */
static int key_down(const RestoreWindowApi *api,HWND window,uint32_t key)
{
    uint32_t step;
    B(0xe38e4e)=1;
    switch(key) {
    case VK_BACK: C0(0x4cc330); break;
    case VK_RETURN: C0(B(0xe38dda)==14?0x4035f0:0x4cc350); C0(0x4cc320); break;
    case VK_SHIFT: U(0xe92e24)=1; break;
    case VK_CONTROL: U(0xe92e20)=1; break;
    case VK_ESCAPE:
        C0(0x539170); C2(0x4cc110,5,0); C2(0x4cc160,5,1);
        step=(uint32_t)C3(0x538cd0,U(0xe20764),U(0xe20768),4);
        B(0xe38e07)=0;
        if(step==6) {B(0xd3b43d)=1; B(0xe38dca)=1;}
        return 1;
    case VK_PRIOR: page_key(api,0); break;
    case VK_NEXT: page_key(api,1); break;
    case VK_END: C0(0x4cc3e0); break;
    case VK_HOME: C0(0x4cc3c0); if(B(0xe38dda)==1) {I(0xd3b198)=0; C0(0x409eb0);} break;
    case VK_LEFT: case VK_RIGHT:
        C0(key==VK_LEFT?0x4cc370:0x4cc390);
        C2(0x407c10,key==VK_LEFT?6:2,I(0xe92e20)?8:1);
        U(key==VK_LEFT?0xe92e30u:0xe92e2cu)=1; break;
    case VK_UP: case VK_DOWN: arrow_key(api,key); break;
    case VK_INSERT: C0(0x4cc360); break;
    case VK_DELETE: C0(B(0xe38dda)==14?0x403770:0x4cc350); break;
    case 'A': case 'a':
        if(!B(0x578c68)) {
            if(I(0xe92e20)&&B(0xe38dda)==1) U(0x605a0c)^=1;
            C0(0x404350); C0(0x4051a0);
            if(I(0xe92e20)&&B(0xe38e74)&&B(0xe38dda)!='6') {
                uintptr_t name; C0(0x41bae0); name=C0(0x516a20); C1(0x41b8a0,name);
            }
        }
        break;
    case 'G': case 'g':
        if(!B(0x578c68)&&I(0xe92e20)&&B(0xe38dda)==1) {U(0x605a08)++; if(I(0x605a08)>4) I(0x605a08)=0;} break;
    case 'H': case 'h':
        if(!B(0x578c68)&&I(0xe92e20)&&B(0xe38dda)==1) {U(0x605a04)++; if(I(0x605a04)>3) I(0x605a04)=1;} break;
    case 'J': case 'j':
        if(!B(0x578c68)&&I(0xe92e20)&&B(0xe38dda)==1) {U(0x605a00)++; if(I(0x605a00)>4) I(0x605a00)=0;} break;
    case 'K': case 'k':
        if(!B(0x578c68)&&I(0xe92e20)&&B(0xe38dda)==1&&I(0x605a00)) C3(0x4c47a0,U(0x605a08),U(0x605a04),U(0x605a00)); break;
    case 'b': if(!B(0x578c68)) {U(0xe38e94)--; if(I(0xe38e94)<0) I(0xe38e94)=27;} break;
    case 'c': if(B(0xe38dda)==14&&B(0x578c68)) {I(0xd3b198)=3; C0(0x4035a0);} break;
    case VK_F1: case VK_F2: case VK_F3: case VK_F4: C1(I(0xe92e20)?0x416b10:0x416ac0,key-VK_F1); break;
    case VK_F5:
        if(I(0xe38e5c)==0) {
            RECT rect; DWORD extra,style; HMENU menu;
            api->client_rect(window,&rect); rect.left=rect.top=0; rect.right=I(0xe38d9c); rect.bottom=I(0xe38d98);
            extra=(DWORD)api->window_long(window,GWL_EXSTYLE); menu=api->menu(window);
            style=(DWORD)api->window_long(window,GWL_STYLE); api->adjust(&rect,style,menu!=0,extra);
            api->position(window,0,(int32_t)(U(0xe38db8)-U(0xe38db0))/2,
                (int32_t)(U(0xe38db4)-U(0xe38dac))/2,rect.right-rect.left,rect.bottom-rect.top,0x14);
        }
        break;
    case VK_F6:
        U(0xe38e5c)=!I(0xe38e5c); U(0x5e06b0)=1; C1(0x4d01e0,1); C0(0x4d0220); U(0x5e06b0)=0;
        api->set_active(0); api->set_active(H(0xe39140)); api->set_foreground(H(0xe39140)); api->show(H(0xe39140),3);
        B(0xe38dca)=1; break;
    case VK_F7: case VK_F8: case VK_F9:
        if(B(0xe38dda)!=15&&B(0xe38dda)!='H'&&B(0xe38dda)!=14&&!(key==VK_F7&&B(0xe38dda)==21))
            change_resolution(api,key-VK_F7+1);
        break;
    case VK_F12: B(0x775680)=!B(0x775680); break;
    }
    return 0;
}
LRESULT restore_window_message(const RestoreWindowApi *api,HWND window,UINT message,WPARAM key,LPARAM value)
{
    uint32_t packed=(uint32_t)value;
    if(I(0x6fdc40)) {
        MSG captured;
        DWORD position;
        captured.hwnd=window; captured.message=message; captured.wParam=key; captured.lParam=value;
        captured.time=(DWORD)api->message_time(); position=api->message_pos();
        captured.pt.x=(LONG)(position&0xffff); captured.pt.y=(LONG)(position>>16);
        C1(0x560330,&captured);
    }
    if((RESTORE_CURRENT_VIDEO&&!RESTORE_CURRENT_VIDEO->stopped)||
       (B(0xe38dda)=='T'&&message!=WM_SETCURSOR&&message!=WM_PAINT&&message!=WM_MOUSEMOVE&&
        message!=WM_RBUTTONUP&&message!=WM_RBUTTONDOWN&&message!=WM_RBUTTONDBLCLK&&
        message!=WM_LBUTTONUP&&message!=WM_LBUTTONDOWN&&message!=WM_LBUTTONDBLCLK))
        return api->def_window(window,message,key,value);
    C2(0x41d110,message,key);
    switch(message) {
    case WM_CREATE: H(0x6fdc44)=window; break;
    case WM_DESTROY: C0(0x5391e0); H(0xe39140)=0; C1(0x4d01e0,1); api->quit(0); break;
    case WM_MOVE: case WM_SIZE:
        if(api->is_iconic(window)) C0(0x4cff90);
        else {C0(0x4cffc0); I(0xe3912c)=1;}
        if(I(0xe38e5c)==0) {
            api->client_rect(window,(RECT *)0xe39130u);
            api->client_to_screen(window,(POINT *)0xe39130u); api->client_to_screen(window,(POINT *)0xe39138u);
        } else {
            int height=api->metrics(SM_CYSCREEN),width=api->metrics(SM_CXSCREEN);
            RECT *rect=(RECT *)0xe39130u; rect->left=rect->top=0; rect->right=width; rect->bottom=height;
        }
        C1(0x4d0280,1); dirty(); break;
    case WM_ACTIVATE:
        if(((uint32_t)key>>16)!=0) {C1(0x55f561,0); C1(0x55f561,2); C1(0x55f561,1); I(0x6fdc48)=1;}
        else if(I(0x6fdc48)&&I(0xe38e5c)) {
            api->set_active(0); api->set_active(H(0xe39140)); api->set_foreground(H(0xe39140)); api->show(H(0xe39140),3); I(0x6fdc48)=0;
        }
        break;
    case WM_PAINT: {
        PAINTSTRUCT paint;
        api->begin_paint(window,&paint); api->end_paint(window,&paint); C1(0x4d0280,1); dirty(); break;
    }
    case WM_CLOSE: B(0xd3b43d)=3; break;
    case WM_ACTIVATEAPP:
        if(key&&api->foreground()==window&&!api->is_iconic(window)) {
            I(0xe3912c)=1; C3(0x4ce6e0,0x5792e8u,0,0);
        } else {I(0xe3912c)=0; C3(0x4ce6e0,0x5792d4u,0,0);}
        if(I(0xe39128)&&I(0xe3912c)) {
            int ready=(int)C0(0x4cffe0);
            if(!ready&&api->foreground()==window) {C3(0x4ce6e0,0x5792b0u,0,0); ready=(int)C0(0x4d0220);}
            if(ready) C0(0x4cffc0);
        }
        dirty(); break;
    case WM_MOUSEMOVE: case WM_LBUTTONDOWN: case WM_LBUTTONUP: case WM_RBUTTONDOWN: case WM_RBUTTONUP:
        if(message==WM_LBUTTONDOWN) {B(0xe38e02)=1; B(0xe38e07)|=1;}
        if(message==WM_LBUTTONUP) B(0xe38e07)&=0xfe;
        if(message==WM_RBUTTONDOWN) B(0xe38e07)|=2;
        if(message==WM_RBUTTONUP) B(0xe38e07)&=0xfd;
        W(0xe38e0a)=(uint16_t)packed; W(0xe38e08)=(uint16_t)(packed>>16); break;
    case WM_DISPLAYCHANGE: U(0xe38dbc)=(uint32_t)key; U(0xe38db8)=packed&0xffff; U(0xe38db4)=packed>>16; break;
    case WM_KEYDOWN: if(key_down(api,window,(uint32_t)key)) return 0; break;
    case WM_KEYUP: if(key==VK_SHIFT) U(0xe92e24)=0; else if(key==VK_CONTROL) U(0xe92e20)=0; break;
    case WM_CHAR: character_key(api,(uint32_t)key); break;
    case WM_SYSKEYDOWN:
        if(is_case((uint32_t)key,'D')) U(0x6fdc50)=!I(0x6fdc50);
        else if(key=='X') api->post(window,WM_KEYDOWN,VK_ESCAPE,0);
        else if(is_case((uint32_t)key,'Z')) C0(0x406090);
        break;
    }
    return api->def_window(window,message,key,value);
}
