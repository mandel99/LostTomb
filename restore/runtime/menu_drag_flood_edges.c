#include "flood_edges.h"
#include "refresh_dispatch.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_00477eb0(uint32_t cell,uint32_t mask,uint8_t diagonal);
extern int32_t FUN_004781a0(uint32_t cell,uint32_t mask);
uint32_t FUN_00477f70(int32_t x_in,int32_t y_in)
{
    uint32_t x=(uint32_t)x_in,y=(uint32_t)y_in,left=x-1u,right,width,top=y-1u,bottom,step,cell,row,col,flags,changed=0;
    width=U(0xbca2d0u);right=width;if((int32_t)left<0)left=0;if((int32_t)right>=(int32_t)(x+1u))right=x+1u;
    bottom=U(0xbca2d4u);if((int32_t)top<0)top=0;if((int32_t)bottom>=(int32_t)(y+1u))bottom=y+1u;
    step=U(0xbca2dcu)-right+width+left-1u;cell=U(0xbca2d8u)+top*228u+left;
    if((int32_t)top<=(int32_t)bottom)for(row=top;;++row){
        if((int32_t)left<=(int32_t)right){
            col=left;
            do{
                flags=U(0xaf598cu+cell*4u);
                if(!(flags&0x10004u)&&!(uint8_t)FUN_00477eb0(cell,0x10004u,1)){
                    flags=U(0xaf598cu+cell*4u);U(0xaf598cu+cell*4u)=(flags&0xd9cbf680u)|0x10000u;changed=1;
                    FUN_00477dd0((int32_t)col,(int32_t)row,2);FUN_00477f70((int32_t)col,(int32_t)row);
                }
                ++col;++cell;
            }while((int32_t)col<=(int32_t)right);
        }
        cell+=step;if((int32_t)(row+1u)>(int32_t)bottom)break;
    }
    return (bottom&0xffffff00u)|changed;
}
uint32_t FUN_00478090(int32_t x_in,int32_t y_in)
{
    uint32_t x=(uint32_t)x_in,y=(uint32_t)y_in,left=x-1u,right,width,top=y-1u,bottom,step,cell,row,col,flags,changed=0;
    width=U(0xbca2d0u);right=width;if((int32_t)left<0)left=0;if((int32_t)right>=(int32_t)(x+1u))right=x+1u;
    bottom=U(0xbca2d4u);if((int32_t)top<0)top=0;if((int32_t)bottom>=(int32_t)(y+1u))bottom=y+1u;
    step=U(0xbca2dcu)-right+width+left-1u;cell=U(0xbca2d8u)+top*228u+left;
    if((int32_t)top>(int32_t)bottom)return top&0xffffff00u;
    for(row=top;;++row){
        if((int32_t)left<=(int32_t)right){
            col=left;
            do{
                flags=U(0xaf598cu+cell*4u);
                if(flags&0x10000u){
                    U(0xaf598cu+cell*4u)=flags&0xfffeffffu;changed=1;FUN_00477dd0((int32_t)col,(int32_t)row,1);
                    flags=U(0xaf598cu+cell*4u);if(flags&0x22341a33u)FUN_00478090((int32_t)col,(int32_t)row);
                }
                ++col;++cell;
            }while((int32_t)col<=(int32_t)right);
        }
        cell+=step;if((int32_t)(row+1u)>(int32_t)bottom)break;
    }
    return (bottom&0xffffff00u)|changed;
}
uint32_t FUN_00478270(int32_t x,int32_t y)
{
    uint32_t offset=U(0xbca2d8u),cell=offset+(uint32_t)y*228u+(uint32_t)x,next,flags;int32_t direction,relative,nx,ny;
    if(U(0xaf598cu+cell*4u)&0x10000u)return offset&0xffffff00u;
    direction=FUN_004781a0(cell,0x10000u);if(!direction)return 0;
    do{
        next=cell+(uint32_t)direction;offset=U(0xbca2d8u);flags=U(0xaf598cu+next*4u);U(0xaf598cu+next*4u)=(flags&0xfffeffffu)|128u;
        relative=(int32_t)(next-offset);nx=relative%228;ny=relative/228;
        FUN_00478270(nx,ny);FUN_00477dd0(nx,ny,1);direction=FUN_004781a0(cell,0x10000u);
    }while(direction);
    return 1;
}
