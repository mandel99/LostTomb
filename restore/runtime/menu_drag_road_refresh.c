#include "road_refresh.h"
#include "city_prepare.h"
#include "map_neighbors_phase.h"
#include "map_refresh_postpass2.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void FUN_00484f20(void)
{
    uint32_t p,v;for(p=0xbc97c0u;p<0xbc97e0u;p+=4u){v=U(p);U(p)=v!=0;}
}
void FUN_004798e0(int32_t x_in,int32_t y_in,int32_t mode)
{
    uint32_t dy=U(0xbca2ecu),dx=U(0xbca2e8u),x=(uint32_t)x_in,y=(uint32_t)y_in,offset,cell,flags,access,sprite;
    uint32_t left,top,columns=3,rows=3,width,height,endrow,endcol,col,row,frame,image,variant;
    if(!(uint8_t)FUN_0046f930((int32_t)(dx+x),(int32_t)(dy+y)))return;
    offset=U(0xbca2d8u);cell=offset+y*228u+x;flags=U(0xaf598cu+cell*4u);access=B(0xb285ccu+cell);
    if(mode==1){U(0xaf598cu+cell*4u)=flags|2u;access|=128u;}
    else{U(0xaf598cu+cell*4u)=flags&0xdfcffffdu;access&=127u;}
    B(0xb285ccu+cell)=(uint8_t)access;
    left=x-1u;top=y-1u;
    if((int32_t)left<0){columns=left+3u;left=0;}
    else{width=U(0xbca2d0u);if((int32_t)(left+3u)>(int32_t)width)columns=width-left;}
    if((int32_t)top<0){rows=top+3u;top=0;}
    else{height=U(0xbca2d4u);if((int32_t)(top+3u)>(int32_t)height)rows=height-top;}
    endrow=top+rows;U(0xbc9760u)=top;cell=offset+top*228u+left;U(0xbc9768u)=cell;
    if((int32_t)top>=(int32_t)endrow)return;
    endcol=left+columns;
    do{
        col=left;U(0xbc9764u)=col;
        if((int32_t)col<(int32_t)endcol)do{
            if((B(0xaf598cu+cell*4u)&2u)&&(B(0xb285ccu+cell)&128u)){
                FUN_0047dee0();FUN_00484f20();
                if(FUN_00484be0((void *)(uintptr_t)0x5cbea8u,17,-1,0)){
                    frame=U(0xbc9740u);image=U(0xf56b6cu);variant=U(0xbc9748u);image+=frame+variant;cell=U(0xbc9768u);
                }else{cell=U(0xbc9768u);image=U(0xf56b6cu);}
                U(0xb7482cu+cell*4u)=image;access=B(0xb285ccu+cell);sprite=B(0xb67d1cu+cell);
                B(0xb285ccu+cell)=(uint8_t)(access&0xf0u);B(0xb67d1cu+cell)=(uint8_t)(sprite|64u);
            }
            col=U(0xbc9764u)+1u;++cell;U(0xbc9764u)=col;U(0xbc9768u)=cell;
        }while((int32_t)col<(int32_t)endcol);
        row=U(0xbc9760u)+1u;U(0xbc9760u)=row;cell+=228u-columns;row=U(0xbc9760u);U(0xbc9768u)=cell;
    }while((int32_t)row<(int32_t)endrow);
}
void FUN_004770f0(int32_t left_in,int32_t top_in,int32_t right_in,int32_t bottom_in)
{
    uint32_t left=(uint32_t)left_in,top=(uint32_t)top_in,right=(uint32_t)right_in,bottom=(uint32_t)bottom_in,width,height,cell,stride,row,col;
    if(left_in<0)left=0;if(top_in<0)top=0;
    width=U(0xbca2d0u);if((int32_t)right>=(int32_t)width)right=width-1u;
    height=U(0xbca2d4u);if((int32_t)bottom>=(int32_t)height)bottom=height-1u;
    cell=U(0xbca2d8u);stride=width-right+left-1u;cell+=top*228u+left;row=top;
    if((int32_t)row>(int32_t)bottom)return;
    do{
        if((int32_t)left<=(int32_t)right){
            col=left;
            do{if((B(0xaf598cu+cell*4u)&2u)&&(B(0xb285ccu+cell)&128u))FUN_004798e0((int32_t)col,(int32_t)row,1);++cell;++col;}while((int32_t)col<=(int32_t)right);
        }
        cell+=U(0xbca2dcu)+stride;++row;
    }while((int32_t)row<=(int32_t)bottom);
}
