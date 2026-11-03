#include "road_scan_model.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(a) ((int32_t)(uint32_t)(a))
static uint32_t answer(uint32_t value,uint32_t result)
{return (value&0xffffff00u)|result;}

/* Roads use terrain bit0x40, a 228-cell row and four live direction offsets.
 * Test every cell in each ray even after a gap; negative/zero distances still
 * test the immediate neighbour. The logical result occupies AL only. */
uint32_t restore_construction_road_access(uint32_t x,int32_t y,int32_t origin_x,int32_t origin_y,int32_t extent)
{
 uint32_t tile=U(0xbca2d8u)+(uint32_t)y*228u+x,value=x;
 uint32_t distance[4],direction,good=0,nonzero=0,last_gap=0;
 if(!(B(0xaf598cu+tile*4u)&0x40u))return answer(value,0);
 distance[0]=(uint32_t)y-(uint32_t)origin_y;
 distance[1]=(uint32_t)origin_x-x+(uint32_t)extent-1u;
 distance[2]=(uint32_t)origin_y-(uint32_t)y+(uint32_t)extent-1u;
 distance[3]=x-(uint32_t)origin_x;
 for(direction=0;direction<4u;++direction){
  uint32_t step=U(0x5d2914u+direction*4u),road=U(0xaf598cu+(tile+step)*4u)&0x40u,n;
  for(n=1;I(n)<I(distance[direction]);++n)road&=U(0xaf598cu+(tile+step*(n+1u))*4u)&0x40u;
  value=road;
  if(road){++good;if(distance[direction])++nonzero;}else last_gap=direction;
 }
 if(extent==2&&good>=3u&&nonzero>=2u){
  value=distance[last_gap];
  if(!value||good==4u)return answer(value,1);
 }
 if(good==4u&&nonzero>=3u)return answer(value,1);
 if(extent!=5&&good==3u&&nonzero==3u){value=distance[last_gap];if(!value)return answer(value,1);}
 return answer(value,0);
}

uint32_t restore_construction_footprint_mask(int32_t x,int32_t y,int32_t extent)
{
 uint32_t a=(uint32_t)x,b=(uint32_t)y,value,at;
 switch(extent){
 case 2:value=0;at=0x5d2924u+a*2u+b;break;
 case 3:value=a;at=0x5d2928u+a*3u+b;break;
 case 4:value=b;at=0x5d2934u+a*4u+b;break;
 case 5:value=a;at=0x5d2944u+a*5u+b;break;
 default:return answer((uint32_t)extent-2u,1);
 }
 return answer(value,B(at));
}

/* Column-major 5x5 scratch grid: clear in y/x order, then fill in y/x order.
 * Clear all25 entries even for a smaller building. No map-bounds clamp exists
 * in the native leaf; its construction caller supplies the coordinates. */
void restore_construction_occupancy(int32_t x,int32_t y,int32_t kind)
{
 uint32_t size=kind==31?2u:kind==30?3u:kind==33?4u:kind==209?5u:1u;
 uint32_t col,row,endx=(uint32_t)x+size,endy=(uint32_t)y+size,maprow;
 for(row=0;row<5u;++row)for(col=0;col<5u;++col)U(0x775614u+(col*5u+row)*4u)=0;
 if(y>=I(endy))return;
 maprow=U(0xbca2d8u)+(uint32_t)y*228u;
 for(row=0;row<size;++row){
  for(col=0;x<I(endx)&&col<size;++col)
   U(0x775614u+(col*5u+row)*4u)=(B(0xaf598cu+(maprow+(uint32_t)x+col)*4u)&0x40u)?1u:0u;
  maprow+=228u;
 }
}

/* Returns success for a valid locations pointer, including an empty result.
 * The final count is published once, after all coordinate pairs. Reads remain
 * live if an output aliases the scratch grid. Native argument slots1/2 serve
 * as private loop scratch; the typed boundary does not expose those slots. */
uint32_t restore_construction_empty_slots(int32_t extent,int32_t rows,int32_t columns,uint32_t *count,int32_t *locations)
{
 uint32_t nrows=rows>1?(uint32_t)extent-(uint32_t)rows+1u:(uint32_t)extent;
 uint32_t ncols=columns>1?(uint32_t)extent-(uint32_t)columns+1u:(uint32_t)extent;
 uint32_t x,y,dx,dy,found=0,cursor=(uint32_t)(uintptr_t)locations;
 if(!locations)return 0;
 for(y=0;I(y)<I(ncols);++y)for(x=0;I(x)<I(nrows);++x){
  int empty=1;
  for(dy=0;I(dy)<columns&&empty;++dy)for(dx=0;I(dx)<rows;++dx)
   if(U(0x775614u+((x+dx)*5u+y+dy)*4u)){empty=0;break;}
  if(empty){U(cursor)=x;U(cursor+4u)=y;cursor+=8u;++found;}
 }
 U((uint32_t)(uintptr_t)count)=found;
 return answer(I(ncols)>0?nrows:ncols,1);
}
uint32_t FUN_0043f250(uint32_t x,int32_t y,int32_t ox,int32_t oy,int32_t size)
{return restore_construction_road_access(x,y,ox,oy,size);}
uint32_t FUN_0043f3a0(int32_t x,int32_t y,int32_t size)
{return restore_construction_footprint_mask(x,y,size);}
uint32_t FUN_00441040(int32_t size,int32_t rows,int32_t columns,uint32_t *count,int32_t *locations)
{return restore_construction_empty_slots(size,rows,columns,count,locations);}
void FUN_004413b0(int32_t x,int32_t y,int32_t kind)
{restore_construction_occupancy(x,y,kind);}
