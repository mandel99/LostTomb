#include "map_region_repair.h"
#include "paving_neighbors.h"
#include "road_place.h"
#include "map_refresh_postpass4.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(a) ((int32_t)(uint32_t)(a))
extern int32_t FUN_00484be0(void *,int32_t,int32_t,uint8_t);
extern uint32_t FUN_0047b920(void);
extern int32_t FUN_0051c620(int32_t);

void restore_map_refresh_paved_region(int32_t left_in,int32_t top_in,int32_t right_in,int32_t bottom_in,uint32_t clear)
{
 uint32_t left=(uint32_t)left_in,top=(uint32_t)top_in,right=(uint32_t)right_in,bottom=(uint32_t)bottom_in;
 uint32_t swap,width,height,tail,cell,rows,columns,x,y,record,variation,image;uint8_t access,sprite;
 if(I(left)>I(right)){swap=left;left=right;right=swap;}if(I(top)>I(bottom)){swap=top;top=bottom;bottom=swap;}
 if(I(left)<0)left=0;if(I(top)<0)top=0;
 width=U(0xbca2d0u);if(I(right)>=I(width))right=width-1u;
 height=U(0xbca2d4u);if(I(bottom)>=I(height))bottom=height-1u;
 if(clear){
  tail=width-right+left-1u;cell=U(0xbca2d8u)+top*228u+left;
  if(I(top)<=I(bottom)){
   rows=bottom-top+1u;
   do{
    if(I(left)<=I(right)){
     columns=right-left+1u;
     do{
      if(B(0xaf598cu+cell*4u)&64u){
       access=B(0xb285ccu+cell);
       if(access&128u){
        record=(uint32_t)(int32_t)(int16_t)W(0xb4e6fcu+cell*2u);
        if(!record||W(0x7999e8u+record*264u)!=138u){
         U(0xb7482cu+cell*4u)=0;B(0xb285ccu+cell)=(uint8_t)(access&0xf0u);
         sprite=B(0xb67d1cu+cell);B(0xb67d1cu+cell)=(uint8_t)(sprite|64u);
        }
       }
      }
      ++cell;--columns;
     }while(columns);
     /* Native reloads width here but retains its original first-pass tail. */
     width=U(0xbca2d0u);
    }
    cell+=U(0xbca2dcu)+tail;--rows;
   }while(rows);
  }
 }
 tail=width-right+left-1u;cell=U(0xbca2d8u)+top*228u+left;y=top;
 if(I(y)>I(bottom))return;
 do{
  x=left;
  if(I(x)<=I(right))do{
   if((B(0xaf598cu+cell*4u)&64u)&&!U(0xb7482cu+cell*4u)&&(B(0xb285ccu+cell)&128u)){
    variation=B(0xacf85cu+cell)&1u;
    if(FUN_00475670(I(cell))){image=U(0xf5695cu)+6u+variation;FUN_0047bf50(0,I(x),I(y),2,2,image,64);}
    else{
     image=U(0xf5695cu);if((x&1u)&&(y&1u))++image;else if((x&1u)!=(y&1u))image+=2u;
     U(0xb7482cu+cell*4u)=image;B(0xb67d1cu+cell)=(uint8_t)(B(0xb67d1cu+cell)|64u);
    }
   }
   ++x;++cell;
  }while(I(x)<=I(right));
  cell+=U(0xbca2dcu)+tail;++y;
 }while(I(y)<=I(bottom));
}

void restore_map_refresh_channel_region(int32_t left_in,int32_t top_in,int32_t right_in,int32_t bottom_in,uint32_t refresh)
{
 uint32_t left=(uint32_t)left_in,top=(uint32_t)top_in,right=(uint32_t)right_in,bottom=(uint32_t)bottom_in;
 uint32_t swap,width,height,columns,cell,x,y,flags,difference,alternate,pattern,offset,image,result;
 uint8_t access,sprite,animation;
 if(I(left)>I(right)){swap=left;left=right;right=swap;}if(I(top)>I(bottom)){swap=top;top=bottom;bottom=swap;}
 --left;++right;--top;++bottom;if(I(left)<0)left=0;
 width=U(0xbca2d0u);if(I(right)>=I(width))right=width-1u;if(I(top)<0)top=0;
 height=U(0xbca2d4u);if(I(bottom)>=I(height))bottom=height-1u;
 columns=right-left+1u;cell=U(0xbca2d8u)+top*228u+left;U(0xbc9760u)=top;U(0xbc9768u)=cell;
 if(I(top)>I(bottom)){U(0xbca328u)=0;return;}
 do{
  U(0xbc9764u)=left;
  if(I(left)<=I(right))do{
   flags=U(0xaf598cu+cell*4u);
   if((flags&0x100u)&&!(flags&12u)&&B(0xae8e7cu+cell)<=15u){
    difference=U(0xb7482cu+cell*4u);difference-=U(0xf56b84u);alternate=(I(difference)<0||I(difference)>=48)?48u:0u;if(refresh)alternate=48;
    FUN_0047e870();(void)FUN_00484be0((void *)(uintptr_t)0x5cc678u,16,-1,0);cell=U(0xbc9768u);
    if(B(0xaf598cu+cell*4u)&64u){
     access=B(0xb285ccu+cell);pattern=U(0xbc9744u);B(0xb285ccu+cell)=(uint8_t)(access&127u);
     if(!pattern){flags=U(0xaf598cu+(cell-228u)*4u);U(0xbc9748u)=(flags&64u)==0;}
     result=FUN_0047b920();offset=U(0xbc9748u)+(result?42u:15u);cell=U(0xbc9768u);U(0xbc9748u)=offset;
    }
    offset=U(0xbc9740u);image=U(0xf56b84u)+offset;image+=U(0xbc9748u);flags=U(0xaf598cu+cell*4u);image+=alternate;
    if(flags&0x10000u){image=(uint32_t)FUN_0051c620(I(image));cell=U(0xbc9768u);}
    access=B(0xb285ccu+cell);U(0xb7482cu+cell*4u)=image;sprite=B(0xb67d1cu+cell);B(0xb285ccu+cell)=(uint8_t)(access&0xf0u);
    animation=B(0xbc9744u);B(0xb67d1cu+cell)=(uint8_t)(sprite|64u);B(0xae8e7cu+cell)=animation;
   }
   x=U(0xbc9764u)+1u;++cell;U(0xbc9764u)=x;U(0xbc9768u)=cell;
  }while(I(x)<=I(right));
  y=U(0xbc9760u)+1u;cell+=228u-columns;U(0xbc9760u)=y;U(0xbc9768u)=cell;
 }while(I(y)<=I(bottom));
 U(0xbca328u)=0;
}
