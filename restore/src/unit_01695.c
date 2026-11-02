#include "restore_types.h"
#include "text_selection.h"
extern void FUN_004cc720();
void FUN_004ccb30(int group,int entry,undefined4 x,undefined4 y,undefined4 width,undefined4 font,undefined4 color)
{
    restore_text_select(group,entry,0x00e20768u);
    FUN_004cc720(*(uint32_t *)0x00e20768u,x,y,width,font,color);
    *(uint32_t *)0x00d3b504u=0;
}
