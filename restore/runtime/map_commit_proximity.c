#include "map_commit_proximity.h"
#include "actor_walking.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
static uint32_t delta(int32_t a,int32_t b)
{
 return a>b?(uint32_t)a-(uint32_t)b:(uint32_t)b-(uint32_t)a;
}
uint32_t FUN_00470900(int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
 int32_t left=x1<x2?x1:x2,right=x1>x2?x1:x2,top=y1<y2?y1:y2,bottom=y1>y2?y1:y2,x,y;uint32_t i;
 for(i=0;i<4;++i){
  x=H(0x784b64u+i*2u);if(x<left||x>right)continue;
  y=H(0x784b6cu+i*2u);if(y>=top&&y<=bottom)return 0x00784b01u;
 }
 for(i=0;i<4;++i){
  x=(int32_t)U(0x784cc0u+i*4u);if(x<left||x>right)continue;
  y=(int32_t)U(0x784cd0u+i*4u);if(y>=top&&y<=bottom)return 0x00784c01u;
 }
 return 0x00784c00u;
}
uint32_t FUN_004b5830(int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
 uint32_t p,type,dx,dy;int32_t x,y;
 for(p=0xbcd790u;p<0xc8ad4cu;p+=388u){
  if(B(p-6u)!=1)continue;
  type=B(p-10u);if(!((type>=43u&&type<=57u)||type==99u))continue;
  x=H(p);y=H(p+2u);dx=delta(x,x1);dy=delta(y,y1);
  if((int32_t)dx<=12&&(int32_t)dy<=12)return 1;
  dx=delta(x,x2);dy=delta(y,y2);
  if((int32_t)dx<=12&&(int32_t)dy<=12)return 1;
 }
 return 0;
}
void FUN_004b9ee0(int32_t x,int32_t y)
{
 uint32_t p,dx,dy,distance;int32_t gx,gy;uint8_t direction;
 for(p=0xbcba9au;p<0xbcd62au;p+=144u){
  if(B(p-66u)!=1)continue;
  if(B(p))continue;
  if(!B(p+39u))continue;
  if(!B(p-26u))continue;
  gx=H(p-20u);gy=H(p-18u);dx=delta(x,gx);dy=delta(y,gy);
  U(0xd3b4fcu)=dx;U(0xd3b4f8u)=dy;
  distance=(int32_t)dx>=(int32_t)dy?dx:dy;if((int32_t)distance>6)continue;
  *(volatile uint16_t *)(uintptr_t)(p+12u)=50;
  gy=H(p-18u);gx=H(p-20u);
  /* The accepted direction provider materializes0..8; this native consumer
   * stores AL only, so its unrelated original upper-EAX residue is unused. */
  direction=(uint8_t)FUN_0051cba0(x,y,gx,gy);B(p+52u)=direction;
 }
}
