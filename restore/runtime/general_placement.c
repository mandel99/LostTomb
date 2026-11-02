#include "general_placement.h"
#include "terrain_redraw.h"
#include <stdio.h>
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
static __declspec(noreturn) void unsupported(uint32_t target)
{fprintf(stderr,"Unreconstructed placement virtual target %08lx\n",(unsigned long)target);abort();}
uint32_t restore_placement_virtual_check(uint32_t self,uint32_t terrain,int32_t piece,uint32_t incoming_eax)
{
 uint32_t target=U(U(self)+16u),bits=terrain&0xfeffd76fu,accepted;
 switch(target){
 case 0x4e5310u:accepted=bits==0;break;
 case 0x401440u:accepted=U(self+0x58u+(uint32_t)piece*52u)==9u?bits==0:bits==0x20000002u;break;
 case 0x547de0u:accepted=(uint32_t)piece==U(self+0x50u)?bits==0:bits==0x20000002u;break;
 case 0x4e6620u:return piece==1?bits==0:bits==2;
 default:unsupported(target);
 }
 return (incoming_eax&0xffffff00u)|accepted;
}
uint32_t restore_placement_virtual_existing(uint32_t self,int32_t building,uint32_t mode)
{
 uint32_t target=U(U(self)+12u);(void)building;(void)mode;
 switch(target){
 /* Every one of the eight actual native vtables binds this exact body:
    004E52D0 33C0 XOR EAX,EAX; 004E52D2 C20800 RET8. */
 case 0x4e52d0u:return 0;
 default:unsupported(target);
 }
}
static uint8_t area(uint32_t self,uint32_t piece,uint32_t index,uint32_t x,uint32_t y,uint8_t accepted)
{
 uint32_t row=0,col,offset=0,cell,terrain;
 U(0xbc9768u)=U(0xbca2d8u)+y*228u+x;
 while((int32_t)row<(int32_t)U(piece+8u)&&accepted){
  col=0;
  while((int32_t)col<(int32_t)U(piece+8u)&&accepted){
   if((uint8_t)FUN_0046f9a0((int32_t)(U(0xbca2e8u)+col+x),(int32_t)(U(0xbca2ecu)+row+y),3))accepted=0;
   else{
    cell=U(0xbc9768u)+U(0x5cc8c0u+(offset+col)*8u);terrain=U(0xaf598cu+cell*4u);
    accepted=(uint8_t)restore_placement_virtual_check(self,terrain,(int32_t)index,terrain);
   }
   ++col;
  }
  offset+=6u;++row;
 }
 return accepted;
}
uint32_t restore_placement_check_004e5330(uint32_t self,uint32_t cell,uint32_t pieces,int32_t count)
{
 int32_t delta=(int32_t)(cell-U(0xbca2d8u));uint32_t x=(uint32_t)(delta%228),y=(uint32_t)(delta/228),index=0;uint8_t accepted=1;
 if(count<=0)return ((uint32_t)count&0xffffff00u)|1u;
 do{
  if(!accepted)break;
  accepted=area(self,pieces,index,U(pieces)+x,U(pieces+4u)+y,accepted);
  ++index;pieces+=52u;
 }while((int32_t)index<count);
 return (index&0xffffff00u)|accepted;
}
uint32_t restore_placement_check_005463e0(uint32_t self,uint32_t cell,uint32_t pieces,int32_t count)
{
 int32_t delta=(int32_t)(cell-U(0xbca2d8u));uint32_t x=(uint32_t)(delta%228),y=(uint32_t)(delta/228),index=0,px,py,center,terrain;uint8_t accepted=1,adjacent,last;
 if(count<=0)return ((uint32_t)count&0xffffff00u)|1u;
 do{
  if(!accepted)break;
  px=U(pieces)+x;py=U(pieces+4u)+y;
  if(index==U(self+0x50u)){
   /* All four native virtual calls execute, including after an early hit. */
   adjacent=0;
   center=U(0xbca2d8u)+py*228u+px;terrain=U(0xaf55fcu+center*4u);
   if((uint8_t)restore_placement_virtual_check(self,terrain,(int32_t)index,terrain))adjacent=1;
   center=U(0xbca2d8u)+py*228u+px;terrain=U(0xaf5d1cu+center*4u);
   if((uint8_t)restore_placement_virtual_check(self,terrain,(int32_t)index,terrain))adjacent=1;
   center=U(0xbca2d8u)+py*228u+px;terrain=U(0xaf5988u+center*4u);
   if((uint8_t)restore_placement_virtual_check(self,terrain,(int32_t)index,terrain))adjacent=1;
   center=U(0xbca2d8u)+py*228u+px;terrain=U(0xaf5990u+center*4u);
   last=(uint8_t)restore_placement_virtual_check(self,terrain,(int32_t)index,terrain);
   if(!last&&!adjacent)accepted=0;
  }
  accepted=area(self,pieces,index,px,py,accepted);
  ++index;pieces+=52u;
 }while((int32_t)index<count);
 return (index&0xffffff00u)|accepted;
}
uint32_t restore_placement_existing_004e5790(uint32_t self,int32_t building_type,int32_t count,uint32_t incoming_eax)
{
 uint32_t index=1,record=0x799ae0u,eax=incoming_eax;uint8_t accepted=0;
 while((int32_t)index<count){
  if(B(record)==1){
   eax=(uint32_t)(int32_t)H(record+16u);
   if((int32_t)eax==building_type){
    eax=restore_placement_virtual_existing(self,(int32_t)index,0);
    if((int32_t)eax<100)accepted=1;
    break;
   }
  }
  ++index;record+=264u;
 }
 return (eax&0xffffff00u)|accepted;
}
