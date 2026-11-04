#include "map_surface_repair.h"
#include "neighbors.h"
#include "road_place.h"
#include "water_refresh.h"
#include "paving_neighbors.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(a) ((int32_t)(uint32_t)(a))
extern int32_t FUN_00484be0(void *,int32_t,int32_t,uint8_t);
extern uint32_t FUN_0047e000(void);
/* The shipped51F780 is exactly RET: both pushed mode values are unused. */
extern void FUN_0051f780(void);

void restore_map_refresh_garden_tiles(void)
{
 uint32_t height=U(0xbca2d4u),row=0,left,limit,cell,remaining,flags,x,variation,image;
 uint8_t access,sprite;
 /* The first pass caches height across every row and reads the access BYTE
    before clearing the image DWORD. The second pass has separate row caches. */
 for(row=0;I(row)<I(height);++row){
  left=U(0xbc9f24u+row*4u);limit=U(0xbc9b94u+row*4u);cell=U(0xbc9804u+row*4u);
  if(I(left)>=I(limit))continue;
  remaining=limit-left;
  do{
   flags=U(0xaf598cu+cell*4u);
   if((flags&0x20u)&&!(flags&0x600u)){
    access=B(0xb285ccu+cell);U(0xb7482cu+cell*4u)=0;B(0xb285ccu+cell)=(uint8_t)(access&0xf0u);
    sprite=B(0xb67d1cu+cell);B(0xb67d1cu+cell)=(uint8_t)(sprite|64u);
   }
   ++cell;--remaining;
  }while(remaining);
 }
 row=0;
 while(I(row)<I(height)){
  x=U(0xbc9f24u+row*4u);limit=U(0xbc9b94u+row*4u);cell=U(0xbc9804u+row*4u);
  if(I(x)<I(limit)){
   do{
    flags=U(0xaf598cu+cell*4u);
    if((flags&0x20u)&&!(flags&0x600u)&&!U(0xb7482cu+cell*4u)){
     if(FUN_0047cae0(I(x),I(row),3,3,0x20,1)){
      image=U(0xf565f4u)+7u;FUN_0047bf50(0,I(x),I(row),3,3,image,0x20);
     }else if(FUN_0047cae0(I(x),I(row),2,2,0x20,1)){
      variation=B(0xacf85cu+cell)&3u;image=U(0xf565f4u)+(variation<=1u?6u:variation==2u?5u:4u);
      FUN_0047bf50(0,I(x),I(row),2,2,image,0x20);
     }else{
      image=U(0xf565f4u)+(x&1u)+(row&1u)*2u;U(0xb7482cu+cell*4u)=image;
     }
    }
    ++x;++cell;
   }while(I(x)<I(limit));
   /* Empty rows do not reload the native cached height. */
   height=U(0xbca2d4u);
  }
  ++row;
 }
}

void restore_map_refresh_canal_tiles(void)
{
 uint32_t height=U(0xbca2d4u),cell=U(0xbca2d8u),width,x,y,padding,flags,offset,image;
 uint8_t access,sprite;
 U(0xbc9768u)=cell;U(0xbc9760u)=0;
 if(I(height)<=0)return;
 width=U(0xbca2d0u);
 do{
  U(0xbc9764u)=0;
  if(I(width)>0)do{
   flags=U(0xaf598cu+cell*4u);
   if((flags&0x4000u)&&!(flags&8u)){
    FUN_0047db00(0x4000);FUN_00484f00();(void)FUN_00484be0((void *)(uintptr_t)0x5cb9d8u,48,-1,0);
    cell=U(0xbc9768u);access=B(0xb285ccu+cell);sprite=B(0xb67d1cu+cell);
    B(0xb285ccu+cell)=(uint8_t)(access&0xf0u);offset=U(0xbc9740u);B(0xb67d1cu+cell)=(uint8_t)(sprite|64u);
    image=U(0xf5686cu)+offset;image+=U(0xbc9748u);U(0xb7482cu+cell*4u)=image;
    if(FUN_0047e000()){
     FUN_0047e750();
     if(FUN_00484be0((void *)(uintptr_t)0x5cbcd8u,15,-1,0)){
      offset=U(0xbc9740u);image=U(0xf5686cu);cell=U(0xbc9768u);image+=offset;image+=U(0xbc9748u);U(0xb7482cu+cell*4u)=image;
     }else FUN_0051f780();
    }
    FUN_0051f780();width=U(0xbca2d0u);
   }
   x=U(0xbc9764u);cell=U(0xbc9768u);++x;++cell;U(0xbc9764u)=x;U(0xbc9768u)=cell;
  }while(I(x)<I(width));
  padding=U(0xbca2dcu);y=U(0xbc9760u);cell+=padding;height=U(0xbca2d4u);++y;
  U(0xbc9768u)=cell;U(0xbc9760u)=y;
 }while(I(y)<I(height));
}
