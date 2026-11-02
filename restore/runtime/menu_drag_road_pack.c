#include "road_pack.h"
#include <stdint.h>
#include "road_place.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_0047cae0(int32_t x,int32_t y,int32_t width,int32_t height,uint32_t mask,uint32_t image_plus_one);
extern int32_t FUN_00475fd0(uint32_t cell);
void FUN_00475dc0(void)
{
    uint32_t height=U(0xbca2d4u),row,left,right,cell,count,flags,access,sprite,col,mask,base,variation;
    int32_t variant;
    if((int32_t)height<=0)return;
    for(row=0;(int32_t)row<(int32_t)height;++row){
        left=U(0xbc9f24u+row*4u);right=U(0xbc9b94u+row*4u);cell=U(0xbc9804u+row*4u);
        if((int32_t)left<(int32_t)right){
            count=right-left;
            do{
                flags=U(0xaf598cu+cell*4u);
                if(flags&2u){
                    access=B(0xb285ccu+cell);
                    if(!(access&128u)&&!(flags&0x600u)){
                        U(0xb7482cu+cell*4u)=0;B(0xb285ccu+cell)=(uint8_t)(access&0xf0u);
                        sprite=B(0xb67d1cu+cell);B(0xb67d1cu+cell)=(uint8_t)(sprite|64u);
                    }
                }
                ++cell;--count;
            }while(count);
        }
    }
    row=0;
    do{
        left=U(0xbc9f24u+row*4u);right=U(0xbc9b94u+row*4u);cell=U(0xbc9804u+row*4u);
        if((int32_t)left<(int32_t)right){
            col=left;
            do{
                flags=U(0xaf598cu+cell*4u);
                if((flags&2u)&&!(B(0xb285ccu+cell)&128u)&&!(flags&0x600u)&&!U(0xb7482cu+cell*4u)){
                    if((flags&0x100002u)==0x100002u){mask=0x100002u;base=U(0xf56c8cu);}
                    else if((flags&0x20000002u)==0x20000002u){
                        base=U(0xf572ecu);variant=FUN_00475fd0(cell);
                        if(variant==-1){variation=B(0xacf85cu+cell)&7u;base=U(0xf56b7cu);U(0xb7482cu+cell*4u)=variation+base;}
                        else U(0xb7482cu+cell*4u)=(uint32_t)variant+base;
                        goto next_cell;
                    }else{mask=2u;base=U(0xf56b7cu);}
                    if(FUN_0047cae0((int32_t)col,(int32_t)row,3,3,mask,1)){
                        variation=B(0xacf85cu+cell)&1u;FUN_0047bf50(0,(int32_t)col,(int32_t)row,3,3,base+variation+12u,mask);
                    }else if(FUN_0047cae0((int32_t)col,(int32_t)row,2,2,mask,1)){
                        variation=B(0xacf85cu+cell)&3u;FUN_0047bf50(0,(int32_t)col,(int32_t)row,2,2,base+variation+8u,mask);
                    }else{variation=B(0xacf85cu+cell)&7u;U(0xb7482cu+cell*4u)=base+variation;}
                }
next_cell:      ++col;++cell;
            }while((int32_t)col<(int32_t)right);
            height=U(0xbca2d4u);
        }
        ++row;
    }while((int32_t)row<(int32_t)height);
}
