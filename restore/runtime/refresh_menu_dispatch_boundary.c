#include "refresh_menu_dispatch.h"
#include "menu_input.h"
#include "selection_popup.h"
/* Native pushes two extra origin DWORDs around the four-argument hit entry.
   The original caller retains and later cleans those slots. The existing
   authenticated four-argument C engine has no origin parameters. */
int restore_refresh_hit(int x,int y,const void *buttons,int count,int origin_x,int origin_y)
{
    (void)origin_x;
    (void)origin_y;
    return FUN_00426450(x,y,(const RestoreMenuButton *)buttons,count);
}
int restore_refresh_hover(int x,int y,const void *buttons,int count,int hit)
{
    return FUN_004d6740(x,y,(int)(uintptr_t)buttons,(uint32_t)count,hit);
}
