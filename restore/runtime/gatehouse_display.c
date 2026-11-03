#include "gatehouse_display.h"
#include "building_controllers.h"
#include "road_place.h"
#include "construction_refresh.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define I(a) ((int32_t)(uint32_t)(a))
int32_t restore_religion_find_god_by_status(int32_t status)
{
 uint32_t i;
 for(i=0;i<5u;++i)if(H(0x7848e2u+i*2u)==status)return (int32_t)i;
 return -1;
}
void restore_gatehouse_redraw(uint32_t building)
{
 uint32_t r=building*264u,x=(uint32_t)H(0x7999deu+r),y=(uint32_t)H(0x7999e0u+r);
 uint32_t group,view,row,part,bias;int32_t god=restore_religion_find_god_by_status(2);
 if(god==-1){god=restore_religion_find_god_by_status(1);if(god==-1)god=0;}
 group=U(0x5df778u+(uint32_t)god*4u);view=B(0x799a82u+r)+U(0xbca2e0u);
 if(I(view)>=8)view-=8u;
 row=0x5df78cu+view*16u;bias=view==0u||view==3u||view==4u||view==7u;
 for(part=0;part<4u;++part){
  uint32_t layout=U(row+part*4u)*16u,px=x,py=y,style,image,width,flags;
  uint32_t orientation=B(0x799a82u+r);
  if(!orientation){px+=U(0x5df81cu+layout);py+=U(0x5df818u+layout);}
  else if(orientation==1u){px+=U(0x5df818u+layout);py+=U(0x5df81cu+layout);}
  style=U(0x5df768u+part*4u);if(style==0x1aeu)style=group;
  image=U(0xf55bb4u+style*8u)+bias;
  flags=B(0x5df810u+layout)?0x8048u:0x8008u;width=U(0x5df814u+layout);
  FUN_0047bf50((uint16_t)building,I(px),I(py),I(width),I(width),image,flags);
 }
 FUN_0047afd0(I(x),I(y),5);
}
void restore_building_control_C_004bfb30(struct restore_building_control_frame *f)
{uint32_t sp=f->r[4];restore_gatehouse_redraw(U(sp+4u));f->r[4]=sp+4u;}
void restore_building_control_C_004c53d0(struct restore_building_control_frame *f)
{uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_religion_find_god_by_status(I(U(sp+4u)));f->r[4]=sp+4u;}
