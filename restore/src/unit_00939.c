#include "city_maintenance.h"
#include "byte_fill.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))

/* Whole468860. Ranking uses signed dword region sizes and first-strict-greater
   insertion. The width stays cached until a flood child; height is reloaded at
   each row end, and the child's published tile becomes the next scan origin. */
void restore_city_rebuild_road_regions(void)
{
 uint32_t off=U(0xeaac28u)*0x49d8u,i,j,region=1u,row=0u,col,tile,width,height,size;
 for(i=0;i<10u;++i){U(0xea5f1cu+off+i*8u)=0;U(0xea5f20u+off+i*8u)=0;}
 FUN_0040f8f0((uint32_t *)(uintptr_t)0xa76aecu,0xcb10u,0);
 FUN_0040f8f0((uint32_t *)(uintptr_t)0xa69fdcu,0xcb10u,0);
 height=U(0xbca2d4u);tile=U(0xbca2d8u);
 U(0x78c408u)=1;U(0x78c404u)=0;U(0xbc9768u)=tile;
 if(I(height)<=0)return;
 width=U(0xbca2d0u);
 do{
  col=0;
  if(I(width)>0)do{
   if((U(0xaf598cu+tile*4u)&0x44u)==0x40u&&B(0xa76aecu+tile)==0){
    U(0x78c400u)=1;restore_city_color_road_region(tile,region);
    off=U(0xeaac28u)*0x49d8u;size=U(0x78c400u);
    for(i=0;i<10u;++i)if(I(size)>I(U(0xea5f20u+off+i*8u)))break;
    if(i<10u){
     for(j=9u;j>i;--j){
      U(0xea5f1cu+off+j*8u)=U(0xea5f1cu+off+(j-1u)*8u);
      U(0xea5f20u+off+j*8u)=U(0xea5f20u+off+(j-1u)*8u);
     }
     U(0xea5f1cu+off+i*8u)=region;U(0xea5f20u+off+i*8u)=size;
    }
    tile=U(0xbc9768u);width=U(0xbca2d0u);++region;
   }
   ++col;++tile;U(0xbc9768u)=tile;
  }while(I(col)<I(width));
  {uint32_t gap=U(0xbca2dcu);height=U(0xbca2d4u);++row;tile+=gap;U(0xbc9768u)=tile;}
 }while(I(row)<I(height));
}
