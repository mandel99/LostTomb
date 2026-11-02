#include "minimap.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern void FUN_004cd680(int32_t,int32_t,int32_t,int32_t,uint32_t);
static int32_t add(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t sub(int32_t a,int32_t b){return (int32_t)((uint32_t)a-(uint32_t)b);}
static int32_t terrain_image(uint32_t flags,uint8_t variation,uint8_t extra)
{
    if((flags&0x40000044u)&&extra)return I(0xf568b4);
    if(flags&0x10000000u)return add(I(0xf56b14),55);
    if(flags&0x20u)return add(I(0xf56b14),50);
    if(flags&4u)return add(I(0xf5688c),variation&3);
    if(flags&0x11u)return add(I(0xf56894),variation&3);
    if(flags&2u)return add(I(0xf568a4),variation&3);
    if(flags&0x2000000u)return add(I(0xf56ab4),variation&3);
    if(flags&0x200u)return add(I(0xf568a4),variation&3);
    if(flags&0x40000040u)return I(0xf568b4);
    if(flags&0x100u)return I(0xf568d4);
    if(flags&0x4000u)return add(I(0xf56b14),60);
    if(flags&0x10800u)return add(I(0xf568ac),variation&3);
    if(flags&0x40000u)return add(I(0xf56894),variation&4);
    return add(I(0xf56884),variation&7);
}
void FUN_0043e810(int32_t x,int32_t y,int32_t width,int32_t height)
{
    uint32_t background,highlight,phase=0;
    int32_t row,column,screen_x,screen_y,image,rect_x,rect_y,rect_w,rect_h;
    uint8_t pixel_mode=B(0x784cb0);
    if(I(0xe92dfc)==2) {
        background=0xf000f000u;highlight=pixel_mode==1?0x7800u:pixel_mode==0?0x181fu:0x80fu;
    } else {
        background=0x78007800u;highlight=pixel_mode==0?0x3c00u:pixel_mode==1?0xc1fu:0x41fu;
    }
    highlight*=0x10001u;
    I(0xd3b55c)=x;I(0xd3b554)=y;I(0xd3b558)=add(x,(int32_t)((uint32_t)width*2u));I(0xd3b550)=add(y,height);
    FUN_0043e670(width,height);
    I(0x70e3c8)=add(y,height);I(0x70e3cc)=I(0xd3b558);I(0x70e3d4)=x;I(0x70e3d0)=y;
    I(0x70e47c)=sub(I(0x70e3d8),4);screen_y=sub(y,4);
    for(row=-4;row<add(height,4);row=add(row,1),screen_y=add(screen_y,1),U(0x70e47c)+=1u) {
        screen_x=sub(x,phase?9:8);phase^=1u;
        I(0x70e480)=sub(I(0x70e3dc),4);
        for(column=-4;column<width;column=add(column,1),screen_x=add(screen_x,2),U(0x70e480)+=1u) {
            uint32_t tile,flags,pair;int32_t building,actor,first,second,third;
            int outside=row<0||row>=height||column<0||column>=width;
            int32_t lookup_x=I(0x70e480),lookup_y=I(0x70e47c);
            if(lookup_x<0||lookup_x>=231||lookup_y<0||lookup_y>=457)continue;
            tile=U(0x70e494u+((uint32_t)lookup_y*231u+(uint32_t)lookup_x)*4u);U(0x70e488)=tile;
            if((int32_t)tile>=0x0fff0000)goto black;
            flags=U(0xaf598cu+tile*4u);
            if(flags&0x80000u)goto black;
            if(!B(0x775680) && (lookup_x<I(0xe94dd0)||lookup_y<I(0xe94dc8)||lookup_x>I(0xe94dcc)||lookup_y>(int32_t)((U(0xe94dc4)-1u)&0xfffeu)))goto black;
            building=H(0xb4e6fcu+tile*2u);actor=H(0xb350dcu+tile*2u);
            first=FUN_0043ee10(actor);second=FUN_0043eeb0(actor);
            pair=background;
            if(first==2)pair=highlight;
            else if(first==3)pair=0;
            else if(!first&&second==2)pair=highlight;
            third=FUN_0043e790(actor);
            if(third||second||first) {
                I(0xd3b5e0)=screen_x;I(0xd3b5dc)=screen_y;
                if(!outside)FUN_0040fc10(pair);
                continue;
            }
            if((flags&8u)&&building) {
                if(!(B(0xb67d1cu+U(0x70e488))&0x40u)||outside)continue;
                if(FUN_00439980(building,screen_x,screen_y))continue;
                image=add(I(0xf56b14),flags&0x10000000u?55:60);
            } else if((flags&0xc04000u)&&I(0xe94e0c)==49) {
                image=add(I(0xf568c4),5);
            } else {
                if(outside)continue;
                tile=U(0x70e488);
                image=terrain_image(flags,B(0xacf85cu+tile),B(0xadc36cu+tile));
            }
            FUN_0053f4a0(image,screen_x,screen_y);continue;
black:
            if(!outside)FUN_0053f4a0(I(0xf568dc),screen_x,screen_y);
        }
    }
    rect_x=add(x,(int32_t)((uint32_t)sub(I(0x70e478),I(0x70e3dc))*2u));
    rect_y=add(sub(I(0x70e474),I(0x70e3d8)),y);
    rect_w=(int32_t)(U(0x70e464)*2u);rect_h=I(0x70e460);
    if(I(0x6fdc50)&&I(0x605a3c)==1){rect_w=sub(rect_w,4);rect_h=sub(rect_h,6);}
    if(rect_x<x)rect_x=x;
    FUN_004cd680(rect_x,rect_y,rect_w,rect_h,0xe70bu);
    U(0xd3b558)=U(0xe38db0);U(0xd3b55c)=0;U(0xd3b554)=0;U(0xd3b550)=U(0xe38dac);
}
