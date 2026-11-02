#include "road_place.h"
#include "terrain_redraw.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void FUN_004746d0(int32_t x,int32_t y,int32_t width)
{
    FUN_004767d0((int32_t)((uint32_t)x-5u),(int32_t)((uint32_t)y-5u),
                 (int32_t)((uint32_t)x+(uint32_t)width+5u),(int32_t)((uint32_t)y+(uint32_t)width+5u),1,0);
}
void FUN_0047bf50(uint16_t building,int32_t x_in,int32_t y_in,int32_t width_in,int32_t height_in,uint32_t image,uint32_t flags)
{
    uint32_t mode=U(0xe94e04u),x=(uint32_t)x_in,y=(uint32_t)y_in,width=(uint32_t)width_in,height=(uint32_t)height_in;
    uint32_t corner_x=0,corner_y=0,bound,base,row,col,p,cell,value,access,sprite;
    if(mode==0)corner_y=height-1u;
    else if(mode==2){}
    else if(mode==4)corner_x=width-1u;
    else{corner_x=width-1u;corner_y=height-1u;}
    if(x_in<0){U(0xbc9734u)=1;return;}if(y_in<0){U(0xbc9734u)=1;return;}
    bound=U(0xbca2d0u);if((int32_t)(x+width-1u)>=(int32_t)bound){U(0xbc9734u)=1;return;}
    bound=U(0xbca2d4u);if((int32_t)(y+height-1u)>=(int32_t)bound){U(0xbc9734u)=1;return;}
    base=U(0xbca2d8u)+y*228u+x;U(0xbc9768u)=base;
    if(height_in>0)for(row=0;(int32_t)row<height_in;++row)if(width_in>0)for(col=0;(int32_t)col<width_in;++col){
        p=0x5cc8c0u+row*48u+col*8u;cell=base+U(p);value=U(0xaf598cu+cell*4u);U(0xaf598cu+cell*4u)=(value&0x03052e80u)|flags;
        H(0xb4e6fcu+cell*2u)=building;access=B(0xb285ccu+cell)&0xe0u;B(0xb285ccu+cell)=(uint8_t)access;
        if(width>=2u&&width<=6u){access|=width-1u;B(0xb285ccu+cell)=(uint8_t)access;}
        U(0xb7482cu+cell*4u)=image;sprite=B(p+4u);B(0xb67d1cu+cell)=(uint8_t)sprite;
        if(col==corner_x&&row==corner_y)B(0xb67d1cu+cell)=(uint8_t)(sprite|64u);
    }
    FUN_004746d0(x_in,y_in,width_in);
}
