#include "refresh_tile.h"
#include "city_prepare.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void FUN_00479ac0(int32_t x,int32_t y,int32_t mode)
{
    uint32_t dy=U(0xbca2ecu),dx=U(0xbca2e8u),cell,flags,v,base,sprite,access;
    if(!(uint8_t)FUN_0046f930((int32_t)(dx+(uint32_t)x),(int32_t)(dy+(uint32_t)y)))return;
    cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;U(0xbc9768u)=cell;flags=U(0xaf598cu+cell*4u);
    if(mode!=1){U(0xaf598cu+cell*4u)=flags&0x2000000u;return;}
    v=B(0xacf85cu+cell);base=U(0xf56ba4u);U(0xaf598cu+cell*4u)=flags|0x2000000u;
    sprite=B(0xb67d1cu+cell);U(0xb7482cu+cell*4u)=base+(v&7u);
    access=B(0xb285ccu+cell);B(0xb285ccu+cell)=(uint8_t)(access&0xf0u);B(0xb67d1cu+cell)=(uint8_t)(sprite|64u);
}
void FUN_00477430(int32_t left_in,int32_t top_in,int32_t right_in,int32_t bottom_in)
{
    uint32_t left=(uint32_t)left_in,top=(uint32_t)top_in,right=(uint32_t)right_in,bottom=(uint32_t)bottom_in,width,height,cell,stride,row,col,next,flags;
    if(left_in<0)left=0;if(top_in<0)top=0;
    width=U(0xbca2d0u);if((int32_t)right>=(int32_t)width)right=width-1u;
    height=U(0xbca2d4u);if((int32_t)bottom>=(int32_t)height)bottom=height-1u;
    cell=U(0xbca2d8u);stride=width-right+left-1u;cell+=top*228u+left;
    row=top;
    if((int32_t)row>(int32_t)bottom)return;
    do{
        if((int32_t)left<=(int32_t)right){
            next=cell+right-left+1u;col=left;
            do{
                flags=U(0xaf598cu+cell*4u);
                if((flags&0x2000000u)&&!(flags&0x600u))FUN_00479ac0((int32_t)col,(int32_t)row,1);
                ++cell;++col;
            }while((int32_t)col<=(int32_t)right);
            cell=next;
        }
        cell+=U(0xbca2dcu)+stride;++row;
    }while((int32_t)row<=(int32_t)bottom);
}
