#include "sprite_render.h"
#include "road_check_model.h"
#include "road_scan_model.h"
#include "world_offset_model.h"
#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
extern void FUN_004f0ba0(int32_t,int32_t,int32_t *,int32_t *);
extern uint32_t FUN_0053f250(int32_t,int32_t,int32_t);
extern int32_t FUN_0053f4f0(int32_t,int32_t,int32_t,uint32_t);

/* Whole43F5B0: venue preview. Images and placement state are DWORDs;
   the global tint is a WORD. Index each orientation table in DWORDs once. */
void FUN_0043f5b0(int32_t kind)
{
    uint32_t ox=U(0xbca304u),oy=U(0xbca308u),size=1,row,col,x,y,image,base,table,offset,orientation;
    int32_t bx=I(ox),by=I(oy),sx=I(ox),sy=I(oy),px=I(ox),py=I(oy),screen_x=0,screen_y=0,first=0,second=0;
    int32_t found=0,band=0;
    int failed=FUN_0043f410(kind)==0,pavilion=kind==33,booth=kind==31||kind==30||kind==33,bandstand=kind==30||kind==33;
    if(kind==31)size=2;else if(kind==30)size=3;else if(kind==33)size=4;else if(kind==209)size=5;
    W(0x70e120u)=(uint16_t)(failed?0xf863u:0x1fe3u);U(0x605994u)=!failed;
    FUN_004413b0(I(ox),I(oy),kind);
    if(pavilion){found=(int32_t)FUN_00441130(I(ox),I(oy),&px,&py,kind,33);if(found==-1)failed=1;}
    if(bandstand){band=(int32_t)FUN_00441130(I(ox),I(oy),&sx,&sy,kind,30);if(band==-1)failed=1;}
    if(booth)found=(int32_t)FUN_00441130(I(ox),I(oy),&bx,&by,kind,31);
    if(found==-1)failed=1;
    for(row=0;row<size;++row){
        y=oy+row;
        for(col=0;col<size;++col){
            x=ox+col;FUN_004f0ba0(I(x),I(y),&screen_x,&screen_y);image=U(0xf567acu);
            if(failed){(void)FUN_0053f4f0(I(U(0xf56be4u)),screen_x,screen_y,0xfffff863u);continue;}
            if(booth&&I(x)==bx&&I(y)==by){
                (void)FUN_0053e590(I(U(0xf567acu)),screen_x,screen_y);
                (void)FUN_0053f250(I(image),screen_x,screen_y);continue;
            }
            if(bandstand){
                int part=-1;
                if(I(x)==sx&&I(y)==sy)part=0;
                else if(band&&I(x)==I((uint32_t)sx+1u)&&I(y)==sy)part=1;
                else if(!band&&I(x)==sx&&I(y)==I((uint32_t)sy+1u))part=1;
                if(part>=0){
                    FUN_004405c0(band,&first,&second);image=(uint32_t)(part?second:first);
                    (void)FUN_0053e590(I(image),screen_x,screen_y);
                    (void)FUN_0053f250(I(image),screen_x,screen_y);continue;
                }
            }
            if(pavilion){
                if(I(x)==px&&I(y)==py){
                    orientation=U(0xbca2e0u);
                    if(orientation==2u)FUN_004f0ba0(I(x+1u),I(y),&screen_x,&screen_y);
                    else if(orientation==4u)FUN_004f0ba0(I(x+1u),I(y+1u),&screen_x,&screen_y);
                    else if(orientation==6u)FUN_004f0ba0(I(x),I(y+1u),&screen_x,&screen_y);
                    image=U(0xf5659cu);(void)FUN_0053e590(I(U(0xf5659cu)),screen_x,screen_y);
                    (void)FUN_0053f250(I(image),screen_x,screen_y);continue;
                }
                if(px<=I(x)&&py<=I(y)&&I(x)<I((uint32_t)px+2u)&&I(y)<I((uint32_t)py+2u))continue;
            }
            orientation=U(0xbca2e0u);table=0;base=0;offset=0;
            if(kind==31){base=U(0xf5679cu);table=orientation==0u?0x5d2078u:orientation==2u?0x5d2088u:orientation==4u?0x5d2098u:orientation==6u?0x5d20a8u:0;}
            else if(kind==30){base=U(0xf565ecu);table=orientation==0u?0x5d20b8u:orientation==2u?0x5d20dcu:orientation==4u?0x5d2100u:orientation==6u?0x5d2124u:0;}
            else if(kind==33){base=U(0xf565acu);table=orientation==0u?0x5d2148u:orientation==2u?0x5d2188u:orientation==4u?0x5d21c8u:orientation==6u?0x5d2208u:0;}
            else if(kind==209){base=U(0xf565a4u);table=orientation==0u?0x5d2248u:orientation==2u?0x5d22acu:orientation==4u?0x5d2310u:orientation==6u?0x5d2374u:0;}
            else base=U(0xf56774u);
            if(table)offset=U(table+(row*size+col)*4u);
            (void)FUN_0053e590(I(base+offset),screen_x,screen_y);
        }
    }
    W(0x70e120u)=0;U(0x605994u)=!failed;
}
