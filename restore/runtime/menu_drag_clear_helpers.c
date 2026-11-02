#include "clear_helpers.h"
#include "city_prepare.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
uint32_t FUN_0047c7b0(int32_t x,int32_t y)
{
    uint32_t cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x,access=B(0xb285ccu+cell)&15u,size,i;
    size=access>=1u&&access<=5u?access+1u:1u;U(0xbc973cu)=size;U(0xbc9738u)=size;
    for(i=0;i<size;++i){if(!(B(0xb67d1cu+cell)&7u))break;--cell;}
    for(i=0;i<size;++i){if(!(B(0xb67d1cu+cell)&56u))break;cell-=228u;}
    return cell;
}
uint32_t FUN_00541ab0(uint32_t building)
{
    uint32_t kind=(uint32_t)(int32_t)(int16_t)W(0x7999eau+building*264u);
    return U(0x601de0u+kind*52u);
}
void FUN_0047a120(int32_t x_in,int32_t y_in)
{
    uint32_t dy=U(0xbca2ecu),dx=U(0xbca2e8u),x=(uint32_t)x_in,y=(uint32_t)y_in,cell,base,access,variation,flags,sprite;
    if(!(uint8_t)FUN_0046f930((int32_t)(dx+x),(int32_t)(dy+y)))return;
    cell=U(0xbca2d8u)+y*228u+x;base=U(0xf56c4cu);access=B(0xb285ccu+cell);
    U(0xbc9768u)=cell;variation=B(0xacf85cu+cell);B(0xb285ccu+cell)=(uint8_t)(access&0xf0u);
    flags=U(0xaf598cu+cell*4u);B(0xae8e7cu+cell)=0;U(0xb7482cu+cell*4u)=base+(variation&7u);
    sprite=B(0xb67d1cu+cell);B(0xb67d1cu+cell)=(uint8_t)(sprite|64u);U(0xaf598cu+cell*4u)=flags|0x1000u;
}
void FUN_00477890(int32_t left_in,int32_t top_in,int32_t right_in,int32_t bottom_in)
{
    uint32_t left=(uint32_t)left_in,top=(uint32_t)top_in,right=(uint32_t)right_in,bottom=(uint32_t)bottom_in,width,height,cell,step,row,col,flags;
    if(left_in<0)left=0;if(top_in<0)top=0;
    width=U(0xbca2d0u);if((int32_t)right>=(int32_t)width)right=width-1u;
    height=U(0xbca2d4u);if((int32_t)bottom>=(int32_t)height)bottom=height-1u;
    cell=U(0xbca2d8u)+top*228u+left;step=width-right+left-1u;
    if((int32_t)top>(int32_t)bottom)return;
    row=top;
    do{
        if((int32_t)left<=(int32_t)right){
            col=left;
            do{
                flags=U(0xaf598cu+cell*4u);
                if((flags&0x1000u)&&!(flags&0x700u)&&!(flags&0x68u))FUN_0047a120((int32_t)col,(int32_t)row);
                ++col;++cell;
            }while((int32_t)col<=(int32_t)right);
        }
        cell+=U(0xbca2dcu)+step;++row;
    }while((int32_t)row<=(int32_t)bottom);
}
