#include "monument_placement_queries.h"
#include "monument_complex_queries.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_0046f9a0(int32_t,int32_t,int32_t);
static int32_t add(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
uint32_t FUN_00541ae0(uint32_t cell)
{
 int32_t delta=(int32_t)(cell-U(0xbca2d8u)),start_x=delta%228,start_y=delta/228,x,y,size,row,column,wy,wx;
 uint32_t record,origin,eax=0,offset,current;
 for(record=0x601ddcu;record<0x60322cu;record+=52u){
  y=add((int32_t)U(record),start_y);origin=U(0xbca2d8u);x=add((int32_t)U(record-4u),start_x);size=(int32_t)U(record+4u);
  U(0xbc9768u)=origin+(uint32_t)y*228u+(uint32_t)x;eax=0;
  for(row=0;row<size;++row){
   for(column=0;column<size;++column){
    wy=add((int32_t)U(0xbca2ecu),add(y,row));wx=add(add((int32_t)U(0xbca2e8u),column),x);
    eax=FUN_0046f9a0(wx,wy,3);if((uint8_t)eax)return eax&0xffffff00u;
    offset=U(0x5cc8c0u+((uint32_t)row*6u+(uint32_t)column)*8u);current=U(0xbc9768u);eax=offset+current;
    if(U(0xaf598cu+eax*4u)&0xfeffd76eu)return eax&0xffffff00u;
   }
   eax=(uint32_t)row+1u;
  }
 }
 return(eax&0xffffff00u)|1u;
}
uint32_t restore_monument_ready_00542ee0(uint32_t incoming_eax)
{
 uint32_t building=1,record=0x799ae0u,eax=incoming_eax;
 while(record<0x89b6d8u){
  if(B(record)==1&&W(record+16u)==213u){
   eax=(uint32_t)FUN_00542ad0(building,0);if(eax==100u)return(eax&0xffffff00u)|1u;
   building+=99u;record+=99u*264u;
  }
  ++building;record+=264u;
 }
 return eax&0xffffff00u;
}
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Physical monument entry requires MSVC x86
#endif
__declspec(naked) uint32_t FUN_00542ee0(void)
{
 __asm {
  push eax
  call restore_monument_ready_00542ee0
  add esp,4
  ret
 }
}
