#include "training_panel.h"
#include "sprite_render.h"
extern uint32_t FUN_00443950(int),FUN_004439c0(int),FUN_00443a20(int),FUN_00443b20(int),FUN_00443b90(int);
extern void FUN_00536820(int,int,int,int);
extern void FUN_004d62f0(uint32_t,uint32_t,int,int,uint32_t,uint32_t,int,int);
extern void FUN_004d6250(uint32_t,uint32_t,int,int,uint32_t,uint32_t,uint32_t,int,int);
extern void FUN_004d63d0(uint32_t,int,int,uint32_t,uint32_t,uint32_t,int,int);
extern void FUN_004d6860(uint32_t,uint32_t,uint32_t,int,int,uint32_t,uint32_t,int,int);
extern void FUN_004ccc30(uint32_t,int,int,int,uint32_t,uint32_t,uint32_t);
static uint32_t tp_word_address(uint32_t base)
{return base+TP_U(0xeaac28)*0x49d8u+TP_U(0x60598c)*2u;}
static uint32_t tp_dword_address(uint32_t base)
{return base+TP_U(0xeaac28)*0x49d8u+TP_U(0x60598c)*4u;}
static uint32_t tp_x(uint32_t dx,int measured)
{return TP_U(0xf53f0c)+dx+(measured?TP_U(0xe20778):0);}
static uint32_t tp_y(uint32_t dy){return TP_U(0xf53f08)+dy;}
static void tp_label(uint32_t group,uint32_t id,uint32_t dx,uint32_t dy,int measured,uint32_t font)
{FUN_004d62f0(group,id,tp_x(dx,measured),tp_y(dy),font,0,TP_U(0xe38da8),TP_U(0xe38da4));}
static void tp_main_label(uint32_t id,uint32_t dx,uint32_t dy,int measured)
{tp_label(0x36,id,dx,dy,measured,TP_U(0xf56f8c));}
static void tp_wide_label(uint32_t id,uint32_t dx,uint32_t dy,uint32_t width)
{FUN_004d6250(0x36,id,tp_x(dx,0),tp_y(dy),width,TP_U(0xf56f8c),0,TP_U(0xe38da8),TP_U(0xe38da4));}
static void tp_number(uint32_t value,uint32_t dx,uint32_t dy,int measured)
{FUN_004d6860(value,0x40,0x57c048,tp_x(dx,measured),tp_y(dy),TP_U(0xf56f8c),0,TP_U(0xe38da8),TP_U(0xe38da4));}
static void tp_percent(uint32_t value,uint32_t dx,uint32_t dy,int measured)
{FUN_004ccc30(8,0x38,value,tp_x(dx,measured),tp_y(dy),TP_U(0xf56f8c),0);}
static void tp_box(uint32_t dx,uint32_t dy,uint32_t width,uint32_t height,uint32_t color)
{FUN_004d63d0(0,tp_x(dx,0),tp_y(dy),width,height,color,TP_U(0xe38da8),TP_U(0xe38da4));}
static int tp_percent_kind(uint32_t kind)
{return kind==24||kind==25||kind==26||kind==30||kind==35;}
static int tp_scaled_kind(uint32_t kind)
{return kind==10||kind==28||kind==36;}
static void tp_option(uint32_t side,uint32_t label)
{
    uint32_t selected,value;
    if(TP_S(tp_word_address(0xea4096))!=(int32_t)side){tp_wide_label(label,side==1?0x26u:0x14eu,0xdd,200);return;}
    TP_PUT(0xe20778,0);tp_main_label(label,side==1?0x1fu:0x14bu,0xdd,0);
    selected=TP_U(0x60598c);value=(uint32_t)TP_S(tp_word_address(0xea40de));
    if(tp_percent_kind(selected))tp_percent(value,side==1?0x18u:0x144u,0xdd,1);
    else tp_number(tp_scaled_kind(selected)?value:value*100u,side==1?0x1au:0x146u,0xdd,1);
}
void FUN_0055ad30(void)
{
    uint32_t left=0,right=0,first,kind,current_address,available_address,count,id,selected,value,highlight;
    int left_enabled=1,right_enabled=1;
    uint32_t panel_y=TP_U(0xf53f08)+0x80u,panel_x=TP_U(0xf53f0c)+0x10u;
    TP_PUT(0xea60a8u+TP_U(0xeaac28)*0x49d8u,1);
    FUN_00536820(panel_x,panel_y,0x26,0xf);
    (void)FUN_0053e590((int)(TP_U(0xf571fc)+TP_U(0x60598c)),(int)tp_x(0x1a,0),(int)tp_y(0x88));
    tp_label(0x17,TP_U(0x60598c),0x3c,0x89,0,TP_U(0xf56fb4));
    first=TP_U(0x60598c);
    switch(TP_S(0xea4096u+TP_U(0xeaac28)*0x49d8u+first*2u)){
    case 0:
        if(FUN_00443950((int)first)!=0)left=0x27;
        else{left=FUN_004439c0((int)TP_U(0x60598c))!=0?0x22u:0x29u;left_enabled=0;}
        if(FUN_00443b20((int)TP_U(0x60598c))!=0)right=0x28;
        else{right=FUN_00443a20((int)TP_U(0x60598c))!=0?0x23u:0x2au;right_enabled=0;}
        break;
    case 1:
        left=0x13;
        if(FUN_00443b20((int)first)!=0)right=0x28;
        else{right=FUN_00443a20((int)TP_U(0x60598c))!=0?0x23u:0x2au;right_enabled=0;}
        break;
    case 2:
        right=0x14;
        if(FUN_00443950((int)first)!=0)left=0x27;
        else{left=FUN_004439c0((int)TP_U(0x60598c))!=0?0x22u:0x29u;left_enabled=0;}
        break;
    case 3:
        left=0x2b;
        if(FUN_00443b20((int)first)!=0)right=0x28;
        else{right=FUN_00443a20((int)TP_U(0x60598c))!=0?0x23u:0x2au;right_enabled=0;}
        break;
    default:
        right=0x2c;
        if(FUN_00443950((int)first)!=0)left=0x27;
        else{left=FUN_004439c0((int)TP_U(0x60598c))!=0?0x22u:0x29u;left_enabled=0;}
        break;
    }
    kind=TP_U(0x60598c);if(kind==9)kind=1;
    TP_PUT(0xe20778,0);current_address=0x78d6dcu+kind*4u;available_address=0x78d64cu+kind*4u;
    if(FUN_00443b90((int)kind)==0)tp_main_label(0x19,0x62,0xac,0);
    else{
        count=TP_U(current_address);
        if((int32_t)count<=0)tp_main_label(7,0x62,0xac,0);
        else if(TP_S(tp_word_address(0xea4126))==1){tp_number(count,0x62,0xac,0);tp_main_label(TP_U(current_address)==1?10u:11u,0x62,0xac,1);}
        else if(count==TP_U(available_address)){tp_number(count,0x62,0xac,0);tp_main_label(TP_U(current_address)==1?8u:9u,0x62,0xac,1);}
        else{
            tp_number(TP_U(available_address),0x62,0xac,0);
            tp_main_label(12,0x62,0xac,1);
            tp_number(TP_U(current_address)-TP_U(available_address),0x62,0xac,1);
            tp_main_label(TP_U(available_address)==1?13u:14u,0x62,0xac,1);
        }
    }
    selected=TP_U(0x60598c);value=TP_U(0xea6160u+TP_U(0xeaac28)*0x49d8u+selected*4u);TP_PUT(0xe20778,0);
    if(tp_percent_kind(selected))tp_percent((uint32_t)((int32_t)value/100),0x62,0xc0,0);
    else tp_number(tp_scaled_kind(selected)?(uint32_t)((int32_t)value/100):value,0x62,0xc0,0);
    tp_main_label(15,0x62,0xc0,1);
    highlight=TP_U(0x7056c4);
    if(highlight==1){
        if(left_enabled){tp_box(0x1a,0xd4,0x123,0x1e,0xf800);highlight=TP_U(0x7056c4);}
        else{if(right_enabled)tp_box(0x141,0xd4,0x123,0x1e,0);goto after_headers;}
    }else{if(left_enabled)tp_box(0x1a,0xd4,0x123,0x1e,0);highlight=TP_U(0x7056c4);}
    if(right_enabled)tp_box(0x141,0xd4,0x123,0x1e,highlight==2?0xf800u:0u);
after_headers:
    tp_option(1,left);tp_option(2,right);
    if((int32_t)TP_U(current_address)>0){
        tp_box(0x62,0xfa,0x1b0,0x1e,TP_U(0x7056c8)!=0);
        id=TP_S(tp_word_address(0xea4126))==0?0x10u:0x11u;tp_wide_label(id,0x72,0x103,400);
    }
    tp_box(0x62,0x120,0x1b0,0x32,TP_U(0x7056c0)!=0);
    id=TP_U(tp_dword_address(0xea42f8))==0?0x1cu:0x1au;
    tp_wide_label(id,0x72,0x128,400);tp_wide_label(id+1u,0x72,0x13c,400);
}
