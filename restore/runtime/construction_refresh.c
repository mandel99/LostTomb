#include "construction_refresh.h"
#include "map_neighbors_phase.h"
#include "paving_neighbors.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_00484f00(void);
extern void FUN_0047db00(uint32_t mask);
extern int32_t FUN_00484be0(void *,int32_t,int32_t,uint8_t);
extern void FUN_0051f780(void);
extern void FUN_004746d0(int32_t,int32_t,int32_t);
extern void FUN_0047e5c0(void);
extern void FUN_0047b670(void);
static int32_t add(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
struct area{int32_t left,top,columns,rows;};
static struct area area(int32_t x,int32_t y,int32_t size)
{
 struct area a;int32_t limit;
 a.left=add(x,-1);a.top=add(y,-1);a.columns=size;a.rows=size;
 if(a.left<0){a.columns=add(a.left,size);a.left=0;}
 else{limit=(int32_t)U(0xbca2d0u);if(add(a.left,size)>limit)a.columns=add(limit,-a.left);}
 if(a.top<0){a.rows=add(a.top,size);a.top=0;}
 else{limit=(int32_t)U(0xbca2d4u);if(add(a.top,size)>limit)a.rows=add(limit,-a.top);}
 return a;
}
static uint32_t water(void)
{
 uint32_t image,cell,sprite,access;int32_t row,column;
 FUN_0047db00(0x4000u);FUN_00484f00();(void)FUN_00484be0((void *)(uintptr_t)0x5cb9d8u,48,-1,0);
 image=U(0xbc9740u);image+=U(0xf5686cu);image+=U(0xbc9748u);cell=U(0xbc9768u);
 sprite=B(0xb67d1cu+cell);U(0xb7482cu+cell*4u)=image;access=B(0xb285ccu+cell);
 B(0xb67d1cu+cell)=(uint8_t)(sprite|0x40u);B(0xb285ccu+cell)=(uint8_t)(access&0xf0u);
 if(FUN_0047e000()){
  FUN_0047e750();
  if(FUN_00484be0((void *)(uintptr_t)0x5cbcd8u,15,-1,0)){
   image=U(0xbc9740u);image+=U(0xf5686cu);image+=U(0xbc9748u);cell=U(0xbc9768u);U(0xb7482cu+cell*4u)=image;
  }else FUN_0051f780();
 }else FUN_0051f780();
 row=(int32_t)U(0xbc9760u);column=(int32_t)U(0xbc9764u);FUN_004746d0(column,row,1);return U(0xbc9768u);
}
void FUN_0047afd0(int32_t x,int32_t y,int32_t size)
{
 struct area a=area(x,y,size);uint32_t cell=U(0xbca2d8u),flags;int32_t row,column,right=add(a.left,a.columns),bottom=add(a.top,a.rows);
 cell+=(uint32_t)a.top*228u+(uint32_t)a.left;U(0xbc9760u)=(uint32_t)a.top;U(0xbc9768u)=cell;
 if(a.top>=bottom)return;
 do{
  U(0xbc9764u)=(uint32_t)a.left;
  if(a.left<right)do{
   flags=U(0xaf598cu+cell*4u);if((flags&0x4000u)&&!(flags&8u))cell=water();
   column=add((int32_t)U(0xbc9764u),1);++cell;U(0xbc9764u)=(uint32_t)column;U(0xbc9768u)=cell;
  }while(column<right);
  row=add((int32_t)U(0xbc9760u),1);U(0xbc9760u)=(uint32_t)row;row=(int32_t)U(0xbc9760u);
  cell+=228u-(uint32_t)a.columns;U(0xbc9768u)=cell;
 }while(row<bottom);
}
void FUN_0047b480(int32_t x,int32_t y,int32_t size)
{
 struct area a=area(x,y,size);uint32_t cell=U(0xbca2d8u),flags;int32_t row,column,right=add(a.left,a.columns),bottom=add(a.top,a.rows);
 cell+=(uint32_t)a.top*228u+(uint32_t)a.left;U(0xbc9760u)=(uint32_t)a.top;U(0xbc9768u)=cell;
 if(a.top>=bottom)return;
 do{
  U(0xbc9764u)=(uint32_t)a.left;
  if(a.left<right)do{
   flags=U(0xaf598cu+cell*4u);
   if((flags&0x40u)&&!(flags&12u)){FUN_0047db00(0x08000040u);FUN_0047e5c0();FUN_0047b670();cell=U(0xbc9768u);}
   column=add((int32_t)U(0xbc9764u),1);++cell;U(0xbc9764u)=(uint32_t)column;U(0xbc9768u)=cell;
  }while(column<right);
  row=add((int32_t)U(0xbc9760u),1);cell+=228u-(uint32_t)a.columns;U(0xbc9760u)=(uint32_t)row;U(0xbc9768u)=cell;
 }while(row<bottom);
}
