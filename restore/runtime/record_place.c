#include "record_place.h"
#include "map_record_refresh.h"
#include "menu_dialog.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void FUN_00407620(int32_t record_index,int32_t preserve_primary)
{
    uint32_t left,right,x,y,top,bottom,record,kind,index,count,w,h,alt,sx,sy;
    uint8_t first=B(0xe38e00u),second=B(0xe38e04u),rebuild=0;
    if(!first&&!second&&!B(0xe38e03u))return;
    x=U(0xe38e34u);left=U(0x5d2a18u);
    if((int32_t)x<(int32_t)left)return;
    right=U(0xe38d9cu)-left*2u;
    if((int32_t)x>=(int32_t)right)return;
    y=U(0xe38e30u);top=U(0x5d2a1cu);
    if((int32_t)y<(int32_t)top)return;
    bottom=U(0xe38d98u);alt=U(0x5d2a20u);bottom=bottom-alt-top;
    if((int32_t)y>=(int32_t)bottom)return;
    record=0x77f748u+(uint32_t)record_index*98u;
    if(!B(record+1u)){
        kind=U(0x775920u);index=U(0x775924u);
        B(record)=(uint8_t)kind;W(record+12u)=(uint16_t)index;
        if(kind==1u){
            uint32_t p;count=1;
            for(p=0x77de6au;p<0x77f742u;p+=106u)if(B(p))++count;
            if((int32_t)count>=61){
                U(0x775928u)=4;FUN_004264d0(0x22,0x51f780,1);return;
            }
            rebuild=1;
        }else if(!kind){
            index-=U(0xf56a0cu);count=U(0xf56a14u);
            if((int32_t)index>=(int32_t)count)index=count;
            W(record+14u)=(uint16_t)index;
        }
    }
    U(0x77591cu)=(uint32_t)record_index;B(record+1u)=1;
    if(second)B(record+1u)=2;
    count=U(0x126517cu);index=W(record+12u);
    alt=(int32_t)index<(int32_t)count?index:0;
    w=(uint32_t)(int32_t)(int16_t)W(0x10d6a0cu+alt*64u);
    alt=(int32_t)index<(int32_t)count?index:0;
    h=(uint32_t)(int32_t)(int16_t)W(0x10d6a0eu+alt*64u);
    if(B(record)==2u){w=90;h=20;}
    alt=U(0x784cb4u);
    if(alt){
        x-=(uint32_t)((int32_t)w/2);x-=U(0x5d2a18u);x+=U(0x775910u);
        y-=(uint32_t)((int32_t)h/2);sy=U(0x77590cu);y-=U(0x5d2a1cu);
        W(record+20u)=(uint16_t)x;y+=sy;
        W(record+22u)=(uint16_t)y;
        if(!preserve_primary){W(record+4u)=(uint16_t)x;W(record+6u)=(uint16_t)y;}
    }else{
        x-=(uint32_t)((int32_t)w/2);sx=U(0x5d2a18u);sy=U(0x775910u);x=x-sx+sy;
        y-=(uint32_t)((int32_t)h/2);sx=U(0x5d2a1cu);sy=U(0x77590cu);y-=sx;
        W(record+4u)=(uint16_t)x;y+=sy;W(record+6u)=(uint16_t)y;
    }
    W(record+8u)=(uint16_t)w;W(record+10u)=(uint16_t)h;
    if(rebuild){B(record+24u)=3;FUN_004426c0();FUN_00444720();}
    U(0x5d2a2cu)=0;B(0xe38dcau)=1;
}
