#include "construction_brush.h"
#include "editor_brush_validation.h"
#include "terrain_redraw.h"
#include "map_refresh_postpass1.h"
#include "water_refresh.h"
#include "clear_helpers.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_004746d0(int32_t,int32_t,int32_t);
extern void FUN_00475cc0(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_004777c0(int32_t,int32_t,int32_t,int32_t);
void FUN_004f4930(int32_t building)
{
 uint32_t index=(uint32_t)building,record,group,cell,origin;int32_t dx,x,y,end_x,end_y,width,right,bottom;
 record=index*264u;if(W(0x7999e8u+record)!=183u)return;
 index-=(uint32_t)(int32_t)(int16_t)W(0x7999eau+record);record=index*264u;group=B(0x799a2cu+record);
 if(!(uint8_t)FUN_004ecea0((int32_t)group))return;
 cell=U(0x5f5d08u+group*4u);cell+=U(0x7999e4u+record);origin=U(0xbca2d8u);dx=(int32_t)(cell-origin);
 x=dx%228;y=dx/228;FUN_004ed360(x,y,5,0);
 cell=U(0xeaae9cu);origin=U(0xbca2d8u);dx=(int32_t)(cell-origin);end_x=dx%228;end_y=dx/228;
 width=(int32_t)((uint32_t)end_x-(uint32_t)x+2u);right=(int32_t)((uint32_t)end_x+2u);bottom=(int32_t)((uint32_t)end_y+2u);
 FUN_004767d0(x,y,right,bottom,0,0);FUN_004746d0(x,y,width);FUN_00477500(x,y,right,bottom);
 FUN_004777c0(x,y,right,bottom);FUN_00475cc0(x,y,right,bottom,0);FUN_004776f0(x,y,right,bottom);FUN_00477890(x,y,right,bottom);
}
