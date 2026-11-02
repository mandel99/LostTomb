#include "map_commit_placement.h"
#include "map_commit_state.h"
#include "map_commit_proximity.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include "map_neighbors_phase.h"
#include "water_refresh.h"
#include "paving_neighbors.h"
#include "road_place.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0040f860(void *,void *,int32_t);
extern uint32_t FUN_0046f930(int32_t,int32_t);
extern int32_t FUN_00484be0(void *,int32_t,int32_t,uint8_t);
extern void FUN_0051f780(void);
static int32_t add(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t clamp(int32_t value,int32_t last)
{int32_t v=value<last?value:last;return v<0?0:v;}
static int32_t magnitude(uint32_t value)
{uint32_t sign=0u-(value>>31);return (int32_t)((value^sign)-sign);}
void FUN_0047ad20(int32_t x,int32_t y,uint32_t place,uint32_t mask)
{
 uint32_t origin,cell,flags,old,cost,count,total,image,sprite,access;int32_t world_y,world_x,left,top,columns=3,rows=3,right,bottom,row,column,limit;
 world_y=add((int32_t)U(0xbca2ecu),y);world_x=add((int32_t)U(0xbca2e8u),x);
 if(!(uint8_t)FUN_0046f930(world_x,world_y))return;
 origin=U(0xbca2d8u);cell=origin+(uint32_t)y*228u+(uint32_t)x;
 if(place==1){
  flags=U(0xaf598cu+cell*4u);old=flags&0xc04000u;
  if(old!=mask){
   cost=0;
   if(old==0x4000u)cost=(uint32_t)((int32_t)U(0x705f94u)/2);
   else if(old==0x404000u&&mask==0x804000u)cost=(uint32_t)((int32_t)U(0x7078e4u)/2);
   count=U(0xbc971cu);U(0xbc971cu)=count+1u;
   total=U(0xbc9718u);U(0xbc9718u)=total+cost;
  }
  U(0xaf598cu+cell*4u)=(flags&0xff3fbfffu)|mask;
 }else U(0xaf598cu+cell*4u)&=0xff3fbfffu;
 access=B(0xb285ccu+cell);B(0xb285ccu+cell)=(uint8_t)(access&0xefu);
 left=add(x,-1);top=add(y,-1);
 if(left<0){columns=add(left,3);left=0;}
 else{limit=(int32_t)U(0xbca2d0u);if(add(left,3)>limit)columns=add(limit,-left);}
 if(top<0){rows=add(top,3);top=0;}
 else{limit=(int32_t)U(0xbca2d4u);if(add(top,3)>limit)rows=add(limit,-top);}
 U(0xbc9760u)=(uint32_t)top;cell=origin+(uint32_t)top*228u+(uint32_t)left;bottom=add(top,rows);U(0xbc9768u)=cell;
 if(top>=bottom)return;
 right=add(left,columns);
 do{
  U(0xbc9764u)=(uint32_t)left;
  if(left<right)do{
   flags=U(0xaf598cu+cell*4u);
   if((flags&0x4000u)&&!(flags&8u)){
    FUN_0047db00(0x4000u);FUN_00484f00();
    (void)FUN_00484be0((void *)(uintptr_t)0x5cb9d8u,48,-1,0);
    image=U(0xbc9740u);image+=U(0xf5686cu);image+=U(0xbc9748u);cell=U(0xbc9768u);
    sprite=B(0xb67d1cu+cell);U(0xb7482cu+cell*4u)=image;
    access=B(0xb285ccu+cell);B(0xb67d1cu+cell)=(uint8_t)(sprite|0x40u);B(0xb285ccu+cell)=(uint8_t)(access&0xf0u);
    if(FUN_0047e000()){
     FUN_0047e750();
     if(FUN_00484be0((void *)(uintptr_t)0x5cbcd8u,15,-1,0)){
      image=U(0xbc9740u);image+=U(0xf5686cu);image+=U(0xbc9748u);cell=U(0xbc9768u);U(0xb7482cu+cell*4u)=image;
     }else FUN_0051f780();
    }else FUN_0051f780();
    row=(int32_t)U(0xbc9760u);column=(int32_t)U(0xbc9764u);
    FUN_004746d0(column,row,1);cell=U(0xbc9768u);
   }
   column=add((int32_t)U(0xbc9764u),1);++cell;U(0xbc9764u)=(uint32_t)column;U(0xbc9768u)=cell;
  }while(column<right);
  row=add((int32_t)U(0xbc9760u),1);U(0xbc9760u)=(uint32_t)row;cell+=228u-(uint32_t)columns;
  row=(int32_t)U(0xbc9760u);U(0xbc9768u)=cell;
 }while(row<bottom);
}
void FUN_00475220(int32_t mode,int32_t x1,int32_t y1,int32_t x2,int32_t y2,uint32_t mask)
{
 int32_t last_x,last_y,t,row,column,world_y,world_x;uint32_t cell,stride,width,pointer;
 FUN_0040f860((void *)(uintptr_t)0x9de62cu,(void *)(uintptr_t)0xaf598cu,0x32c40);
 FUN_0040f860((void *)(uintptr_t)0x9d1b1cu,(void *)(uintptr_t)0xae8e7cu,0xcb10);
 FUN_00485830();
 last_x=add((int32_t)U(0xbca2d0u),-1);x1=clamp(x1,last_x);
 last_y=add((int32_t)U(0xbca2d4u),-1);y1=clamp(y1,last_y);x2=clamp(x2,last_x);y2=clamp(y2,last_y);
 if(magnitude((uint32_t)y2-(uint32_t)y1)<magnitude((uint32_t)x2-(uint32_t)x1)){
  if(y1<y2){t=add(y1,1);if(t<=y2)y2=t;}
  else if(y2<y1){t=add(y1,-1);if(y2<=t)y2=t;}
 }else if(x1<x2){t=add(x1,1);if(t<=x2)x2=t;}
 else if(x2<x1){t=add(x1,-1);if(x2<=t)x2=t;}
 if(x2<x1){t=x1;x1=x2;x2=t;}
 if(y2<y1){t=y1;y1=y2;y2=t;}
 width=U(0xbca2d0u);U(0xbc971cu)=0;
 stride=width-(uint32_t)x2+(uint32_t)x1-1u;cell=U(0xbca2d8u)+(uint32_t)y1*228u+(uint32_t)x1;
 row=y1;
 if(row<=y2)do{
  column=x1;
  if(column<=x2)do{
   if(!(U(0xaf598cu+cell*4u)&0x2635177fu)){
    pointer=U(0xd05720u+cell*4u);
    if(!pointer||B(pointer+24u)){
     world_y=add((int32_t)U(0xbca2ecu),row);world_x=add((int32_t)U(0xbca2e8u),column);
     if((uint8_t)FUN_0046f930(world_x,world_y)){
      if(!(uint8_t)FUN_00470900(column,row,column,row)&&H(0xb350dcu+cell*2u)==0){
       U(0xbc9768u)=cell;FUN_0047ad20(column,row,1,mask);
      }
     }
    }
   }
   column=add(column,1);++cell;
  }while(column<=x2);
  cell+=U(0xbca2dcu)+stride;row=add(row,1);
 }while(row<=y2);
 if(mode==0){FUN_00516f90(x1,y1,x2,y2,1);FUN_005173a0(x1,y1,x2,y2);FUN_005178e0();B(0xe38dcau)=1;}
}
