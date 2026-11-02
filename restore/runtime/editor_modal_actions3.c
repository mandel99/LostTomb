#include "editor_modal_actions3.h"
#include "numeric_input.h"
#include "selection_popup.h"
#include "mission_load_closure.h"
#include "redraw_dispatch_model.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Three actual15-instruction callbacks. Native loads the Y-origin before
   the X-origin, then performs wrapped DWORD additions and seven PUSHes. */
static void open_number(uint32_t destination)
{
 uint32_t y,x;
 y=U(0xe38da4u);
 x=U(0xe38da8u);
 FUN_00403790(x+0x8cu,y+0x38u,4,0,0,destination,0);
 B(0xe38dcau)=1;
}
void restore_editor_modal3_00408ac0(void)
{open_number(0x7848f8u);} /* Native408AC0..408AF2. */
void restore_editor_modal3_00408b00(void)
{open_number(0x784c94u);} /* Native408B00..408B32. */
void restore_editor_modal3_00408b40(void)
{open_number(0x784ddcu);} /* Native408B40..408B72. */
/* Native408B80..408BB0:19 instructions, fifteen DWORD arguments.
   Storage kind2 writes a WORD destination, not a BYTE or DWORD target. */
void restore_editor_modal3_00408b80(void)
{
 FUN_00403c00(20,40,12,37,0,0,0x7848fcu,0,0,2,0,0,0,-1,0);
 B(0xe38dcau)=1;
}
/* Native414730..414745:6 instructions. Both calls are actual accepted
   algorithms; no fill/stub replaces the map update or redraw graph. */
uint32_t restore_editor_palette_00414730(uint32_t palette)
{
 B(0x784cb0u)=(uint8_t)palette;
 FUN_00486e70();
 FUN_004dea50();
 return 0;
}
/* Native408BC0..408BF8:14 instructions. Keep the intermediate BYTE write
   before reset and the subsequent full DWORD read before the update flag. */
void restore_editor_modal3_00408bc0(void)
{
 uint8_t next=(uint8_t)(B(0x784cb0u)+1u);
 uint32_t value;
 B(0x784cb0u)=next;
 if(next>=3u)B(0x784cb0u)=0;
 value=U(0x784cb0u);
 B(0xbca30cu)=1;
 (void)restore_editor_palette_00414730(value&255u);
 B(0xe38dcau)=1;
}
/* Native408C00..408C18:7 instructions. Dirty precedes the flag write. */
void restore_editor_modal3_00408c00(void)
{
 uint8_t previous=B(0x784cbdu);
 B(0xe38dcau)=1;
 B(0x784cbdu)=previous!=1u?1u:0u;
}
int restore_editor_modal3_action(uint32_t token)
{
 switch(token){
 case 0x408ac0u:restore_editor_modal3_00408ac0();return 1;
 case 0x408b00u:restore_editor_modal3_00408b00();return 1;
 case 0x408b40u:restore_editor_modal3_00408b40();return 1;
 case 0x408b80u:restore_editor_modal3_00408b80();return 1;
 case 0x408bc0u:restore_editor_modal3_00408bc0();return 1;
 case 0x408c00u:restore_editor_modal3_00408c00();return 1;
 default:return 0;
 }
}
