#include "editor_event_list_actions.h"
#include "actor_mission_init.h"
#include "map_file_load.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S16(a) ((int32_t)(int16_t)W(a))

/* Entire4489C0:34 instructions and a real tail JMP to the accepted10-
   instruction448A60. These are field stores, never a whole-record memset. */
void restore_event_record_reset_004489c0(uint32_t self)
{
 W(self)=0xffff;B(self+2u)=0;B(self+3u)=0;
 W(self+0x24u)=0xffff;W(self+0x26u)=0xffff;
 W(self+0x52u)=0xffff;W(self+0x54u)=0xffff;
 U(self+0x28u)=2;
 W(self+0x18u)=1;W(self+0x1au)=2;W(self+0x16u)=0xffff;
 W(self+0x10u)=5;W(self+0x12u)=10;W(self+0xeu)=0xffff;
 W(self+0x5cu)=0;W(self+0x5eu)=49;W(self+0x5au)=0xffff;
 W(self+6u)=1;W(self+8u)=0xffff;W(self+0xau)=0xffff;
 W(self+0x20u)=1;W(self+0x22u)=8;W(self+0x1eu)=0xffff;
 W(self+0x2cu)=2;W(self+0x2eu)=0;W(self+0x30u)=0;
 B(self+0x32u)=0;B(self+0x33u)=0;W(self+0x34u)=0xffff;
 B(self+0x36u)=0;W(self+0x38u)=0;
 restore_town_actor_links_00448a60((void *)(uintptr_t)self);
}
/* Entire44D680:21 instructions. Only AL is promised: callers test AL.
   Both calendar globals are DWORD; event/base years are signed WORD. */
uint8_t restore_event_record_due_0044d680(uint32_t self,uint32_t ignore_flags)
{
 int32_t year,base_year;uint32_t current_year,month,current_month;
 if((uint8_t)ignore_flags==0&&(B(self+0x28u)&5u)!=0)return 0;
 year=S16(self+0x14u);base_year=S16(0x7848d8u);
 current_year=U(0x78d7bcu);
 if(current_year!=(uint32_t)year+(uint32_t)base_year)return 0;
 month=B(self+3u);current_month=U(0x78d7c0u);
 return (uint8_t)(current_month==month);
}
static int event_referenced(uint32_t index)
{
 uint32_t cursor=0xbcba9au;
 do{
  if(B(cursor-0x42u)==1u&&B(cursor)==0&&B(cursor+0x27u)==0&&
     S16(cursor+0x36u)==(int32_t)index)return 1;
  cursor+=144u;
 }while(cursor<0xbcd62au);
 cursor=0xbcd786u;
 do{
  if(B(cursor+4u)!=0&&B(cursor)==0x5cu&&U(cursor+0x172u)==index)return 1;
  cursor+=388u;
 }while(cursor<0xc8ad42u);
 return 0;
}
/* Entire448D10:113 instructions. Native thiscall1/RET4, full EAX pointer.
   Limited=true rejects count>119 BEFORE searching reusable slots. The later
   generic count>139 rejection and its existing warning are separate branches. */
uint32_t restore_event_list_append_00448d10(uint32_t self,uint32_t limited)
{
 uint32_t count,index,cursor,flags,record;
 if((uint8_t)limited!=0&&((int32_t)U(self)>119))return 0;
 count=U(self);index=0;
 if((int32_t)count>0){
  cursor=self+0x2cu;
  do{
   flags=U(cursor);
   if((flags&8u)!=0&&(flags&4u)!=0&&B(cursor+0xau)==0&&
      !restore_event_record_due_0044d680(cursor-0x28u,1)){
    if(B(cursor-0x26u)!=2u||!event_referenced(index))return self+4u+index*124u;
   }
   count=U(self);++index;cursor+=124u;
  }while((int32_t)index<(int32_t)count);
 }
 count=U(self);
 if((int32_t)count>139){
  if((uint8_t)limited==0)FUN_004ce6e0((char *)(uintptr_t)0x5da174u,0,0);
  return 0;
 }
 ++count;U(self)=count;
 record=self+count*124u-120u;
 restore_event_record_reset_004489c0(record);
 W(record)=(uint16_t)(W(self)-1u);
 return record;
}
/* Entire448E30:88 instructions. Native thiscall1/RET4, logical AL result.
   Shift each124-byte record in native forward DWORD order, then rewrite its
   WORD id. Equal references are retained; only greater signed WORDs decrement. */
uint8_t restore_event_list_erase_00448e30(uint32_t self,int32_t index)
{
 uint32_t count,current,record,source,k,value,cursor,seen,scroll;
 uint16_t reference;
 if(index<0)return 0;
 count=U(self);
 if(index>=(int32_t)count)return 0;
 current=(uint32_t)index;
 if((int32_t)current<(int32_t)(count-1u)){
  record=self+4u+current*124u;
  do{
   source=record+124u;
   for(k=0;k<31u;++k){value=U(source+k*4u);U(record+k*4u)=value;}
   record+=124u;
   W(record-124u)=(uint16_t)current;
   count=U(self)-1u;++current;
  }while((int32_t)current<(int32_t)count);
 }
 count=U(self)-1u;U(self)=count;
 seen=0;
 if((int32_t)count>0){
  cursor=self+0x2au;
  do{
   reference=W(cursor-2u);
   if((int16_t)reference>index)W(cursor-2u)=(uint16_t)(reference-1u);
   reference=W(cursor);
   if((int16_t)reference>index)W(cursor)=(uint16_t)(reference-1u);
   reference=W(cursor+0x2cu);
   if((int16_t)reference>index)W(cursor+0x2cu)=(uint16_t)(reference-1u);
   reference=W(cursor+0x2eu);
   if((int16_t)reference>index)W(cursor+0x2eu)=(uint16_t)(reference-1u);
   count=U(self);++seen;cursor+=124u;
  }while((int32_t)seen<(int32_t)count);
 }
 scroll=U(self+0x48b0u);count=U(self);
 if((int32_t)scroll>=(int32_t)count){
  --scroll;U(self+0x48b0u)=scroll;
  if((int32_t)scroll<0)U(self+0x48b0u)=0;
 }
 if((int32_t)U(self+0x48acu)>=(int32_t)count)U(self+0x48acu)=UINT32_MAX;
 return 1;
}
/* Entire448CA0:20 instructions. Mode is published before dirty, and the
   prior mode is zero-extended into a DWORD only after both BYTE stores. */
void restore_event_detail_open_00448ca0(void)
{
 uint32_t selected=U(0x78a61cu),record,previous;
 if((int32_t)selected<0)return;
 if((int32_t)selected>=(int32_t)U(0x785d70u))return;
 record=0x785d74u+selected*124u;
 if(record==0)return;
 previous=B(0xe38ddau);
 if(previous==0x18u)return;
 B(0xe38ddau)=0x18;
 B(0xe38dcau)=1;
 U(0x785d6cu)=previous;
}
/* Entire448560:11 instructions, including the actual tail JMP448CA0. */
void restore_editor_event_list_00448560(void)
{
 uint32_t action=U(0xd3b198u),top=U(0x78a620u),selected=action+top;
 if((int32_t)selected<0){U(0x78a61cu)=UINT32_MAX;return;}
 if((int32_t)selected>=(int32_t)U(0x785d70u)){U(0x78a61cu)=UINT32_MAX;return;}
 U(0x78a61cu)=selected;
 restore_event_detail_open_00448ca0();
}
/* Entire448590:39 instructions. The allocator may reset a new slot before
   this callback intentionally resets it again and installs these exact fields. */
void restore_editor_event_list_00448590(void)
{
 uint32_t record=restore_event_list_append_00448d10(0x785d70u,1);
 uint16_t id;
 if(record==0)return;
 id=W(record);
 restore_event_record_reset_004489c0(record);
 W(record)=id;B(record+2u)=1;B(record+0x60u)=0;U(record+0x28u)=0;
 W(record+6u)=1;W(record+8u)=0xffff;W(record+0xau)=0xffff;
 W(record+0x16u)=1;W(record+0x18u)=0xffff;W(record+0x1au)=0xffff;
 W(record+0x1eu)=1;W(record+0x20u)=0xffff;W(record+0x22u)=0xffff;
 W(record+0xeu)=8;W(record+0x10u)=0xffff;W(record+0x12u)=0xffff;
 W(record+0x2cu)=12;W(record+0x24u)=0xffff;W(record+0x26u)=0xffff;
 W(record+0x52u)=0xffff;W(record+0x54u)=0xffff;
}
/* Entire448620:5 instructions; caller ignores the native incidental return. */
void restore_editor_event_list_00448620(void)
{
 uint32_t selected=U(0x78a61cu);
 (void)restore_event_list_erase_00448e30(0x785d70u,(int32_t)selected);
}
int restore_editor_event_list_action(uint32_t token)
{
 switch(token){
 case 0x448560u:restore_editor_event_list_00448560();return 1;
 case 0x448590u:restore_editor_event_list_00448590();return 1;
 case 0x448620u:restore_editor_event_list_00448620();return 1;
 default:return 0;
 }
}
