#include "map_refresh_postpass7.h"
#include "map_refresh_postpass8.h"
#ifdef MAP_REFRESH_POSTPASS9_TRACE
uint32_t map_post9_read(uint32_t);
void map_post9_child(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
#define P9_R(a) map_post9_read((uint32_t)(a))
#define P9_CHILD(a,p,n,x0,y0,x1,y1) map_post9_child((a),(p),(n),(uint32_t)(x0),(uint32_t)(y0),(uint32_t)(x1),(uint32_t)(y1))
#else
#define P9_R(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define P9_CHILD(a,p,n,x0,y0,x1,y1) ((void)0)
#endif
static __inline void p9_expand(int32_t x0,int32_t y0,int32_t x1,int32_t y1)
{
 P9_CHILD(0x4877f0,0,4,x0,y0,x1,y1);FUN_004877f0(x0,y0,x1,y1);P9_CHILD(0x4877f0,1,4,x0,y0,x1,y1);
}
static __inline void p9_cell(int32_t x,int32_t y)
{
 P9_CHILD(0x47a220,0,3,x,y,1,0);FUN_0047a220(x,y,1);P9_CHILD(0x47a220,1,3,x,y,1,0);
}

/* Whole00475cc0. The native fifth DWORD slot is later reused as a local.
   Only its incoming lowBYTE gates the prepass; all meaningful globals are read
   in native order. Physical scan indexing is independent of child x/y indexing. */
void FUN_00475cc0(int32_t xmin,int32_t ymin,int32_t xmax,int32_t ymax,uint32_t flags)
{
 uint32_t width,height,rowstep,tile,scan,terrain;int32_t row,x;
 if((uint8_t)flags)p9_expand((int32_t)((uint32_t)xmin-5u),(int32_t)((uint32_t)ymin-5u),(int32_t)((uint32_t)xmax+5u),(int32_t)((uint32_t)ymax+5u));
 if(xmin<0)xmin=0;if(ymin<0)ymin=0;
 width=P9_R(0xbca2d0u);if(xmax>=(int32_t)width)xmax=(int32_t)(width-1u);
 height=P9_R(0xbca2d4u);if(ymax>=(int32_t)height)ymax=(int32_t)(height-1u);
 rowstep=width-(uint32_t)xmax+(uint32_t)xmin-1u;
 tile=P9_R(0xbca2d8u)+(uint32_t)ymin*228u+(uint32_t)xmin;
 row=ymin;
 if(row<=ymax){do{
  if(xmin<=xmax){
   scan=tile;tile+=(uint32_t)xmax-(uint32_t)xmin+1u;x=xmin;
   do{
    terrain=P9_R(0xaf598cu+scan*4u);
    if((terrain&4u)&&!(terrain&8u))p9_cell(x,row);
    x=(int32_t)((uint32_t)x+1u);++scan;
   }while(x<=xmax);
  }
  tile+=P9_R(0xbca2dcu)+rowstep;row=(int32_t)((uint32_t)row+1u);
 }while(row<=ymax);}
}
