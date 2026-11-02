#include "selection_button_actions.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Pinned native4062D0..4062ED:8 instructions, table5C5A78.
   Toggle is equality-to-one, not a low-byte XOR or Boolean negation. */
void restore_selection_button_004062d0(void)
{
 uint32_t previous=U(0xeadd44u);
 U(0xeadd44u)=previous!=1u?1u:0u;
 B(0xe38dcau)=1;
}
/* Native4062F0..4062FF:4 instructions, tables5C5A98/B8/D8. */
void restore_selection_button_004062f0(void)
{
 U(0xeadcc0u)=1;
 B(0xe38dcau)=1;
}
/* Each list DWORD is an offset into a196-byte player record table. It is
   not a record number to multiply again. Zero/negative counts skip stores. */
static void mark_offsets(uint32_t player_offset,uint32_t count)
{
 uint32_t cursor=0xf53e70u;
 while(count){
  uint32_t offset=U(cursor);
  cursor+=4u;--count;
  B(0x7900c2u+player_offset+offset)=1;
 }
}
/* Native406620..406674:29 instructions, table5C5BA8. The owner byte is
   loaded even for an empty list, before the signed DWORD count check. */
void restore_selection_button_00406620(void)
{
 uint32_t selected=U(0xeadd6cu);
 uint32_t owner=B(0x799a99u+selected*264u);
 uint32_t count=U(0xf53dd8u);
 if((int32_t)count>0)mark_offsets(owner*196u,count);
 B(0xe38dcau)=1;
}
/* Native406680..4066F3:35 instructions, table5C5BC8. The owner byte load
   precedes the mode read even though mode1 uses the selected DWORD instead. */
void restore_selection_button_00406680(void)
{
 uint32_t selected=U(0xeadd6cu);
 uint32_t owner=B(0x799a99u+selected*264u);
 uint32_t mode=U(0xd3b198u);
 if(mode==1u){
  uint32_t player=U(0xeaac28u);
  U(0xea5ee4u+player*0x49d8u)=selected;
 }else if(mode==2u){
  uint32_t count=U(0xf53e6cu);
  if((int32_t)count>0)mark_offsets(owner*196u,count);
 }
 B(0xe38dcau)=1;
}
int restore_selection_button_action(uint32_t token)
{
 switch(token){
 case 0x4062d0u:restore_selection_button_004062d0();return 1;
 case 0x4062f0u:restore_selection_button_004062f0();return 1;
 case 0x406620u:restore_selection_button_00406620();return 1;
 case 0x406680u:restore_selection_button_00406680();return 1;
 default:return 0;
 }
}
