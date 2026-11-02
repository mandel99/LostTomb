#include "growth_update.h"
#include "growth.h"
#include "map_cells_phase.h"
#include "map_refresh_postpass2.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void FUN_00487450(int32_t left_in,int32_t top_in,int32_t right_in,int32_t bottom_in)
{
    uint32_t left=(uint32_t)left_in,top=(uint32_t)top_in,right=(uint32_t)right_in,bottom=(uint32_t)bottom_in;
    uint32_t width,height,saved,step,padding,climate,limit,offset,cell,row,col,flags,denominator,arg,orientation,variant;
    int32_t water,high,a,b,v,threshold;
    if(left_in<0)left=0;if(top_in<0)top=0;
    width=U(0xbca2d0u);if((int32_t)right>=(int32_t)width)right=width-1u;
    height=U(0xbca2d4u);if((int32_t)bottom>=(int32_t)height)bottom=height-1u;
    saved=U(0xbc9768u);padding=U(0xbca2dcu);step=padding-right+width+left-1u;
    climate=B(0x784cb0u);limit=climate==0?46u:climate==2?8u:16u;
    offset=U(0xbca2d8u);cell=offset+top*228u+left;
    if((int32_t)top<=(int32_t)bottom)for(row=top;;++row){
        if((int32_t)left<=(int32_t)right){
            col=left;
            do{
                flags=U(0xaf598cu+cell*4u);
                if(flags&0x50004u){B(0xa509bcu+cell)=0;U(0xaf598cu+cell*4u)=flags&0xffffff7fu;}
                else{
                    water=FUN_00486d30((int32_t)col,(int32_t)row,4,(int32_t)limit);
                    high=FUN_00486d30((int32_t)col,(int32_t)row,0x50000u,8);
                    if(water<0&&high<0){flags=U(0xaf598cu+cell*4u);U(0xaf598cu+cell*4u)=flags&0xffffff7fu;B(0xa509bcu+cell)=0;}
                    else{
                        a=0;
                        if(water>=0){
                            threshold=(int32_t)(limit-12u);v=(int32_t)((uint32_t)water-(uint32_t)threshold+1u);denominator=limit;
                            if(water<threshold)water=0;else if(v>=0){water=v;denominator=12;}
                            arg=(uint32_t)water-1u;if((int32_t)arg<=0)arg=0;
                            a=FUN_00486ec0((int32_t)arg,(int32_t)denominator);
                        }
                        if(high<0)b=0;
                        else{arg=(uint32_t)high-1u;if((int32_t)arg<=0)arg=0;b=FUN_00486ec0((int32_t)arg,8);}
                        if(a>b)b=a;if(b<1)b=1;
                        flags=U(0xaf598cu+cell*4u);U(0xaf598cu+cell*4u)=flags|128u;B(0xa509bcu+cell)=(uint8_t)b;
                    }
                }
                ++col;++cell;
            }while((int32_t)col<=(int32_t)right);
        }
        cell+=step;if((int32_t)(row+1u)>(int32_t)bottom)break;
    }
    offset=U(0xbca2d8u);cell=offset+top*228u+left;
    if((int32_t)top<=(int32_t)bottom)for(row=top;;++row){
        if((int32_t)left<=(int32_t)right){
            col=left;
            do{
                if((B(0xaf598cu+cell*4u)&128u)&&FUN_00486f20(cell)){
                    flags=U(0xaf598cu+cell*4u);orientation=U(0xbca2e0u);
                    U(0xaf598cu+cell*4u)=flags&0xfffff7ffu;U(0xbca2e0u)=0;
                    FUN_00484be0((void *)(uintptr_t)0x5cbfb8u,26,-1,0);variant=U(0xbc9748u);
                    B(0xa509bcu+cell)=variant==14u?100u:(uint8_t)(variant|128u);U(0xbca2e0u)=orientation;
                }
                ++col;++cell;
            }while((int32_t)col<=(int32_t)right);
        }
        cell+=step;if((int32_t)(row+1u)>(int32_t)bottom)break;
    }
    U(0xbc9768u)=saved;
}
