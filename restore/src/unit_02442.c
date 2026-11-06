#include "view_state_actions.h"
#include "menu_building_overlay.h"
#include "menu_text_glyph.h"
void restore_view_draw_network_hint(void)
{
 uint32_t position,color;
 if (VIEW_U(0xf53f68u)==0) return;
 position=VIEW_U(0xe38da4u)*2u+220u;
 FUN_00536dc0(6,0,(int32_t)position,12,15,1);
 color=VIEW_U(0xf56f8cu);
 FUN_004cc720((const uint8_t *)0x5f8f28u,0,(int32_t)(position+6u),192,(int32_t)color,65535);
}
void FUN_00536620(void){restore_view_draw_network_hint();}
